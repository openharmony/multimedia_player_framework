/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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

#ifndef MONITOR_SERVICE_STUB_H
#define MONITOR_SERVICE_STUB_H

#include <map>
#include "i_standard_monitor_service.h"
#include "nocopyable.h"

namespace OHOS {
namespace Media {
using MonitorStubFunc = std::function<int32_t(MessageParcel &data, MessageParcel &reply)>;
class MonitorServiceStub : public IRemoteStub<IStandardMonitorService>, public NoCopyable {
public:
    virtual ~MonitorServiceStub();

    static sptr<MonitorServiceStub> GetInstance();

    int32_t DumpInfo(int32_t fd, bool needDetail);
    int32_t OnClientDie(int32_t pid);

    int OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override;
    int32_t Click() override;
    int32_t EnableMonitor() override;
    int32_t DisableMonitor() override;

private:
    MonitorServiceStub();
    int32_t Init();
    int32_t Click(MessageParcel &data, MessageParcel &reply);
    int32_t EnableMonitor(MessageParcel &data, MessageParcel &reply);
    int32_t DisableMonitor(MessageParcel &data, MessageParcel &reply);

    std::map<uint32_t, MonitorStubFunc> monitorFuncs_;
};
} // namespace Media
} // namespace OHOS
#endif // MONITOR_SERVICE_STUB_H
