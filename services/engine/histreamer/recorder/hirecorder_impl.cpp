/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy  of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hirecorder_impl.h"

#include "meta/audio_types.h"

namespace OHOS {
namespace Media {
    class RecorderEventReceiver : public Pipeline::EventReceiver {
    public:
        explicit (HiRecorderImpl *hiRecorderImpl)
        {
            hiRecorderImpl_ = hiRecorderImpl;
        }

        void OnEvent(const Event &event)
        {
            hiRecorderImpl_->OnEvent(event);
        }

    private:
        HiRecorderImpl *hiRecorderImpl_;
    };

    class RecorderFilterCallback : public Pipeline::FilterCallback {
    public:
        explicit RecorderFilterCallback(HiRecorderImpl *hiRecorderImpl)
        {
            hiRecorderImpl_ = hiRecorderImpl;
        }

        void OnCallback(const std::shared_ptr<Pipeline::Filter>& filter, Pipeline::FilterCallBackCommand cmd,
            Pipeline::StreamType outType)
        {
            hiRecorderImpl_->OnCallback(filter, cmd, outType);
        }

    private:
        HiRecorderImpl *hiRecorderImpl_;
    };

    HiRecorderImpl::HiRecorderImpl(int32_t appUid, int32_t appPid, uint32_t appTokenId, uint64_t appFullTokenId)
        : appUid_(appUid), appPid_(appPid), appTokenId_(appTokenId), appFullTokenId_(appFullTokenId)
    {
        pipeline_ = std::make_shared<Pipeline::Pipeline>();
    }

    HiRecorderImpl::~HiRecorderImpl() {}

    int32_t HiRecorderImpl::Init()
    {
        MEDIA_LOG_I("Init enter.");
        recorderEventReceiver_ = std::make_shared<RecorderEventReceiver>(this);
        recorderCallback_ = std::make_shared<RecorderFilterCallback>(this);
        pipeline_->Init(recorderEventReceiver_, recorderCallback_);
        return (int32_t)Status::OK;
    }

    int32_t HiRecorderImpl::SetVideoSource(VideoSourceType source, int32_t &sourceId)
    {
        MEDIA_LOG_I("SetVideoSource enter.");
        sourceId = INVALID_SOURCE_ID;
        FALSE_RETURN_V(source != VideoSourceType::VIDEO_SOURCE_BUTT,
                       (int32_t)Status::ERROR_INVALID_PARAMETER);
        FALSE_RETURN_V(videoCount_ < static_cast<int32_t>(VIDEO_SOURCE_MAX_COUNT),
                       (int32_t)Status::ERROR_INVALID_OPERATION);
        auto tempSourceId = SourceIdGenerator::GenerateVideoSourceId(videoCount_);

        videoEncoderFilter_ = Pipeline::FilterFactory::Instance().CreateFilter<Pipeline::EncoderFilter>
            ("videoEncoderFilter", Pipeline::FilterType::FILTERTYPE_VENC);
        
        Status ret = pipeline_->AddHeadFilters({videoEncoderFilter_});
        FALSE_RETURN_V_MSG_E(ret == Status::OK, (int32_t)ret, "AddFilters videoEncoder to pipeline fail");
        if (ret == Status::OK) {
            videoCount_++;
            videoSourceId_ = tempSourceId;
            sourceId = videoSourceId_;
            OnStateChanged(StateId::RECORDING_SETTING);
        }
        return (int32_t)ret;
    }

    int32_t HiRecorderImpl::SetAudioSource(AudioSourceType source, int32_t &sourceId)
    {
        MEDIA_LOG_I("SetAudioSource enter.");
        sourceId = INVALID_SOURCE_ID;
        FALSE_RETURN_V(source != AudioSourceType::AUDIO_SOURCE_INVALID,
                       (int32_t)Status::ERROR_INVALID_PARAMETER);
        FALSE_RETURN_V(audioCount_ < static_cast<int32_t>(AUDIO_SOURCE_MAX_COUNT),
                       (int32_t)Status::ERROR_INVALID_OPERATION);
        auto tempSourceId = SourceIdGenerator::GenerateAudioSourceId(audioCount_);

        audioCaptureFilter_ = Pipeline::FilterFactory::Instance().CreateFilter<Pipeline::AudioCaptureFilter>
            ("audioCaptureFilter", Pipeline::FilterType::FILTERTYPE_SOURCE);
        if (audioCaptureFilter_ == nullptr) {
            MEDIA_LOG_E("HiRecorderImpl::audioCaptureFilter_ == nullptr");
        }
        Status ret = pipeline_->AddHeadFilters({audioCaptureFilter_});
        FALSE_RETURN_V_MSG_E(ret == Status::OK, (int32_t)ret, "AddFilters audioCapture to pipeline fail");
        if (ret == Status::OK) {
            audioCount_++;
            audioSourceId_ = tempSourceId;
            sourceId = static_cast<int32_t>(audioSourceId_);
            OnStateChanged(StateId::RECORDING_SETTING);
        }
        return (int32_t)ret;
    }

    int32_t HiRecorderImpl::SetOutputFormat(OutputFormatType format)
    {
        MEDIA_LOG_I("SetOutputFormat enter. " PUBLIC_LOG_D32, static_cast<int32_t>(format));
        outputFormatType_ = format;
        OnStateChanged(StateId::RECORDING_SETTING);
        return (int32_t)Status::OK;
    }

    int32_t HiRecorderImpl::SetObs(const std::weak_ptr<IRecorderEngineObs> &obs)
    {
        MEDIA_LOG_I("SetObs enter.");
        obs_ = obs;
        return (int32_t)Status::OK;
    }

    int32_t HiRecorderImpl::Configure(int32_t sourceId, const RecorderParam &recParam)
    {
        MEDIA_LOG_I("Configure enter.");
        FALSE_RETURN_V(outputFormatType_ != OutputFormatType::FORMAT_BUTT,
                       (int32_t)Status::ERROR_INVALID_OPERATION);
        FALSE_RETURN_V(CheckParamType(sourceId, recParam), (int32_t)Status::ERROR_INVALID_PARAMETER);
        switch (recParam.type) {
            case RecorderPublicParamType::AUD_SAMPLERATE:
            case RecorderPublicParamType::AUD_CHANNEL:
            case RecorderPublicParamType::AUD_BITRATE:
            case RecorderPublicParamType::AUD_ENC_FMT:
                ConfigureAudio(recParam);
                break;
            case RecorderPublicParamType::VID_CAPTURERATE:
            case RecorderPublicParamType::VID_RECTANGLE:
            case RecorderPublicParamType::VID_BITRATE:
            case RecorderPublicParamType::VID_FRAMERATE:
            case RecorderPublicParamType::VID_ENC_FMT:
                ConfigureVideo(recParam);
                break;
            case RecorderPublicParamType::OUT_PATH:
            case RecorderPublicParamType::OUT_FD:
            case RecorderPublicParamType::VID_ORIENTATION_HINT:
            case RecorderPublicParamType::GEO_LOCATION:
                ConfigureMuxer(recParam);
                break;
            default:
                break;
        }
        OnStateChanged(StateId::RECORDING_SETTING);
        return (int32_t)Status::OK;
    }

    sptr<Surface> HiRecorderImpl::GetSurface(int32_t sourceId)
    {
        MEDIA_LOG_I("GetSurface enter.");
        return videoEncoderFilter_->GetInputSurface();
    }

    int32_t HiRecorderImpl::Prepare()
    {
        MEDIA_LOG_I("Prepare enter.");
        if (audioCaptureFilter_) {
            audioCaptureFilter_->Init(recorderEventReceiver_, recorderCallback_);
            audioEncFormat_->Set<Tag::APP_TOKEN_ID>(appTokenId_);
            audioEncFormat_->Set<Tag::APP_UID>(appUid_);
            audioEncFormat_->Set<Tag::APP_PID>(appPid_);
            audioEncFormat_->Set<Tag::APP_FULL_TOKEN_ID>(appFullTokenId_);
            audioCaptureFilter_->SetParameter(audioEncFormat_);
        }
        if (videoEncoderFilter_) {
            videoEncoderFilter_->SetCodecFormat(videoEncFormat_);
            videoEncoderFilter_->Init(recorderEventReceiver_, recorderCallback_);
            videoEncoderFilter_->Configure(videoEncFormat_);
        }
        Status ret = pipeline_->Prepare();
        if (ret != Status::OK) {
            return (int32_t)ret;
        }
        return (int32_t)ret;
    }

    int32_t HiRecorderImpl::Start()
    {
        MEDIA_LOG_I("Start enter.");
        Status ret = Status::OK;
        if (curState_ == StateId::PAUSE) {
            ret = pipeline_->Resume();
        } else {
            ret = pipeline_->Start();
        }
        if (ret == Status::OK) {
            OnStateChanged(StateId::RECORDING);
        }
        return (int32_t)ret;
    }

    int32_t HiRecorderImpl::Pause()
    {
        MEDIA_LOG_I("Pause enter.");
        Status ret = Status::OK;
        if (curState_ != StateId::READY) {
            ret = pipeline_->Pause();
        }
        if (ret == Status::OK) {
            OnStateChanged(StateId::PAUSE);
        }
        return (int32_t)ret;
    }

    int32_t HiRecorderImpl::Resume()
    {
        MEDIA_LOG_I("Resume enter.");
        Status ret = Status::OK;
        ret = pipeline_->Resume();
        if (ret == Status::OK) {
            OnStateChanged(StateId::RECORDING);
        }
        return (int32_t)ret;
    }

    int32_t HiRecorderImpl::Stop(bool isDrainAll)
    {
        MEDIA_LOG_I("Stop enter.");
        Status ret = Status::OK;
        if (curState_ == StateId::INIT || curState_ == StateId::READY) {
            return (int32_t)Status::ERROR_INVALID_OPERATION;
        }
        outputFormatType_ = OutputFormatType::FORMAT_BUTT;
        ret = audioCaptureFilter_->SendEos();
        ret = pipeline_->Stop();
        if (ret == Status::OK) {
            OnStateChanged(StateId::INIT);
        }
        audioCount_ = 0;
        videoCount_ = 0;
        audioSourceId_ = 0;
        videoSourceId_ = 0;
        FALSE_RETURN_V_MSG_E(curState_ == StateId::INIT, ERR_UNKNOWN_REASON, "stop fail");
        return (int32_t)ret;
    }

    int32_t HiRecorderImpl::Reset()
    {
        MEDIA_LOG_I("Reset enter.");
        Status ret = Status::OK;
        if (curState_ == StateId::RECORDING) {
            Stop(false);
        }
        ret = pipeline_->Stop();
        if (ret == Status::OK) {
            OnStateChanged(StateId::INIT);
        }
        audioCount_ = 0;
        videoCount_ = 0;
        audioSourceId_ = 0;
        videoSourceId_ = 0;
        return (int32_t)ret;
    }

    int32_t HiRecorderImpl::SetParameter(int32_t sourceId, const RecorderParam &recParam)
    {
        MEDIA_LOG_I("SetParameter enter.");
        return Configure(sourceId, recParam);
    }

    void HiRecorderImpl::OnEvent(const Event &event)
    {
        switch (event.type) {
            case EventType::EVENT_ERROR: {
                OnStateChanged(StateId::ERROR);
                auto ptr = obs_.lock();
                if (ptr != nullptr) {
                    MEDIA_LOG_I("OnEvent EVENT_ERROR obs is nullptr.");
                }
                break;
            }
            case EventType::EVENT_READY: {
                OnStateChanged(StateId::READY);
                break;
            }
            case EventType::EVENT_COMPLETE: {
                break;
            }
            default:
                break;
        }
    }

    void HiRecorderImpl::OnCallback(std::shared_ptr<Pipeline::Filter> filter, const Pipeline::FilterCallBackCommand cmd,
        Pipeline::StreamType outType)
    {
        MEDIA_LOG_I("OnCallback enter.");
        if (cmd == Pipeline::FilterCallBackCommand::NEXT_FILTER_NEEDED) {
            switch (outType) {
                case Pipeline::StreamType::STREAMTYPE_RAW_AUDIO:
                    audioEncoderFilter_ = Pipeline::FilterFactory::Instance().CreateFilter<Pipeline::EncoderFilter>
                        ("audioEncoderFilter", Pipeline::FilterType::FILTERTYPE_AENC);
                    audioEncoderFilter_->SetCodecFormat(audioEncFormat_);
                    audioEncoderFilter_->Init(recorderEventReceiver_, recorderCallback_);
                    audioEncoderFilter_->Configure(audioEncFormat_);
                    pipeline_->LinkFilters(filter, {audioEncoderFilter_}, outType);
                    break;
                case Pipeline::StreamType::STREAMTYPE_ENCODED_AUDIO:
                    if (muxerFilter_ == nullptr) {
                        muxerFilter_ = Pipeline::FilterFactory::Instance().CreateFilter<Pipeline::MuxerFilter>
                            ("muxerFilter", Pipeline::FilterType::FILTERTYPE_MUXER);
                        muxerFilter_->Init(recorderEventReceiver_, recorderCallback_);
                        muxerFilter_->SetOutputParameter(appUid_, appPid_, fd_, outputFormatType_);
                        close(fd_);
                        fd_ = -1;
                    }
                    pipeline_->LinkFilters(filter, {muxerFilter_}, outType);
                    break;
                case Pipeline::StreamType::STREAMTYPE_ENCODED_VIDEO:
                    if (muxerFilter_ == nullptr) {
                        muxerFilter_ = Pipeline::FilterFactory::Instance().CreateFilter<Pipeline::MuxerFilter>
                            ("muxerFilter", Pipeline::FilterType::FILTERTYPE_MUXER);
                        muxerFilter_->Init(recorderEventReceiver_, recorderCallback_);
                        muxerFilter_->SetOutputParameter(appUid_, appPid_, fd_, outputFormatType_);
                        close(fd_);
                        fd_ = -1;
                    }
                    pipeline_->LinkFilters(filter, {muxerFilter_}, outType);
                    break;
                default:
                    break;
            }
        }
    }

    void HiRecorderImpl::ConfigureAudio(const RecorderParam &recParam)
    {
        MEDIA_LOG_I("ConfigureAudio enter.");
        switch (recParam.type) {
            case RecorderPublicParamType::AUD_SAMPLERATE: {
                AudSampleRate audSampleRate = static_cast<const AudSampleRate&>(recParam);
                audioEncFormat_->Set<Tag::AUDIO_SAMPLE_RATE>(audSampleRate.sampleRate);
                break;
            }
            case RecorderPublicParamType::AUD_CHANNEL: {
                AudChannel audChannel = static_cast<const AudChannel&>(recParam);
                audioEncFormat_->Set<Tag::AUDIO_CHANNEL_COUNT>(audChannel.channel);
                break;
            }
            case RecorderPublicParamType::AUD_BITRATE: {
                AudBitRate audBitRate = static_cast<const AudBitRate&>(recParam);
                audioEncFormat_->Set<Tag::MEDIA_BITRATE>(audBitRate.bitRate);
                break;
            }
            case RecorderPublicParamType::AUD_ENC_FMT: {
                AudEnc audEnc = static_cast<const AudEnc&>(recParam);
                switch (audEnc.encFmt) {
                    case OHOS::Media::AudioCodecFormat::AUDIO_DEFAULT:
                    case OHOS::Media::AudioCodecFormat::AAC_LC:
                        audioEncFormat_->Set<Tag::MIME_TYPE>(Plugin::MimeType::AUDIO_AAC);
                        audioEncFormat_->Set<Tag::AUDIO_AAC_PROFILE>(Plugin::AudioAacProfile::LC);
                    default:
                        break;
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    void HiRecorderImpl::ConfigureVideo(const RecorderParam &recParam)
    {
        MEDIA_LOG_I("ConfigureVideo enter.");
        switch (recParam.type) {
            case RecorderPublicParamType::VID_RECTANGLE: {
                VidRectangle vidRectangle = static_cast<const VidRectangle&>(recParam);
                videoEncFormat_->Set<Tag::VIDEO_WIDTH>(vidRectangle.width);
                videoEncFormat_->Set<Tag::VIDEO_HEIGHT>(vidRectangle.height);
                break;
            }
            case RecorderPublicParamType::VID_CAPTURERATE: {
                CaptureRate captureRate = static_cast<const CaptureRate&>(recParam);
                videoEncFormat_->Set<Tag::VIDEO_CAPTURE_RATE>(captureRate.capRate);
                break;
            }
            case RecorderPublicParamType::VID_BITRATE: {
                VidBitRate vidBitRate = static_cast<const VidBitRate&>(recParam);
                videoEncFormat_->Set<Tag::MEDIA_BITRATE>(vidBitRate.bitRate);
                break;
            }
            case RecorderPublicParamType::VID_FRAMERATE: {
                VidFrameRate vidFrameRate = static_cast<const VidFrameRate&>(recParam);
                videoEncFormat_->Set<Tag::VIDEO_FRAME_RATE>(vidFrameRate.frameRate);
                break;
            }
            case RecorderPublicParamType::VID_ENC_FMT: {
                videoEncFormat_ = std::make_shared<Meta>();
                VidEnc vidEnc = static_cast<const VidEnc&>(recParam);
                switch (vidEnc.encFmt) {
                    case OHOS::Media::VideoCodecFormat::H264:
                        videoEncFormat_->Set<Tag::MIME_TYPE>(Plugin::MimeType::VIDEO_AVC);
                        videoEncFormat_->Set<Tag::VIDEO_H264_PROFILE>(Plugin::VideoH264Profile::BASELINE);
                        videoEncFormat_->Set<Tag::VIDEO_H264_LEVEL>(32); // 32: LEVEL 3.2
                        break;
                    case OHOS::Media::VideoCodecFormat::MPEG4:
                        videoEncFormat_->Set<Tag::MIME_TYPE>(Plugin::MimeType::VIDEO_MPEG4);
                        break;
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

    void HiRecorderImpl::ConfigureMuxer(const RecorderParam &recParam)
    {
        MEDIA_LOG_I("ConfigureMuxer enter");
        switch (recParam.type) {
            case RecorderPublicParamType::OUT_FD: {
                OutFd outFd = static_cast<const OutFd&>(recParam);
                fd_ = dup(outFd.fd);
                MEDIA_LOG_I("ConfigureMuxer enter " PUBLIC_LOG_D32, fd_);
                break;
            }
            case RecorderPublicParamType::MAX_DURATION: {
                MaxDuration maxDuration = static_cast<const MaxDuration&>(recParam);
                maxDuration_ = maxDuration.duration;
                break;
            }
            case RecorderPublicParamType::MAX_SIZE: {
                MaxFileSize maxFileSize = static_cast<const MaxFileSize&>(recParam);
                maxSize_ = maxFileSize.size;
                break;
            }
            case RecorderPublicParamType::VID_ORIENTATION_HINT:
            case RecorderPublicParamType::GEO_LOCATION:
            default:
                break;
        }
    }

    bool HiRecorderImpl::CheckParamType(int32_t sourceId, const RecorderParam &recParam)
    {
        FALSE_RETURN_V((SourceIdGenerator::IsAudio(sourceId) && recParam.IsAudioParam() &&
                        static_cast<int32_t>(audioSourceId_) == sourceId) ||
                       (SourceIdGenerator::IsVideo(sourceId) && recParam.IsVideoParam() &&
                        static_cast<int32_t>(videoSourceId_) == sourceId) ||
                       ((sourceId == DUMMY_SOURCE_ID) && !(recParam.IsAudioParam() || recParam.IsVideoParam())), false);
        return true;
    }

    void HiRecorderImpl::OnStateChanged(StateId state)
    {
        curState_ = state;
    }
} // namespace MEDIA
} // namespace OHOS