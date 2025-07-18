/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#ifndef AV_PLAYER_NAPI_H
#define AV_PLAYER_NAPI_H

#include <shared_mutex>
#include "player.h"
#include "media_errors.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "avplayer_callback.h"
#include "media_data_source_callback.h"
#include "common_napi.h"
#include "audio_info.h"
#include "audio_effect.h"
#include "task_queue.h"

namespace OHOS {
namespace Media {
namespace AVPlayerState {
const std::string STATE_IDLE = "idle";
const std::string STATE_INITIALIZED = "initialized";
const std::string STATE_PREPARED = "prepared";
const std::string STATE_PLAYING = "playing";
const std::string STATE_PAUSED = "paused";
const std::string STATE_STOPPED = "stopped";
const std::string STATE_RELEASED = "released";
const std::string STATE_ERROR = "error";
const std::string STATE_COMPLETED = "completed";
}

namespace AVPlayerEvent {
const std::string EVENT_STATE_CHANGE = "stateChange";
const std::string EVENT_VOLUME_CHANGE = "volumeChange";
const std::string EVENT_END_OF_STREAM = "endOfStream";
const std::string EVENT_SEEK_DONE = "seekDone";
const std::string EVENT_SPEED_DONE = "speedDone";
const std::string EVENT_RATE_DONE = "playbackRateDone";
const std::string EVENT_BITRATE_DONE = "bitrateDone";
const std::string EVENT_TIME_UPDATE = "timeUpdate";
const std::string EVENT_DURATION_UPDATE = "durationUpdate";
const std::string EVENT_SUBTITLE_TEXT_UPDATE = "subtitleTextUpdate";
const std::string EVENT_BUFFERING_UPDATE = "bufferingUpdate";
const std::string EVENT_START_RENDER_FRAME = "startRenderFrame";
const std::string EVENT_VIDEO_SIZE_CHANGE = "videoSizeChange";
const std::string EVENT_AUDIO_INTERRUPT = "audioInterrupt";
const std::string EVENT_AVAILABLE_BITRATES = "availableBitrates";
const std::string EVENT_TRACKCHANGE = "trackChange";
const std::string EVENT_TRACK_INFO_UPDATE = "trackInfoUpdate";
const std::string EVENT_DRM_INFO_UPDATE = "mediaKeySystemInfoUpdate";
const std::string EVENT_SET_DECRYPT_CONFIG_DONE = "setDecryptConfigDone";
const std::string EVENT_AUDIO_DEVICE_CHANGE = "audioOutputDeviceChangeWithInfo";
const std::string EVENT_SUBTITLE_UPDATE = "subtitleUpdate";
const std::string EVENT_ERROR = "error";
const std::string EVENT_AMPLITUDE_UPDATE = "amplitudeUpdate";
const std::string EVENT_SEI_MESSAGE_INFO = "seiMessageReceived";
const std::string EVENT_SUPER_RESOLUTION_CHANGED = "superResolutionChanged";
}

using TaskRet = std::pair<int32_t, std::string>;

class AVPlayerNapi : public AVPlayerNotify {
public:
    __attribute__((visibility("default"))) static napi_value Init(napi_env env, napi_value exports);

private:
    static napi_value Constructor(napi_env env, napi_callback_info info);
    static void Destructor(napi_env env, void *nativeObject, void *finalize);
    /**
     * createAVPlayer(callback: AsyncCallback<VideoPlayer>): void
     * createAVPlayer(): Promise<VideoPlayer>
     */
    static napi_value JsCreateAVPlayer(napi_env env, napi_callback_info info);
    /**
     * prepare(callback: AsyncCallback<void>): void
     * prepare(): Promise<void>
     */
    static napi_value JsPrepare(napi_env env, napi_callback_info info);
    /**
     * play(callback: AsyncCallback<void>): void
     * play(): Promise<void>
     */
    static napi_value JsPlay(napi_env env, napi_callback_info info);
    /**
     * pause(callback: AsyncCallback<void>): void
     * pause(): Promise<void>
     */
    static napi_value JsPause(napi_env env, napi_callback_info info);
    /**
     * stop(callback: AsyncCallback<void>): void
     * stop(): Promise<void>
     */
    static napi_value JsStop(napi_env env, napi_callback_info info);
    /**
     * reset(callback: AsyncCallback<void>): void
     * reset(): Promise<void>
     */
    static napi_value JsReset(napi_env env, napi_callback_info info);
    /**
     * release(callback: AsyncCallback<void>): void
     * release(): Promise<void>
     */
    static napi_value JsRelease(napi_env env, napi_callback_info info);
    /**
     * seek(timeMs: number, mode?:SeekMode): void
     */
    static napi_value JsSeek(napi_env env, napi_callback_info info);
    /**
     * setPlayRange(startTimeMs: number, endTimeMs: number, mode?: SeekMode): void
     */
    static napi_value JsSetPlaybackRange(napi_env env, napi_callback_info info);
    /**
     * setSpeed(speed: number): void
     */
    static napi_value JsSetSpeed(napi_env env, napi_callback_info info);
    /**
     * setPlaybackRate(rate: float): void
     */
    static napi_value JsSetPlaybackRate(napi_env env, napi_callback_info info);
    /**
     * setVolume(vol: number): void
     */
    static napi_value JsSetVolume(napi_env env, napi_callback_info info);
    /**
     * selectBitrate(bitRate: number): void
     */
    static napi_value JsSelectBitrate(napi_env env, napi_callback_info info);
    /**
     * addSubtitleUrl: string
     */
    static napi_value JsAddSubtitleUrl(napi_env env, napi_callback_info info);
    /**
     * addSubtitleFdSrc: AVFileDescriptor
     */
    static napi_value JsAddSubtitleAVFileDescriptor(napi_env env, napi_callback_info info);
    /**
     * url: string
     */
    static napi_value JsSetUrl(napi_env env, napi_callback_info info);
    static napi_value JsGetUrl(napi_env env, napi_callback_info info);
    /**
     * fdSrc: AVFileDescriptor
     */
    static napi_value JsGetAVFileDescriptor(napi_env env, napi_callback_info info);
    static napi_value JsSetAVFileDescriptor(napi_env env, napi_callback_info info);
    /**
     * dataSrc: DataSrcDescriptor
     */
    static napi_value JsSetDataSrc(napi_env env, napi_callback_info info);
    static napi_value JsGetDataSrc(napi_env env, napi_callback_info info);
    /**
     * surfaceId?: string
     */
    static napi_value JsSetSurfaceID(napi_env env, napi_callback_info info);
    static napi_value JsGetSurfaceID(napi_env env, napi_callback_info info);
    /**
     * loop: boolenan
     */
    static napi_value JsSetLoop(napi_env env, napi_callback_info info);
    static napi_value JsGetLoop(napi_env env, napi_callback_info info);
    /**
     * videoScaleType?: VideoScaleType
     */
    static napi_value JsSetVideoScaleType(napi_env env, napi_callback_info info);
    static napi_value JsGetVideoScaleType(napi_env env, napi_callback_info info);
    /**
     * audioInterruptMode?: audio.AudioInterruptMode
     */
    static napi_value JsGetAudioInterruptMode(napi_env env, napi_callback_info info);
    static napi_value JsSetAudioInterruptMode(napi_env env, napi_callback_info info);

    /**
     * audioRendererInfo?: audio.AudioRendererInfo
     */
    static napi_value JsGetAudioRendererInfo(napi_env env, napi_callback_info info);
    static napi_value JsSetAudioRendererInfo(napi_env env, napi_callback_info info);

    /**
     * audioEffectMode ?: audio.AudioEffectMode;
     */
    static napi_value JsGetAudioEffectMode(napi_env env, napi_callback_info info);
    static napi_value JsSetAudioEffectMode(napi_env env, napi_callback_info info);
    /**
     * readonly currentTime: number
     */
    static napi_value JsGetCurrentTime(napi_env env, napi_callback_info info);
    /**
     * readonly currentPlaybackPosition: number
     */
    static napi_value JsGetPlaybackPosition(napi_env env, napi_callback_info info);
    /**
     * readonly forceLoadVideo: boolean
     */
    static napi_value JsForceLoadVideo(napi_env env, napi_callback_info info);
    /**
     * readonly duration: number
     */
    static napi_value JsGetDuration(napi_env env, napi_callback_info info);
    /**
     * readonly state: AVPlayState
     */
    static napi_value JsGetState(napi_env env, napi_callback_info info);
    /**
     * readonly width: number
     */
    static napi_value JsGetWidth(napi_env env, napi_callback_info info);
    /**
     * readonly height: number
     */
    static napi_value JsGetHeight(napi_env env, napi_callback_info info);
    /**
     * getTrackDescription(callback:AsyncCallback<Array<MediaDescription>>): void
     * getTrackDescription(): Promise<Array<MediaDescription>>
     */
    static napi_value JsGetTrackDescription(napi_env env, napi_callback_info info);
    /**
     * JsGetSelectedTracks(callback:AsyncCallback<Array<number>>): void
     * JsGetSelectedTracks(): Promise<Array<number>>
     */
    static napi_value JsGetSelectedTracks(napi_env env, napi_callback_info info);
    /**
     * selectTrack(index: number, mode?: SwitchMode): void;
     */
    static napi_value JsSelectTrack(napi_env env, napi_callback_info info);
    /**
     * deselectTrack(index: number): void;
     */
    static napi_value JsDeselectTrack(napi_env env, napi_callback_info info);
    /**
     * GetCurrentTrack(trackType: MediaType, callback: AsyncCallback<number>): void;
     * GetCurrentTrack(trackType: MediaType): Promise<number>;
     */
    static napi_value JsGetCurrentTrack(napi_env env, napi_callback_info info);
    /**
     * setDecryptionConfig(mediaKeySession: drm.MediaKeySession, secureVideoPath: boolean): void;
     */
    static napi_value JsSetDecryptConfig(napi_env env, napi_callback_info info);

    static napi_value JsSetMediaSource(napi_env env, napi_callback_info info);
    /**
     * getMediaKeySystemInfos(): Array<MediaKeySystemInfo>;
     */
    static napi_value JsGetMediaKeySystemInfos(napi_env env, napi_callback_info info);

    static napi_value JsSetPlaybackStrategy(napi_env env, napi_callback_info info);

    static napi_value JsSetMediaMuted(napi_env env, napi_callback_info info);

    static napi_value JsSetSuperResolution(napi_env env, napi_callback_info info);

    static napi_value JsSetVideoWindowSize(napi_env env, napi_callback_info info);

    static napi_value JsSetStartFrameRateOptEnabled(napi_env env, napi_callback_info info);

    static napi_value JsEnableCameraPostprocessing(napi_env env, napi_callback_info info);

    /**
     * getPlaybackInfo(): playbackInfo;
     */
    static napi_value JsGetPlaybackInfo(napi_env env, napi_callback_info info);

    static napi_value JsIsSeekContinuousSupported(napi_env env, napi_callback_info info);

    /**
     * on(type: 'stateChange', callback: (state: AVPlayerState, reason: StateChangeReason) => void): void;
     * off(type: 'stateChange'): void;
     * on(type: 'volumeChange', callback: Callback<number>): void;
     * off(type: 'volumeChange'): void;
     * on(type: 'endOfStream', callback: Callback<void>): void;
     * off(type: 'endOfStream'): void;
     * on(type: 'seekDone', callback: Callback<number>): void;
     * off(type: 'seekDone'): void;
     * on(type: 'speedDone', callback: Callback<number>): void;
     * off(type: 'speedDone'): void;
     * on(type: 'bitrateDone', callback: Callback<number>): void;
     * off(type: 'bitrateDone'): void;
     * on(type: 'timeUpdate', callback: Callback<number>): void;
     * off(type: 'timeUpdate'): void;
     * on(type: 'durationUpdate', callback: Callback<number>): void;
     * off(type: 'durationUpdate'): void;
     * on(type: 'bufferingUpdate', callback: (infoType: BufferingInfoType, value: number) => void): void;
     * off(type: 'bufferingUpdate'): void;
     * on(type: 'startRenderFrame', callback: Callback<void>): void;
     * off(type: 'startRenderFrame'): void;
     * on(type: 'videoSizeChange', callback: (width: number, height: number) => void): void;
     * off(type: 'videoSizeChange'): void;
     * on(type: 'audioInterrupt', callback: (info: audio.InterruptEvent) => void): void;
     * off(type: 'audioInterrupt'): void;
     * on(type: 'availableBitrates', callback: (bitrates: Array<number>) => void): void;
     * off(type: 'availableBitrates'): void;
     * on(type: 'error', callback: ErrorCallback): void;
     * off(type: 'error'): void;
     * on(type: 'mediaKeySystemInfoUpdate', callback: (mediaKeySystemInfo: Array<MediaKeySystemInfo>) => void): void;
     * off(type: 'mediaKeySystemInfoUpdate'): void;
     */
    static napi_value JsSetOnCallback(napi_env env, napi_callback_info info);
    static napi_value JsClearOnCallback(napi_env env, napi_callback_info info);

    static AVPlayerNapi* GetJsInstance(napi_env env, napi_callback_info info);
    static AVPlayerNapi* GetJsInstanceWithParameter(napi_env env, napi_callback_info info,
        size_t &argc, napi_value *argv);
    static bool JsHandleParameter(napi_env env, napi_value args, AVPlayerNapi *jsPlayer);
    static void SeekEnqueueTask(AVPlayerNapi *jsPlayer, int32_t time, int32_t mode);
    static bool VerifyExpectedType(const NapiTypeCheckUnit &unit, AVPlayerNapi *jsPlayer, const std::string &msg);
    static std::shared_ptr<AVMediaSource> GetAVMediaSource(napi_env env, napi_value value,
        std::shared_ptr<AVMediaSourceTmp> &srcTmp);
    static bool IsSystemApp();
    AVPlayerNapi();
    ~AVPlayerNapi() override;
    void SaveCallbackReference(const std::string &callbackName, std::shared_ptr<AutoRef> ref);
    void ClearCallbackReference();
    void ClearCallbackReference(const std::string &callbackName);
    void StartListenCurrentResource();
    void PauseListenCurrentResource();
    void QueueOnErrorCb(MediaServiceExtErrCodeAPI9 errorCode, const std::string &errorMsg);
    void OnErrorCb(MediaServiceExtErrCodeAPI9 errorCode, const std::string &errorMsg);
    void SetSource(std::string url);
    void AddSubSource(std::string url);
    void SetSurface(const std::string &surfaceStr);
    void ResetUserParameters();
    void SetDataSource(AVPlayerNapi *jsPlayer);

    std::shared_ptr<TaskHandler<TaskRet>> PrepareTask();
    std::shared_ptr<TaskHandler<TaskRet>> PlayTask();
    std::shared_ptr<TaskHandler<TaskRet>> PauseTask();
    std::shared_ptr<TaskHandler<TaskRet>> StopTask();
    std::shared_ptr<TaskHandler<TaskRet>> ResetTask();
    std::shared_ptr<TaskHandler<TaskRet>> ReleaseTask();
    std::shared_ptr<TaskHandler<TaskRet>> SetPlaybackStrategyTask(AVPlayStrategy playStrategy);
    std::shared_ptr<TaskHandler<TaskRet>> SetMediaMutedTask(MediaType type, bool isMuted);
    std::shared_ptr<TaskHandler<TaskRet>> SetSuperResolutionTask(bool enabled);
    std::shared_ptr<TaskHandler<TaskRet>> SetVideoWindowSizeTask(int32_t width, int32_t height);
    std::shared_ptr<TaskHandler<TaskRet>> EnableCameraPostprocessingTask();
    std::shared_ptr<TaskHandler<TaskRet>> EqueueSetPlayRangeTask(int32_t start, int32_t end, int32_t mode);
    std::shared_ptr<TaskHandler<TaskRet>> ForceLoadVideoTask(bool status);

    std::string GetCurrentState();
    bool IsControllable();
    bool CanSetPlayRange();
    bool CanSetSuperResolution();
    bool IsVideoWindowSizeValid(int32_t width, int32_t height);
    bool CanCameraPostprocessing();
    bool IsLiveSource() const;
    void EnqueueNetworkTask(const std::string url);
    void EnqueueFdTask(const int32_t fd);

    PlayerSeekMode TransferSeekMode(int32_t mode);
    PlayerSwitchMode TransferSwitchMode(int32_t mode);

    void NotifyDuration(int32_t duration) override;
    void NotifyPosition(int32_t position) override;
    void NotifyState(PlayerStates state) override;
    void NotifyVideoSize(int32_t width, int32_t height) override;
    void NotifyIsLiveStream() override;
    void NotifyDrmInfoUpdated(const std::multimap<std::string, std::vector<uint8_t>> &infos) override;
    void StopTaskQue();
    void WaitTaskQueStop();
    void HandleListenerStateChange(std::string callbackName, bool state);
    void DeviceChangeCallbackOn(AVPlayerNapi *jsPlayer, std::string callbackName);
    void DeviceChangeCallbackOff(AVPlayerNapi *jsPlayer, std::string callbackName);
    void SeiMessageCallbackOn(
        AVPlayerNapi *jsPlayer, std::string callbackName, const std::vector<int32_t> &payloadTypes);
    void SeiMessageCallbackOff(
        AVPlayerNapi *jsPlayer, std::string &callbackName, const std::vector<int32_t> &payloadTypes);
    int32_t GetJsApiVersion() override;
    void GetAVPlayStrategyFromStrategyTmp(AVPlayStrategy &strategy, const AVPlayStrategyTmp &strategyTmp);
    bool IsPalyingDurationValid(const AVPlayStrategyTmp &strategyTmp);
    void EnqueueMediaSourceTask(AVPlayerNapi *jsPlayer, const std::shared_ptr<AVMediaSource> &mediaSource,
                                const struct AVPlayStrategy &strategy);
    void AddMediaStreamToAVMediaSource(
        const std::shared_ptr<AVMediaSourceTmp> &srcTmp, std::shared_ptr<AVMediaSource> &mediaSource);
    bool IsLivingMaxDelayTimeValid(const AVPlayStrategyTmp &strategyTmp);
    bool IsRateValid(float rate);

    std::condition_variable stopTaskQueCond_;
    bool taskQueStoped_ = false;
    bool deviceChangeCallbackflag_ = false;
    bool seiMessageCallbackflag_ = false;
    bool reportMediaProgressCallbackflag_ = false;

    struct AVPlayerContext : public MediaAsyncContext {
        explicit AVPlayerContext(napi_env env) : MediaAsyncContext(env) {}
        ~AVPlayerContext() = default;
        void CheckTaskResult(bool isTimeLimited = false, uint32_t milliseconds = 0)
        {
            if (asyncTask != nullptr) {
                auto result = isTimeLimited ? asyncTask->GetResultWithTimeLimit(milliseconds) : asyncTask->GetResult();
                if (result.HasResult() && result.Value().first != MSERR_EXT_API9_OK) {
                    SignError(result.Value().first, result.Value().second);
                }
            }
        }
        std::shared_ptr<TaskHandler<TaskRet>> asyncTask = nullptr;
        AVPlayerNapi *napi = nullptr;
        std::vector<Format> trackInfoVec_;
    };
    std::shared_ptr<TaskHandler<TaskRet>> GetTrackDescriptionTask(const std::unique_ptr<AVPlayerContext> &promiseCtx);
    void GetCurrentTrackTask(std::unique_ptr<AVPlayerContext> &promiseCtx, napi_env env, napi_value args);
    void HandleSelectTrack(std::unique_ptr<AVPlayerContext> &promiseCtx, napi_env env, napi_value args[],
        size_t argCount);
    static thread_local napi_ref constructor_;
    napi_env env_ = nullptr;
    std::shared_ptr<Player> player_ = nullptr;
    std::shared_ptr<AVPlayerCallback> playerCb_ = nullptr;
    std::shared_ptr<MediaDataSourceCallback> dataSrcCb_ = nullptr;
    std::atomic<bool> isReleased_ = false;
    std::atomic<bool> isInterrupted_ = false;
    std::string url_ = "";
    bool enabled_ = false;
    struct AVFileDescriptor fileDescriptor_;
    struct AVDataSrcDescriptor dataSrcDescriptor_;
    std::string surface_ = "";
    bool loop_ = false;
    int32_t videoScaleType_ = 0;
    std::vector<Format> trackInfoVec_;
    OHOS::AudioStandard::InterruptMode interruptMode_ = AudioStandard::InterruptMode::SHARE_MODE;
    OHOS::AudioStandard::AudioRendererInfo audioRendererInfo_ = OHOS::AudioStandard::AudioRendererInfo {
        OHOS::AudioStandard::ContentType::CONTENT_TYPE_MUSIC,
        OHOS::AudioStandard::StreamUsage::STREAM_USAGE_MEDIA,
        0
    };
    int32_t audioEffectMode_ = OHOS::AudioStandard::AudioEffectMode::EFFECT_DEFAULT;
    std::unique_ptr<TaskQueue> taskQue_;
    std::mutex mutex_;
    std::mutex taskMutex_;
    std::map<std::string, std::shared_ptr<AutoRef>> refMap_;
    PlayerStates state_ = PLAYER_IDLE;
    std::condition_variable stateChangeCond_;
    std::atomic<bool> stopWait_;
    bool avplayerExit_ = false;
    int32_t width_ = 0;
    int32_t height_ = 0;
    int32_t position_ = -1;
    int32_t duration_ = -1;
    bool isLiveStream_ = false;
    std::shared_mutex drmMutex_{};
    std::multimap<std::string, std::vector<uint8_t>> localDrmInfos_;
    Format playbackInfo_;
    int32_t index_ = -1;
    int32_t mode_ = SWITCH_SMOOTH;
    std::mutex syncMutex_;
    bool getApiVersionFlag_ = true;

    std::atomic<bool> isReadyReleased_ = false;
    bool isForceLoadVideo_ = false;
    bool hasSetStateChangeCb_ = false;
    int32_t mutedMediaType_ = OHOS::Media::MediaType::MEDIA_TYPE_MAX_COUNT;
};
} // namespace Media
} // namespace OHOS
#endif // AV_PLAYER_NAPI_H