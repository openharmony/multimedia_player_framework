/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LPP_AUDIO_DATA_MANAGER_UNIT_TEST_H
#define LPP_AUDIO_DATA_MANAGER_UNIT_TEST_H

#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "lpp_audio_data_manager.h"
#include "mock_avbuffer_queue_producer.h"
#include "mock_event_receiver.h"
#include "osal/task/pipeline_threadpool.h"

namespace OHOS {
namespace Media {
class LppAudioDataManagerUnitTest : public testing::Test {
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
    void HandleBufferAvailableFunTest0031();
    void HandleBufferAvailableFunTest0032();
    std::shared_ptr<LppAudioDataManager> audioDataMgr_ {nullptr};
    std::string streamerId_ {};
};
} // namespace Media
} // namespace OHOS
#endif // LPP_AUDIO_DATA_MANAGER_UNIT_TEST_H
