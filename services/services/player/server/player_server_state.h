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

#ifndef PLAYER_SERVER_STATE_H
#define PLAYER_SERVER_STATE_H

#include "player_server.h"

namespace OHOS {
namespace Media {
class PlayerServer::BaseState : public PlayerServerState {
public:
    BaseState(PlayerServer &server, const std::string &name) : PlayerServerState(name), server_(server) {}
    virtual ~BaseState() = default;

    virtual int32_t Prepare();
    virtual int32_t Play();
    virtual int32_t Pause(bool isSystemOperation);
    virtual int32_t Freeze();
    virtual int32_t UnFreeze();
    virtual int32_t Seek(int32_t mSeconds, PlayerSeekMode mode);
    virtual int32_t SetPlaybackSpeed(PlaybackRateMode mode);
    virtual int32_t SetPlaybackRate(float rate);
    virtual int32_t Stop();
    virtual int32_t SeekContinous(int32_t mSeconds, int64_t batchNo);
    virtual int32_t PauseDemuxer();
    virtual int32_t ResumeDemuxer();
    virtual int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode);
protected:
    int32_t OnMessageReceived(PlayerOnInfoType type, int32_t extra, const Format &infoBody) override;
    virtual void HandleStateChange(int32_t newState)
    {
        (void)newState;
    }
    virtual void HandlePlaybackComplete(int32_t extra)
    {
        (void)extra;
    }
    void ReportInvalidOperation() const;
    virtual void HandleEos() {}
    virtual void HandleInterruptEvent(const Format &infoBody)
    {
        (void)infoBody;
    }
    virtual void HandleAudioDeviceChangeEvent(const Format &infoBody)
    {
        (void)infoBody;
    }
    int32_t MessageSeekDone(int32_t extra);
    int32_t MessageTrackDone(int32_t extra);
    int32_t MessageTrackInfoUpdate();
    int32_t MessageSpeedDone();
    int32_t MessageRateDone();
    int32_t MessageStateChange(int32_t extra);

    PlayerServer &server_;
};

class PlayerServer::IdleState : public PlayerServer::BaseState {
public:
    explicit IdleState(PlayerServer &server) : BaseState(server, "idle_state") {}
    ~IdleState() = default;

protected:
    void StateEnter() override;
};

class PlayerServer::InitializedState : public PlayerServer::BaseState {
public:
    explicit InitializedState(PlayerServer &server) : BaseState(server, "inited_state") {}
    ~InitializedState() = default;

    int32_t Prepare() override;
    int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode) override;
};

class PlayerServer::PreparingState : public PlayerServer::BaseState {
public:
    explicit PreparingState(PlayerServer &server) : BaseState(server, "preparing_state") {}
    ~PreparingState() = default;

    int32_t Stop() override;

protected:
    void HandleStateChange(int32_t newState) override;
    void StateEnter() override;
};

class PlayerServer::PreparedState : public PlayerServer::BaseState {
public:
    explicit PreparedState(PlayerServer &server) : BaseState(server, "prepared_state") {}
    ~PreparedState() = default;

    int32_t Prepare() override;
    int32_t Play() override;
    int32_t Seek(int32_t mSeconds, PlayerSeekMode mode) override;
    int32_t Stop() override;
    int32_t SetPlaybackSpeed(PlaybackRateMode mode) override;
    int32_t SetPlaybackRate(float rate) override;
    int32_t SeekContinous(int32_t mSeconds, int64_t batchNo) override;
    int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode) override;

protected:
    void HandleStateChange(int32_t newState) override;
    void HandleEos() override;
};

class PlayerServer::PlayingState : public PlayerServer::BaseState {
public:
    explicit PlayingState(PlayerServer &server) : BaseState(server, "playing_state") {}
    ~PlayingState() = default;

    int32_t Play() override;
    int32_t Pause(bool isSystemOperation) override;
    int32_t Freeze() override;
    int32_t UnFreeze() override;
    int32_t Seek(int32_t mSeconds, PlayerSeekMode mode) override;
    int32_t Stop() override;
    int32_t SetPlaybackSpeed(PlaybackRateMode mode) override;
    int32_t SetPlaybackRate(float rate) override;
    int32_t SeekContinous(int32_t mSeconds, int64_t batchNo) override;
    int32_t PauseDemuxer() override;
    int32_t ResumeDemuxer() override;

protected:
    void HandleStateChange(int32_t newState) override;
    void HandlePlaybackComplete(int32_t extra) override;
    void HandleInterruptEvent(const Format &infoBody) override;
    void HandleAudioDeviceChangeEvent(const Format &infoBody) override;
    void HandleEos() override;
    void StateEnter() override;
    void StateExit() override;
};

class PlayerServer::PausedState : public PlayerServer::BaseState {
public:
    explicit PausedState(PlayerServer &server) : BaseState(server, "paused_state") {}
    ~PausedState() = default;

    int32_t Play() override;
    int32_t Pause(bool isSystemOperation) override;
    int32_t UnFreeze() override;
    int32_t Seek(int32_t mSeconds, PlayerSeekMode mode) override;
    int32_t Stop() override;
    int32_t SetPlaybackSpeed(PlaybackRateMode mode) override;
    int32_t SetPlaybackRate(float rate) override;
    int32_t SeekContinous(int32_t mSeconds, int64_t batchNo) override;
    int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode) override;

protected:
    void HandleStateChange(int32_t newState) override;
    void HandleEos() override;
};

class PlayerServer::StoppedState : public PlayerServer::BaseState {
public:
    explicit StoppedState(PlayerServer &server) : BaseState(server, "stopped_state") {}
    ~StoppedState() = default;

    int32_t Prepare() override;
    int32_t Stop() override;
    void HandleStateChange(int32_t newState) override;
    int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode) override;
};

class PlayerServer::PlaybackCompletedState : public PlayerServer::BaseState {
public:
    explicit PlaybackCompletedState(PlayerServer &server) : BaseState(server, "playbackCompleted_state") {}
    ~PlaybackCompletedState() = default;

    int32_t Play() override;
    int32_t Seek(int32_t mSeconds, PlayerSeekMode mode) override;
    int32_t UnFreeze() override;
    int32_t Stop() override;
    int32_t SetPlaybackSpeed(PlaybackRateMode mode) override;
    int32_t SetPlaybackRate(float rate) override;
    int32_t SeekContinous(int32_t mSeconds, int64_t batchNo) override;
    int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode) override;
    void StateEnter() override;

protected:
    void HandleStateChange(int32_t newState) override;

private:
    int64_t stateEnterTimeMs_ = 0;
};
}
}
#endif // PLAYER_SERVER_STATE_H
