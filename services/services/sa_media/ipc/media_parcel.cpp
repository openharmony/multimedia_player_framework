/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "media_parcel.h"
#include "media_log.h"

namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, LOG_DOMAIN_PLAYER, "MediaParcel"};
}

namespace OHOS {
namespace Media {
static bool DoMarshalling(MessageParcel &parcel, const Format &format)
{
    auto dataMap = format.GetFormatMap();
    (void)parcel.WriteUint32(dataMap.size());
    for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
        (void)parcel.WriteString(it->first);
        (void)parcel.WriteUint32(it->second.type);
        switch (it->second.type) {
            case FORMAT_TYPE_INT32:
                (void)parcel.WriteInt32(it->second.val.int32Val);
                break;
            case FORMAT_TYPE_INT64:
                (void)parcel.WriteInt64(it->second.val.int64Val);
                break;
            case FORMAT_TYPE_FLOAT:
                (void)parcel.WriteFloat(it->second.val.floatVal);
                break;
            case FORMAT_TYPE_DOUBLE:
                (void)parcel.WriteDouble(it->second.val.doubleVal);
                break;
            case FORMAT_TYPE_STRING:
                (void)parcel.WriteString(it->second.stringVal);
                break;
            case FORMAT_TYPE_ADDR:
                (void)parcel.WriteInt32(static_cast<int32_t>(it->second.size));
                (void)parcel.WriteBuffer(reinterpret_cast<const void *>(it->second.addr), it->second.size);
                break;
            default:
                MEDIA_LOGE("fail to Marshalling Key: %{public}s", it->first.c_str());
                return false;
        }
        MEDIA_LOGD("success to Marshalling Key: %{public}s", it->first.c_str());
    }
    return true;
}

bool MediaParcel::Marshalling(MessageParcel &parcel, const Format &format)
{
    auto vecMap = format.GetFormatVectorMap();
    if (!vecMap.empty()) {
        for (auto it = vecMap.begin(); it != vecMap.end(); ++it) {
            uint32_t vecSize = static_cast<uint32_t>(it->second.size());
            (void)parcel.WriteUint32(vecSize);
            if (vecSize == 0) {
                MEDIA_LOGW("Marshal FormatVectorMap, vector is empty!");
                (void)parcel.WriteUint32(0);
            } else {
                (void)parcel.WriteString(it->first);
            }

            for (uint32_t index = 0; index < vecSize; index++) {
                CHECK_AND_RETURN_RET(DoMarshalling(parcel, it->second[index]), false);
            }
        }
    } else {
        (void)parcel.WriteUint32(0); // vecSize is 0
        return DoMarshalling(parcel, format);
    }

    return true;
}

static bool DoUnmarshalling(MessageParcel &parcel, Format &format)
{
    uint32_t size = parcel.ReadUint32();
    constexpr uint32_t MAX_PARCEL_SIZE = 256;
    CHECK_AND_RETURN_RET(size <= MAX_PARCEL_SIZE, false);
    for (uint32_t index = 0; index < size; index++) {
        std::string key = parcel.ReadString();
        uint32_t valType = parcel.ReadUint32();
        switch (valType) {
            case FORMAT_TYPE_INT32:
                (void)format.PutIntValue(key, parcel.ReadInt32());
                break;
            case FORMAT_TYPE_INT64:
                (void)format.PutLongValue(key, parcel.ReadInt64());
                break;
            case FORMAT_TYPE_FLOAT:
                (void)format.PutFloatValue(key, parcel.ReadFloat());
                break;
            case FORMAT_TYPE_DOUBLE:
                (void)format.PutDoubleValue(key, parcel.ReadDouble());
                break;
            case FORMAT_TYPE_STRING:
                (void)format.PutStringValue(key, parcel.ReadString());
                break;
            case FORMAT_TYPE_ADDR: {
                auto addrSize = parcel.ReadInt32();
                auto addr = parcel.ReadBuffer(static_cast<size_t>(addrSize));
                if (addr == nullptr) {
                    MEDIA_LOGE("fail to ReadBuffer Key: %{public}s", key.c_str());
                    return false;
                }
                (void)format.PutBuffer(key, addr, static_cast<size_t>(addrSize));
                break;
            }
            default:
                MEDIA_LOGE("fail to Unmarshalling Key: %{public}s", key.c_str());
                return false;
        }
        MEDIA_LOGD("success to Unmarshalling Key: %{public}s", key.c_str());
    }

    return true;
}

bool MediaParcel::Unmarshalling(MessageParcel &parcel, Format &format)
{
    uint32_t vecSize = parcel.ReadUint32();
    if (vecSize != 0) {
        constexpr uint32_t MAX_PARCEL_SIZE = 256;
        CHECK_AND_RETURN_RET(vecSize <= MAX_PARCEL_SIZE, false);
        std::string key = parcel.ReadString();
        std::vector<Format> vecFormat;
        for (uint32_t index = 0; index < vecSize; index++) {
            Format formatItem;
            if (!DoUnmarshalling(parcel, formatItem)) {
                return false;
            }
            vecFormat.emplace_back(formatItem);
        }
        (void)format.PutFormatVector(key, vecFormat);
    } else {
        return DoUnmarshalling(parcel, format);
    }

    return true;
}
} // namespace Media
} // namespace OHOS
