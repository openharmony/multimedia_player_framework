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

#ifndef PLAYER_SERVICE_SERVER_H
#define PLAYER_SERVICE_SERVER_H

#include "i_player_service.h"
#include "i_player_engine.h"
#include "nocopyable.h"
#include "uri_helper.h"
#include "player_server_task_mgr.h"
#include "audio_effect.h"
#include "account_subscriber.h"
#include "os_account_manager.h"
#include "hitrace/tracechain.h"
#include "plugin/plugin_time.h"

namespace OHOS {
namespace Media {
class PlayerServerState {
public:
    explicit PlayerServerState(const std::string &name) : name_(name) {}
    virtual ~PlayerServerState() = default;

    std::string GetStateName() const;

    DISALLOW_COPY_AND_MOVE(PlayerServerState);

protected:
    virtual void StateEnter() {}
    virtual void StateExit() {}
    virtual int32_t OnMessageReceived(PlayerOnInfoType type, int32_t extra, const Format &infoBody) = 0;

    friend class PlayerServerStateMachine;

private:
    std::string name_;
};

class PlayerServerStateMachine {
public:
    PlayerServerStateMachine() = default;
    virtual ~PlayerServerStateMachine() = default;

    DISALLOW_COPY_AND_MOVE(PlayerServerStateMachine);

protected:
    int32_t HandleMessage(PlayerOnInfoType type, int32_t extra, const Format &infoBody);
    void Init(const std::shared_ptr<PlayerServerState> &state);
    void ChangeState(const std::shared_ptr<PlayerServerState> &state);
    std::shared_ptr<PlayerServerState> GetCurrState();

private:
    std::recursive_mutex recMutex_;
    std::shared_ptr<PlayerServerState> currState_ = nullptr;
};

class PlayerServer
    : public IPlayerService,
      public IPlayerEngineObs,
      public NoCopyable,
      public PlayerServerStateMachine {
public:
    static std::shared_ptr<IPlayerService> Create();
    PlayerServer();
    virtual ~PlayerServer();

    int32_t Freeze() override;
    int32_t UnFreeze() override;
    int32_t Play() override;
    int32_t Prepare() override;
    int32_t SetRenderFirstFrame(bool display) override;
    int32_t SetPlayRange(int64_t start, int64_t end) override;
    int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode) override;
    int32_t PrepareAsync() override;
    int32_t Stop() override;
    int32_t Reset() override;
    int32_t Release() override;
    int32_t Pause() override;
    int32_t SetVolume(float leftVolume, float rightVolume) override;
    int32_t SetVolumeMode(int32_t mode) override;
    int32_t Seek(int32_t mSeconds, PlayerSeekMode mode) override;
    int32_t GetCurrentTime(int32_t &currentTime) override;
    int32_t GetPlaybackPosition(int32_t &playbackPosition) override;
    int32_t GetVideoWidth() override;
    int32_t GetVideoHeight() override;
    int32_t GetVideoTrackInfo(std::vector<Format> &videoTrack) override;
    int32_t GetPlaybackInfo(Format &playbackInfo) override;
    int32_t GetAudioTrackInfo(std::vector<Format> &audioTrack) override;
    int32_t GetSubtitleTrackInfo(std::vector<Format> &subtitleTrack) override;
    int32_t GetDuration(int32_t &duration) override;
    int32_t GetApiVersion(int32_t &apiVersion) override;
    int32_t SetPlaybackSpeed(PlaybackRateMode mode) override;
    int32_t SetPlaybackRate(float rate) override;
    int32_t SetPlayerProducer(const PlayerProducer producer) override;
    int32_t SetSource(const std::string &url) override;
    int32_t SetSource(const std::shared_ptr<IMediaDataSource> &dataSrc) override;
    int32_t SetSource(int32_t fd, int64_t offset, int64_t size) override;
    int32_t AddSubSource(const std::string &url) override;
    int32_t AddSubSource(int32_t fd, int64_t offset, int64_t size) override;
    int32_t GetPlaybackSpeed(PlaybackRateMode &mode) override;
    int32_t SetMediaSource(const std::shared_ptr<AVMediaSource> &mediaSource, AVPlayStrategy strategy) override;
    int32_t SetPlaybackStrategy(AVPlayStrategy playbackStrategy) override;
    int32_t SetMediaMuted(MediaType mediaType, bool isMuted) override;
    int32_t SetSuperResolution(bool enabled) override;
    int32_t SetVideoWindowSize(int32_t width, int32_t height) override;
    bool GetInterruptState() { return isInterruptNeeded_.load(); };
#ifdef SUPPORT_VIDEO
    int32_t SetVideoSurface(sptr<Surface> surface) override;
#endif
    int32_t SetDecryptConfig(const sptr<DrmStandard::IMediaKeySessionService> &keySessionProxy,
        bool svp) override;
    bool IsPlaying() override;
    bool IsLooping() override;
    int32_t SetLooping(bool loop) override;
    int32_t SetParameter(const Format &param) override;
    int32_t SetPlayerCallback(const std::shared_ptr<PlayerCallback> &callback) override;
    virtual int32_t DumpInfo(int32_t fd);
    int32_t SelectBitRate(uint32_t bitRate) override;
    int32_t BackGroundChangeState(PlayerStates state, bool isBackGroundCb);
    int32_t SelectTrack(int32_t index, PlayerSwitchMode mode) override;
    int32_t DeselectTrack(int32_t index) override;
    int32_t GetCurrentTrack(int32_t trackType, int32_t &index) override;

    // IPlayerEngineObs override
    void OnError(PlayerErrorType errorType, int32_t errorCode) override;
    void OnErrorMessage(int32_t errorCode, const std::string &errorMsg) override;
    void OnInfo(PlayerOnInfoType type, int32_t extra, const Format &infoBody = {}) override;
    void OnSystemOperation(PlayerOnSystemOperationType type, PlayerOperationReason reason) override;
    void OnDfxInfo(const DfxEvent &event) override;
    void OnBufferingUpdate(PlayerOnInfoType type, int32_t extra, const Format &infoBody);
    void OnNotifyBufferingStart();
    void OnNotifyBufferingEnd();

    uint32_t GetMemoryUsage() override;
    void OnCommonEventReceived(const std::string &event);
    int32_t GetUserId();
    std::shared_ptr<CommonEventReceiver> GetCommonEventReceiver();
    bool IsBootCompleted();
    int32_t SetMaxAmplitudeCbStatus(bool status) override;
    int32_t SetDeviceChangeCbStatus(bool status) override;
    bool IsSeekContinuousSupported() override;
    int32_t SetSeiMessageCbStatus(bool status, const std::vector<int32_t> &payloadTypes) override;
    int32_t SetStartFrameRateOptEnabled(bool enabled) override;
    int32_t SetReopenFd(int32_t fd) override;
    int32_t EnableCameraPostprocessing() override;
    int32_t SetCameraPostprocessing(bool isOpen) override;
    int32_t EnableReportMediaProgress(bool enable) override;
    int32_t EnableReportAudioInterrupt(bool enable) override;
    int32_t ForceLoadVideo(bool status) override;

protected:
    class BaseState;
    class IdleState;
    class InitializedState;
    class PreparingState;
    class PreparedState;
    class PlayingState;
    class PausedState;
    class StoppedState;
    class PlaybackCompletedState;
    std::shared_ptr<IdleState> idleState_;
    std::shared_ptr<InitializedState> initializedState_;
    std::shared_ptr<PreparingState> preparingState_;
    std::shared_ptr<PreparedState> preparedState_;
    std::shared_ptr<PlayingState> playingState_;
    std::shared_ptr<PausedState> pausedState_;
    std::shared_ptr<StoppedState> stoppedState_;
    std::shared_ptr<PlaybackCompletedState> playbackCompletedState_;
    std::shared_ptr<CommonEventReceiver> commonEventReceiver_ = nullptr;

    std::shared_ptr<PlayerCallback> playerCb_ = nullptr;
    std::unique_ptr<IPlayerEngine> playerEngine_ = nullptr;
    bool errorCbOnce_ = false;
    bool disableStoppedCb_ = false;
    std::string lastErrMsg_;
    std::unique_ptr<UriHelper> uriHelper_ = nullptr;
    std::vector<std::shared_ptr<UriHelper>> subUriHelpers_;
    std::mutex mutex_;
    std::mutex mutexCb_;
    std::atomic<PlayerStates> lastOpStatus_ = PLAYER_IDLE;
    PlayerServerTaskMgr taskMgr_;
    bool isLiveStream_ = false;
    virtual int32_t Init();
    void ClearConfigInfo();
    bool IsPrepared();
    bool IsCompleted();
    bool IsValidSeekMode(PlayerSeekMode mode);
    void OnInfoNoChangeStatus(PlayerOnInfoType type, int32_t extra, const Format &infoBody = {});
    const std::string &GetStatusDescription(int32_t status);
    struct ConfigInfo {
        std::atomic<bool> looping = false;
        float leftVolume = INVALID_VALUE;
        float rightVolume = INVALID_VALUE;
        PlaybackRateMode speedMode = SPEED_FORWARD_1_00_X;
        float speedRate = 1.0f;
        std::string url;
        int32_t effectMode = OHOS::AudioStandard::AudioEffectMode::EFFECT_DEFAULT;
        std::map<std::string, std::string> header;
        AVPlayStrategy strategy_;
    } config_;

private:
    bool IsEngineStarted();
    int32_t InitPlayEngine(const std::string &url);
    int32_t OnPrepare(bool sync);
    int32_t OnPlay();
    int32_t OnFreeze();
    int32_t OnUnFreeze();
    int32_t OnPause(bool isSystemOperation);
    int32_t OnStop(bool sync);
    int32_t OnReset();
    int32_t HandlePrepare();
    int32_t HandlePlay();
    int32_t HandlePause(bool isSystemOperation);
    int32_t HandleFreeze();
    int32_t HandleLiteFreeze();
    int32_t HandleUnFreeze();
    int32_t HandleLiteUnFreeze();
    int32_t HandlePauseDemuxer();
    int32_t HandleResumeDemuxer();
    int32_t HandleStop();
    int32_t HandleReset();
    int32_t HandleSeek(int32_t mSeconds, PlayerSeekMode mode);
    int32_t HandleSetPlayRange(int64_t start, int64_t end, PlayerSeekMode mode);
    int32_t HandleSetPlaybackSpeed(PlaybackRateMode mode);
    int32_t HandleSetPlaybackRate(float rate);
    int32_t SetAudioEffectMode(const int32_t effectMode);
    int32_t CheckandDoUnFreeze();

    void HandleEos();
    void PreparedHandleEos();
    void HandleInterruptEvent(const Format &infoBody);
    void HandleAudioDeviceChangeEvent(const Format &infoBody);
    void OnFlvAutoSelectBitRate(uint32_t bitRate);
    void FormatToString(std::string &dumpString, std::vector<Format> &videoTrack);
    void OnErrorCb(int32_t errorCode, const std::string &errorMsg);
    void InnerOnInfo(PlayerOnInfoType type, int32_t extra, const Format &infoBody, const int32_t ret);

    int32_t CheckSeek(int32_t mSeconds, PlayerSeekMode mode);
    int32_t SeekContinous(int32_t mSeconds);
    int32_t HandleSeekContinous(int32_t mSeconds, int64_t batchNo);
    int32_t ExitSeekContinous(bool align);
    int32_t ExitSeekContinousAsync(bool align);
    void UpdateContinousBatchNo();

    bool CheckState(PlayerOnInfoType type, int32_t extra);
    void DoCheckLiveDelayTime();
    int64_t CalculatePauseTime();
    void HandleFlvLiveRestartLink();
    void TryFlvLiveRestartLink();
    void UpdateFlvLivePauseTime();

#ifdef SUPPORT_VIDEO
    sptr<Surface> surface_ = nullptr;
#endif
    std::shared_ptr<IMediaDataSource> dataSrc_ = nullptr;
    static constexpr float INVALID_VALUE = 2.0f;
    bool disableNextSeekDone_ = false;
    bool isBackgroundCb_ = false;
    bool isBackgroundChanged_ = false;
    PlayerStates backgroundState_ = PLAYER_IDLE;
    PlayerStates interruptEventState_ = PLAYER_IDLE;
    PlayerStates audioDeviceChangeState_ = PLAYER_IDLE;
    uint32_t appTokenId_ = 0;
    uint32_t subtitleTrackNum_ = 0;
    int32_t appUid_ = 0;
    int32_t appPid_ = 0;
    std::string appName_;
    std::atomic<int32_t> apiVersion_ = -1;
    std::atomic<bool> inReleasing_ = false;
    std::atomic<int32_t> userId_ = -1;
    std::atomic<bool> isBootCompleted_ = false;
    std::shared_ptr<AVMediaSource> mediaSource_ = nullptr;
    AVPlayStrategy strategy_;
    uint64_t instanceId_ = 0;
    std::atomic<bool> isInterruptNeeded_{false};
    bool isAudioMuted_ = false;
    std::mutex seekContinousMutex_;
    std::atomic<bool> isInSeekContinous_ {false};
    std::atomic<int64_t> seekContinousBatchNo_ {-1};
    bool deviceChangeCallbackflag_ = false;
    bool maxAmplitudeCbStatus_ = false;
    bool seiMessageCbStatus_ = false;
    bool enableReportMediaProgress_ = false;
    bool enableReportAudioInterrupt_ = false;
    std::vector<int32_t> payloadTypes_ {};
    bool isStreamUsagePauseRequired_ = true;
    std::mutex surfaceMutex_;
    int64_t pauseTimestamp_ {Plugins::HST_TIME_NONE};
    int64_t sumPauseTime_ {0};
    bool isXSpeedPlay_ {false};
    bool isCalledBySystemApp_ = false;
    bool isForceLoadVideo_ {false};
    std::atomic<uint32_t> totalMemoryUage_ {0};
    PlayerProducer playerProducer_ = PlayerProducer::INNER;
    std::atomic<bool> isFrozen_ = false;
    bool isMemoryExchanged_ = false;
    OHOS::Media::MediaType mutedMediaType_ = OHOS::Media::MediaType::MEDIA_TYPE_MAX_COUNT;
};
} // namespace Media
} // namespace OHOS
#endif // PLAYER_SERVICE_SERVER_H
