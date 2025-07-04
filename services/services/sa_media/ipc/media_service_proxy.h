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

#ifndef MEDIA_SERVICE_PROXY_H
#define MEDIA_SERVICE_PROXY_H

#include "i_standard_media_service.h"
#include "nocopyable.h"

namespace OHOS {
namespace Media {
class MediaServiceProxy : public IRemoteProxy<IStandardMediaService>, public NoCopyable {
public:
    explicit MediaServiceProxy(const sptr<IRemoteObject> &impl);
    virtual ~MediaServiceProxy();

    sptr<IRemoteObject> GetSubSystemAbility(IStandardMediaService::MediaSystemAbility subSystemId,
        const sptr<IRemoteObject> &listener) override;
    
    sptr<IRemoteObject> GetSubSystemAbilityWithTimeOut(IStandardMediaService::MediaSystemAbility subSystemId,
        const sptr<IRemoteObject> &listener, uint32_t timeoutMs) override;
    void ReleaseClientListener() override;

    bool CanKillMediaService() override;

    std::vector<pid_t> GetPlayerPids() override;
    int32_t FreezeStubForPids(const std::set<int32_t> &pidList, bool isProxy) override;

    int32_t ResetAllProxy() override;

private:
    bool IsGetSubSystemAbilityASync() const;
    static inline BrokerDelegator<MediaServiceProxy> delegator_;
};
} // namespace Media
} // namespace OHOS
#endif // MEDIA_SERVICE_PROXY_H
