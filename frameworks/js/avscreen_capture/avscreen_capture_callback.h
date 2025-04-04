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

#ifndef AV_SCREEN_CAPTURE_CALLBACK_H
#define AV_SCREEN_CAPTURE_CALLBACK_H

#include <uv.h>
#include "screen_capture.h"
#include "avscreen_capture_napi.h"

namespace OHOS {
namespace Media {
class AVScreenCaptureCallback : public ScreenCaptureCallBack {
public:
    explicit AVScreenCaptureCallback(napi_env env);
    virtual ~AVScreenCaptureCallback();

    void SendErrorCallback(int32_t errCode, const std::string &msg);
    void SendStateCallback(AVScreenCaptureStateCode stateCode);
    void SaveCallbackReference(const std::string &name, std::weak_ptr<AutoRef> ref);
    void CancelCallbackReference(const std::string &name);
    void ClearCallbackReference();
protected:
    void OnError(ScreenCaptureErrorType errorType, int32_t errorCode) override;
    void OnStateChange(AVScreenCaptureStateCode stateCode) override;
    void OnAudioBufferAvailable(bool isReady, AudioCaptureSourceType type) override
    {
        (void)isReady;
        (void)type;
    }
    void OnVideoBufferAvailable(bool isReady) override
    {
        (void)isReady;
    }
private:
    struct AVScreenCaptureJsCallback {
        std::weak_ptr<AutoRef> autoRef;
        std::string callbackName = "unknown";
        std::string errorMsg = "unknown";
        int32_t errorCode = MSERR_EXT_UNKNOWN;
        AVScreenCaptureStateCode stateCode;
    };
    void OnJsErrorCallBack(AVScreenCaptureJsCallback *jsCb) const;
    void OnJsStateChangeCallBack(AVScreenCaptureJsCallback *jsCb) const;
    napi_env env_ = nullptr;
    std::mutex mutex_;
    std::map<std::string, std::weak_ptr<AutoRef>> refMap_;
};
} // namespace Media
} // namespace OHOS
#endif