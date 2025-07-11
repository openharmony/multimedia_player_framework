/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef PLAYER_MOCK_H
#define PLAYER_MOCK_H

#include "player_server.h"
#include "unittest_log.h"
#include "wm/window.h"

namespace OHOS {
namespace Media {
namespace PlayerTestParam {
inline constexpr int32_t SEEK_TIME_9_SEC = 9000;
inline constexpr int32_t SEEK_TIME_5_SEC = 5000;
inline constexpr int32_t SEEK_TIME_4_SEC = 4000;
inline constexpr int32_t SEEK_TIME_2_SEC = 2000;
inline constexpr int32_t waitsecond = 10;
inline constexpr int32_t DELTA_TIME = 1000;
inline constexpr int32_t PLAYING_TIME_2_SEC = 2;
inline constexpr int32_t PLAYING_TIME_1_SEC = 1;
inline constexpr int32_t PLAYING_TIME_10_SEC = 10;
inline constexpr int32_t SEEK_CONTINUOUS_WAIT_US = 16666;
const std::string MEDIA_ROOT = "file:///data/test/";
const std::string VIDEO_FILE1 = MEDIA_ROOT + "H264_AAC.mp4";
const std::string VIDEO_FILE2 = MEDIA_ROOT + "H264_AAC_multi_track.mp4";
const std::string VIDEO_FILE3 = MEDIA_ROOT + "H264_AAC.mkv";
const std::string SUBTITLE_SRT_FIELE = MEDIA_ROOT + "utf8.srt";
const std::string SUBTITLE_SRT_FIELE1 = MEDIA_ROOT + "utf8_test1.srt";
const std::string SUBTITLE_0_SEC = "MediaOS: test for subtitle_1";
const std::string SUBTITLE_1_SEC = "MediaOS: test for subtitle_2";
const std::string SUBTITLE_2_SEC = "MediaOS: test for subtitle_3";
const std::string SUBTITLE_3_SEC = "MediaOS: test for subtitle_4";
const std::string SUBTITLE_4_SEC = "MediaOS: test for subtitle_5";
const std::string SUBTITLE_5_SEC = "MediaOS: test for subtitle_6";
const std::string SUBTITLE_6_SEC = "MediaOS: test for subtitle_7";
const std::string SUBTITLE_8_SEC = "MediaOS: test for subtitle_9";
const std::string SUBTITLE_10_SEC = "MediaOS: test for subtitle_10";
const std::string SUBTITLE_TEST1_8_SEC = "MediaOS: test1 for subtitle_9";
const std::string HTTPS_PLAY = "HTTPS";
const std::string HTTP_PLAY = "HTTP";
const std::string LOCAL_PLAY = "LOCAL";
const std::string HLS_PLAY = "HLS";
const std::string INVALID_FILE = MEDIA_ROOT + "invalid.mp4";
const std::string MEDIA_URL = "http";
} // namespace PlayerTestParam

class PlayerSignal {
protected:
    PlayerStates state_ = PLAYER_IDLE;
    int32_t seekPosition_;
    bool seekDoneFlag_;
    bool speedDoneFlag_;
    bool rateDoneFlag_;
    bool trackDoneFlag_ = false;
    bool trackChange_ = false;
    bool trackInfoUpdate_ = false;
    bool textUpdate_ = false;
    std::string text_ = "";
    PlayerSeekMode seekMode_ = PlayerSeekMode::SEEK_CLOSEST;
    std::mutex mutexCond_;
    std::mutex subtitleMutex_;
    std::condition_variable condVarText_;
    std::condition_variable condVarPrepare_;
    std::condition_variable condVarPlay_;
    std::condition_variable condVarPause_;
    std::condition_variable condVarStop_;
    std::condition_variable condVarReset_;
    std::condition_variable condVarSeek_;
    std::condition_variable condVarSpeed_;
    std::condition_variable condVarRate_;
    std::condition_variable condVarTrackDone_;
    std::condition_variable condVarTrackInfoUpdate_;
};

class PlayerCallbackTest : public PlayerCallback, public NoCopyable, public PlayerSignal {
public:
    ~PlayerCallbackTest() {}
    void OnError(int32_t errorCode, const std::string &errorMsg) override;
    void OnInfo(PlayerOnInfoType type, int32_t extra, const Format &infoBody) override;
    void SeekNotify(int32_t extra, const Format &infoBody);
    void Notify(PlayerStates currentState);
    void SetSeekDoneFlag(bool seekDoneFlag);
    void SetSpeedDoneFlag(bool speedDoneFlag);
    void SetRateDoneFlag(bool rateDoneFlag);
    void SetSeekPosition(int32_t seekPosition);
    void SetState(PlayerStates state);
    void SetTrackDoneFlag(bool trackDoneFlag);
    int32_t PrepareSync();
    int32_t PlaySync();
    int32_t PauseSync();
    int32_t StopSync();
    int32_t ResetSync();
    int32_t SeekSync();
    int32_t SpeedSync();
    int32_t RateSync();
    int32_t TrackSync(bool &trackChange);
    int32_t TrackInfoUpdateSync();
    std::string SubtitleTextUpdate(std::string text);
    PlayerStates GetState();
private:
    void HandleTrackChangeCallback(int32_t extra, const Format &infoBody);
    void HandleSubtitleCallback(int32_t extra, const Format &infoBody);
    void HandleTrackInfoCallback(int32_t extra, const Format &infoBody);
    bool errInfo_ {false};
};

class PlayerServerMock : public NoCopyable {
public:
    explicit PlayerServerMock(std::shared_ptr<PlayerCallbackTest> &callback);
    virtual ~PlayerServerMock();
    bool CreatePlayer();
    int32_t Freeze();
    int32_t UnFreeze();
    int32_t SetSource(const std::string url);
    int32_t SetSource(const std::string &path, int64_t offset, int64_t size);
    int32_t SetSource(int32_t fd, int64_t offset, int64_t size);
    int32_t SetMediaSource(const std::shared_ptr<AVMediaSource> &mediaSource, AVPlayStrategy strategy);
    int32_t Prepare();
    int32_t PrepareAsync();
    int32_t Play();
    int32_t Pause();
    int32_t Stop();
    int32_t Reset();
    int32_t Release();
    int32_t ReleaseSync();
    int32_t Seek(int32_t mseconds, PlayerSeekMode mode);
    int32_t SetVolume(float leftVolume, float rightVolume);
    int32_t SetLooping(bool loop);
    int32_t GetCurrentTime(int32_t &currentTime);
    int32_t GetVideoTrackInfo(std::vector<Format> &videoTrack);
    int32_t GetPlaybackInfo(Format &playbackInfo);
    int32_t GetAudioTrackInfo(std::vector<Format> &audioTrack);
    int32_t GetSubtitleTrackInfo(std::vector<Format> &subtitleTrack);
    int32_t GetVideoWidth();
    int32_t GetVideoHeight();
    int32_t GetDuration(int32_t &duration);
    int32_t SetPlaybackRate(float rate);
    int32_t SetPlaybackSpeed(PlaybackRateMode mode);
    int32_t GetPlaybackSpeed(PlaybackRateMode &mode);
    int32_t SelectBitRate(uint32_t bitRate);
    bool IsPlaying();
    bool IsLooping();
    int32_t SetParameter(const Format &param);
    int32_t SetVolumeMode(int32_t mode);
    int32_t SetPlayerCallback(const std::shared_ptr<PlayerCallback> &callback);
    int32_t SetVideoSurface(sptr<Surface> surface);
    sptr<Surface> GetVideoSurface();
    int32_t SelectTrack(int32_t index, bool &trackChange);
    int32_t DeselectTrack(int32_t index, bool &trackChange);
    int32_t GetCurrentTrack(int32_t trackType, int32_t &index);
    int32_t AddSubSource(const std::string &url);
    int32_t AddSubSource(const std::string &path, int64_t offset, int64_t size);
    std::string GetSubtitleText(std::string text);
    sptr<Surface> GetVideoSurfaceNext();
    PlayerStates GetState();
    int32_t SetPlayRange(int64_t start, int64_t end);
    int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode);
    int32_t SeekContinuous(int32_t mseconds);
    int32_t SetMaxAmplitudeCbStatus(bool status);
    int32_t SetPlaybackStrategy(AVPlayStrategy strategy);
    int32_t SetMediaMuted(OHOS::Media::MediaType mediaType, bool isMuted);
    int32_t SetDeviceChangeCbStatus(bool status);
    int32_t SetRenderFirstFrame(bool display);
    int32_t EnableReportMediaProgress(bool enable);
    int32_t GetPlaybackPosition(int32_t &playbackPosition);
private:
    void SeekPrepare(int32_t &mseconds, PlayerSeekMode &mode);
    std::shared_ptr<IPlayerService> player_ = nullptr;
    std::shared_ptr<PlayerCallbackTest> callback_ = nullptr;
    sptr<Rosen::Window> previewWindow_ = nullptr;
    int32_t height_ = 1080;
    int32_t width_ = 1920;
    std::mutex mutex_;
    sptr<Rosen::Window> previewWindowNext_ = nullptr;
    int32_t nextSurfaceHeight_ = 100;
    int32_t nextSurfaceWidth_ = 100;
};

class PlayerMediaDataSourceMock : public IMediaDataSource, public NoCopyable {
public:
    explicit PlayerMediaDataSourceMock(int32_t size) : size_(size) {}
    ~PlayerMediaDataSourceMock() = default;
    int32_t ReadAt(const std::shared_ptr<AVSharedMemory> &mem, uint32_t length, int64_t pos = -1) override;
    int32_t GetSize(int64_t &size) override;

    // This interface has been deprecated
    int32_t ReadAt(int64_t pos, uint32_t length, const std::shared_ptr<AVSharedMemory> &mem) override;
    // This interface has been deprecated
    int32_t ReadAt(uint32_t length, const std::shared_ptr<AVSharedMemory> &mem) override;
private:
    int32_t size_ = -1;
};

class PlayerEngineMock : public IPlayerEngine {
public:
    PlayerEngineMock() = default;
    ~PlayerEngineMock() = default;
    int32_t SetSource(const std::string &url) override;
    int32_t SetSource(const std::shared_ptr<IMediaDataSource> &dataSrc) override;
    int32_t SetObs(const std::weak_ptr<IPlayerEngineObs> &obs) override;
    int32_t Play() override;
    int32_t PrepareAsync() override;
    int32_t Pause(bool isSystemOperation) override;
    int32_t Stop() override;
    int32_t Reset() override;
    int32_t Freeze(bool &isNoNeedToFreeze) override;
    int32_t UnFreeze() override;
    int32_t SetVolume(float leftVolume, float rightVolume) override;
    int32_t SetVolumeMode(int32_t mode) override;
    int32_t Seek(int32_t mSeconds, PlayerSeekMode mode) override;
    int32_t GetCurrentTime(int32_t &currentTime) override;
    int32_t GetVideoTrackInfo(std::vector<Format> &videoTrack) override;
    int32_t GetPlaybackInfo(Format &playbackInfo) override;
    int32_t GetAudioTrackInfo(std::vector<Format> &audioTrack) override;
    int32_t GetVideoWidth() override;
    int32_t GetVideoHeight() override;
    int32_t GetDuration(int32_t &duration) override;
    int32_t SetPlaybackSpeed(PlaybackRateMode mode) override;
    int32_t SetPlaybackRate(float rate) override;
    int32_t GetPlaybackSpeed(PlaybackRateMode &mode) override;
    int32_t SetMediaSource(const std::shared_ptr<AVMediaSource> &mediaSource, AVPlayStrategy strategy) override;
    int32_t SetVideoSurface(sptr<Surface> surface) override;
    float GetMaxAmplitude() override;
    int32_t SetLooping(bool loop) override;
    int32_t SetParameter(const Format &param) override;
    bool IsFlvLive() override;
    int32_t GetPlaybackPosition(int32_t &playbackPosition) override;
    bool IsNeedChangePlaySpeed(PlaybackRateMode &mode, bool &isXSpeedPlay) override;
};
} // namespace Media
} // namespace OHOS
#endif