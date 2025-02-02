/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "avrecorder.h"

#include <mutex>
#include <shared_mutex>

#include "media_log.h"
#include "string_ex.h"
#include "av_common.h"
#include "meta/meta.h"
#include "media_errors.h"
#include "surface_utils.h"
#include "native_window.h"
#include "native_player_magic.h"

namespace {
    constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, LOG_DOMAIN_RECORDER, "NativeAVRecorder" };
}

using namespace OHOS;
using namespace OHOS::Media;

class NativeRecorderCallback;

/**
Enumerates the video rotation.
*/
enum RotationAngle {
    /**
    * Video without rotation
    */
    ROTATION_0 = 0,
    /**
    * Video rotated 90 degrees
    */
    ROTATION_90 = 90,
    /**
    * Video rotated 180 degrees
    */
    ROTATION_180 = 180,
    /**
    * Video rotated 270 degrees
    */
    ROTATION_270 = 270
};

struct RecorderObject : public OH_AVRecorder {
    explicit RecorderObject(const std::shared_ptr<Recorder>& recorder)
        : recorder_(recorder) {}
    ~RecorderObject() = default;

    const std::shared_ptr<Recorder> recorder_ = nullptr;
    std::atomic<bool> isReleased_ = false;
    std::shared_ptr<NativeRecorderCallback> callback_ = nullptr;
    int32_t videoSourceId_ = -1;
    int32_t audioSourceId_ = -1;
    bool hasConfigured_ = false;
};

class NativeRecorderStateChangeCallback {
public:
    NativeRecorderStateChangeCallback(OH_AVRecorder_OnStateChange callback, void *userData)
        : callback_(callback), userData_(userData) {}
    virtual ~NativeRecorderStateChangeCallback() = default;

    void OnStateChange(struct OH_AVRecorder *recorder, OH_AVRecorder_State state,
        OH_AVRecorder_StateChangeReason reason)
    {
        CHECK_AND_RETURN(recorder != nullptr && callback_ != nullptr);
        callback_(recorder, static_cast<OH_AVRecorder_State>(state), reason, userData_);
    }

private:
    OH_AVRecorder_OnStateChange callback_;
    void *userData_;
};

class NativeRecorderErrorCallback {
public:
    NativeRecorderErrorCallback(OH_AVRecorder_OnError callback, void *userData)
        : callback_(callback), userData_(userData) {}
    virtual ~NativeRecorderErrorCallback() = default;

    void OnError(struct OH_AVRecorder *recorder, int32_t errorCode, const char *errorMsg)
    {
        CHECK_AND_RETURN(recorder != nullptr && callback_ != nullptr);
        callback_(recorder, errorCode, errorMsg, userData_);
    }

private:
    OH_AVRecorder_OnError callback_;
    void *userData_;
};

class NativeRecorderCallback : public RecorderCallback {
public:
    explicit NativeRecorderCallback(struct OH_AVRecorder *recorder) : recorder_(recorder) {}
    virtual ~NativeRecorderCallback() = default;

    void OnStateChange(OH_AVRecorder_State state, OH_AVRecorder_StateChangeReason reason)
    {
        MEDIA_LOGI("OnStateChange() is called, state: %{public}d", state);
        std::shared_lock<std::shared_mutex> lock(mutex_);
        CHECK_AND_RETURN(recorder_ != nullptr);

        if (stateChangeCallback_ != nullptr) {
            stateChangeCallback_->OnStateChange(recorder_, state, reason);
            return;
        }
    }

    void OnError(RecorderErrorType errorType, int32_t errorCode) override
    {
        MEDIA_LOGE("OnError() is called, errorType: %{public}d, errorCode: %{public}d", errorType, errorCode);
        std::shared_lock<std::shared_mutex> lock(mutex_);
        CHECK_AND_RETURN(recorder_ != nullptr);

        if (errorCallback_ != nullptr) {
            errorCallback_->OnError(recorder_, errorCode, "error occurred!");
            return;
        }
    }

    void OnInfo(int32_t type, int32_t extra) override
    {
        // No specific implementation is required and can be left blank.
    }

    bool SetStateChangeCallback(OH_AVRecorder_OnStateChange callback, void *userData)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        stateChangeCallback_ = std::make_shared<NativeRecorderStateChangeCallback>(callback, userData);
        return stateChangeCallback_ != nullptr;
    }

    bool SetErrorCallback(OH_AVRecorder_OnError callback, void *userData)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        errorCallback_ = std::make_shared<NativeRecorderErrorCallback>(callback, userData);
        return errorCallback_ != nullptr;
    }

private:
    std::shared_mutex mutex_;
    OH_AVRecorder *recorder_ = nullptr;
    std::shared_ptr<NativeRecorderStateChangeCallback> stateChangeCallback_ = nullptr;
    std::shared_ptr<NativeRecorderErrorCallback> errorCallback_ = nullptr;
};

OH_AVRecorder *OH_AVRecorder_Create(void)
{
    std::shared_ptr<Recorder> recorder = RecorderFactory::CreateRecorder();
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, nullptr, "failed to create recorder in RecorderFactory.");
    struct RecorderObject *recorderObj = new (std::nothrow) RecorderObject(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, nullptr, "failed to new RecorderObject");
    MEDIA_LOGI("0x%{public}06" PRIXPTR " OH_Recorder_Create", FAKE_POINTER(recorderObj));
    return recorderObj;
}

OH_AVErrCode SetProfile(OH_AVRecorder *recorder, OH_AVRecorder_Config *config)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    OutputFormatType outputFormat = static_cast<OutputFormatType>(config->profile.fileFormat);
    int32_t ret = recorderObj->recorder_->SetOutputFormat(outputFormat);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set output format failed!");

    ret = recorderObj->recorder_->SetAudioEncodingBitRate(recorderObj->audioSourceId_, config->profile.audioBitrate);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set audio encoding bitrate failed!");

    ret = recorderObj->recorder_->SetAudioChannels(recorderObj->audioSourceId_, config->profile.audioChannels);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set audio channels failed!");

    AudioCodecFormat audioCodec_ = static_cast<AudioCodecFormat>(config->profile.audioCodec);
    ret = recorderObj->recorder_->SetAudioEncoder(recorderObj->audioSourceId_, audioCodec_);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set audio encoder failed!");

    ret = recorderObj->recorder_->SetAudioSampleRate(recorderObj->audioSourceId_, config->profile.audioSampleRate);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set audio sample rate failed!");

    ret = recorderObj->recorder_->SetVideoEncodingBitRate(recorderObj->videoSourceId_, config->profile.videoBitrate);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set video encoding bitrate failed!");

    VideoCodecFormat videoCodec_ = static_cast<VideoCodecFormat>(config->profile.videoCodec);
    ret = recorderObj->recorder_->SetVideoEncoder(recorderObj->videoSourceId_, videoCodec_);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set video encoder failed!");

    ret = recorderObj->recorder_->SetVideoSize(recorderObj->videoSourceId_, config->profile.videoFrameWidth,
        config->profile.videoFrameHeight);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set video size failed!");

    ret = recorderObj->recorder_->SetVideoFrameRate(recorderObj->videoSourceId_, config->profile.videoFrameRate);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set video frame rate failed!");

    config->profile.isHdr = false;
    ret = recorderObj->recorder_->SetVideoIsHdr(recorderObj->videoSourceId_, config->profile.isHdr);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "Set video IsHdr failed!");

    config->profile.enableTemporalScale = false;
    ret = recorderObj->recorder_->SetVideoEnableTemporalScale(recorderObj->videoSourceId_,
        config->profile.enableTemporalScale);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "SetVideoEnableTemporalScale failed!");
    
    return AV_ERR_OK;
}

OH_AVErrCode ConfigureUrl(OH_AVRecorder *recorder, OH_AVRecorder_Config *config)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    int32_t ret;
    FileGenerationMode fileGenerationMode = static_cast<FileGenerationMode>(config->fileGenerationMode);
    std::string url = std::string(config->url);
    if (fileGenerationMode == FileGenerationMode::AUTO_CREATE_CAMERA_SCENE) {
        ret = recorderObj->recorder_->SetFileGenerationMode(fileGenerationMode);
        CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "SetFileGenerationMode failed!");
    } else {
        ret = MSERR_PARAMETER_VERIFICATION_FAILED;
        const std::string fdHead = "fd://";
        CHECK_AND_RETURN_RET_LOG(url.find(fdHead) != std::string::npos, AV_ERR_INVALID_VAL,
            "url wrong: missing 'fd://' prefix!");

        int32_t fd = -1;
        std::string inputFd = url.substr(fdHead.size());
        CHECK_AND_RETURN_RET_LOG(StrToInt(inputFd, fd) == true && fd >= 0, AV_ERR_INVALID_VAL,
            "url wrong: invalid file descriptor in url!");

        ret = recorderObj->recorder_->SetOutputFile(fd);
        CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "SetOutputFile failed!");
    }

    return AV_ERR_OK;
}

OH_AVErrCode Configure(OH_AVRecorder *recorder, OH_AVRecorder_Config *config)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    if (recorderObj->hasConfigured_) {
        MEDIA_LOGE("OH_AVRecorder_Config has been configured and will not be configured again.");
        return AV_ERR_OK;
    }

    AudioSourceType audioSourceType_ = static_cast<AudioSourceType>(config->audioSourceType);
    int32_t ret = recorderObj->recorder_->SetAudioSource(audioSourceType_, recorderObj->audioSourceId_);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL,
        "The audio parameter is not supported. Please check the type and range.");

    VideoSourceType videoSourceType_ = static_cast<VideoSourceType>(config->videoSourceType);
    ret = recorderObj->recorder_->SetVideoSource(videoSourceType_, recorderObj->videoSourceId_);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL,
        "The video parameter is not supported. Please check the type and range.");

    OH_AVErrCode err = SetProfile(recorder, config);
    CHECK_AND_RETURN_RET_LOG(err == AV_ERR_OK, AV_ERR_INVALID_VAL, "SetProfile failed!");

    int32_t videoOrientation = static_cast<int32_t>(std::stoi(config->metadata.videoOrientation));
    recorderObj->recorder_->SetOrientationHint(videoOrientation);
    recorderObj->recorder_->SetLocation(config->metadata.location.latitude, config->metadata.location.longitude);

    err = ConfigureUrl(recorder, config);
    CHECK_AND_RETURN_RET_LOG(err == AV_ERR_OK, AV_ERR_INVALID_VAL, "ConfigureUrl failed!");

    recorderObj->hasConfigured_ = true;
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_Prepare(OH_AVRecorder *recorder, OH_AVRecorder_Config *config)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    CHECK_AND_RETURN_RET_LOG(config != nullptr, AV_ERR_INVALID_VAL, "input config is nullptr!");

    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");
    
    int32_t ret = Configure(recorder, config);
    ret = recorderObj->recorder_->Prepare();
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "recorder Prepare failed");

    CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL,
        "recorderObj->callback_ is nullptr!");
    MEDIA_LOGI("Change state to state code: %{public}d", OH_AVRecorder_State::PREPARED);
    recorderObj->callback_->OnStateChange(OH_AVRecorder_State::PREPARED, OH_AVRecorder_StateChangeReason::USER);
    return AV_ERR_OK;
}

Location ConvertToLocation(const OH_AVRecorder_Location &ohLocation)
{
    Location location;
    location.latitude = static_cast<float>(ohLocation.latitude);
    location.longitude = static_cast<float>(ohLocation.longitude);
    return location;
}

OH_AVErrCode OH_AVRecorder_GetAVRecorderConfig(OH_AVRecorder *recorder, OH_AVRecorder_Config **config)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    CHECK_AND_RETURN_RET_LOG(config != nullptr, AV_ERR_INVALID_VAL, "input config pointer is nullptr!");
    CHECK_AND_RETURN_RET_LOG(*config != nullptr, AV_ERR_INVALID_VAL, "input config is nullptr!");

    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    ConfigMap configMap;
    recorderObj->recorder_->GetAVRecorderConfig(configMap);

    OH_AVRecorder_Location ohlocation;
    Location location = ConvertToLocation(ohlocation);
    recorderObj->recorder_->GetLocation(location);

    (*config)->profile.audioBitrate = configMap["audioBitrate"];
    (*config)->profile.audioChannels = configMap["audioChannels"];
    (*config)->profile.audioCodec = static_cast<OH_AVRecorder_CodecMimeType>(configMap["audioCodec"]);
    (*config)->profile.audioSampleRate = configMap["audioSampleRate"];
    (*config)->profile.fileFormat = static_cast<OH_AVRecorder_ContainerFormatType>(configMap["fileFormat"]);
    (*config)->profile.videoBitrate = configMap["videoBitrate"];
    (*config)->profile.videoCodec = static_cast<OH_AVRecorder_CodecMimeType>(configMap["videoCodec"]);
    (*config)->profile.videoFrameHeight = configMap["videoFrameHeight"];
    (*config)->profile.videoFrameWidth = configMap["videoFrameWidth"];
    (*config)->profile.videoFrameRate = configMap["videoFrameRate"];

    (*config)->audioSourceType = static_cast<OH_AVRecorder_AudioSourceType>(configMap["audioSourceType"]);
    (*config)->videoSourceType = static_cast<OH_AVRecorder_VideoSourceType>(configMap["videoSourceType"]);

    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_GetInputSurface(OH_AVRecorder *recorder, OHNativeWindow **window)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "recorder is nullptr!");
    CHECK_AND_RETURN_RET_LOG(window != nullptr, AV_ERR_INVALID_VAL, "window pointer is nullptr!");

    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    sptr<OHOS::Surface> surface = recorderObj->recorder_->GetSurface(recorderObj->videoSourceId_);
    CHECK_AND_RETURN_RET_LOG(surface != nullptr, AV_ERR_INVALID_VAL, "surface is nullptr!");

    (*window) = CreateNativeWindowFromSurface(&surface);
    CHECK_AND_RETURN_RET_LOG(*window != nullptr, AV_ERR_INVALID_VAL, "*window pointer is nullptr!");
    CHECK_AND_RETURN_RET_LOG((*window)->surface != nullptr, AV_ERR_INVALID_VAL, "failed to get surface from recorder");

    SurfaceError error = SurfaceUtils::GetInstance()->Add((*window)->surface->GetUniqueId(), (*window)->surface);
    CHECK_AND_RETURN_RET_LOG(error == SURFACE_ERROR_OK, AV_ERR_INVALID_VAL, "failed to AddSurface");

    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_UpdateRotation(OH_AVRecorder *recorder, int32_t rotation)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");
    CHECK_AND_RETURN_RET_LOG(rotation == ROTATION_0 || rotation == ROTATION_90 || rotation == ROTATION_180 ||
        rotation == ROTATION_270, AV_ERR_INVALID_VAL, "Invalid rotation value. Must be 0, 90, 180, or 270.");
    recorderObj->recorder_->SetOrientationHint(rotation);
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_Start(OH_AVRecorder *recorder)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    int32_t ret = recorderObj->recorder_->Start();
    CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL,
        "recorderObj->callback_ is nullptr!");

    if (ret != MSERR_OK) {
        MEDIA_LOGI("Change state to state code: %{public}d", OH_AVRecorder_State::ERROR);
        recorderObj->callback_->OnStateChange(OH_AVRecorder_State::ERROR, OH_AVRecorder_StateChangeReason::USER);
    }
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_OPERATE_NOT_PERMIT, "StartRecorder failed!");

    MEDIA_LOGI("Change state to state code: %{public}d", OH_AVRecorder_State::STARTED);
    recorderObj->callback_->OnStateChange(OH_AVRecorder_State::STARTED, OH_AVRecorder_StateChangeReason::USER);
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_Pause(OH_AVRecorder *recorder)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    int32_t ret = recorderObj->recorder_->Pause();
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "recorder Pause failed");

    CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL,
        "recorderObj->callback_ is nullptr!");
    MEDIA_LOGI("Change state to state code: %{public}d", OH_AVRecorder_State::PAUSED);
    recorderObj->callback_->OnStateChange(OH_AVRecorder_State::PAUSED, OH_AVRecorder_StateChangeReason::USER);
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_Resume(OH_AVRecorder *recorder)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    int32_t ret = recorderObj->recorder_->Resume();
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "recorder Resume failed");

    CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL,
        "recorderObj->callback_ is nullptr!");
    MEDIA_LOGI("Change state to state code: %{public}d", OH_AVRecorder_State::STARTED);
    recorderObj->callback_->OnStateChange(OH_AVRecorder_State::STARTED, OH_AVRecorder_StateChangeReason::USER);
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_Stop(OH_AVRecorder *recorder)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    bool block = false;
    int32_t ret = recorderObj->recorder_->Stop(block);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "recorder Stop failed");

    CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL,
        "recorderObj->callback_ is nullptr!");
    MEDIA_LOGI("Change state to state code: %{public}d", OH_AVRecorder_State::STOPPED);
    recorderObj->callback_->OnStateChange(OH_AVRecorder_State::STOPPED, OH_AVRecorder_StateChangeReason::USER);

    recorderObj->hasConfigured_ = false;
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_Reset(OH_AVRecorder *recorder)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    int32_t ret = recorderObj->recorder_->Reset();
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "recorder Reset failed");

    CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL,
        "recorderObj->callback_ is nullptr!");
    MEDIA_LOGI("Change state to state code: %{public}d", OH_AVRecorder_State::IDLE);
    recorderObj->callback_->OnStateChange(OH_AVRecorder_State::IDLE, OH_AVRecorder_StateChangeReason::USER);

    recorderObj->hasConfigured_ = false;
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_Release(OH_AVRecorder *recorder)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");

    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    CHECK_AND_RETURN_RET_LOG(!recorderObj->isReleased_.load(), AV_ERR_OK, "recorder already isReleased");

    int32_t ret = recorderObj->recorder_->Release();
    recorderObj->isReleased_.store(true);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "recorder Release failed");
    MEDIA_LOGI("0x%{public}06" PRIXPTR " OH_AVRecorder_Release", FAKE_POINTER(recorderObj));

    CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL,
        "recorderObj->callback_ is nullptr!");
    MEDIA_LOGI("Change state to state code: %{public}d", OH_AVRecorder_State::RELEASED);
    recorderObj->callback_->OnStateChange(OH_AVRecorder_State::RELEASED, OH_AVRecorder_StateChangeReason::USER);
    
    recorderObj->hasConfigured_ = false;
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_GetAvailableEncoder(OH_AVRecorder *recorder,
    OH_AVRecorder_EncoderInfo **info, int32_t *length)
{
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    CHECK_AND_RETURN_RET_LOG(info != nullptr, AV_ERR_INVALID_VAL, "input info is nullptr!");

    struct RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");
    
    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_SetStateCallback(OH_AVRecorder *recorder,
    OH_AVRecorder_OnStateChange callback, void *userData)
{
    MEDIA_LOGD("OH_AVRecorder_SetStateCallback Start");
    
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    CHECK_AND_RETURN_RET_LOG(callback != nullptr, AV_ERR_INVALID_VAL, "input stateCallback is nullptr!");

    RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    if (recorderObj->callback_ == nullptr) {
        recorderObj->callback_ = std::make_shared<NativeRecorderCallback>(recorder);
        CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL, "callback_ is nullptr!");
        int32_t ret = recorderObj->recorder_->SetRecorderCallback(recorderObj->callback_);
        CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "SetRecorderCallback failed!");
    }
    
    if (recorderObj->callback_ == nullptr || !recorderObj->callback_->SetStateChangeCallback(callback, userData)) {
        MEDIA_LOGE("OH_AVRecorder_SetStateCallback error");
        return AV_ERR_NO_MEMORY;
    }

    MEDIA_LOGD("OH_AVRecorder_SetStateCallback End");

    return AV_ERR_OK;
}

OH_AVErrCode OH_AVRecorder_SetErrorCallback(OH_AVRecorder *recorder, OH_AVRecorder_OnError callback, void *userData)
{
    MEDIA_LOGD("OH_AVRecorder_SetErrorCallback Start");
    
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, AV_ERR_INVALID_VAL, "input recorder is nullptr!");
    CHECK_AND_RETURN_RET_LOG(callback != nullptr, AV_ERR_INVALID_VAL, "input errorCallback is nullptr!");

    RecorderObject *recorderObj = reinterpret_cast<RecorderObject *>(recorder);
    CHECK_AND_RETURN_RET_LOG(recorderObj != nullptr, AV_ERR_INVALID_VAL, "recorderObj is nullptr");
    CHECK_AND_RETURN_RET_LOG(recorderObj->recorder_ != nullptr, AV_ERR_INVALID_VAL, "recorder_ is null");

    if (recorderObj->callback_ == nullptr) {
        recorderObj->callback_ = std::make_shared<NativeRecorderCallback>(recorder);
        CHECK_AND_RETURN_RET_LOG(recorderObj->callback_ != nullptr, AV_ERR_INVALID_VAL, "callback_ is nullptr!");
        int32_t ret = recorderObj->recorder_->SetRecorderCallback(recorderObj->callback_);
        CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, AV_ERR_INVALID_VAL, "SetRecorderCallback failed!");
    }

    if (recorderObj->callback_ == nullptr || !recorderObj->callback_->SetErrorCallback(callback, userData)) {
        MEDIA_LOGE("OH_AVRecorder_SetErrorCallback error");
        return AV_ERR_NO_MEMORY;
    }

    MEDIA_LOGD("OH_AVRecorder_SetErrorCallback End");

    return AV_ERR_OK;
}
