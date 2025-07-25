/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "recorder_unit_test.h"
#include <fcntl.h>
#include <nativetoken_kit.h>
#include <token_setproc.h>
#include <accesstoken_kit.h>
#include "media_errors.h"
#include "media_log.h"

using namespace OHOS;
using namespace OHOS::Media;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Media::RecorderTestParam;
using namespace Security::AccessToken;
namespace OHOS {
namespace Media {
// config for video to request buffer from surface
static VideoRecorderConfig g_videoRecorderConfig;

// HapParams for permission
static HapInfoParams hapInfo = {
    .userID = 100, // 100 user ID
    .bundleName = "com.ohos.test.recordertdd",
    .instIndex = 0, // 0 index
    .appIDDesc = "com.ohos.test.recordertdd",
    .isSystemApp = true
};

static HapPolicyParams hapPolicy = {
    .apl = APL_SYSTEM_BASIC,
    .domain = "test.avrecorder",
    .permList = { },
    .permStateList = {
        {
            .permissionName = "ohos.permission.MICROPHONE",
            .isGeneral = true,
            .resDeviceID = { "local" },
            .grantStatus = { PermissionState::PERMISSION_GRANTED },
            .grantFlags = { 1 }
        },
        {
            .permissionName = "ohos.permission.READ_MEDIA",
            .isGeneral = true,
            .resDeviceID = { "local" },
            .grantStatus = { PermissionState::PERMISSION_GRANTED },
            .grantFlags = { 1 }
        },
        {
            .permissionName = "ohos.permission.WRITE_MEDIA",
            .isGeneral = true,
            .resDeviceID = { "local" },
            .grantStatus = { PermissionState::PERMISSION_GRANTED },
            .grantFlags = { 1 }
        },
        {
            .permissionName = "ohos.permission.KEEP_BACKGROUND_RUNNING",
            .isGeneral = true,
            .resDeviceID = { "local" },
            .grantStatus = { PermissionState::PERMISSION_GRANTED },
            .grantFlags = { 1 }
        },
        {
            .permissionName = "ohos.permission.DUMP",
            .isGeneral = true,
            .resDeviceID = { "local" },
            .grantStatus = { PermissionState::PERMISSION_GRANTED },
            .grantFlags = { 1 }
        }
    }
};

void RecorderUnitTest::SetUpTestCase(void)
{
    SetSelfTokenPremission();
}

void RecorderUnitTest::TearDownTestCase(void) {}

void RecorderUnitTest::SetUp(void)
{
    recorder_ = std::make_shared<RecorderMock>();
    ASSERT_NE(nullptr, recorder_);
    ASSERT_TRUE(recorder_->CreateRecorder());
}

void RecorderUnitTest::TearDown(void)
{
    if (recorder_ != nullptr) {
        recorder_->Release();
    }
}

void RecorderUnitTest::SetSelfTokenPremission()
{
    AccessTokenIDEx tokenIdEx = { 0 };
    tokenIdEx = AccessTokenKit::AllocHapToken(hapInfo, hapPolicy);
    int ret = SetSelfTokenID(tokenIdEx.tokenIDEx);
    if (ret != 0) {
        MEDIA_LOGE("Set hap token failed, err: %{public}d", ret);
    }
}


/**
 * @tc.name: recorder_GetCurrentCapturerChangeInfo_001
 * @tc.desc: recorder_GetCurrentCapturerChangeInfo_001
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_GetCurrentCapturerChangeInfo_001, TestSize.Level0)
{
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
    "recorder_GetCurrentCapturerChangeInfo_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    AudioRecorderChangeInfo changeInfo;
    EXPECT_EQ(MSERR_OK, recorder_->GetCurrentCapturerChangeInfo(changeInfo));
    ASSERT_TRUE(changeInfo.capturerInfo.sourceType == g_videoRecorderConfig.aSource);
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetLocation_001
 * @tc.desc: record video with setLocation
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetLocation_001, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetLocation_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    recorder_->SetLocation(1, 1);
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_Repeat_001
 * @tc.desc: record video with Repeat
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_Repeat_001, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_Repeat_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_INVALID_OPERATION, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_INVALID_OPERATION, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    EXPECT_EQ(MSERR_INVALID_OPERATION, recorder_->Pause());
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_INVALID_OPERATION, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetFileSplitDuration_001
 * @tc.desc: record video with SetFileSplitDuration
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetFileSplitDuration_001, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
            "recorder_video_SetFileSplitDuration_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    recorder_->SetFileSplitDuration(FileSplitType::FILE_SPLIT_POST, -1, 1000);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetAudioEncoder_Error_001
 * @tc.desc: record video with SetAudioEncoder
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetAudioEncoder_Error_001, TestSize.Level2)
{
    g_videoRecorderConfig.audioSourceId = 0;
    g_videoRecorderConfig.audioFormat = AUDIO_DEFAULT;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
            "recorder_video_SetAudioEncoder_Error_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_NE(MSERR_OK,
              recorder_->SetAudioEncoder(g_videoRecorderConfig.audioSourceId, g_videoRecorderConfig.audioFormat));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_GetSurface_Error_001
 * @tc.desc: record video with GetSurface
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_GetSurface_Error_001, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_GetSurface_Error_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    OHOS::sptr<OHOS::Surface> surface = recorder_->GetSurface(2);
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetVideoSourceRepeat_001
 * @tc.desc: record video with SetFileSplitDuration
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetVideoSourceRepeat_001, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
            "recorder_video_SetVideoSourceRepeat_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    int32_t videoSourceIdTwo = 0;
    EXPECT_NE(MSERR_OK, recorder_->SetVideoSource(VIDEO_SOURCE_SURFACE_ES, videoSourceIdTwo));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetAudioSourceRepeat_001
 * @tc.desc: record video with SetFileSplitDuration
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetAudioSourceRepeat_001, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.aSource = AUDIO_SOURCE_DEFAULT;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
            "recorder_video_SetAudioSourceRepeat_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    int32_t audioSourceIdTwo = 0;
    EXPECT_NE(MSERR_OK, recorder_->SetAudioSource(AUDIO_SOURCE_DEFAULT, audioSourceIdTwo));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetAudioSourceRepeat_002
 * @tc.desc: record video with AudioSourceRepeat
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetAudioSourceRepeat_002, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.aSource = AUDIO_SOURCE_DEFAULT;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
            "recorder_video_SetAudioSourceRepeat_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    int32_t audioSourceId = 0;
    EXPECT_NE(MSERR_OK, recorder_->SetAudioSource(AUDIO_SOURCE_INVALID, audioSourceId));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_DrainBufferTrue_001
 * @tc.desc: record video with DrainBufferTrue, stop true
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_DrainBufferTrue_001, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.outPutFormat = FORMAT_DEFAULT;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_DrainBufferTrue_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(true));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetVideoSourceRGBA_001
 * @tc.desc: record video with SetVideoSourceRGBA
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetVideoSourceRGBA_001, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_RGBA;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.outPutFormat = FORMAT_DEFAULT;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetVideoSourceRGBA_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_DrainBufferStarted_001
 * @tc.desc: record video with DrainBufferStarted, stop after pause
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_DrainBufferStarted_001, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.outPutFormat = FORMAT_DEFAULT;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_DrainBufferStarted_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_001
 * @tc.desc: record with sampleRate -1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_001, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.sampleRate = -1;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_002
 * @tc.desc: record with channelCount -1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_002, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.channelCount = -1;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_003
 * @tc.desc: record with audioEncodingBitRate -1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_003, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.audioEncodingBitRate = -1;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_004
 * @tc.desc: record with videoFormat VIDEO_CODEC_FORMAT_BUTT
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_004, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = VIDEO_CODEC_FORMAT_BUTT;
    videoRecorderConfig.audioEncodingBitRate = -1;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_005
 * @tc.desc: record with width -1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_005, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.width = -1;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_006
 * @tc.desc: record with height -1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_006, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.height = -1;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_007
 * @tc.desc: record with frameRate -1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_007, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.frameRate = -1;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_008
 * @tc.desc: record with videoEncodingBitRate -1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_008, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.videoEncodingBitRate = -1;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_009
 * @tc.desc: record with videoFormat VIDEO_CODEC_FORMAT_BUTT
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_009, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = VIDEO_CODEC_FORMAT_BUTT;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_010
 * @tc.desc: record with videoFormat VIDEO_CODEC_FORMAT_BUTT
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_010, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_error.mp4").c_str(), O_RDWR);

    EXPECT_NE(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_011
 * @tc.desc: record with videoFormat FORMAT_BUTT
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_011, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.outPutFormat = FORMAT_BUTT;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);

    EXPECT_NE(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_012
 * @tc.desc: record with videoFormat FORMAT_DEFAULT
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_012, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.outPutFormat = FORMAT_DEFAULT;
    videoRecorderConfig.enableBFrame = false;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_013
 * @tc.desc: record
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_013, TestSize.Level2)
{
    EXPECT_NE(MSERR_OK, recorder_->Prepare());

    EXPECT_NE(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_NE(MSERR_OK, recorder_->Pause());
    EXPECT_NE(MSERR_OK, recorder_->Resume());
    EXPECT_NE(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
}

/**
 * @tc.name: recorder_configure_014
 * @tc.desc: record with enableBFrame and enableTemporalScale true
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_014, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.enableTemporalScale = true;
    videoRecorderConfig.enableBFrame = true;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_015
 * @tc.desc: record with audioCodec mp3 + fileFormat mp3
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_015, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_MPEG;
    videoRecorderConfig.outPutFormat = FORMAT_MP3;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_015.mp3").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_016
 * @tc.desc: record with audioCodec mp3 + fileFormat mp4
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_016, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_MPEG;
    videoRecorderConfig.outPutFormat = FORMAT_MPEG_4;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_016.mp3").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_NE(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_017
 * @tc.desc: record with audioCodec mp3 + fileFormat m4a
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_017, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_MPEG;
    videoRecorderConfig.outPutFormat = FORMAT_M4A;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_017.mp3").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_NE(MSERR_OK, recorder_->Start());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_018
 * @tc.desc: record mp3 with samplerate 64000
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_018, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_MPEG;
    videoRecorderConfig.outPutFormat = FORMAT_MP3;
    videoRecorderConfig.sampleRate = 64000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_018.mp3").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_019
 * @tc.desc: record wav with samplerate 64000
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_019, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_G711MU;
    videoRecorderConfig.outPutFormat = FORMAT_WAV;
    videoRecorderConfig.audioEncodingBitRate = 64000;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 64000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_019.wav").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_020
 * @tc.desc: record wav with BitRate 128000
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_020, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_G711MU;
    videoRecorderConfig.outPutFormat = FORMAT_WAV;
    videoRecorderConfig.audioEncodingBitRate = 128000;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 8000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_020.wav").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_NE(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_021
 * @tc.desc: record wav with channelCount 2
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_021, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_G711MU;
    videoRecorderConfig.outPutFormat = FORMAT_WAV;
    videoRecorderConfig.audioEncodingBitRate = 64000;
    videoRecorderConfig.channelCount = 2;
    videoRecorderConfig.sampleRate = 8000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_021.wav").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_configure_022
 * @tc.desc: record wav with channelCount 2
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_022, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_CODEC_FORMAT_BUTT;
    videoRecorderConfig.outPutFormat = FORMAT_WAV;
    videoRecorderConfig.audioEncodingBitRate = 64000;
    videoRecorderConfig.channelCount = 2;
    videoRecorderConfig.sampleRate = 8000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_022.wav").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

#ifdef SUPPORT_CODEC_TYPE_HEVC
/**
 * @tc.name: recorder_configure_023
 * @tc.desc: record audioFormat = AUDIO_AMR_NB, outPutFormat = FORMAT_AMR, audioEncodingBitRate = 4750;
 *           not support rk3568
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_023, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_AMR_NB;
    videoRecorderConfig.outPutFormat = FORMAT_AMR;
    videoRecorderConfig.audioEncodingBitRate = 4750;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 8000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_023.amr").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);
    // correct config
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    
    // incorrect config
    videoRecorderConfig.audioEncodingBitRate = 2750;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 8000;
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    
    videoRecorderConfig.audioEncodingBitRate = 4750;
    videoRecorderConfig.channelCount = 2;
    videoRecorderConfig.sampleRate = 8000;
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    
    videoRecorderConfig.audioEncodingBitRate = 4750;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 16000;
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}
#endif

#ifdef SUPPORT_CODEC_TYPE_HEVC
/**
 * @tc.name: recorder_configure_024
 * @tc.desc: record audioFormat = AUDIO_AMR_WB, outPutFormat = FORMAT_AMR, audioEncodingBitRate = 6600;
 *           not support rk3568
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_configure_024, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_AMR_WB;
    videoRecorderConfig.outPutFormat = FORMAT_AMR;
    videoRecorderConfig.audioEncodingBitRate = 6600;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 16000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_configure_024.amr").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);
    // incorrect config
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    
    // incorrect config
    videoRecorderConfig.audioEncodingBitRate = 2600;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 16000;
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    
    videoRecorderConfig.audioEncodingBitRate = 6600;
    videoRecorderConfig.channelCount = 2;
    videoRecorderConfig.sampleRate = 16000;
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    
    videoRecorderConfig.audioEncodingBitRate = 6600;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 26000;
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_NE(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}
#endif

/**
 * @tc.name: recorder_mp3_001
 * @tc.desc: record mp3
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_mp3_001, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_MPEG;
    videoRecorderConfig.outPutFormat = FORMAT_MP3;
    videoRecorderConfig.audioEncodingBitRate = 64000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_mp3_001.mp3").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_G711MU_001
 * @tc.desc: record G711MU
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_G711MU_001, TestSize.Level2)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.audioFormat = AUDIO_G711MU;
    videoRecorderConfig.outPutFormat = FORMAT_WAV;
    videoRecorderConfig.audioEncodingBitRate = 64000;
    videoRecorderConfig.channelCount = 1;
    videoRecorderConfig.sampleRate = 8000;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_WAV_001.wav").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_prepare
 * @tc.desc: record prepare
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_prepare, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_prepare.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_yuv_H264
 * @tc.desc: record video with yuv H264
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_yuv_H264, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_yuv_H264.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    system("hidumper -s 3002 -a recorder");
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_es
 * @tc.desc: record video with es
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_es, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_ES;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_es.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_audio_es
 * @tc.desc: record audio with es
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_audio_es, TestSize.Level0)
{
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_audio_es.m4a").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_audio_es_0100
 * @tc.desc: record audio with es
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_audio_es_0100, TestSize.Level0)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.outPutFormat = FORMAT_M4A;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_audio_es.m4a").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_av_yuv_H264
 * @tc.desc: record audio with yuv H264
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_av_yuv_H264, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_av_yuv_H264.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_av_yuv_h264
 * @tc.desc: record audio with yuv h264
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_av_yuv_h264, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_av_yuv_h264.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    recorder_->Start();
    sleep(RECORDER_TIME);
    recorder_->Stop(false);
    recorder_->StopBuffer(PURE_VIDEO);
    recorder_->Reset();
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_pause_resume
 * @tc.desc: record video, then pause resume
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_pause_resume, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_pause_resume.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME / 2);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_stop_start
 * @tc.desc: record video, then stop start
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_stop_start, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.audioFormat = AUDIO_DEFAULT;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_stop_start.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_NE(MSERR_OK, recorder_->Start());
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_stop_start
 * @tc.desc: record video, then stop start
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_wrongsize, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_wrongsize.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_ERROR, g_videoRecorderConfig));
    recorder_->Start();
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    recorder_->Reset();
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetOrientationHint_001
 * @tc.desc: record video, SetOrientationHint
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetOrientationHint_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
    "recorder_video_SetOrientationHint_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetLocation(1, 1);
    recorder_->SetOrientationHint(90);
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetOrientationHint_002
 * @tc.desc: record video, SetOrientationHint
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetOrientationHint_002, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_video_SetOrientationHint_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetLocation(-91, 0);
    recorder_->SetOrientationHint(720);
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetOrientationHint_003
 * @tc.desc: record video, SetOrientationHint
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetOrientationHint_003, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_video_SetOrientationHint_003.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetLocation(91, 0);
    recorder_->SetOrientationHint(180);
    system("param set sys.media.dump.surfacesrc.enable true");
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetOrientationHint_004
 * @tc.desc: record video, SetOrientationHint
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetOrientationHint_004, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_video_SetOrientationHint_004.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetLocation(1, 181);
    recorder_->SetLocation(1, -181);
    recorder_->SetOrientationHint(270);
    system("param set sys.media.dump.surfacesrc.enable false");
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_UpdateRotation_001
 * @tc.desc: record video, Update rotation
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_UpdateRotation_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_video_UpdateRotation_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    system("param set sys.media.dump.surfacesrc.enable false");
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetOrientationHint(0);
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_UpdateRotation_002
 * @tc.desc: record video, Update rotation
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_UpdateRotation_002, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_video_UpdateRotation_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    system("param set sys.media.dump.surfacesrc.enable false");
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetOrientationHint(90);
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_UpdateRotation_003
 * @tc.desc: record video, Update rotation
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_UpdateRotation_003, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_video_UpdateRotation_003.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    system("param set sys.media.dump.surfacesrc.enable false");
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetOrientationHint(180);
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_UpdateRotation_004
 * @tc.desc: record video, Update rotation
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_UpdateRotation_004, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_video_UpdateRotation_004.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    system("param set sys.media.dump.surfacesrc.enable false");
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetOrientationHint(270);
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetCaptureRate_001
 * @tc.desc: record video ,SetCaptureRate
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetCaptureRate_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetCaptureRate_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    recorder_->SetCaptureRate(0, -0.1);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetMaxFileSize_001
 * @tc.desc: record video ,SetMaxFileSize
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetMaxFileSize_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetMaxFileSize_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetCaptureRate(0, 30);
    recorder_->SetMaxFileSize(-1);
    recorder_->SetMaxFileSize(5000);
    recorder_->SetNextOutputFile(g_videoRecorderConfig.outputFd);
    recorder_->SetFileSplitDuration(FileSplitType::FILE_SPLIT_POST, -1, 1000);
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetMaxFileSize_002
 * @tc.desc: record video ,SetMaxFileSize
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetMaxFileSize_002, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetMaxFileSize_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    recorder_->SetMaxFileSize(-1);
    recorder_->SetNextOutputFile(g_videoRecorderConfig.outputFd);
    recorder_->SetFileSplitDuration(FileSplitType::FILE_SPLIT_POST, -1, 1000);
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_GetAvailableEncoder_001
 * @tc.desc: record GetAvailableEncoder
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_GetAvailableEncoder_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = MPEG4;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_GetAvailableEncoder_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    std::vector<EncoderCapabilityData> encoderInfo;
    EXPECT_EQ(MSERR_OK, recorder_->GetAvailableEncoder(encoderInfo));
    EXPECT_NE(0, encoderInfo.size());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_GetAvailableEncoder_002
 * @tc.desc: record GetAvailableEncoder
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_GetAvailableEncoder_002, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H265;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_GetAvailableEncoder_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    std::vector<EncoderCapabilityData> encoderInfo;
    EXPECT_EQ(MSERR_OK, recorder_->GetAvailableEncoder(encoderInfo));
    EXPECT_EQ(MSERR_OK, recorder_->GetAvailableEncoder(encoderInfo));
    EXPECT_NE(0, encoderInfo.size());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_IsWatermarkSupported_001
 * @tc.desc: record IsWatermarkSupported
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_IsWatermarkSupported_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_IsWatermarkSupported_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    bool isWatermarkSupported = false;
    EXPECT_EQ(MSERR_OK, recorder_->IsWatermarkSupported(isWatermarkSupported));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_IsWatermarkSupported_002
 * @tc.desc: record IsWatermarkSupported
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_IsWatermarkSupported_002, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H265;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_IsWatermarkSupported_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    bool isWatermarkSupported = false;
    EXPECT_EQ(MSERR_OK, recorder_->IsWatermarkSupported(isWatermarkSupported));
    EXPECT_EQ(MSERR_OK, recorder_->IsWatermarkSupported(isWatermarkSupported));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetVideoIsHdr_001
 * @tc.desc: record SetVideoIsHdr
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetVideoIsHdr_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H265;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetVideoIsHdr_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    bool isHdr = false;
    EXPECT_EQ(MSERR_OK, recorder_->SetVideoIsHdr(g_videoRecorderConfig.videoSourceId, isHdr));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetVideoIsHdr_002
 * @tc.desc: record SetVideoIsHdr
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetVideoIsHdr_002, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H265;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetVideoIsHdr_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    bool isHdr = true;
    EXPECT_EQ(MSERR_OK, recorder_->SetVideoIsHdr(g_videoRecorderConfig.videoSourceId, isHdr));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetParameter_001
 * @tc.desc: record video, SetParameter
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetParameter_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetParameter_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_VIDEO, g_videoRecorderConfig));
    Format format;
    format.PutIntValue("SetParameter", 0);
    EXPECT_EQ(MSERR_INVALID_OPERATION, recorder_->SetParameter(0, format));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetDataSource_001
 * @tc.desc: record video, SetDataSource
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetDataSource_001, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetDataSource_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);
    EXPECT_EQ(MSERR_INVALID_OPERATION,
        recorder_->SetDataSource(DataSourceType::METADATA, g_videoRecorderConfig.videoSourceId));
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetGenre_001
 * @tc.desc: record video, SetGenre
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetGenre_001, TestSize.Level0)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.outPutFormat = FORMAT_DEFAULT;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetGenre_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    recorder_->SetGenre(videoRecorderConfig.genre);
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetGenre_002
 * @tc.desc: record audio SetGenre
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetGenre_002, TestSize.Level0)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.outPutFormat = FORMAT_M4A;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetGenre_002.m4a").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    recorder_->SetGenre(videoRecorderConfig.genre);
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetCustomInfo_001
 * @tc.desc: record video, SetCustomInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetCustomInfo_001, TestSize.Level0)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    videoRecorderConfig.videoFormat = H264;
    videoRecorderConfig.outPutFormat = FORMAT_DEFAULT;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetCustomInfo_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, videoRecorderConfig));
    Meta customInfo;
    customInfo.SetData("key", "value");
    recorder_->SetUserCustomInfo(customInfo);
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetCustomInfo_002
 * @tc.desc: record audio SetCustomInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetCustomInfo_002, TestSize.Level0)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.outPutFormat = FORMAT_M4A;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetCustomInfo_002.m4a").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    Meta customInfo;
    customInfo.SetData("key", "value");
    recorder_->SetUserCustomInfo(customInfo);
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_GetMetaSurface
 * @tc.desc: record video with meta data
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_GetMetaSurface, TestSize.Level0)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.metaSourceType = VIDEO_META_MAKER_INFO;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_GetMetaSurface.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    OHOS::sptr<OHOS::Surface> surface = recorder_->GetMetaSurface(g_videoRecorderConfig.metaSourceId);
    ASSERT_TRUE(surface != nullptr);
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetAudioSourceType_001
 * @tc.desc: record video source as voice recognition
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetAudioSourceType_001, TestSize.Level2)
{
    g_videoRecorderConfig.aSource = AUDIO_SOURCE_VOICE_RECOGNITION;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetAudioSourceType_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetAudioSourceType_003
 * @tc.desc: record video source as voice message
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetAudioSourceType_003, TestSize.Level2)
{
    g_videoRecorderConfig.aSource = AUDIO_SOURCE_VOICE_MESSAGE;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetAudioSourceType_003.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetMaxDuration_001
 * @tc.desc: record set max duration is undefined
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetMaxDuration_001, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetMaxDuration_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetMaxDuration_002
 * @tc.desc: record set max duration -1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetMaxDuration_002, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.maxDuration = -1;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetMaxDuration_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetMaxDuration_003
 * @tc.desc: record set max duration 0
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetMaxDuration_003, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.maxDuration = 0;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetMaxDuration_003.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetMaxDuration_004
 * @tc.desc: record set max duration 1
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetMaxDuration_004, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.maxDuration = 1;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetMaxDuration_004.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetMaxDuration_005
 * @tc.desc: record set max duration 5
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetMaxDuration_005, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.maxDuration = 5;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetMaxDuration_005.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetMaxDuration_006
 * @tc.desc: record set max duration but stop first
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetMaxDuration_006, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.maxDuration = INT32_MAX;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetMaxDuration_006.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetMaxDuration_007
 * @tc.desc: record set max duration, pause, resume
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetMaxDuration_007, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.maxDuration = INT32_MAX;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_SetMaxDuration_007.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));

    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME / 2);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    sleep(RECORDER_TIME);
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetVideoEnableStableQualityMode_001
 * @tc.desc: enableStableQualityMode with default value
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetVideoEnableStableQualityMode_001, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_SetVideoEnableStableQualityMode_001.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME/2);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME/2);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_SetVideoEnableStableQualityMode_002
 * @tc.desc: enableStableQualityMode sets to false while enableTemporalScale is true
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_SetVideoEnableStableQualityMode_002, TestSize.Level2)
{
    g_videoRecorderConfig.vSource = VIDEO_SOURCE_SURFACE_YUV;
    g_videoRecorderConfig.videoFormat = H264;
    g_videoRecorderConfig.enableTemporalScale = true;
    g_videoRecorderConfig.enableStableQualityMode = false;
    g_videoRecorderConfig.outputFd = open((RECORDER_ROOT +
        "recorder_SetVideoEnableStableQualityMode_002.mp4").c_str(), O_RDWR);
    ASSERT_TRUE(g_videoRecorderConfig.outputFd >= 0);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->RequesetBuffer(AUDIO_VIDEO, g_videoRecorderConfig));
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    sleep(RECORDER_TIME/2);
    EXPECT_EQ(MSERR_OK, recorder_->Pause());
    sleep(RECORDER_TIME/2);
    EXPECT_EQ(MSERR_OK, recorder_->Resume());
    EXPECT_EQ(MSERR_OK, recorder_->Stop(false));
    recorder_->StopBuffer(PURE_VIDEO);
    EXPECT_EQ(MSERR_OK, recorder_->Reset());
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(g_videoRecorderConfig.outputFd);
}

/**
 * @tc.name: recorder_video_SetWillMuteWhenInterrupted_001
 * @tc.desc: set mute when interrupted before prepare
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(RecorderUnitTest, recorder_video_SetWillMuteWhenInterrupted_001, TestSize.Level0)
{
    VideoRecorderConfig videoRecorderConfig;
    videoRecorderConfig.outPutFormat = FORMAT_M4A;
    videoRecorderConfig.outputFd = open((RECORDER_ROOT + "recorder_video_SetGenre_002.m4a").c_str(), O_RDWR);
    ASSERT_TRUE(videoRecorderConfig.outputFd >= 0);
    ASSERT_NE(recorder_, nullptr);

    EXPECT_EQ(MSERR_OK, recorder_->SetFormat(PURE_AUDIO, videoRecorderConfig));
    int32_t ret = recorder_->SetWillMuteWhenInterrupted(true);
    EXPECT_EQ(MSERR_OK, ret);
    EXPECT_EQ(MSERR_OK, recorder_->Prepare());
    EXPECT_EQ(MSERR_OK, recorder_->Start());
    ret = recorder_->SetWillMuteWhenInterrupted(true);
    EXPECT_EQ(MSERR_INVALID_OPERATION, ret);
    EXPECT_EQ(MSERR_OK, recorder_->Release());
    close(videoRecorderConfig.outputFd);
}
} // namespace Media
} // namespace OHOS