/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef HIPLAYER_CALLBACK_LOOPER_UNITTEST_H
#define HIPLAYER_CALLBACK_LOOPER_UNITTEST_H

#include "gtest/gtest.h"
#include "live_controller_unittest.h"
#include "hiplayer_callback_looper.h"
#include <gmock/gmock.h>
#include "osal/utils/steady_clock.h"

namespace OHOS {
namespace Media {
class HiplayerCallbackLooperUnittest : public testing::Test {
public:
    // SetUpTestCase: Called before all test cases
    static void SetUpTestCase(void);
    // TearDownTestCase: Called after all test case
    static void TearDownTestCase(void);
    // SetUp: Called before each test cases
    void SetUp(void);
    // TearDown: Called after each test cases
    void TearDown(void);
protected:
    std::unique_ptr<HiPlayerCallbackLooper> callback_;
    MockIPlayerEngine* mockIPlayerEngine_{nullptr};
    std::shared_ptr<MockIPlayerEngineObs> testObs_{nullptr};
};
} // namespace Media
} // namespace OHOS
#endif // HIPLAYER_CALLBACK_LOOPER_UNIT_TEST_H