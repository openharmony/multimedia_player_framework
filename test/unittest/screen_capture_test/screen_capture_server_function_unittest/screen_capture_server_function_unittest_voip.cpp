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

#include <unistd.h>
#include <sys/stat.h>
#include "screen_capture_server_function_unittest_voip.h"
#include "ui_extension_ability_connection.h"
#include "image_source.h"
#include "image_type.h"
#include "pixel_map.h"
#include "media_log.h"
#include "media_errors.h"
#include "media_utils.h"
#include "uri_helper.h"
#include "media_dfx.h"
#include "scope_guard.h"
#include "param_wrapper.h"

using namespace testing::ext;
using namespace OHOS::Media::ScreenCaptureTestParam;
using namespace OHOS::Media;

namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, LOG_DOMAIN_SCREENCAPTURE, "ScreenCaptureServerFunctionTest"};
constexpr int32_t FLIE_CREATE_FLAGS = 0777;
}
namespace OHOS {
namespace Media {

void ScreenCaptureServerFunctionTest::SetHapPermission()
{
    Security::AccessToken::HapInfoParams info = {
        .userID = 100, // 100 UserID
        .bundleName = "com.ohos.test.screencapturetdd",
        .instIndex = 0, // 0 index
        .appIDDesc = "com.ohos.test.screencapturetdd",
        .isSystemApp = true
    };
    Security::AccessToken::HapPolicyParams policy = {
        .apl = Security::AccessToken::APL_SYSTEM_BASIC,
        .domain = "test.domain.screencapturetdd",
        .permList = {},
        .permStateList = {
            {
                .permissionName = "ohos.permission.MICROPHONE",
                .isGeneral = true,
                .resDeviceID = { "local" },
                .grantStatus = { Security::AccessToken::PermissionState::PERMISSION_GRANTED },
                .grantFlags = { 1 }
            },
            {
                .permissionName = "ohos.permission.READ_MEDIA",
                .isGeneral = true,
                .resDeviceID = { "local" },
                .grantStatus = { Security::AccessToken::PermissionState::PERMISSION_GRANTED },
                .grantFlags = { 1 }
            },
            {
                .permissionName = "ohos.permission.WRITE_MEDIA",
                .isGeneral = true,
                .resDeviceID = { "local" },
                .grantStatus = { Security::AccessToken::PermissionState::PERMISSION_GRANTED },
                .grantFlags = { 1 }
            },
            {
                .permissionName = "ohos.permission.KEEP_BACKGROUND_RUNNING",
                .isGeneral = true,
                .resDeviceID = { "local" },
                .grantStatus = { Security::AccessToken::PermissionState::PERMISSION_GRANTED },
                .grantFlags = { 1 }
            }
        }
    };
    Security::AccessToken::AccessTokenIDEx tokenIdEx = {0};
    tokenIdEx = Security::AccessToken::AccessTokenKit::AllocHapToken(info, policy);
    int ret = SetSelfTokenID(tokenIdEx.tokenIDEx);
    if (ret != 0) {
        MEDIA_LOGE("Set hap token failed, err: %{public}d", ret);
    }
}

void ScreenCaptureServerFunctionTest::SetUp()
{
    SetHapPermission();
    std::shared_ptr<IScreenCaptureService> tempServer_ = ScreenCaptureServer::Create();
    screenCaptureServer_ = std::static_pointer_cast<ScreenCaptureServer>(tempServer_);
    ASSERT_NE(screenCaptureServer_, nullptr);
}

void ScreenCaptureServerFunctionTest::TearDown()
{
    screenCaptureServer_ = nullptr;
}

int32_t ScreenCaptureServerFunctionTest::SetConfig()
{
    AudioCaptureInfo miccapinfo = {
        .audioSampleRate = 16000,
        .audioChannels = 2,
        .audioSource = SOURCE_DEFAULT
    };

    VideoCaptureInfo videocapinfo = {
        .videoFrameWidth = 720,
        .videoFrameHeight = 1280,
        .videoSource = VIDEO_SOURCE_SURFACE_RGBA
    };

    VideoEncInfo videoEncInfo = {
        .videoCodec = VideoCodecFormat::H264,
        .videoBitrate = 2000000,
        .videoFrameRate = 30
    };

    AudioInfo audioinfo = {
        .micCapInfo = miccapinfo,
    };

    VideoInfo videoinfo = {
        .videoCapInfo = videocapinfo,
        .videoEncInfo = videoEncInfo
    };

    config_ = {
        .captureMode = CAPTURE_HOME_SCREEN,
        .dataType = ORIGINAL_STREAM,
        .audioInfo = audioinfo,
        .videoInfo = videoinfo
    };
    return MSERR_OK;
}

int32_t ScreenCaptureServerFunctionTest::SetConfigFile(RecorderInfo &recorderInfo)
{
    AudioEncInfo audioEncInfo = {
        .audioBitrate = 48000,
        .audioCodecformat = AudioCodecFormat::AAC_LC
    };

    VideoCaptureInfo videoCapInfo = {
        .videoFrameWidth = 720,
        .videoFrameHeight = 1080,
        .videoSource = VideoSourceType::VIDEO_SOURCE_SURFACE_RGBA
    };

    VideoEncInfo videoEncInfo = {
        .videoCodec = VideoCodecFormat::H264,
        .videoBitrate = 2000000,
        .videoFrameRate = 30
    };

    AudioCaptureInfo innerCapInfo = {
        .audioSampleRate = 0,
        .audioChannels = 0,
        .audioSource = AudioCaptureSourceType::SOURCE_DEFAULT,
        //.state = AVScreenCaptureParamValidationState::VALIDATION_VALID,
    };

    AudioCaptureInfo micCapInfo = {
        .audioSampleRate = 0,
        .audioChannels = 0,
        .audioSource = AudioCaptureSourceType::SOURCE_DEFAULT,
        //.state = AVScreenCaptureParamValidationState::VALIDATION_VALID,
    };

    AudioInfo audioInfo = {
        .micCapInfo = micCapInfo,
        .innerCapInfo = innerCapInfo,
        .audioEncInfo = audioEncInfo
    };

    VideoInfo videoInfo = {
        .videoCapInfo = videoCapInfo,
        .videoEncInfo = videoEncInfo
    };

    config_ = {
        .captureMode = CaptureMode::CAPTURE_HOME_SCREEN,
        .dataType = DataType::CAPTURE_FILE,
        .audioInfo = audioInfo,
        .videoInfo = videoInfo,
        .recorderInfo = recorderInfo
    };
    return MSERR_OK;
}

int32_t ScreenCaptureServerFunctionTest::SetRecorderInfo(std::string name,
    RecorderInfo &recorderInfo)
{
    OpenFileFd(name);
    recorderInfo.url = "fd://" + std::to_string(outputFd_);
    recorderInfo.fileFormat = "mp4";
    return MSERR_OK;
}

static const std::string SCREEN_CAPTURE_ROOT_DIR = "/data/test/media/";

void ScreenCaptureServerFunctionTest::OpenFileFd(std::string name)
{
    if (outputFd_ != -1) {
        (void)::close(outputFd_);
        outputFd_ = -1;
    }
    outputFd_ = open((SCREEN_CAPTURE_ROOT_DIR + name).c_str(), O_RDWR | O_CREAT, FLIE_CREATE_FLAGS);
}

int32_t ScreenCaptureServerFunctionTest::InitScreenCaptureServer()
{
    int32_t ret = screenCaptureServer_->SetCaptureMode(config_.captureMode);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "SetCaptureMode failed");
    ret = screenCaptureServer_->SetDataType(config_.dataType);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "SetDataType failed");
    ret = screenCaptureServer_->SetRecorderInfo(config_.recorderInfo);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "SetRecorderInfo failed");
    const std::string fdHead = "fd://";
    CHECK_AND_RETURN_RET_LOG(config_.recorderInfo.url.find(fdHead) != std::string::npos, MSERR_INVALID_VAL,
        "check url failed");
    int32_t outputFd = -1;
    std::string inputFd = config_.recorderInfo.url.substr(fdHead.size());
    CHECK_AND_RETURN_RET_LOG(StrToInt(inputFd, outputFd) == true && outputFd >= 0, MSERR_INVALID_VAL,
        "open file failed");
    ret = screenCaptureServer_->SetOutputFile(outputFd);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "SetOutputFile failed");
    ret = screenCaptureServer_->InitAudioEncInfo(config_.audioInfo.audioEncInfo);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "InitAudioEncInfo failed");

    bool isMicAudioCapInfoIgnored = config_.audioInfo.micCapInfo.audioChannels == 0 ||
        config_.audioInfo.micCapInfo.audioSampleRate == 0;
    if (!isMicAudioCapInfoIgnored) {
        if ((config_.audioInfo.micCapInfo.audioChannels != config_.audioInfo.innerCapInfo.audioChannels) ||
            (config_.audioInfo.micCapInfo.audioSampleRate != config_.audioInfo.innerCapInfo.audioSampleRate)) {
            MEDIA_LOGE("InitCaptureFile error, inner and mic param not consistent");
            return MSERR_INVALID_VAL;
        }
        ret = screenCaptureServer_->InitAudioCap(config_.audioInfo.micCapInfo);
        CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "init micAudioCap failed");
    }
    ret = screenCaptureServer_->InitAudioCap(config_.audioInfo.innerCapInfo);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "init innerCapInfo failed, innerCapInfo should be valid");

    ret = screenCaptureServer_->InitVideoEncInfo(config_.videoInfo.videoEncInfo);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "InitVideoEncInfo failed");
    ret = screenCaptureServer_->InitVideoCap(config_.videoInfo.videoCapInfo);
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "InitVideoCap failed");
    return MSERR_OK;
}

int32_t ScreenCaptureServerFunctionTest::StartAudioCapture()
{
    screenCaptureServer_->audioSource_ = std::make_unique<AudioDataSource>(
        AVScreenCaptureMixMode::MIX_MODE, screenCaptureServer_.get());
    MEDIA_LOGI("StartAudioCapture start");
    int32_t ret = screenCaptureServer_->StartFileInnerAudioCapture();
    CHECK_AND_RETURN_RET_LOG(ret == MSERR_OK, ret, "StartFileInnerAudioCapture failed, ret:%{public}d,"
        "dataType:%{public}d", ret, screenCaptureServer_->captureConfig_.dataType);
    ret = screenCaptureServer_->StartFileMicAudioCapture();
    if (ret != MSERR_OK) {
        MEDIA_LOGE("StartFileMicAudioCapture failed");
    }
    MEDIA_LOGI("StartAudioCapture end");
    return ret;
}

HWTEST_F(ScreenCaptureServerFunctionTest, OnVoIPStatusChanged_001, TestSize.Level2)
{
    RecorderInfo recorderInfo;
    SetRecorderInfo("screen_capture_voip_001.mp4", recorderInfo);
    SetConfigFile(recorderInfo);
    config_.audioInfo.micCapInfo.audioSampleRate = 16000;
    config_.audioInfo.micCapInfo.audioChannels = 2;
    config_.audioInfo.micCapInfo.audioSource = AudioCaptureSourceType::SOURCE_DEFAULT;
    config_.audioInfo.innerCapInfo.audioSampleRate = 16000;
    config_.audioInfo.innerCapInfo.audioChannels = 2;
    config_.audioInfo.innerCapInfo.audioSource = AudioCaptureSourceType::ALL_PLAYBACK;
    ASSERT_EQ(InitScreenCaptureServer(), MSERR_OK);
    screenCaptureServer_->SetMicrophoneEnabled(true);
    ASSERT_EQ(StartAudioCapture(), MSERR_OK);
    sleep(RECORDER_TIME);
    ASSERT_EQ(screenCaptureServer_->OnVoIPStatusChanged(true), MSERR_OK);
    sleep(RECORDER_TIME);
    ASSERT_EQ(screenCaptureServer_->OnVoIPStatusChanged(false), MSERR_OK);
    sleep(RECORDER_TIME);
    ASSERT_EQ(screenCaptureServer_->StopScreenCapture(), MSERR_OK);
    screenCaptureServer_->Release();
}

HWTEST_F(ScreenCaptureServerFunctionTest, OnVoIPStatusChanged_002, TestSize.Level2)
{
    RecorderInfo recorderInfo;
    SetRecorderInfo("screen_capture_voip_002.mp4", recorderInfo);
    SetConfigFile(recorderInfo);
    config_.audioInfo.micCapInfo.audioSampleRate = 16000;
    config_.audioInfo.micCapInfo.audioChannels = 2;
    config_.audioInfo.micCapInfo.audioSource = AudioCaptureSourceType::SOURCE_DEFAULT;
    config_.audioInfo.innerCapInfo.audioSampleRate = 16000;
    config_.audioInfo.innerCapInfo.audioChannels = 2;
    config_.audioInfo.innerCapInfo.audioSource = AudioCaptureSourceType::ALL_PLAYBACK;
    ASSERT_EQ(InitScreenCaptureServer(), MSERR_OK);
    screenCaptureServer_->SetMicrophoneEnabled(false);
    ASSERT_EQ(StartAudioCapture(), MSERR_OK);
    sleep(RECORDER_TIME);
    ASSERT_EQ(screenCaptureServer_->OnVoIPStatusChanged(true), MSERR_OK);
    sleep(RECORDER_TIME);
    ASSERT_EQ(screenCaptureServer_->OnVoIPStatusChanged(false), MSERR_OK);
    sleep(RECORDER_TIME);
    ASSERT_EQ(screenCaptureServer_->StopScreenCapture(), MSERR_OK);
    screenCaptureServer_->Release();
}
} // Media
} // OHOS