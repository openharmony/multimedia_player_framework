/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "screen_capture_capi_mock.h"

using namespace std;
using namespace OHOS;
using namespace OHOS::Media;
using namespace testing::ext;
using namespace OHOS::Media::ScreenCaptureTestParam;
std::mutex ScreenCaptureCapiMock::mutex_;
std::map<OH_AVScreenCapture *, std::shared_ptr<ScreenCaptureCallBackMock>> ScreenCaptureCapiMock::mockCbMap_;

void ScreenCaptureCapiMock::OnError(OH_AVScreenCapture *screenCapture, int32_t errorCode)
{
    std::shared_ptr<ScreenCaptureCallBackMock> mockCb = GetCallback(screenCapture);
    if (mockCb != nullptr) {
        mockCb->OnError(errorCode);
    }
}

void ScreenCaptureCapiMock::OnAudioBufferAvailable(OH_AVScreenCapture *screenCapture, bool isReady,
    OH_AudioCaptureSourceType type)
{
    std::shared_ptr<ScreenCaptureCallBackMock> mockCb = GetCallback(screenCapture);
    if (mockCb != nullptr) {
        mockCb->OnAudioBufferAvailable(isReady, static_cast<AudioCaptureSourceType>(type));
    }
}

void ScreenCaptureCapiMock::OnVideoBufferAvailable(OH_AVScreenCapture *screenCapture, bool isReady)
{
    std::shared_ptr<ScreenCaptureCallBackMock> mockCb = GetCallback(screenCapture);
    if (mockCb != nullptr) {
        mockCb->OnVideoBufferAvailable(isReady);
    }
}

OH_AVScreenCaptureConfig ScreenCaptureCapiMock::Convert(AVScreenCaptureConfig config)
{
    OH_AVScreenCaptureConfig config_;
    config_.captureMode = static_cast<OH_CaptureMode>(config.captureMode);
    config_.dataType = static_cast<OH_DataType>(config.dataType);
    config_.audioInfo.micCapInfo = {
        .audioSampleRate = config.audioInfo.micCapInfo.audioSampleRate,
        .audioChannels = config.audioInfo.micCapInfo.audioChannels,
        .audioSource = static_cast<OH_AudioCaptureSourceType>(config.audioInfo.micCapInfo.audioSource)
    };
    config_.audioInfo.innerCapInfo = {
        .audioSampleRate = config.audioInfo.innerCapInfo.audioSampleRate,
        .audioChannels = config.audioInfo.innerCapInfo.audioChannels,
        .audioSource = static_cast<OH_AudioCaptureSourceType>(config.audioInfo.innerCapInfo.audioSource)
    };
    config_.audioInfo.audioEncInfo.audioBitrate = config.audioInfo.audioEncInfo.audioBitrate;
    config_.audioInfo.audioEncInfo.audioCodecformat =
        static_cast<OH_AudioCodecFormat>(config.audioInfo.audioEncInfo.audioCodecformat);
    config_.videoInfo.videoCapInfo.displayId = config.videoInfo.videoCapInfo.displayId;
    std::list<int32_t> taskIds = config.videoInfo.videoCapInfo.taskIDs;
    if (taskIds.size() > 0) {
        int32_t *taskIds_temp = (int*)malloc(sizeof(int));
        for (std::list<int32_t>::iterator its = taskIds.begin(); its != taskIds.end(); ++its) {
            *taskIds_temp = *its;
            taskIds_temp++;
        }
        config_.videoInfo.videoCapInfo.missionIDs = taskIds_temp;
    }
    config_.videoInfo.videoCapInfo.missionIDsLen = taskIds.size();
    config_.videoInfo.videoCapInfo.videoFrameWidth = config.videoInfo.videoCapInfo.videoFrameWidth;
    config_.videoInfo.videoCapInfo.videoFrameHeight = config.videoInfo.videoCapInfo.videoFrameHeight;
    config_.videoInfo.videoCapInfo.videoSource =
        static_cast<OH_VideoSourceType>(config.videoInfo.videoCapInfo.videoSource);
    config_.videoInfo.videoEncInfo = {
        .videoCodec = static_cast<OH_VideoCodecFormat>(config_.videoInfo.videoEncInfo.videoCodec),
        .videoBitrate = static_cast<OH_VideoCodecFormat>(config_.videoInfo.videoEncInfo.videoBitrate),
        .videoFrameRate = static_cast<OH_VideoCodecFormat>(config_.videoInfo.videoEncInfo.videoFrameRate)
    };
    std::string url = config.recorderInfo.url;
    if (!(url.empty())) {
        config_.recorderInfo.url = (char*)(url.data());
        config_.recorderInfo.urlLen = url.size();
    }
    if (config.recorderInfo.fileFormat == ContainerFormatType::CFT_MPEG_4A) {
        config_.recorderInfo.fileFormat = OH_ContainerFormatType::CFT_MPEG_4A;
    } else if (config.recorderInfo.fileFormat == ContainerFormatType::CFT_MPEG_4) {
        config_.recorderInfo.fileFormat = OH_ContainerFormatType::CFT_MPEG_4;
    }
    return config_;
}

std::shared_ptr<ScreenCaptureCallBackMock> ScreenCaptureCapiMock::GetCallback(OH_AVScreenCapture *screenCapture)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (mockCbMap_.empty()) {
        return nullptr;
    }
    if (mockCbMap_.find(screenCapture) != mockCbMap_.end()) {
        return mockCbMap_.at(screenCapture);
    }
    return nullptr;
}

void ScreenCaptureCapiMock::DelCallback(OH_AVScreenCapture *screenCapture)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (mockCbMap_.empty()) {
        return;
    }
    auto it = mockCbMap_.find(screenCapture);
    if (it != mockCbMap_.end()) {
        mockCbMap_.erase(it);
    }
}

void ScreenCaptureCapiMock::SetScreenCaptureCallback(OH_AVScreenCapture *screencapture,
    std::shared_ptr<ScreenCaptureCallBackMock> cb)
{
    std::lock_guard<std::mutex> lock(mutex_);
    mockCbMap_[screencapture] = cb;
}

int32_t ScreenCaptureCapiMock::SetScreenCaptureCallback(const std::shared_ptr<ScreenCaptureCallBackMock>& cb)
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    if (cb != nullptr) {
        SetScreenCaptureCallback(screenCapture_, cb);
        struct OH_AVScreenCaptureCallback callback;
        callback.onError = ScreenCaptureCapiMock::OnError;
        callback.onAudioBufferAvailable = ScreenCaptureCapiMock::OnAudioBufferAvailable;
        callback.onVideoBufferAvailable = ScreenCaptureCapiMock::OnVideoBufferAvailable;
        return OH_AVScreenCapture_SetCallback(screenCapture_, callback);
    }
    return MSERR_INVALID_OPERATION;
}

int32_t ScreenCaptureCapiMock::StartScreenCapture()
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    return OH_AVScreenCapture_StartScreenCapture(screenCapture_);
}

int32_t ScreenCaptureCapiMock::Init(AVScreenCaptureConfig config)
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    OH_AVScreenCaptureConfig config_ = Convert(config);
    return OH_AVScreenCapture_Init(screenCapture_, config_);
}

int32_t ScreenCaptureCapiMock::StopScreenCapture()
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    return OH_AVScreenCapture_StopScreenCapture(screenCapture_);
}

int32_t ScreenCaptureCapiMock::Release()
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    DelCallback(screenCapture_);
    return OH_AVScreenCapture_Release(screenCapture_);
}

int32_t ScreenCaptureCapiMock::SetMicrophoneEnabled(bool isMicrophone)
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    return OH_AVScreenCapture_SetMicrophoneEnabled(screenCapture_, isMicrophone);
}

int32_t ScreenCaptureCapiMock::AcquireAudioBuffer(std::shared_ptr<AudioBuffer> &audioBuffer,
    AudioCaptureSourceType type)
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    OH_AudioBuffer *buffer = (OH_AudioBuffer *)malloc(sizeof(OH_AudioBuffer));
    if (buffer == nullptr) {
        return MSERR_INVALID_OPERATION;
    }
    auto t_type = static_cast<OH_AudioCaptureSourceType>(type);
    int32_t ret = OH_AVScreenCapture_AcquireAudioBuffer(screenCapture_, &buffer, t_type);
    audioBuffer =
        std::make_shared<AudioBuffer>(buffer->buf, buffer->size, buffer->timestamp,
            static_cast<AudioCaptureSourceType>(buffer->type));
    free(buffer);
    buffer = nullptr;
    return ret;
}

sptr<OHOS::SurfaceBuffer> ScreenCaptureCapiMock::AcquireVideoBuffer(int32_t &fence, int64_t &timestamp,
    OHOS::Rect &damage)
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, nullptr, "screenCapture_ == nullptr");
    OH_Rect damage_ = {
        .x = damage.x,
        .y = damage.y,
        .width = damage.w,
        .height = damage.h,
    };
    OH_NativeBuffer* buffer = OH_AVScreenCapture_AcquireVideoBuffer(screenCapture_, &fence, &timestamp, &damage_);
    sptr<OHOS::SurfaceBuffer> surfacebuffer;
    if (buffer != nullptr) {
        surfacebuffer =  OHOS::SurfaceBuffer::NativeBufferToSurfaceBuffer(buffer);
        OH_NativeBuffer_Unreference(buffer);
    }
    return surfacebuffer;
}

int32_t ScreenCaptureCapiMock::ReleaseAudioBuffer(OHOS::Media::AudioCaptureSourceType type)
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    auto t_type = static_cast<OH_AudioCaptureSourceType>(type);
    return OH_AVScreenCapture_ReleaseAudioBuffer(screenCapture_, t_type);
}

int32_t ScreenCaptureCapiMock::ReleaseVideoBuffer()
{
    UNITTEST_CHECK_AND_RETURN_RET_LOG(screenCapture_ != nullptr, MSERR_INVALID_OPERATION, "screenCapture_ == nullptr");
    return OH_AVScreenCapture_ReleaseVideoBuffer(screenCapture_);
}