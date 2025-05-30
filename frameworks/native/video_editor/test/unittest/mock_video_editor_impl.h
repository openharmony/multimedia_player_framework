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

#ifndef OH_VEF_VIDEO_EDITOR_IMPL_MOCK_H
#define OH_VEF_VIDEO_EDITOR_IMPL_MOCK_H

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "video_editor_impl.h"

namespace OHOS {
namespace Media {

class MockVideoEditorImpl : public VideoEditorImpl {
public:
    MockVideoEditorImpl(uint64_t id) : VideoEditorImpl(id) {};
    MOCK_METHOD(VEFError, Init, ());
    MOCK_METHOD(VEFError, AppendVideoFile, (int fileFd, const std::string &effectDescription), (override));
    MOCK_METHOD(VEFError, StartComposite, (const std::shared_ptr<CompositionOptions> &options), (override));
    MOCK_METHOD(VEFError, CancelComposite, (), (override));
};

} // namespace Media
} // namespace OHOS

#endif // OH_VEF_VIDEO_EDITOR_IMPL_MOCK_H
