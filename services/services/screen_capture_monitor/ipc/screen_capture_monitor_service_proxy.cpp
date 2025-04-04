/*
* Copyright (C) 2024 Huawei Device Co., Ltd.
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

#include "screen_capture_monitor_service_proxy.h"
#include "media_log.h"
#include "media_errors.h"

namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, LOG_DOMAIN_SCREENCAPTURE, "ScreenCaptureMonitorServiceProxy"};
constexpr int MAX_LIST_COUNT = 1000;
}

namespace OHOS {
namespace Media {
ScreenCaptureMonitorServiceProxy::ScreenCaptureMonitorServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IStandardScreenCaptureMonitorService>(impl)
{
    MEDIA_LOGD("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

ScreenCaptureMonitorServiceProxy::~ScreenCaptureMonitorServiceProxy()
{
    MEDIA_LOGD("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
}

int32_t ScreenCaptureMonitorServiceProxy::SetListenerObject(const sptr<IRemoteObject> &object)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(ScreenCaptureMonitorServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    (void)data.WriteRemoteObject(object);
    int error = Remote()->SendRequest(SET_LISTENER_OBJ, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetListenerObject failed, error: %{public}d", error);
    return reply.ReadInt32();
}

int32_t ScreenCaptureMonitorServiceProxy::CloseListenerObject()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(ScreenCaptureMonitorServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");
    int error = Remote()->SendRequest(CLOSE_LISTENER_OBJ, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
                             "CloseListenerObject failed, error: %{public}d", error);
    return reply.ReadInt32();
}

std::list<int32_t> ScreenCaptureMonitorServiceProxy::IsScreenCaptureWorking()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(ScreenCaptureMonitorServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, {}, "Failed to write descriptor!");
    int error = Remote()->SendRequest(IS_SCREEN_CAPTURE_WORKING, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, {}, "IsScreenCaptureWorking failed, error: %{public}d", error);

    MEDIA_LOGD("ScreenCaptureMonitorServiceProxy::IsScreenCaptureWorking pid start.");
    std::list<int32_t> pidList;
    int32_t size = reply.ReadInt32();
    CHECK_AND_RETURN_RET_LOG(size < MAX_LIST_COUNT, {}, "content filter size exceed max range.");
    for (int32_t i = 0; i < size; i++) {
        pidList.push_back(reply.ReadInt32());
    }
    for (auto pid: pidList) {
        MEDIA_LOGD("ScreenCaptureMonitorServiceProxy::IsScreenCaptureWorking pid: %{public}d", pid);
    }
    MEDIA_LOGD("ScreenCaptureMonitorServiceProxy::IsScreenCaptureWorking pid end.");
    return pidList;
}

int32_t ScreenCaptureMonitorServiceProxy::DestroyStub()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(ScreenCaptureMonitorServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(DESTROY, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "DestroyStub failed, error: %{public}d", error);

    return reply.ReadInt32();
}

bool ScreenCaptureMonitorServiceProxy::IsSystemScreenRecorder(int32_t pid)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(ScreenCaptureMonitorServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, false, "Failed to write descriptor!");
    token = data.WriteInt32(pid);
    CHECK_AND_RETURN_RET_LOG(token, false, "Failed to write pid!");

    int error = Remote()->SendRequest(IS_SYSTEM_SCREEN_RECORDER, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK,
        false, "IsSystemScreenRecorder failed, error: %{public}d", error);

    return reply.ReadBool();
}

bool ScreenCaptureMonitorServiceProxy::IsSystemScreenRecorderWorking()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(ScreenCaptureMonitorServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, false, "Failed to write descriptor!");

    int error = Remote()->SendRequest(IS_SYSTEM_SCREEN_RECORDER_WORKING, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK,
        false, "IsSystemScreenRecorderWorking failed, error: %{public}d", error);

    return reply.ReadBool();
}
} // namespace Media
} // namespace OHOS