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

#ifndef OH_VEF_GRAPHICS_RENDER_MATH_UTIL_H
#define OH_VEF_GRAPHICS_RENDER_MATH_UTIL_H

#ifdef USE_M133_SKIA
#include "samples/RhythmGame/third_party/glm/gtc/type_ptr.hpp"
#else
#include "third_party/externals/oboe/samples/RhythmGame/third_party/glm/gtc/type_ptr.hpp"
#endif

namespace OHOS {
namespace Media {
class MathUtils {
public:
    template <class T> static const void* NativePtr(const T& v)
    {
        return reinterpret_cast<const void*>(glm::value_ptr(v));
    }
};
}
}
#endif