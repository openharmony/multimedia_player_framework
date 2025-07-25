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

#include "recorder_service_proxy.h"
#include "recorder_listener_stub.h"
#include "media_log.h"
#include "media_errors.h"

namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, LOG_DOMAIN_RECORDER, "RecorderServiceProxy"};
}

namespace OHOS {
namespace Media {
RecorderServiceProxy::RecorderServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IStandardRecorderService>(impl)
{
    MEDIA_LOGD("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

RecorderServiceProxy::~RecorderServiceProxy()
{
    MEDIA_LOGD("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
}

int32_t RecorderServiceProxy::SetListenerObject(const sptr<IRemoteObject> &object)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteRemoteObject(object);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write RemoteObject!");

    int error = Remote()->SendRequest(SET_LISTENER_OBJ, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetListenerObject failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoSource(VideoSourceType source, int32_t &sourceId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(source);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write source!");

    int error = Remote()->SendRequest(SET_VIDEO_SOURCE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoSource failed, error: %{public}d", error);

    sourceId = reply.ReadInt32();
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoEncoder(int32_t sourceId, VideoCodecFormat encoder)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteInt32(encoder);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_VIDEO_ENCODER, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoEncoder failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoSize(int32_t sourceId, int32_t width, int32_t height)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteInt32(width) && data.WriteInt32(height);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_VIDEO_SIZE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoSize failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoFrameRate(int32_t sourceId, int32_t frameRate)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteInt32(frameRate);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_VIDEO_FARAME_RATE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoFrameRate failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoEncodingBitRate(int32_t sourceId, int32_t rate)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteInt32(rate);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_VIDEO_ENCODING_BIT_RATE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoEncodingBitRate failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoIsHdr(int32_t sourceId, bool isHdr)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteBool(isHdr);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_VIDEO_IS_HDR, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoIsHdr failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoEnableTemporalScale(int32_t sourceId, bool enableTemporalScale)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteBool(enableTemporalScale);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_VIDEO_ENABLE_TEMPORAL_SCALE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoEnableTemporalScale failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoEnableStableQualityMode(int32_t sourceId, bool enableStableQualityMode)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
 
    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");
 
    token = data.WriteInt32(sourceId) && data.WriteBool(enableStableQualityMode);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");
 
    int error = Remote()->SendRequest(SET_VIDEO_ENABLE_STABLE_QUALITY_MODE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoEnableStableQualityMode failed, error: %{public}d", error);
 
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetVideoEnableBFrame(int32_t sourceId, bool enableBFrame)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
 
    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");
 
    token = data.WriteInt32(sourceId) && data.WriteBool(enableBFrame);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");
 
    int error = Remote()->SendRequest(SET_VIDEO_ENABLE_B_FRAME, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetVideoEnableBFrame failed, error: %{public}d", error);
 
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetMetaConfigs(int32_t sourceId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_META_CONFIGS, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetMetaConfigs failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetMetaSource(MetaSourceType source, int32_t &sourceId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(source);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_META_SOURCE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetMetaSource failed, error: %{public}d", error);

    sourceId = reply.ReadInt32();
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetMetaMimeType(int32_t sourceId, const std::string_view &type)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteCString(type.data());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_META_MIME_TYPE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetMetaMimeType failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetMetaTimedKey(int32_t sourceId, const std::string_view &timedKey)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteCString(timedKey.data());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_META_TIMED_KEY, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetMetaTimedKey failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetMetaSourceTrackMime(int32_t sourceId, const std::string_view &srcTrackMime)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteCString(srcTrackMime.data());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_META_TRACK_SRC_MIME_TYPE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetMetaSourceTrackMime failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetCaptureRate(int32_t sourceId, double fps)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteDouble(fps);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_CAPTURE_RATE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetCaptureRate failed, error: %{public}d", error);

    return reply.ReadInt32();
}

sptr<OHOS::Surface> RecorderServiceProxy::GetSurface(int32_t sourceId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, nullptr, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId);
    CHECK_AND_RETURN_RET_LOG(token, nullptr, "write data failed");

    int error = Remote()->SendRequest(GET_SURFACE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, nullptr,
        "GetSurface failed, error: %{public}d", error);

    sptr<IRemoteObject> object = reply.ReadRemoteObject();
    CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr, "failed to read surface object");

    sptr<IBufferProducer> producer = iface_cast<IBufferProducer>(object);
    CHECK_AND_RETURN_RET_LOG(producer != nullptr, nullptr, "failed to convert object to producer");

    return OHOS::Surface::CreateSurfaceAsProducer(producer);
}

sptr<OHOS::Surface> RecorderServiceProxy::GetMetaSurface(int32_t sourceId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, nullptr, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId);
    CHECK_AND_RETURN_RET_LOG(token, nullptr, "write data failed");

    int error = Remote()->SendRequest(GET_META_SURFACE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, nullptr,
        "GetMetaSurface failed, error: %{public}d", error);

    sptr<IRemoteObject> object = reply.ReadRemoteObject();
    CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr, "failed to read surface object");

    sptr<IBufferProducer> producer = iface_cast<IBufferProducer>(object);
    CHECK_AND_RETURN_RET_LOG(producer != nullptr, nullptr, "failed to convert object to producer");

    return OHOS::Surface::CreateSurfaceAsProducer(producer);
}

int32_t RecorderServiceProxy::SetAudioSource(AudioSourceType source, int32_t &sourceId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(static_cast<int32_t>(source));
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_AUDIO_SOURCE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetAudioSource failed, error: %{public}d", error);

    sourceId = reply.ReadInt32();
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetAudioEncoder(int32_t sourceId, AudioCodecFormat encoder)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteInt32(static_cast<int32_t>(encoder));
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_AUDIO_ENCODER, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetAudioEncoder failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetAudioSampleRate(int32_t sourceId, int32_t rate)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteInt32(rate);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_AUDIO_SAMPLE_RATE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetAudioSampleRate failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetAudioChannels(int32_t sourceId, int32_t num)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteInt32(num);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_AUDIO_CHANNELS, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetAudioChannels failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetAudioEncodingBitRate(int32_t sourceId, int32_t bitRate)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(sourceId) && data.WriteInt32(bitRate);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_AUDIO_ENCODING_BIT_RATE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetAudioEncodingBitRate failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetDataSource(DataSourceType dataType, int32_t &sourceId)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(static_cast<int32_t>(dataType));
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_DATA_SOURCE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetDataSource failed, error: %{public}d", error);

    sourceId = reply.ReadInt32();
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetUserCustomInfo(Meta &userCustomInfo)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    bool ret = userCustomInfo.ToParcel(data);
    CHECK_AND_RETURN_RET_LOG(ret, MSERR_INVALID_OPERATION, "userCustomInfo ToParcel failed");
    int error = Remote()->SendRequest(SET_USER_CUSTOM_INFO, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetUserCustomInfo failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetGenre(std::string &genre)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteString(genre);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_GENRE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetGenre failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetMaxDuration(int32_t duration)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(duration);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_MAX_DURATION, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetMaxDuration failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetOutputFormat(OutputFormatType format)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(static_cast<int32_t>(format));
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_OUTPUT_FORMAT, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetOutputFormat failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetOutputFile(int32_t fd)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteFileDescriptor(fd);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write FileDescriptor failed");

    int error = Remote()->SendRequest(SET_OUTPUT_FILE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetOutputFile failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetFileGenerationMode(FileGenerationMode mode)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
 
    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");
 
    token = data.WriteInt32(static_cast<int32_t>(mode));
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_FILE_GENERATION_MODE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetFileGenerationMode failed, error: %{public}d", error);
 
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetNextOutputFile(int32_t fd)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteFileDescriptor(fd);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_NEXT_OUTPUT_FILE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetNextOutputFile failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetMaxFileSize(int64_t size)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt64(size);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_MAX_FILE_SIZE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetMaxFileSize failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetLocation(float latitude, float longitude)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteFloat(latitude) && data.WriteFloat(longitude);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_LOCATION, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetLocation failed, error: %{public}d", error);
    return MSERR_OK;
}

int32_t RecorderServiceProxy::SetOrientationHint(int32_t rotation)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(rotation);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_ORIENTATION_HINT, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetOrientationHint failed, error: %{public}d", error);
    return MSERR_OK;
}

int32_t RecorderServiceProxy::Prepare()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(PREPARE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "Prepare failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::Start()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(START, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "Start failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::Pause()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(PAUSE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "Pause failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::Resume()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(RESUME, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "Resume failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::Stop(bool block)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteBool(block);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(STOP, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "Stop failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::Reset()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(RESET, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "Reset failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::Release()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(RELEASE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "Release failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetFileSplitDuration(FileSplitType type, int64_t timestamp, uint32_t duration)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    token = data.WriteInt32(static_cast<int32_t>(type)) && data.WriteInt64(timestamp) && data.WriteUint32(duration);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_FILE_SPLIT_DURATION, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetFileSplitDuration failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::DestroyStub()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(DESTROY, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "DestroyStub failed, error: %{public}d", error);

    return reply.ReadInt32();
}
int32_t RecorderServiceProxy::GetAVRecorderConfig(ConfigMap &configMap)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(GET_AV_RECORDER_CONFIG, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "GetAVRecorderConfig failed, error: %{public}d", error);

    configMap["audioBitrate"] = reply.ReadInt32();
    configMap["audioChannels"] = reply.ReadInt32();
    configMap["audioCodec"] = reply.ReadInt32();
    configMap["audioSampleRate"] = reply.ReadInt32();
    configMap["fileFormat"] = reply.ReadInt32();
    configMap["videoBitrate"] = reply.ReadInt32();
    configMap["videoCodec"] = reply.ReadInt32();
    configMap["videoFrameHeight"] = reply.ReadInt32();
    configMap["videoFrameWidth"] = reply.ReadInt32();
    configMap["videoFrameRate"] = reply.ReadInt32();
    configMap["audioSourceType"] = reply.ReadInt32();
    configMap["videoSourceType"] = reply.ReadInt32();
    configMap["url"] = reply.ReadInt32();
    configMap["rotation"] = reply.ReadInt32();
    configMap["withVideo"] = reply.ReadInt32();
    configMap["withAudio"] = reply.ReadInt32();
    configMap["withLocation"] = reply.ReadInt32();

    return MSERR_OK;
}

int32_t RecorderServiceProxy::GetLocation(Location &location)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(GET_LOCATION, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "GetAVRecorderConfig failed, error: %{public}d", error);

    location.latitude = reply.ReadFloat();
    location.longitude = reply.ReadFloat();
    return MSERR_OK;
}

int32_t RecorderServiceProxy::GetCurrentCapturerChangeInfo(AudioRecorderChangeInfo &changeInfo)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(GET_AUDIO_CAPTURER_CHANGE_INFO, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "GetCurrentCapturerChangeInfo failed, error: %{public}d", error);
    changeInfo.Unmarshalling(reply);
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::GetAvailableEncoder(std::vector<EncoderCapabilityData> &encoderInfo)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(GET_AVAILABLE_ENCODER, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "GetAvailableEncoder failed, error: %{public}d", error);
    int32_t encoderCnt = reply.ReadInt32();
    int32_t codecFormatCntMax = AudioCodecFormat::AUDIO_CODEC_FORMAT_BUTT + VideoCodecFormat::VIDEO_CODEC_FORMAT_BUTT;
    CHECK_AND_RETURN_RET_LOG(0 < encoderCnt && encoderCnt < codecFormatCntMax, MSERR_INVALID_OPERATION,
        "Get encoderCnt exceed the limit(0 < encoderCnt < codecFormatCntMax)");
    for (int32_t i = 0; i < encoderCnt; i++) {
        EncoderCapabilityData codecData;
        codecData.Unmarshalling(reply);
        encoderInfo.push_back(codecData);
    }
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::GetMaxAmplitude()
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(GET_MAX_AMPLITUDE, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "GetMaxAmplitude failed, error: %{public}d", error);
    int32_t amplitude = reply.ReadInt32();
    MEDIA_LOGI("GetMaxAmplitude amplitude result: %{public}d", amplitude);
    return amplitude;
}

int32_t RecorderServiceProxy::IsWatermarkSupported(bool &isWatermarkSupported)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");

    int error = Remote()->SendRequest(IS_WATERMARK_SUPPORTED, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "IsWatermarkSupported failed, error: %{public}d", error);
    isWatermarkSupported = reply.ReadBool();
    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetWatermark(std::shared_ptr<AVBuffer> &waterMarkBuffer)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");
    
    CHECK_AND_RETURN_RET_LOG(waterMarkBuffer->WriteToMessageParcel(data),
        MSERR_INVALID_OPERATION, "Failed to write waterMarkBuffer!");

    int error = Remote()->SendRequest(SET_WATERMARK, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetWatermark failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetUserMeta(const std::shared_ptr<Meta> &userMeta)
{
    (void)userMeta;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");
    
    CHECK_AND_RETURN_RET_LOG(userMeta->ToParcel(data), MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_USERMETA, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetUserMeta failed, error: %{public}d", error);

    return reply.ReadInt32();
}

int32_t RecorderServiceProxy::SetWillMuteWhenInterrupted(bool muteWhenInterrupted)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    bool token = data.WriteInterfaceToken(RecorderServiceProxy::GetDescriptor());
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "Failed to write descriptor!");
    
    token = data.WriteBool(muteWhenInterrupted);
    CHECK_AND_RETURN_RET_LOG(token, MSERR_INVALID_OPERATION, "write data failed");

    int error = Remote()->SendRequest(SET_INTERRUPT_STRATEGY, data, reply, option);
    CHECK_AND_RETURN_RET_LOG(error == MSERR_OK, MSERR_INVALID_OPERATION,
        "SetWillMuteWhenInterrupted failed, error: %{public}d", error);

    return reply.ReadInt32();
}
} // namespace Media
} // namespace OHOS
