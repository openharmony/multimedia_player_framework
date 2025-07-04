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

#ifndef I_STANDARD_MEDIA_SERVICE_H
#define I_STANDARD_MEDIA_SERVICE_H

#include "ipc_types.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"
#include "iremote_stub.h"
#include <set>

namespace OHOS {
namespace Media {
class IStandardMediaService : public IRemoteBroker {
public:
    /**
     * sub system ability ID
     */
    enum MediaSystemAbility : int32_t {
        MEDIA_PLAYER = 0,
        MEDIA_RECORDER = 1,
        MEDIA_CODEC = 2,
        MEDIA_AVMETADATAHELPER = 3,
        MEDIA_CODECLIST = 4,
        MEDIA_AVCODEC = 5,
        RECORDER_PROFILES = 6,
        MEDIA_MONITOR = 7,
        MEDIA_SCREEN_CAPTURE = 8,
        MEDIA_SCREEN_CAPTURE_CONTROLLER = 9,
        MEDIA_TRANSCODER = 10,
        MEDIA_SCREEN_CAPTURE_MONITOR = 11,
        MEDIA_LPP_AUDIO_PLAYER = 12,
        MEDIA_LPP_VIDEO_PLAYER = 13
    };

    /**
     * Create Player/Recorder/AVCodec/Codeclist/Split/AVmetadata Service Ability
     *
     * @return Returns remote object sptr, nullptr on failure.
     */
    virtual sptr<IRemoteObject> GetSubSystemAbility(IStandardMediaService::MediaSystemAbility subSystemId,
        const sptr<IRemoteObject> &listener) = 0;

    /**
     * Create Player/Recorder/AVCodec/Codeclist/Split/AVmetadata Service Ability
     *
     * @return Returns remote object sptr, nullptr on failure or timeout when wait beyond timeoutMs milliseconds.
     */
    virtual sptr<IRemoteObject> GetSubSystemAbilityWithTimeOut(IStandardMediaService::MediaSystemAbility subSystemId,
        const sptr<IRemoteObject> &listener, uint32_t timeoutMs) = 0;

    /**
     * Release the proxy object monitoring client process.
     */
    virtual void ReleaseClientListener() = 0;

    /**
     * Release the proxy object monitoring client process.
     */
    virtual bool CanKillMediaService() = 0;

    /**
     * Get pid vector of players.
     */
    virtual std::vector<pid_t> GetPlayerPids() = 0;

    virtual int32_t FreezeStubForPids(const std::set<int32_t> &pidList, bool isProxy) = 0;

    virtual int32_t ResetAllProxy() = 0;

    /**
     * IPC code ID
     */
    enum MediaServiceMsg : int32_t {
        GET_SUBSYSTEM = 0,
        GET_SUBSYSTEM_ASYNC = 1,
        RELEASE_CLIENT_LISTENER = 2,
        CAN_KILL_MEDIA_SERVICE = 3,
        GET_PLAYER_PIDS = 4,
        FREEZE = 5,
        RESET_ALL_PROXY = 6,
    };

    DECLARE_INTERFACE_DESCRIPTOR(u"IStandardMediaService");
};
} // namespace Media
} // namespace OHOS
#endif // I_STANDARD_MEDIA_SERVICE_H
