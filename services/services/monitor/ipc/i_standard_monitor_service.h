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

#ifndef I_STANDARD_MONITOR_SERVICE_H
#define I_STANDARD_MONITOR_SERVICE_H

#include "ipc_types.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"
#include "iremote_stub.h"

namespace OHOS {
namespace Media {
class IStandardMonitorService : public IRemoteBroker {
public:
    virtual ~IStandardMonitorService() = default;
    virtual int32_t Click() = 0;
    virtual int32_t EnableMonitor() = 0;
    virtual int32_t DisableMonitor() = 0;

    /**
     * IPC code ID
     */
    enum MonitorServiceMsg {
        MONITOR_CLICK = 0,
        MONITOR_ENABLE,
        MONITOR_DISABLE,
    };

    DECLARE_INTERFACE_DESCRIPTOR(u"IStandardMoniterService");
};
} // namespace Media
} // namespace OHOS
#endif // I_STANDARD_MONITOR_SERVICE_H
