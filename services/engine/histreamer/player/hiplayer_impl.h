/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd.
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

#ifndef HI_PLAYER_IMPL_H
#define HI_PLAYER_IMPL_H

#include <memory>
#include <unordered_map>

#include "audio_sink_filter.h"
#include "codec_filter.h"
#include "common/status.h"
#include "demuxer_filter.h"
#include "filter/filter.h"
#include "filter/filter_factory.h"
#include "hiplayer_callback_looper.h"
#include "i_player_engine.h"
#include "media_sync_manager.h"
#include "pipeline/pipeline.h"
#ifdef VIDEO_SUPPORT
#include "video_sink_filter.h"
#endif

namespace OHOS {
namespace Media {
using namespace Pipeline;

enum class PlayerStateId {
    IDLE = 0,
    INIT = 1,
    PREPARING = 2,
    READY = 3,
    PAUSE = 4,
    PLAYING = 5,
    STOPPED = 6,
    EOS = 7,
    ERROR = 8,
};

class HiPlayerImpl : public IPlayerEngine, public std::enable_shared_from_this<HiPlayerImpl> {
public:
    HiPlayerImpl(int32_t appUid, int32_t appPid, uint32_t appTokenId, uint64_t appFullTokenId);
    ~HiPlayerImpl() override;
    HiPlayerImpl(const HiPlayerImpl& other) = delete;
    HiPlayerImpl& operator=(const HiPlayerImpl& other) = delete;
    Status Init();
    // interface from PlayerInterface
    int32_t SetSource(const std::string& uri) override;
    int32_t SetSource(const std::shared_ptr<IMediaDataSource>& dataSrc) override;
    int32_t Prepare() override;
    int32_t PrepareAsync() override;
    int32_t Play() override;
    int32_t Pause() override;
    int32_t Stop() override;
    int32_t Reset() override;
    int32_t Seek(int32_t mSeconds, PlayerSeekMode mode) override;
    int32_t SetVolume(float leftVolume, float rightVolume) override;
    int32_t SetVideoSurface(sptr<Surface> surface) override;
    int32_t SetLooping(bool loop) override;
    int32_t SetParameter(const Format& params) override;
    int32_t SetObs(const std::weak_ptr<IPlayerEngineObs>& obs) override;
    int32_t GetCurrentTime(int32_t& currentPositionMs) override;
    int32_t GetDuration(int32_t& durationMs) override;
    int32_t SetPlaybackSpeed(PlaybackRateMode mode) override;
    int32_t GetPlaybackSpeed(PlaybackRateMode& mode) override;

    int32_t GetVideoTrackInfo(std::vector<Format>& videoTrack) override;
    int32_t GetAudioTrackInfo(std::vector<Format>& audioTrack) override;
    int32_t GetVideoWidth() override;
    int32_t GetVideoHeight() override;
    int32_t SetVideoScaleType(OHOS::Media::VideoScaleType videoScaleType) override;
    int32_t SetAudioRendererInfo(const int32_t contentType, const int32_t streamUsage,
                                 const int32_t rendererFlag) override;
    int32_t SetAudioInterruptMode(const int32_t interruptMode) override;

    // internal interfaces
    int TransStatus(Status status);
    void OnEvent(const Event &event);
    void OnStateChanged(PlayerStateId state);
    void OnCallback(std::shared_ptr<Filter> filter, const FilterCallBackCommand cmd,
                    StreamType outType);
private:
    Status DoSetSource(const std::shared_ptr<MediaSource> source);
    Status Resume();
    Status LinkAudioDecoderFilter(const std::shared_ptr<Filter>& preFilter, StreamType type);
    Status LinkAudioSinkFilter(const std::shared_ptr<Filter>& preFilter, StreamType type);
#ifdef VIDEO_SUPPORT
    Status LinkVideoDecoderFilter(const std::shared_ptr<Filter>& preFilter, StreamType type);
    Status LinkVideoSinkFilter(const std::shared_ptr<Filter>& preFilter, StreamType type);
#endif
    int32_t appUid_{0};
    int32_t appPid_{0};
    int32_t appTokenId_{0};
    int64_t appFullTokenId_{0};
    OHOS::Media::Mutex stateMutex_{};
    OHOS::Media::ConditionVariable cond_{};
    int64_t duration_{-1};
    std::atomic<bool> singleLoop_ {false};

    std::shared_ptr<EventReceiver> playerEventReceiver_;
    std::shared_ptr<FilterCallback> playerFilterCallback_;
    std::weak_ptr<Meta> sourceMeta_{};
    std::vector<std::weak_ptr<Meta>> streamMeta_{};
    std::shared_ptr<OHOS::Media::Pipeline::Pipeline> pipeline_;
    std::shared_ptr<DemuxerFilter> demuxer_;
    std::shared_ptr<CodecFilter> audioDecoder_;
    std::shared_ptr<AudioSinkFilter> audioSink_;
#ifdef VIDEO_SUPPORT
    std::shared_ptr<CodecFilter> videoDecoder_;
    std::shared_ptr<VideoSinkFilter> videoSink_;
#endif
    std::shared_ptr<MediaSyncManager> syncManager_;
    std::atomic<PlayerStateId> curState_;
    HiPlayerCallbackLooper callbackLooper_{};
    sptr<Surface> surface_ {nullptr};
    std::string url_;
    int32_t videoWidth_{0};
    int32_t videoHeight_{0};
};
} // namespace Media
} // namespace OHOS
#endif // HI_PLAYER_IMPL_H