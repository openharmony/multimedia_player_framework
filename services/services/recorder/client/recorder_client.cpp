/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "recorder_client.h"
#include "media_log.h"
#include "media_errors.h"

namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, LOG_DOMAIN_RECORDER, "RecorderClient"};
}

namespace OHOS {
namespace Media {
std::shared_ptr<RecorderClient> RecorderClient::Create(const sptr<IStandardRecorderService> &ipcProxy)
{
    CHECK_AND_RETURN_RET_LOG(ipcProxy != nullptr, nullptr, "ipcProxy is nullptr..");

    std::shared_ptr<RecorderClient> recorder = std::make_shared<RecorderClient>(ipcProxy);
    CHECK_AND_RETURN_RET_LOG(recorder != nullptr, nullptr, "failed to new RecorderClient..");

    int32_t ret = recorder->CreateListenerObject();
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, nullptr, "failed to create listener object..");

    return recorder;
}

RecorderClient::RecorderClient(const sptr<IStandardRecorderService> &ipcProxy)
    : recorderProxy_(ipcProxy)
{
    MEDIA_LOGD("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

RecorderClient::~RecorderClient()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        (void)DisableMonitor();
        if (recorderProxy_ != nullptr) {
            (void)recorderProxy_->DestroyStub();
            recorderProxy_ = nullptr;
        }
    }
    MEDIA_LOGD("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
}

void RecorderClient::MediaServerDied()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        recorderProxy_ = nullptr;
        listenerStub_ = nullptr;
        
        CHECK_AND_RETURN(callback_ != nullptr);
        callback_->OnError(RECORDER_ERROR_INTERNAL, MSERR_SERVICE_DIED);
    }
}

int32_t RecorderClient::CreateListenerObject()
{
    std::lock_guard<std::mutex> lock(mutex_);
    listenerStub_ = new(std::nothrow) RecorderListenerStub();
    CHECK_AND_RETURN_RET_LOG(listenerStub_ != nullptr, MSERR_NO_MEMORY, "failed to new RecorderListenerStub object");
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    (void)listenerStub_->SetMonitor(weak_from_this());
    sptr<IRemoteObject> object = listenerStub_->AsObject();
    CHECK_AND_RETURN_RET_LOG(object != nullptr, MSERR_NO_MEMORY, "listener object is nullptr..");

    MEDIA_LOGD("SetListenerObject");
    return recorderProxy_->SetListenerObject(object);
}

int32_t RecorderClient::SetVideoSource(VideoSourceType source, int32_t &sourceId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetVideoSource source(%{public}d), sourceId(%{public}d)", source, sourceId);
    return recorderProxy_->SetVideoSource(source, sourceId);
}

int32_t RecorderClient::SetVideoEncoder(int32_t sourceId, VideoCodecFormat encoder)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetVideoSource sourceId(%{public}d), encoder(%{public}d)", sourceId, encoder);
    return recorderProxy_->SetVideoEncoder(sourceId, encoder);
}

int32_t RecorderClient::SetVideoSize(int32_t sourceId, int32_t width, int32_t height)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetVideoSize sourceId(%{public}d), width(%{public}d), height(%{public}d)", sourceId, width, height);
    return recorderProxy_->SetVideoSize(sourceId, width, height);
}

int32_t RecorderClient::SetVideoFrameRate(int32_t sourceId, int32_t frameRate)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetVideoFrameRate sourceId(%{public}d), frameRate(%{public}d)", sourceId, frameRate);
    return recorderProxy_->SetVideoFrameRate(sourceId, frameRate);
}

int32_t RecorderClient::SetVideoEncodingBitRate(int32_t sourceId, int32_t rate)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetVideoEncodingBitRate sourceId(%{public}d), rate(%{public}d)", sourceId, rate);
    return recorderProxy_->SetVideoEncodingBitRate(sourceId, rate);
}

int32_t RecorderClient::SetVideoIsHdr(int32_t sourceId, bool isHdr)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetVideoIsHdr sourceId(%{public}d), isHdr(%{public}d)", sourceId, isHdr);
    return recorderProxy_->SetVideoIsHdr(sourceId, isHdr);
}

int32_t RecorderClient::SetVideoEnableTemporalScale(int32_t sourceId, bool enableTemporalScale)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetVideoEnableTemporalScale sourceId(%{public}d), enableTemporalScale(%{public}d)",
        sourceId, enableTemporalScale);
    return recorderProxy_->SetVideoEnableTemporalScale(sourceId, enableTemporalScale);
}

int32_t RecorderClient::SetVideoEnableStableQualityMode(int32_t sourceId, bool enableStableQualityMode)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");
 
    MEDIA_LOGD("SetVideoEnableStableQualityMode sourceId(%{public}d), enableStableQualityMode(%{public}d)",
        sourceId, enableStableQualityMode);
    return recorderProxy_->SetVideoEnableStableQualityMode(sourceId, enableStableQualityMode);
}

int32_t RecorderClient::SetVideoEnableBFrame(int32_t sourceId, bool enableBFrame)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");
 
    MEDIA_LOGD("SetVideoEnableBFrame sourceId(%{public}d), enableBFrame(%{public}d)", sourceId, enableBFrame);
    return recorderProxy_->SetVideoEnableBFrame(sourceId, enableBFrame);
}

int32_t RecorderClient::SetMetaSource(MetaSourceType source, int32_t &sourceId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetMetaSourceType source(%{public}d), sourceId(%{public}d)",
        source, sourceId);
    return recorderProxy_->SetMetaSource(source, sourceId);
}

int32_t RecorderClient::SetMetaConfigs(int32_t sourceId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetMetaConfigs sourceId(%{public}d)", sourceId);
    return recorderProxy_->SetMetaConfigs(sourceId);
}

int32_t RecorderClient::SetMetaMimeType(int32_t sourceId, const std::string_view &type)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetMetaMimeType sourceId(%{public}d), MetaMimeType(%{public}s)",
        sourceId, type.data());
    return recorderProxy_->SetMetaMimeType(sourceId, type);
}

int32_t RecorderClient::SetMetaTimedKey(int32_t sourceId, const std::string_view &timedKey)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetMetaTimedKey sourceId(%{public}d), MetaTimedKey(%{public}s)",
        sourceId, timedKey.data());
    return recorderProxy_->SetMetaTimedKey(sourceId, timedKey);
}

int32_t RecorderClient::SetMetaSourceTrackMime(int32_t sourceId, const std::string_view &srcTrackMime)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetMetaSourceTrackMime sourceId(%{public}d), srcTrackMime(%{public}s)",
        sourceId, srcTrackMime.data());
    return recorderProxy_->SetMetaSourceTrackMime(sourceId, srcTrackMime);
}

int32_t RecorderClient::SetCaptureRate(int32_t sourceId, double fps)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetCaptureRate sourceId(%{public}d), fps(%{public}lf)", sourceId, fps);
    return recorderProxy_->SetCaptureRate(sourceId, fps);
}

sptr<OHOS::Surface> RecorderClient::GetSurface(int32_t sourceId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, nullptr, "recorder service does not exist.");

    MEDIA_LOGD("GetSurface sourceId(%{public}d)", sourceId);
    return recorderProxy_->GetSurface(sourceId);
}

sptr<OHOS::Surface> RecorderClient::GetMetaSurface(int32_t sourceId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, nullptr, "recorder service does not exist.");

    MEDIA_LOGD("GetMetaSurface sourceId(%{public}d)", sourceId);
    return recorderProxy_->GetMetaSurface(sourceId);
}

int32_t RecorderClient::SetAudioSource(AudioSourceType source, int32_t &sourceId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetAudioSource source(%{public}d), sourceId(%{public}d)", source, sourceId);
    return recorderProxy_->SetAudioSource(source, sourceId);
}

int32_t RecorderClient::SetAudioEncoder(int32_t sourceId, AudioCodecFormat encoder)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetAudioEncoder sourceId(%{public}d), encoder(%{public}d)", sourceId, encoder);
    return recorderProxy_->SetAudioEncoder(sourceId, encoder);
}

int32_t RecorderClient::SetAudioSampleRate(int32_t sourceId, int32_t rate)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetAudioSampleRate sourceId(%{public}d), rate(%{public}d)", sourceId, rate);
    return recorderProxy_->SetAudioSampleRate(sourceId, rate);
}

int32_t RecorderClient::SetAudioChannels(int32_t sourceId, int32_t num)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetAudioChannels sourceId(%{public}d), num(%{public}d)", sourceId, num);
    return recorderProxy_->SetAudioChannels(sourceId, num);
}

int32_t RecorderClient::SetAudioEncodingBitRate(int32_t sourceId, int32_t bitRate)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetAudioEncodingBitRate sourceId(%{public}d), bitRate(%{public}d)", sourceId, bitRate);
    return recorderProxy_->SetAudioEncodingBitRate(sourceId, bitRate);
}

int32_t RecorderClient::SetDataSource(DataSourceType dataType, int32_t &sourceId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetDataSource dataType(%{public}d), sourceId(%{public}d)", dataType, sourceId);
    return recorderProxy_->SetDataSource(dataType, sourceId);
}

int32_t RecorderClient::SetUserCustomInfo(Meta &userCustomInfo)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetUserCustomInfo");
    return recorderProxy_->SetUserCustomInfo(userCustomInfo);
}

int32_t RecorderClient::SetGenre(std::string &genre)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetGenre");
    return recorderProxy_->SetGenre(genre);
}

int32_t RecorderClient::SetMaxDuration(int32_t duration)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetMaxDuration duration(%{public}d)", duration);
    return recorderProxy_->SetMaxDuration(duration);
}

int32_t RecorderClient::SetOutputFormat(OutputFormatType format)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetOutputFormat format(%{public}d)", format);
    return recorderProxy_->SetOutputFormat(format);
}

int32_t RecorderClient::SetOutputFile(int32_t fd)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetOutputFile fd(%{public}d)", fd);
    return recorderProxy_->SetOutputFile(fd);
}

int32_t RecorderClient::SetFileGenerationMode(FileGenerationMode mode)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");
 
    MEDIA_LOGD("SetFileGenerationMode FileGenerationMode(%{public}d)", static_cast<int32_t>(mode));
    return recorderProxy_->SetFileGenerationMode(mode);
}

int32_t RecorderClient::SetNextOutputFile(int32_t fd)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetNextOutputFile fd(%{public}d)", fd);
    return recorderProxy_->SetNextOutputFile(fd);
}

int32_t RecorderClient::SetMaxFileSize(int64_t size)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetMaxFileSize size(%{public}" PRId64 ")", size);
    return recorderProxy_->SetMaxFileSize(size);
}

void RecorderClient::SetLocation(float latitude, float longitude)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_LOG(recorderProxy_ != nullptr, "recorder service does not exist.");

    recorderProxy_->SetLocation(latitude, longitude);
}

void RecorderClient::SetOrientationHint(int32_t rotation)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_LOG(recorderProxy_ != nullptr, "recorder service does not exist.");

    MEDIA_LOGD ("SetLocation orientation hint: %{public}d", rotation);
    recorderProxy_->SetOrientationHint(rotation);
}

int32_t RecorderClient::SetRecorderCallback(const std::shared_ptr<RecorderCallback> &callback)
{
    CHECK_AND_RETURN_RET_LOG(callback != nullptr, MSERR_NO_MEMORY, "input param callback is nullptr.");
    CHECK_AND_RETURN_RET_LOG(listenerStub_ != nullptr, MSERR_NO_MEMORY, "listenerStub_ is nullptr.");

    callback_ = callback;
    MEDIA_LOGD("SetRecorderCallback");
    listenerStub_->SetRecorderCallback(callback);
    return MSERR_OK;
}

int32_t RecorderClient::Prepare()
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("Prepare");
    return recorderProxy_->Prepare();
}

int32_t RecorderClient::ExecuteWhen(int32_t ret, bool ok)
{
    CHECK_AND_RETURN_RET((ok && (ret == MSERR_OK)) || ((!ok) && (ret != MSERR_OK)), ret);
    (void)DisableMonitor();
    return ret;
}

int32_t RecorderClient::Start()
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("RecorderClie Start");
    (void)EnableMonitor();
    return ExecuteWhen(recorderProxy_->Start(), false);
}

int32_t RecorderClient::Pause()
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("Pause");
    return ExecuteWhen(recorderProxy_->Pause(), true);
}

int32_t RecorderClient::Resume()
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("Resume");
    (void)EnableMonitor();
    return ExecuteWhen(recorderProxy_->Resume(), false);
}

int32_t RecorderClient::Stop(bool block)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("Stop");
    return ExecuteWhen(recorderProxy_->Stop(block), true);
}

int32_t RecorderClient::Reset()
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("Reset");
    return ExecuteWhen(recorderProxy_->Reset(), true);
}

int32_t RecorderClient::Release()
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("Release");
    return ExecuteWhen(recorderProxy_->Release(), true);
}

int32_t RecorderClient::SetFileSplitDuration(FileSplitType type, int64_t timestamp, uint32_t duration)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetFileSplitDuration FileSplitType(%{public}d), timestamp(%{public}" PRId64 "), duration(%{public}u)",
        type, timestamp, duration);
    return recorderProxy_->SetFileSplitDuration(type, timestamp, duration);
}

int32_t RecorderClient::SetParameter(int32_t sourceId, const Format &format)
{
    (void)sourceId;
    (void)format;
    return MSERR_INVALID_OPERATION;
}

int32_t RecorderClient::GetAVRecorderConfig(ConfigMap &configMap)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("GetAVRecorderConfig");
    return recorderProxy_->GetAVRecorderConfig(configMap);
}

int32_t RecorderClient::GetLocation(Location &location)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("GetLocation");
    return recorderProxy_->GetLocation(location);
}

int32_t RecorderClient::GetCurrentCapturerChangeInfo(AudioRecorderChangeInfo &changeInfo)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("GetCurrentCapturerChangeInfo");
    return recorderProxy_->GetCurrentCapturerChangeInfo(changeInfo);
}

int32_t RecorderClient::GetAvailableEncoder(std::vector<EncoderCapabilityData> &encoderInfo)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("GetAvailableEncoder");
    return recorderProxy_->GetAvailableEncoder(encoderInfo);
}

int32_t RecorderClient::GetMaxAmplitude()
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("GetMaxAmplitude");
    return recorderProxy_->GetMaxAmplitude();
}

int32_t RecorderClient::IsWatermarkSupported(bool &isWatermarkSupported)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("IsWatermarkSupported");
    return recorderProxy_->IsWatermarkSupported(isWatermarkSupported);
}

int32_t RecorderClient::SetWatermark(std::shared_ptr<AVBuffer> &waterMarkBuffer)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetWatermark");
    return recorderProxy_->SetWatermark(waterMarkBuffer);
}

int32_t RecorderClient::SetUserMeta(const std::shared_ptr<Meta> &userMeta)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetUserMeta");
    return recorderProxy_->SetUserMeta(userMeta);
}

int32_t RecorderClient::SetWillMuteWhenInterrupted(bool muteWhenInterrupted)
{
    std::lock_guard<std::mutex> lock(mutex_);
    CHECK_AND_RETURN_RET_LOG(recorderProxy_ != nullptr, MSERR_NO_MEMORY, "recorder service does not exist.");

    MEDIA_LOGD("SetWillMuteWhenInterrupted");
    return recorderProxy_->SetWillMuteWhenInterrupted(muteWhenInterrupted);
}
} // namespace Media
} // namespace OHOS
