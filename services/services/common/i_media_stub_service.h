/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#ifndef I_MEDIA_STUB_SERVICE_H
#define I_MEDIA_STUB_SERVICE_H

#include "iremote_broker.h"

namespace OHOS {
namespace Media {
class IMediaStubService : public IRemoteBroker {
public:
    virtual int32_t DumpInfo(int32_t fd)
    {
        (void)fd;
        return 0;
    };
    virtual ~IMediaStubService() = default;
};
}
}
#endif