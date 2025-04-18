/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef AVMETADATA_SERVICE_PROXY_FUZZER_H
#define AVMETADATA_SERVICE_PROXY_FUZZER_H

#include "stub_common.h"
#include "i_standard_avmetadatahelper_service.h"

namespace OHOS {
namespace Media {
class AVMetadataServiceProxyFuzzer : public IRemoteProxy<IStandardAVMetadataHelperService> {
public:
    static sptr<AVMetadataServiceProxyFuzzer> Create();
    explicit AVMetadataServiceProxyFuzzer(const sptr<IRemoteObject> &impl);
    virtual ~AVMetadataServiceProxyFuzzer() {}
    void SendRequest(uint32_t code, uint8_t *inputData, size_t size, bool isFuzz);
    int32_t SetSource(const std::string &uri, int32_t usage) override
    {
        return 0;
    }
    int32_t SetSource(int32_t fd, int64_t offset, int64_t size, int32_t usage) override
    {
        return 0;
    }
    int32_t SetSource(const sptr<IRemoteObject> &object) override
    {
        return 0;
    }
    std::string ResolveMetadata(int32_t key) override
    {
        return std::string("");
    }
    std::unordered_map<int32_t, std::string> ResolveMetadataMap() override
    {
        std::unordered_map<int32_t, std::string> metadata;
        return metadata;
    }
    std::shared_ptr<Meta> GetAVMetadata() override
    {
        return nullptr;
    }
    std::shared_ptr<AVSharedMemory> FetchArtPicture() override
    {
        return nullptr;
    }
    std::shared_ptr<AVSharedMemory> FetchFrameAtTime(
        int64_t timeUs, int32_t option, const OutputConfiguration &param) override
    {
        return nullptr;
    }
    std::shared_ptr<AVBuffer> FetchFrameYuv(
        int64_t timeUs, int32_t option, const OutputConfiguration &param) override
    {
        return nullptr;
    }
    void Release() override
    {
        return;
    }
    int32_t DestroyStub() override
    {
        return 0;
    }
    int32_t SetHelperCallback() override
    {
        return 0;
    }
    int32_t SetListenerObject(const sptr<IRemoteObject> &object) override
    {
        return 0;
    }
    int32_t GetTimeByFrameIndex(uint32_t index, uint64_t &time) override
    {
        return 0;
    }
    int32_t GetFrameIndexByTime(uint64_t time, uint32_t &index) override
    {
        return 0;
    }
    static int32_t SetUriSourceStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size, bool isFuzz);
    static int32_t SetFdSourceStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size, bool isFuzz);
    static int32_t ResolveMetadataStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size,
        bool isFuzz);
    static int32_t ResolveMetadataMapStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size,
        bool isFuzz);
    static int32_t GetAVMetadataStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size, bool isFuzz);
    static int32_t FetchArtPictureStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size,
        bool isFuzz);
    static int32_t FetchFrameAtTimeStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size,
        bool isFuzz);
    static int32_t ReleaseStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size, bool isFuzz);
    static int32_t DestroyStubStatic(AVMetadataServiceProxyFuzzer* ptr, uint8_t *inputData, size_t size, bool isFuzz);
private:
    int32_t SendRequest(uint32_t code, MessageParcel &inputData, MessageParcel &reply, MessageOption &option);
    static inline BrokerDelegator<AVMetadataServiceProxyFuzzer> delegator_;
};
}
}
#endif // AVMETADATA_SERVICE_PROXY_FUZZER_H
