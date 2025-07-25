/*
 * Copyright (C) 2021-2025 Huawei Device Co., Ltd.
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

#include "media_errors.h"
#include <map>
#include <set>
#include <string>

namespace OHOS {
namespace Media {
using ErrorMessageFunc = std::function<std::string(const std::string& param1, const std::string& param2)>;
const std::map<MediaServiceErrCode, std::string> MSERRCODE_INFOS = {
    {MSERR_OK, "success"},
    {MSERR_NO_MEMORY, "no memory"},
    {MSERR_INVALID_OPERATION, "operation not be permitted"},
    {MSERR_INVALID_VAL, "invalid argument"},
    {MSERR_UNKNOWN, "unkown error"},
    {MSERR_MANDATORY_PARAMETER_UNSPECIFIED, "mandatory parameters are left unspecified"},
    {MSERR_INCORRECT_PARAMETER_TYPE, "Incorrect parameter types"},
    {MSERR_PARAMETER_VERIFICATION_FAILED, "Parameter verification failed"},
    {MSERR_SERVICE_DIED, "media service died"},
    {MSERR_CREATE_REC_ENGINE_FAILED, "create recorder engine failed"},
    {MSERR_CREATE_PLAYER_ENGINE_FAILED, "create player engine failed"},
    {MSERR_CREATE_AVMETADATAHELPER_ENGINE_FAILED, "create avmetadatahelper engine failed"},
    {MSERR_INVALID_STATE, "the state is not support this operation"},
    {MSERR_UNSUPPORT, "unsupport interface"},
    {MSERR_UNSUPPORT_AUD_SRC_TYPE, "unsupport audio source type"},
    {MSERR_UNSUPPORT_AUD_SAMPLE_RATE, "unsupport audio sample rate"},
    {MSERR_UNSUPPORT_AUD_CHANNEL_NUM, "unsupport audio channel"},
    {MSERR_UNSUPPORT_AUD_ENC_TYPE, "unsupport audio encoder type"},
    {MSERR_UNSUPPORT_AUD_PARAMS, "unsupport audio params(other params)"},
    {MSERR_UNSUPPORT_VID_SRC_TYPE, "unsupport video source type"},
    {MSERR_UNSUPPORT_VID_ENC_TYPE, "unsupport video encoder type"},
    {MSERR_UNSUPPORT_VID_PARAMS, "unsupport video params(other params)"},
    {MSERR_UNSUPPORT_CONTAINER_TYPE, "unsupport container format type"},
    {MSERR_UNSUPPORT_PROTOCOL_TYPE, "unsupport protocol type"},
    {MSERR_UNSUPPORT_VID_DEC_TYPE, "unsupport video decoder type"},
    {MSERR_UNSUPPORT_AUD_DEC_TYPE, "unsupport audio decoder type"},
    {MSERR_UNSUPPORT_STREAM, "internal data stream error"},
    {MSERR_UNSUPPORT_FILE, "this appears to be a text file"},
    {MSERR_UNSUPPORT_SOURCE, "unsupport source type"},
    {MSERR_VID_RESIZE_FAILED, "video resize failed"},
    {MSERR_AUD_ENC_FAILED, "audio encode failed"},
    {MSERR_AUD_RENDER_FAILED, "audio render failed"},
    {MSERR_VID_ENC_FAILED, "video encode failed"},
    {MSERR_AUD_DEC_FAILED, "audio decode failed"},
    {MSERR_VID_DEC_FAILED, "video decode failed"},
    {MSERR_MUXER_FAILED, "stream avmuxer failed"},
    {MSERR_DEMUXER_FAILED, "stream demuxer or parser failed"},
    {MSERR_OPEN_FILE_FAILED, "open file failed"},
    {MSERR_FILE_ACCESS_FAILED, "read or write file failed"},
    {MSERR_START_FAILED, "audio or video start failed"},
    {MSERR_PAUSE_FAILED, "audio or video pause failed"},
    {MSERR_STOP_FAILED, "audio or video stop failed"},
    {MSERR_SEEK_FAILED, "audio or video seek failed"},
    {MSERR_NETWORK_TIMEOUT, "network timeout"},
    {MSERR_NOT_FIND_CONTAINER, "not find a demuxer"},
    {MSERR_EXTEND_START, "extend start error code"},
    {MSERR_AUD_INTERRUPT, "audio interrupted"},
    {MSERR_USER_NO_PERMISSION, "user no permission"},
    {MSERR_DATA_SOURCE_ERROR_UNKNOWN, "media data source error unknow"},
    {MSERR_DATA_SOURCE_IO_ERROR, "media data source IO failed"},
    {MSERR_DRM_VERIFICATION_FAILED, "DRM verification failed"},
    {MSERR_UNSUPPORT_WATER_MARK, "unsupported water mark"},
    {MSERR_DEMUXER_BUFFER_NO_MEMORY, "demuxer cache data reached its limit"},
    {MSERR_IO_CANNOT_FIND_HOST, "IO can not find host"},
    {MSERR_IO_CONNECTION_TIMEOUT, "IO connection timeout"},
    {MSERR_IO_NETWORK_ABNORMAL, "IO network abnormal"},
    {MSERR_IO_NETWORK_UNAVAILABLE, "IO network unavailable"},
    {MSERR_IO_NO_PERMISSION, "IO no permission"},
    {MSERR_IO_NETWORK_ACCESS_DENIED, "IO request denied"},
    {MSERR_IO_RESOURE_NOT_FOUND, "IO resource not found"},
    {MSERR_IO_SSL_CLIENT_CERT_NEEDED, "IO SSL client cert needed"},
    {MSERR_IO_SSL_CONNECT_FAIL, "IO SSL connect fail"},
    {MSERR_IO_SSL_SERVER_CERT_UNTRUSTED, "IO SSL server cert untrusted"},
    {MSERR_IO_UNSUPPORTTED_REQUEST, "IO unsupported request"},
    {MSERR_SEEK_CONTINUOUS_UNSUPPORTED, "seek continonous is unsupported for this source"},
    {MSERR_SUPER_RESOLUTION_UNSUPPORTED, "super resolution not supported"},
    {MSERR_SUPER_RESOLUTION_NOT_ENABLED, "super resolution not enabled"},
    {MSERR_GET_INPUT_SURFACE_FAILED, "video encoder or resize get input surface failed"},
    {MSERR_SET_OUTPUT_SURFACE_FAILED, "video decoder or resize set output surface failed"},
    {MSERR_DSS_THREAD_CREATION_FAILED, "DSS of the LPP thread creation failed"},
    {MSERR_DSS_TASK_CREATION_FAILED, "DSS of the LPP task creation failed"},
    {MSERR_DSS_STRAT_FAILED, "DSS of the LPP start failed"},
    {MSERR_SHB_THREAD_CREATION_FAILED, "SHB of the LPP thread creation failed"},
    {MSERR_SHB_INIT_FAILED, "SHB of the LPP init failed"},
    {MSERR_SHB_MSG_QUE_CREATION_FAILED, "SHB of the LPP message queue creation failed"},
    {MSERR_SHB_NO_MEMORY, "SHB of the LPP failed to allocate memory."},
    {MSERR_SHB_CRASH_ERROR, "SHB of the LPP crash happend."},
    {MSERR_RS_INIT_FAILED, "RS of the LPP init failed"},
    {MSERR_RS_DATA_FALLBACK_FAILED, "RS of the LPP data fallback failed"},
    {MSERR_LPP_INSTANCE_EXCEED_LIMIT, "LPP instance limit exceeded"},
    {MSERR_HARDWARE_ERROR, "underlying hardware error happened"},
};

const std::map<MediaServiceErrCode, MediaServiceExtErrCode> MSERRCODE_TO_EXTERRORCODE = {
    {MSERR_OK,                                  MSERR_EXT_OK},
    {MSERR_NO_MEMORY,                           MSERR_EXT_NO_MEMORY},
    {MSERR_DEMUXER_BUFFER_NO_MEMORY,            MSERR_EXT_IO},
    {MSERR_INVALID_OPERATION,                   MSERR_EXT_OPERATE_NOT_PERMIT},
    {MSERR_INVALID_VAL,                         MSERR_EXT_INVALID_VAL},
    {MSERR_UNKNOWN,                             MSERR_EXT_UNKNOWN},
    {MSERR_SERVICE_DIED,                        MSERR_EXT_SERVICE_DIED},
    {MSERR_CREATE_REC_ENGINE_FAILED,            MSERR_EXT_UNKNOWN},
    {MSERR_CREATE_PLAYER_ENGINE_FAILED,         MSERR_EXT_UNKNOWN},
    {MSERR_INVALID_STATE,                       MSERR_EXT_INVALID_STATE},
    {MSERR_UNSUPPORT,                           MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_AUD_SRC_TYPE,              MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_AUD_SAMPLE_RATE,           MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_AUD_CHANNEL_NUM,           MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_AUD_ENC_TYPE,              MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_AUD_PARAMS,                MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_VID_SRC_TYPE,              MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_VID_ENC_TYPE,              MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_VID_PARAMS,                MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_CONTAINER_TYPE,            MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_PROTOCOL_TYPE,             MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_VID_DEC_TYPE,              MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_AUD_DEC_TYPE,              MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_STREAM,                    MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_FILE,                      MSERR_EXT_UNSUPPORT},
    {MSERR_UNSUPPORT_SOURCE,                    MSERR_EXT_UNSUPPORT},
    {MSERR_AUD_RENDER_FAILED,                   MSERR_EXT_UNSUPPORT},
    {MSERR_VID_RESIZE_FAILED,                   MSERR_EXT_UNKNOWN},
    {MSERR_AUD_ENC_FAILED,                      MSERR_EXT_UNKNOWN},
    {MSERR_VID_ENC_FAILED,                      MSERR_EXT_UNKNOWN},
    {MSERR_AUD_DEC_FAILED,                      MSERR_EXT_UNKNOWN},
    {MSERR_VID_DEC_FAILED,                      MSERR_EXT_UNKNOWN},
    {MSERR_MUXER_FAILED,                        MSERR_EXT_UNKNOWN},
    {MSERR_DEMUXER_FAILED,                      MSERR_EXT_UNKNOWN},
    {MSERR_OPEN_FILE_FAILED,                    MSERR_EXT_UNKNOWN},
    {MSERR_FILE_ACCESS_FAILED,                  MSERR_EXT_UNKNOWN},
    {MSERR_START_FAILED,                        MSERR_EXT_UNKNOWN},
    {MSERR_PAUSE_FAILED,                        MSERR_EXT_UNKNOWN},
    {MSERR_STOP_FAILED,                         MSERR_EXT_UNKNOWN},
    {MSERR_SEEK_FAILED,                         MSERR_EXT_UNKNOWN},
    {MSERR_NETWORK_TIMEOUT,                     MSERR_EXT_TIMEOUT},
    {MSERR_NOT_FIND_CONTAINER,                  MSERR_EXT_UNSUPPORT},
    {MSERR_EXTEND_START,                        MSERR_EXT_EXTEND_START},
    {MSERR_IO_CANNOT_FIND_HOST,                 MSERR_EXT_IO},
    {MSERR_IO_CONNECTION_TIMEOUT,               MSERR_EXT_IO},
    {MSERR_IO_NETWORK_ABNORMAL,                 MSERR_EXT_IO},
    {MSERR_IO_NETWORK_UNAVAILABLE,              MSERR_EXT_IO},
    {MSERR_IO_NO_PERMISSION,                    MSERR_EXT_IO},
    {MSERR_IO_NETWORK_ACCESS_DENIED,            MSERR_EXT_IO},
    {MSERR_IO_RESOURE_NOT_FOUND,                MSERR_EXT_IO},
    {MSERR_IO_SSL_CLIENT_CERT_NEEDED,           MSERR_EXT_IO},
    {MSERR_IO_SSL_CONNECT_FAIL,                 MSERR_EXT_IO},
    {MSERR_IO_SSL_SERVER_CERT_UNTRUSTED,        MSERR_EXT_IO},
    {MSERR_IO_UNSUPPORTTED_REQUEST,             MSERR_EXT_IO},
    {MSERR_SUPER_RESOLUTION_UNSUPPORTED,        MSERR_EXT_UNSUPPORT},
    {MSERR_SUPER_RESOLUTION_NOT_ENABLED,        MSERR_EXT_UNKNOWN},
    {MSERR_GET_INPUT_SURFACE_FAILED,            MSERR_EXT_UNKNOWN},
    {MSERR_SET_OUTPUT_SURFACE_FAILED,           MSERR_EXT_UNKNOWN},
    {MSERR_DSS_THREAD_CREATION_FAILED,          MSERR_EXT_UNKNOWN},
    {MSERR_DSS_TASK_CREATION_FAILED,            MSERR_EXT_UNKNOWN},
    {MSERR_DSS_STRAT_FAILED,                    MSERR_EXT_EXTEND_START},
    {MSERR_SHB_THREAD_CREATION_FAILED,          MSERR_EXT_UNKNOWN},
    {MSERR_SHB_INIT_FAILED,                     MSERR_EXT_UNKNOWN},
    {MSERR_SHB_MSG_QUE_CREATION_FAILED,         MSERR_EXT_UNKNOWN},
    {MSERR_SHB_NO_MEMORY,                       MSERR_EXT_NO_MEMORY},
    {MSERR_SHB_CRASH_ERROR,                     MSERR_EXT_SERVICE_DIED},
    {MSERR_RS_INIT_FAILED,                      MSERR_EXT_UNKNOWN},
    {MSERR_RS_DATA_FALLBACK_FAILED,             MSERR_EXT_UNKNOWN},
    {MSERR_LPP_INSTANCE_EXCEED_LIMIT,           MSERR_EXT_UNKNOWN},
    {MSERR_HARDWARE_ERROR,                      MSERR_EXT_UNKNOWN},
};

const std::map<MediaServiceExtErrCode, std::string> MSEXTERRCODE_INFOS = {
    {MSERR_EXT_OK, "success"},
    {MSERR_EXT_NO_MEMORY, "no memory"},
    {MSERR_EXT_OPERATE_NOT_PERMIT, "operation not be permitted"},
    {MSERR_EXT_INVALID_VAL, "invalid argument"},
    {MSERR_EXT_IO, "IO error"},
    {MSERR_EXT_TIMEOUT, "network timeout"},
    {MSERR_EXT_UNKNOWN, "unkown error"},
    {MSERR_EXT_SERVICE_DIED, "media service died"},
    {MSERR_EXT_INVALID_STATE, "the state is not support this operation"},
    {MSERR_EXT_UNSUPPORT, "unsupport interface"},
    {MSERR_EXT_EXTEND_START, "extend err start"},
};

const std::map<MediaServiceErrCode, MediaServiceExtErrCodeAPI9> MSERRCODE_TO_EXTERRORCODEAPI9 = {
    {MSERR_OK,                                  MSERR_EXT_API9_OK},
    {MSERR_NO_MEMORY,                           MSERR_EXT_API9_NO_MEMORY},
    {MSERR_INVALID_OPERATION,                   MSERR_EXT_API9_OPERATE_NOT_PERMIT},
    {MSERR_INVALID_VAL,                         MSERR_EXT_API9_INVALID_PARAMETER},
    {MSERR_MANDATORY_PARAMETER_UNSPECIFIED,     MSERR_EXT_API9_INVALID_PARAMETER},
    {MSERR_INCORRECT_PARAMETER_TYPE,            MSERR_EXT_API9_INVALID_PARAMETER},
    {MSERR_PARAMETER_VERIFICATION_FAILED,       MSERR_EXT_API9_INVALID_PARAMETER},
    {MSERR_SERVICE_DIED,                        MSERR_EXT_API9_SERVICE_DIED},
    {MSERR_CREATE_REC_ENGINE_FAILED,            MSERR_EXT_API9_NO_MEMORY},
    {MSERR_CREATE_PLAYER_ENGINE_FAILED,         MSERR_EXT_API9_NO_MEMORY},
    {MSERR_INVALID_STATE,                       MSERR_EXT_API9_OPERATE_NOT_PERMIT},
    {MSERR_UNSUPPORT,                           MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_AUD_SRC_TYPE,              MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_AUD_SAMPLE_RATE,           MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_AUD_CHANNEL_NUM,           MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_AUD_ENC_TYPE,              MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_AUD_PARAMS,                MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_VID_SRC_TYPE,              MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_VID_ENC_TYPE,              MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_VID_PARAMS,                MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_CONTAINER_TYPE,            MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_PROTOCOL_TYPE,             MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_VID_DEC_TYPE,              MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_AUD_DEC_TYPE,              MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_STREAM,                    MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_FILE,                      MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNSUPPORT_SOURCE,                    MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_VID_RESIZE_FAILED,                   MSERR_EXT_API9_IO},
    {MSERR_AUD_RENDER_FAILED,                   MSERR_EXT_API9_IO},
    {MSERR_AUD_ENC_FAILED,                      MSERR_EXT_API9_IO},
    {MSERR_VID_ENC_FAILED,                      MSERR_EXT_API9_IO},
    {MSERR_AUD_DEC_FAILED,                      MSERR_EXT_API9_IO},
    {MSERR_VID_DEC_FAILED,                      MSERR_EXT_API9_IO},
    {MSERR_MUXER_FAILED,                        MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_DEMUXER_FAILED,                      MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_OPEN_FILE_FAILED,                    MSERR_EXT_API9_IO},
    {MSERR_FILE_ACCESS_FAILED,                  MSERR_EXT_API9_IO},
    {MSERR_START_FAILED,                        MSERR_EXT_API9_OPERATE_NOT_PERMIT},
    {MSERR_PAUSE_FAILED,                        MSERR_EXT_API9_OPERATE_NOT_PERMIT},
    {MSERR_STOP_FAILED,                         MSERR_EXT_API9_OPERATE_NOT_PERMIT},
    {MSERR_SEEK_FAILED,                         MSERR_EXT_API9_OPERATE_NOT_PERMIT},
    {MSERR_DRM_VERIFICATION_FAILED,             MSERR_EXT_API9_OPERATE_NOT_PERMIT},
    {MSERR_NETWORK_TIMEOUT,                     MSERR_EXT_API9_TIMEOUT},
    {MSERR_NOT_FIND_CONTAINER,                  MSERR_EXT_API9_UNSUPPORT_FORMAT},
    {MSERR_UNKNOWN,                             MSERR_EXT_API9_IO},
    {MSERR_DATA_SOURCE_IO_ERROR,                MSERR_EXT_API9_IO},
    {MSERR_DATA_SOURCE_ERROR_UNKNOWN,           MSERR_EXT_API9_IO},
    {MSERR_AUD_INTERRUPT,                       MSERR_EXT_API9_AUDIO_INTERRUPTED},
    {MSERR_USER_NO_PERMISSION,                  MSERR_EXT_API9_NO_PERMISSION},
    {MSERR_UNSUPPORT_WATER_MARK,                MSERR_EXT_API9_UNSUPPORT_CAPABILITY},
    {MSERR_DEMUXER_BUFFER_NO_MEMORY,            MSERR_EXT_API9_IO},
    {MSERR_IO_CANNOT_FIND_HOST,                 MSERR_EXT_API14_IO_CANNOT_FIND_HOST},
    {MSERR_IO_CONNECTION_TIMEOUT,               MSERR_EXT_API14_IO_CONNECTION_TIMEOUT},
    {MSERR_IO_NETWORK_ABNORMAL,                 MSERR_EXT_API14_IO_NETWORK_ABNORMAL},
    {MSERR_IO_NETWORK_UNAVAILABLE,              MSERR_EXT_API14_IO_NETWORK_UNAVAILABLE},
    {MSERR_IO_NO_PERMISSION,                    MSERR_EXT_API14_IO_NO_PERMISSION},
    {MSERR_IO_NETWORK_ACCESS_DENIED,            MSERR_EXT_API14_IO_NETWORK_ACCESS_DENIED},
    {MSERR_IO_RESOURE_NOT_FOUND,                MSERR_EXT_API14_IO_RESOURE_NOT_FOUND},
    {MSERR_IO_SSL_CLIENT_CERT_NEEDED,           MSERR_EXT_API14_IO_SSL_CLIENT_CERT_NEEDED},
    {MSERR_IO_SSL_CONNECT_FAIL,                 MSERR_EXT_API14_IO_SSL_CONNECT_FAIL},
    {MSERR_IO_SSL_SERVER_CERT_UNTRUSTED,        MSERR_EXT_API14_IO_SSL_SERVER_CERT_UNTRUSTED},
    {MSERR_IO_UNSUPPORTTED_REQUEST,             MSERR_EXT_API14_IO_UNSUPPORTTED_REQUEST},
    {MSERR_SEEK_CONTINUOUS_UNSUPPORTED,         MSERR_EXT_API16_SEEK_CONTINUOUS_UNSUPPORTED},
    {MSERR_SUPER_RESOLUTION_UNSUPPORTED,        MSERR_EXT_API16_SUPER_RESOLUTION_UNSUPPORTED},
    {MSERR_SUPER_RESOLUTION_NOT_ENABLED,        MSERR_EXT_API16_SUPER_RESOLUTION_NOT_ENABLED},
    {MSERR_GET_INPUT_SURFACE_FAILED,            MSERR_EXT_API9_IO},
    {MSERR_SET_OUTPUT_SURFACE_FAILED,           MSERR_EXT_API9_IO},
    {MSERR_DSS_THREAD_CREATION_FAILED,          MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_DSS_TASK_CREATION_FAILED,            MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_DSS_STRAT_FAILED,                    MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_SHB_THREAD_CREATION_FAILED,          MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_SHB_INIT_FAILED,                     MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_SHB_MSG_QUE_CREATION_FAILED,         MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_SHB_NO_MEMORY,                       MSERR_EXT_API9_NO_MEMORY},
    {MSERR_SHB_CRASH_ERROR,                     MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_RS_INIT_FAILED,                      MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_RS_DATA_FALLBACK_FAILED,             MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_LPP_INSTANCE_EXCEED_LIMIT,           MSERR_EXT_API20_HARDWARE_FAILED},
    {MSERR_HARDWARE_ERROR,                      MSERR_EXT_API20_HARDWARE_FAILED},
};

const std::map<MediaServiceExtErrCodeAPI9, std::string> MSEXTERRCODE_API9_INFOS = {
    {MSERR_EXT_API9_OK, "Success: "},
    {MSERR_EXT_API9_NO_PERMISSION, "No Permission: "},
    {MSERR_EXT_API9_PERMISSION_DENIED, "Permission Denied"},
    {MSERR_EXT_API9_INVALID_PARAMETER, "Invalid Parameter: "},
    {MSERR_EXT_API9_UNSUPPORT_CAPABILITY, "Unsupport Capability: "},
    {MSERR_EXT_API9_NO_MEMORY, "No Memory: "},
    {MSERR_EXT_API9_OPERATE_NOT_PERMIT, "Operate Not Permit: "},
    {MSERR_EXT_API9_IO, "IO Error: "},
    {MSERR_EXT_API9_TIMEOUT, "Network Timeout: "},
    {MSERR_EXT_API9_SERVICE_DIED, "Service Died: "},
    {MSERR_EXT_API9_UNSUPPORT_FORMAT, "Unsupported Format: "},
    {MSERR_EXT_API9_AUDIO_INTERRUPTED, "Audio Interruped: "},
    {MSERR_EXT_API14_IO_CANNOT_FIND_HOST, "IO Cannot Find Host: "},
    {MSERR_EXT_API14_IO_CONNECTION_TIMEOUT, "IO Connection Timeout: "},
    {MSERR_EXT_API14_IO_NETWORK_ABNORMAL, "IO Network Abnormal: "},
    {MSERR_EXT_API14_IO_NETWORK_UNAVAILABLE, "IO Network Unavailable: "},
    {MSERR_EXT_API14_IO_NO_PERMISSION, "IO No Permission: "},
    {MSERR_EXT_API14_IO_NETWORK_ACCESS_DENIED, "IO Request Denied: "},
    {MSERR_EXT_API14_IO_RESOURE_NOT_FOUND, "IO Resource Not Found: "},
    {MSERR_EXT_API14_IO_SSL_CLIENT_CERT_NEEDED, "IO SSL Client Cert Needed: "},
    {MSERR_EXT_API14_IO_SSL_CONNECT_FAIL, "IO SSL Connect Fail: "},
    {MSERR_EXT_API14_IO_SSL_SERVER_CERT_UNTRUSTED, "IO SSL Server Cert Untrusted: "},
    {MSERR_EXT_API14_IO_UNSUPPORTTED_REQUEST, "IO Unsupported Request: "},
    {MSERR_EXT_API16_SEEK_CONTINUOUS_UNSUPPORTED, "Seek continuous unsupported: "},
    {MSERR_EXT_API20_HARDWARE_FAILED, "Hardware failed: "}
};

const std::set<MediaServiceErrCode> API14_EXT_IO_ERRORS = {
    MSERR_IO_CANNOT_FIND_HOST,
    MSERR_IO_CONNECTION_TIMEOUT,
    MSERR_IO_NETWORK_ABNORMAL,
    MSERR_IO_NETWORK_UNAVAILABLE,
    MSERR_IO_NO_PERMISSION,
    MSERR_IO_NETWORK_ACCESS_DENIED,
    MSERR_IO_RESOURE_NOT_FOUND,
    MSERR_IO_SSL_CLIENT_CERT_NEEDED,
    MSERR_IO_SSL_CONNECT_FAIL,
    MSERR_IO_SSL_SERVER_CERT_UNTRUSTED,
    MSERR_IO_UNSUPPORTTED_REQUEST,
};

std::string ErrorMessageOk(const std::string& param1, const std::string& param2)
{
    (void)param1;
    (void)param2;
    return "success";
}

std::string ErrorMessageNoPermission(const std::string& param1, const std::string& param2)
{
    std::string message = "Try to do " + param1 + " failed. User should request permission " + param2 + " first.";
    return message;
}

std::string ErrorMessageInvalidParameter(const std::string& param1, const std::string& param2)
{
    (void)param2;
    std::string message = "The Parameter " + param1 + " is invalid. Please check the type and range.";
    return message;
}

std::string ErrorMessageUnsupportCapability(const std::string& param1, const std::string& param2)
{
    (void)param2;
    std::string message = "Function " + param1 + " can not work correctly due to limited device capability.";
    return message;
}

std::string ErrorMessageNoMemory(const std::string& param1, const std::string& param2)
{
    (void)param2;
    std::string message = "Create " + param1 + " failed due to system memory.";
    return message;
}

std::string ErrorMessageOperateNotPermit(const std::string& param1, const std::string& param2)
{
    (void)param2;
    std::string message = "The operate " + param1 + " failed due to not permit in current state.";
    return message;
}

std::string ErrorMessageIO(const std::string& param1, const std::string& param2)
{
    (void)param2;
    std::string message = "IO error happened due to " + param1 + ".";
    return message;
}

std::string ErrorMessageTimeout(const std::string& param1, const std::string& param2)
{
    std::string message = "Timeout happend when " + param1 + " due to " + param2 + ".";
    return message;
}

std::string ErrorMessageServiceDied(const std::string& param1, const std::string& param2)
{
    (void)param1;
    (void)param2;
    std::string message = "Media Serviced Died.";
    return message;
}

std::string ErrorMessageUnsupportFormat(const std::string& param1, const std::string& param2)
{
    (void)param2;
    std::string message = "The format " + param1 + " is not support.";
    return message;
}

std::string ErrorMessageAudioInterruped(const std::string & param1, const std::string& param2)
{
    (void)param1;
    (void)param2;
    std::string message = "Audio Interrupted by other process.";
    return message;
}

const std::map<MediaServiceExtErrCodeAPI9, ErrorMessageFunc> MSEXTERRAPI9CODE_FUNCS = {
    {MSERR_EXT_API9_OK, &ErrorMessageOk},
    {MSERR_EXT_API9_NO_PERMISSION, &ErrorMessageNoPermission},
    {MSERR_EXT_API9_INVALID_PARAMETER, &ErrorMessageInvalidParameter},
    {MSERR_EXT_API9_UNSUPPORT_CAPABILITY, &ErrorMessageUnsupportCapability},
    {MSERR_EXT_API9_NO_MEMORY, &ErrorMessageNoMemory},
    {MSERR_EXT_API9_OPERATE_NOT_PERMIT, &ErrorMessageOperateNotPermit},
    {MSERR_EXT_API9_IO, &ErrorMessageIO},
    {MSERR_EXT_API9_TIMEOUT, &ErrorMessageTimeout},
    {MSERR_EXT_API9_SERVICE_DIED, &ErrorMessageServiceDied},
    {MSERR_EXT_API9_UNSUPPORT_FORMAT, &ErrorMessageUnsupportFormat},
    {MSERR_EXT_API9_AUDIO_INTERRUPTED, &ErrorMessageAudioInterruped},
};

std::string MSErrorToString(MediaServiceErrCode code)
{
    if (MSERRCODE_INFOS.count(code) != 0) {
        return MSERRCODE_INFOS.at(code);
    }

    if (code > MSERR_EXTEND_START) {
        return "extend error:" + std::to_string(static_cast<int32_t>(code - MSERR_EXTEND_START));
    }

    return "invalid error code:" + std::to_string(static_cast<int32_t>(code));
}

std::string MSExtErrorToString(MediaServiceExtErrCode code)
{
    if (MSEXTERRCODE_INFOS.count(code) != 0) {
        return MSEXTERRCODE_INFOS.at(code);
    }

    if (code > MSERR_EXT_EXTEND_START) {
        return "extend error:" + std::to_string(static_cast<int32_t>(code - MSERR_EXTEND_START));
    }

    return "invalid error code:" + std::to_string(static_cast<int32_t>(code));
}

std::string MSExtErrorAPI9ToString(MediaServiceExtErrCodeAPI9 code,
    const std::string& param1, const std::string& param2)
{
    if (MSEXTERRAPI9CODE_FUNCS.count(code) != 0) {
        return MSEXTERRAPI9CODE_FUNCS.at(code)(param1, param2);
    }

    return "invalid error code:" + std::to_string(static_cast<int32_t>(code));
}

std::string MSExtAVErrorToString(MediaServiceExtErrCodeAPI9 code)
{
    if (MSEXTERRCODE_API9_INFOS.count(code) != 0) {
        return MSEXTERRCODE_API9_INFOS.at(code);
    }

    return "invalid error code:";
}

std::string MSErrorToExtErrorString(MediaServiceErrCode code)
{
    if (MSERRCODE_INFOS.count(code) != 0 && MSERRCODE_TO_EXTERRORCODE.count(code) != 0) {
        MediaServiceExtErrCode extCode = MSERRCODE_TO_EXTERRORCODE.at(code);
        if (MSEXTERRCODE_INFOS.count(extCode) != 0) {
            return MSEXTERRCODE_INFOS.at(extCode);
        }
    }

    return "unkown error";
}

std::string MSErrorToExtErrorAPI9String(MediaServiceErrCode code, const std::string& param1, const std::string& param2)
{
    if (MSERRCODE_INFOS.count(code) != 0 && MSERRCODE_TO_EXTERRORCODEAPI9.count(code) != 0) {
        MediaServiceExtErrCodeAPI9 extCode = MSERRCODE_TO_EXTERRORCODEAPI9.at(code);
        if (MSEXTERRAPI9CODE_FUNCS.count(extCode) != 0) {
            return MSEXTERRAPI9CODE_FUNCS.at(extCode)(param1, param2);
        }
    }

    return "unkown error";
}

MediaServiceExtErrCode MSErrorToExtError(MediaServiceErrCode code)
{
    if (MSERRCODE_INFOS.count(code) != 0 && MSERRCODE_TO_EXTERRORCODE.count(code) != 0) {
        return MSERRCODE_TO_EXTERRORCODE.at(code);
    }

    return MSERR_EXT_UNKNOWN;
}

MediaServiceExtErrCodeAPI9 MSErrorToExtErrorAPI9(MediaServiceErrCode code)
{
    if (MSERRCODE_INFOS.count(code) != 0 && MSERRCODE_TO_EXTERRORCODEAPI9.count(code) != 0) {
        return MSERRCODE_TO_EXTERRORCODEAPI9.at(code);
    }
    // If error not in map, need add error and should not return default MSERR_EXT_API9_IO.
    return MSERR_EXT_API9_IO;
}

bool IsAPI14IOError(MediaServiceErrCode code)
{
    return API14_EXT_IO_ERRORS.find(code) != API14_EXT_IO_ERRORS.end();
}
} // namespace Media
} // namespace OHOS
