/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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

#ifndef MEDIA_SERVICE_HELPER_H
#define MEDIA_SERVICE_HELPER_H

#include <set>

namespace OHOS {
namespace Media {
class __attribute__((visibility("default"))) MediaServiceHelper {
public:
    static bool CanKillMediaService();

    static int32_t ProxyForFreeze(const std::set<int32_t> &pidList, bool isProxy);

    static int32_t ResetAllProxy();
};
} // namespace Media
} // namespace OHOS
#endif // MEDIA_SERVICE_HELPER_H
