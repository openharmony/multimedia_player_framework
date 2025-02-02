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

#ifndef MEDIA_APP_FROZEN_STATE_OBSERVER_H
#define MEDIA_APP_FROZEN_STATE_OBSERVER_H
#include "suspend_state_observer_stub.h"

namespace OHOS {
namespace Media {
class MediaAppFrozenStateObserver : public SuspendManager::SuspendStateObserverStub {
public:
    void OnActive(const std::vector<int32_t> &pidList, const int32_t uid) override;
    void OnDoze(const int32_t uid) override;
    void OnFrozen(const std::vector<int32_t> &pidList, const int32_t uid) override;
};
} // namespace Media
} // namespace OHOS
#endif // MEDIA_APP_FROZEN_STATE_OBSERVER_H
