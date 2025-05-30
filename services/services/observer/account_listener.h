/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
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

#ifndef ACCOUNT_LISTENER_H
#define ACCOUNT_LISTENER_H

#include "event_handler.h"
#include "os_account_subscribe_info.h"
#include "os_account_subscriber.h"

namespace OHOS {
namespace Media {
class AccountListener : public AccountSA::OsAccountSubscriber {
public:
    explicit AccountListener(const AccountSA::OsAccountSubscribeInfo &subscribeInfo);
    ~AccountListener() override;
    void OnAccountsChanged(const int &id) override {};
    void OnAccountsSwitch(const int &newId, const int &oldId) override;
};
} // namespace Media
} // namespace OHOS
#endif // ACCOUNT_LISTENER_H