/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
 
#ifndef RECORDER_MEDIA_LIBRARY_ADAPTER_H
#define RECORDER_MEDIA_LIBRARY_ADAPTER_H
 
#include <cstdint>
#include <string>
namespace OHOS {
namespace Media {
namespace MeidaLibraryAdapter {
using namespace std;
std::string CreateDisplayName();
bool CreateMediaLibrary(int32_t &fd, std::string &uri);
}
} // namespace Media
} // namespace OHOS
#endif