/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include "hiplayer_impl_unit_test.h"
#include "pipeline/pipeline.h"

namespace OHOS {
namespace Media {
using namespace std;
using namespace testing::ext;

namespace {
    const int64_t PLAY_RANGE_DEFAULT_VALUE = -1; // play range default value.
    const std::string MEDIA_ROOT = "file:///data/test/";
    const std::string VIDEO_FILE1 = MEDIA_ROOT + "H264_AAC.mp4";
}

void HiplayerImplUnitTest::SetUpTestCase(void)
{
}

void HiplayerImplUnitTest::TearDownTestCase(void)
{
}

void HiplayerImplUnitTest::SetUp(void)
{
    hiplayer_ = std::make_unique<HiPlayerImpl>(0, 0, 0, 0);
}

void HiplayerImplUnitTest::TearDown(void)
{
    hiplayer_ = nullptr;
}

/**
* @tc.name    : Test GetRealPath API
* @tc.number  : GetRealPath_001
* @tc.desc    : Test GetRealPath interface, set url to "file://".
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, GetRealPath_001, TestSize.Level0)
{
    // 1. Set up the test environment
    std::string url = "file://";
    std::string realUrlPath;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->GetRealPath(url, realUrlPath);

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_OPEN_FILE_FAILED);
}

/**
* @tc.name    : Test GetRealPath API
* @tc.number  : GetRealPath_002
* @tc.desc    : Test GetRealPath interface, set url to "file".
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, GetRealPath_002, TestSize.Level0)
{
    // 1. Set up the test environment
    std::string url = "file";
    std::string realUrlPath;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->GetRealPath(url, realUrlPath);

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_OPEN_FILE_FAILED);
}

/**
* @tc.name    : Test GetRealPath API
* @tc.number  : GetRealPath_003
* @tc.desc    : Test GetRealPath interface, set url to "file:///storage/../test.mp3".
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, GetRealPath_003, TestSize.Level0)
{
    // 1. Set up the test environment
    std::string url = "file:///storage/../test.mp3";
    std::string realUrlPath;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->GetRealPath(url, realUrlPath);

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_FILE_ACCESS_FAILED);
}

/**
* @tc.name    : Test SetSource API
* @tc.number  : SetSource_001
* @tc.desc    : Test SetSource interface, set uri to "".
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetSource_001, TestSize.Level0)
{
    // 1. Set up the test environment
    std::string uri = "";

    // 2. Call the function to be tested
    int32_t result = hiplayer_->SetSource(uri);

    // 3. Verify the result
    EXPECT_EQ(result, MSERR_OPEN_FILE_FAILED);
}

/**
* @tc.name    : Test SetSource API
* @tc.number  : SetSource_002
* @tc.desc    : Test SetSource interface, set uri to "file://".
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetSource_002, TestSize.Level0)
{
    // 1. Set up the test environment
    std::string uri = "file://";

    // 2. Call the function to be tested
    int32_t result = hiplayer_->SetSource(uri);

    // 3. Verify the result
    EXPECT_EQ(result, MSERR_OPEN_FILE_FAILED);
}

/**
* @tc.name    : Test SetSource API
* @tc.number  : SetSource_003
* @tc.desc    : Test SetSource interface, set uri to "http://".
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetSource_003, TestSize.Level0)
{
    // 1. Set up the test environment
    std::string uri = "http://";

    // 2. Call the function to be tested
    int32_t result = hiplayer_->SetSource(uri);

    // 3. Verify the result
    EXPECT_EQ(result, MSERR_OK);
}

/**
* @tc.name    : Test SetSource API
* @tc.number  : SetSource_004
* @tc.desc    : Test SetSource interface, set uri to "https://".
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetSource_004, TestSize.Level0)
{
    // 1. Set up the test environment
    std::string uri = "https://";

    // 2. Call the function to be tested
    int32_t result = hiplayer_->SetSource(uri);

    // 3. Verify the result
    EXPECT_EQ(result, MSERR_OK);
}

/**
* @tc.name    : Test SetSource API
* @tc.number  : SetSource_005
* @tc.desc    : Test SetSource interface, set uri to "invalid".
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetSource_005, TestSize.Level0)
{
    // 1. Set up the test environment
    std::string uri = "invalid";

    // 2. Call the function to be tested
    int32_t result = hiplayer_->SetSource(uri);

    // 3. Verify the result
    EXPECT_EQ(result, MSERR_OPEN_FILE_FAILED);
}

/**
* @tc.name    : Test SetMediaSource API
* @tc.number  : SetMediaSource_001
* @tc.desc    : Test SetMediaSource interface, set mediaSource to nullptr.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetMediaSource_001, TestSize.Level0)
{
    // 1. Set up the test environment
    std::shared_ptr<AVMediaSource> mediaSource = nullptr;
    AVPlayStrategy strategy;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->SetMediaSource(mediaSource, strategy);

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_INVALID_VAL);
}

/**
* @tc.name    : Test SetMediaSource API
* @tc.number  : SetMediaSource_002
* @tc.desc    : Test SetMediaSource interface, set url to file url.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetMediaSource_002, TestSize.Level0)
{
    // 1. Set up the test environment
    std::map<std::string, std::string> sourceHeader;
    std::shared_ptr<AVMediaSource> mediaSource = std::make_shared<AVMediaSource>("/storage/test.mp3", sourceHeader);
    AVPlayStrategy strategy;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->SetMediaSource(mediaSource, strategy);

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_OPEN_FILE_FAILED);
}

/**
* @tc.name    : Test SetMediaSource API
* @tc.number  : SetMediaSource_003
* @tc.desc    : Test SetMediaSource interface, set url to http url.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetMediaSource_003, TestSize.Level0)
{
    // 1. Set up the test environment
    std::map<std::string, std::string> sourceHeader;
    std::shared_ptr<AVMediaSource> mediaSource = std::make_shared<AVMediaSource>("http://test.mp3", sourceHeader);
    AVPlayStrategy strategy;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->SetMediaSource(mediaSource, strategy);

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_OK);
}

/**
* @tc.name    : Test SetMediaSource API
* @tc.number  : SetMediaSource_004
* @tc.desc    : Test SetMediaSource interface, set mimeType to AVMimeTypes::APPLICATION_M3U8.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, SetMediaSource_004, TestSize.Level0)
{
    // 1. Set up the test environment
    std::map<std::string, std::string> sourceHeader;
    std::shared_ptr<AVMediaSource> mediaSource = std::make_shared<AVMediaSource>("http://test.mp3", sourceHeader);
    mediaSource->mimeType_ = AVMimeTypes::APPLICATION_M3U8;
    AVPlayStrategy strategy;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->SetMediaSource(mediaSource, strategy);

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_OK);
}

/**
* @tc.name    : Test ResetIfSourceExisted API
* @tc.number  : ResetIfSourceExisted_001
* @tc.desc    : Test ResetIfSourceExisted interface, pipeline is not nullptr.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, ResetIfSourceExisted_001, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->demuxer_ = FilterFactory::Instance().CreateFilter<DemuxerFilter>("builtin.player.demuxer",
        FilterType::FILTERTYPE_DEMUXER);

    // 2. Call the function to be tested
    hiplayer_->ResetIfSourceExisted();

    // 3. Verify the result
    EXPECT_NE(hiplayer_->pipeline_, nullptr);
    EXPECT_EQ(hiplayer_->audioDecoder_, nullptr);
}

/**
* @tc.name    : Test ResetIfSourceExisted API
* @tc.number  : ResetIfSourceExisted_002
* @tc.desc    : Test ResetIfSourceExisted interface, audioDecoder is not nullptr.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, ResetIfSourceExisted_002, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->demuxer_ = FilterFactory::Instance().CreateFilter<DemuxerFilter>("builtin.player.demuxer",
        FilterType::FILTERTYPE_DEMUXER);
    hiplayer_->audioDecoder_ = FilterFactory::Instance().CreateFilter<AudioDecoderFilter>("player.audiodecoder",
        FilterType::FILTERTYPE_ADEC);

    hiplayer_->audioDecoder_->mediaCodec_ = std::make_shared<MediaCodec>();
    // 2. Call the function to be tested
    hiplayer_->ResetIfSourceExisted();

    // 3. Verify the result
    EXPECT_NE(hiplayer_->pipeline_, nullptr);
    EXPECT_EQ(hiplayer_->audioDecoder_, nullptr);
}


/**
* @tc.name    : Test DoInitializeForHttp API
* @tc.number  : DoInitializeForHttp_001
* @tc.desc    : Test DoInitializeForHttp interface, isNetWorkPlay_ is false.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, DoInitializeForHttp_001, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->isNetWorkPlay_ = false;

    // 2. Call the function to be tested
    hiplayer_->DoInitializeForHttp();

    // 3. Verify the result
    EXPECT_EQ(hiplayer_->isNetWorkPlay_, false);
}

/**
* @tc.name    : Test DoInitializeForHttp API
* @tc.number  : DoInitializeForHttp_002
* @tc.desc    : Test DoInitializeForHttp interface, GetBitRates return OK and vBitRates is not empty.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, DoInitializeForHttp_002, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->isNetWorkPlay_ = true;
    std::string name = "builtin.player.demuxer";
    std::shared_ptr<DemuxerFilterMock> demuxerMock =
        std::make_shared<DemuxerFilterMock>(name, FilterType::FILTERTYPE_DEMUXER);
    hiplayer_->demuxer_ = demuxerMock;
    demuxerMock->pushData_ = true;

    // 2. Call the function to be tested
    hiplayer_->DoInitializeForHttp();

    // 3. Verify the result
    EXPECT_EQ(hiplayer_->isNetWorkPlay_, true);
}

/**
* @tc.name    : Test DoInitializeForHttp API
* @tc.number  : DoInitializeForHttp_003
* @tc.desc    : Test DoInitializeForHttp interface, GetBitRates return not OK   or vBitRates is empty.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, DoInitializeForHttp_003, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->isNetWorkPlay_ = true;
    std::string name = "builtin.player.demuxer";
    std::shared_ptr<DemuxerFilterMock> demuxerMock =
        std::make_shared<DemuxerFilterMock>(name, FilterType::FILTERTYPE_DEMUXER);
    hiplayer_->demuxer_ = demuxerMock;
    demuxerMock->getBitRatesStatus_ = Status::ERROR_UNKNOWN;

    // 2. Call the function to be tested
    hiplayer_->DoInitializeForHttp();

    // 3. Verify the result
    EXPECT_EQ(hiplayer_->isNetWorkPlay_, true);
}

/**
* @tc.name    : Test DoInitializeForHttp API
* @tc.number  : DoInitializeForHttp_004
* @tc.desc    : Test DoInitializeForHttp interface, vBitRates is empty.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, DoInitializeForHttp_004, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->isNetWorkPlay_ = true;
    std::string name = "builtin.player.demuxer";
    std::shared_ptr<DemuxerFilterMock> demuxerMock =
        std::make_shared<DemuxerFilterMock>(name, FilterType::FILTERTYPE_DEMUXER);
    hiplayer_->demuxer_ = demuxerMock;
    demuxerMock->pushData_ = false;

    // 2. Call the function to be tested
    hiplayer_->DoInitializeForHttp();

    // 3. Verify the result
    EXPECT_EQ(hiplayer_->isNetWorkPlay_, true);
}

/**
* @tc.name    : Test Play API
* @tc.number  : Play_001
* @tc.desc    : Test Play interface, pipelineStates_ is PLAYER_PLAYBACK_COMPLETE.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, Play_001, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_PLAYBACK_COMPLETE;
    hiplayer_->isStreaming_ = false;
    hiplayer_->playRangeStartTime_ = 100;
    hiplayer_->playRangeEndTime_ = 200;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->Play();

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_INVALID_OPERATION);
    EXPECT_EQ(hiplayer_->isStreaming_, false);
}

/**
* @tc.name    : Test Play API
* @tc.number  : Play_002
* @tc.desc    : Test Play interface, pipelineStates_ is PLAYER_PAUSED.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, Play_002, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_PAUSED;
    hiplayer_->isStreaming_ = false;
    hiplayer_->playRangeStartTime_ = 100;
    hiplayer_->playRangeEndTime_ = 200;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->Play();

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_INVALID_OPERATION);
    EXPECT_EQ(hiplayer_->isStreaming_, false);
}

/**
* @tc.name    : Test Play API
* @tc.number  : Play_003
* @tc.desc    : Test Play interface, pipelineStates_ is other states.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, Play_003, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_PREPARING;
    hiplayer_->isStreaming_ = false;
    hiplayer_->playRangeStartTime_ = 100;
    hiplayer_->playRangeEndTime_ = 200;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->Play();

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_INVALID_OPERATION);
    EXPECT_EQ(hiplayer_->isStreaming_, false);
}

/**
* @tc.name    : Test Play API
* @tc.number  : Play_004
* @tc.desc    : Test Play interface, playRangeStartTime_ and playRangeEndTime_ is invalid.
* @tc.require : issueI5NZAQ
*/
HWTEST_F(HiplayerImplUnitTest, Play_004, TestSize.Level0)
{
    // 1. Set up the test environment
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_PLAYBACK_COMPLETE;
    hiplayer_->isStreaming_ = false;
    hiplayer_->playRangeStartTime_ = 200;
    hiplayer_->playRangeEndTime_ = 100;

    // 2. Call the function to be tested
    int32_t ret = hiplayer_->Play();

    // 3. Verify the result
    EXPECT_EQ(ret, MSERR_INVALID_OPERATION);
    EXPECT_EQ(hiplayer_->isStreaming_, false);
}

HWTEST_F(HiplayerImplUnitTest, GetCurrentTrack_001, TestSize.Level0)
{
    hiplayer_->demuxer_ = nullptr;
    hiplayer_->currentAudioTrackId_ = -1;
    hiplayer_->currentSubtitleTrackId_ = -1;
    hiplayer_->currentVideoTrackId_ = -1;
    EXPECT_EQ(hiplayer_->IsSubtitleMime("application/x-subrip"), true);
    EXPECT_EQ(hiplayer_->IsSubtitleMime("text/vtt"), true);

    int32_t index;
    EXPECT_EQ(hiplayer_->GetCurrentTrack(OHOS::Media::MediaType::MEDIA_TYPE_AUD, index), MSERR_UNKNOWN);
    EXPECT_EQ(hiplayer_->GetCurrentTrack(OHOS::Media::MediaType::MEDIA_TYPE_VID, index), MSERR_UNKNOWN);
    EXPECT_EQ(hiplayer_->GetCurrentTrack(OHOS::Media::MediaType::MEDIA_TYPE_SUBTITLE, index), MSERR_UNKNOWN);

    hiplayer_->currentAudioTrackId_ = 1;
    hiplayer_->currentSubtitleTrackId_ = 2;
    hiplayer_->currentVideoTrackId_ = 3;
    EXPECT_EQ(hiplayer_->GetCurrentTrack(OHOS::Media::MediaType::MEDIA_TYPE_AUD, index), MSERR_OK);
    EXPECT_EQ(hiplayer_->GetCurrentTrack(OHOS::Media::MediaType::MEDIA_TYPE_VID, index), MSERR_OK);
    EXPECT_EQ(hiplayer_->GetCurrentTrack(OHOS::Media::MediaType::MEDIA_TYPE_SUBTITLE, index), MSERR_OK);
}

HWTEST_F(HiplayerImplUnitTest, SelectTrack_001, TestSize.Level0)
{
    std::string name = "builtin.player.demuxer";
    std::shared_ptr<DemuxerFilterMock> demuxerMock =
        std::make_shared<DemuxerFilterMock>(name, FilterType::FILTERTYPE_DEMUXER);

    std::shared_ptr<Meta> meta1 = std::make_shared<Meta>();
    meta1->SetData(Tag::MIME_TYPE, "audio/xxx");
    std::shared_ptr<Meta> meta2 = std::make_shared<Meta>();
    meta2->SetData(Tag::MIME_TYPE, "video/xxx");
    std::shared_ptr<Meta> meta3 = std::make_shared<Meta>();
    meta3->SetData(Tag::MIME_TYPE, "text/vtt");
    std::shared_ptr<Meta> meta4 = std::make_shared<Meta>();

    demuxerMock->demuxer_->mediaMetaData_.trackMetas.push_back(meta1);
    demuxerMock->demuxer_->mediaMetaData_.trackMetas.push_back(meta2);
    demuxerMock->demuxer_->mediaMetaData_.trackMetas.push_back(meta3);
    demuxerMock->demuxer_->mediaMetaData_.trackMetas.push_back(meta4);

    hiplayer_->demuxer_ = demuxerMock;

    EXPECT_EQ(hiplayer_->SelectTrack(3, SWITCH_SMOOTH), MSERR_INVALID_VAL);
    hiplayer_->currentAudioTrackId_ = -1;
    hiplayer_->currentSubtitleTrackId_ = -1;
    hiplayer_->currentVideoTrackId_ = -1;
    EXPECT_EQ(hiplayer_->SelectTrack(0, SWITCH_SMOOTH), MSERR_UNKNOWN);
    EXPECT_EQ(hiplayer_->SelectTrack(1, SWITCH_SMOOTH), MSERR_UNKNOWN);
    EXPECT_EQ(hiplayer_->SelectTrack(2, SWITCH_SMOOTH), MSERR_UNKNOWN);

    hiplayer_->currentAudioTrackId_ = 101;
    hiplayer_->currentSubtitleTrackId_ = 102;
    hiplayer_->currentVideoTrackId_ = 103;
    EXPECT_EQ(hiplayer_->SelectTrack(0, SWITCH_SMOOTH), MSERR_UNKNOWN);
    EXPECT_EQ(hiplayer_->SelectTrack(1, SWITCH_SMOOTH), MSERR_UNKNOWN);
    EXPECT_EQ(hiplayer_->SelectTrack(2, SWITCH_SMOOTH), MSERR_UNKNOWN);
}

HWTEST_F(HiplayerImplUnitTest, DeselectTrack_001, TestSize.Level0)
{
    std::string name = "builtin.player.demuxer";
    std::shared_ptr<DemuxerFilterMock> demuxerMock =
        std::make_shared<DemuxerFilterMock>(name, FilterType::FILTERTYPE_DEMUXER);

    std::shared_ptr<Meta> meta1 = std::make_shared<Meta>();
    meta1->SetData(Tag::MIME_TYPE, "audio/xxx");
    std::shared_ptr<Meta> meta2 = std::make_shared<Meta>();
    meta2->SetData(Tag::MIME_TYPE, "video/xxx");
    std::shared_ptr<Meta> meta3 = std::make_shared<Meta>();
    meta3->SetData(Tag::MIME_TYPE, "text/vtt");
    std::shared_ptr<Meta> meta4 = std::make_shared<Meta>();

    demuxerMock->demuxer_->mediaMetaData_.trackMetas.push_back(meta1);
    demuxerMock->demuxer_->mediaMetaData_.trackMetas.push_back(meta2);
    demuxerMock->demuxer_->mediaMetaData_.trackMetas.push_back(meta3);
    demuxerMock->demuxer_->mediaMetaData_.trackMetas.push_back(meta4);

    hiplayer_->demuxer_ = demuxerMock;

    EXPECT_EQ(hiplayer_->DeselectTrack(3), MSERR_INVALID_VAL);
    hiplayer_->currentAudioTrackId_ = 0;
    hiplayer_->currentSubtitleTrackId_ = 2;
    hiplayer_->currentVideoTrackId_ = 1;

    hiplayer_->defaultAudioTrackId_ = 100;
    hiplayer_->defaultSubtitleTrackId_ = 102;
    hiplayer_->defaultVideoTrackId_ = 101;
    EXPECT_EQ(hiplayer_->DeselectTrack(0), MSERR_INVALID_VAL);
    EXPECT_EQ(hiplayer_->DeselectTrack(1), MSERR_INVALID_VAL);
    EXPECT_EQ(hiplayer_->DeselectTrack(2), MSERR_OK);
    EXPECT_EQ(hiplayer_->DeselectTrack(2), MSERR_OK);
}

HWTEST_F(HiplayerImplUnitTest, GetPlaybackInfo_001, TestSize.Level0)
{
    std::string name = "builtin.player.demuxer";
    std::shared_ptr<DemuxerFilterMock> demuxerMock =
        std::make_shared<DemuxerFilterMock>(name, FilterType::FILTERTYPE_DEMUXER);
    hiplayer_->demuxer_ = demuxerMock;
    hiplayer_->audioSink_ = nullptr;

    hiplayer_->OnEvent({"hiplayer", EventType::EVENT_IS_LIVE_STREAM, false});
    hiplayer_->OnEvent({"hiplayer", EventType::EVENT_READY, false});
    hiplayer_->OnEvent({"hiplayer", EventType::BUFFERING_END, 2});
    hiplayer_->OnEvent({"hiplayer", EventType::BUFFERING_START, 1});
    hiplayer_->OnEvent({"hiplayer", EventType::EVENT_CACHED_DURATION, 100});
    hiplayer_->OnEvent({"hiplayer", EventType::EVENT_BUFFER_PROGRESS, 100});
    hiplayer_->OnEvent({"hiplayer", EventType::EVENT_AUDIO_SERVICE_DIED, 1});

    Format playbackInfo;
    EXPECT_EQ(hiplayer_->GetPlaybackInfo(playbackInfo), 0);
    PlaybackRateMode mode;
    EXPECT_EQ(hiplayer_->GetPlaybackSpeed(mode), MSERR_OK);
    int32_t effectMode;
    EXPECT_EQ(hiplayer_->GetAudioEffectMode(effectMode), MSERR_OK);
}

HWTEST_F(HiplayerImplUnitTest, InitAudioDefaultTrackIndex_001, TestSize.Level0)
{
    hiplayer_->demuxer_ = nullptr;
    EXPECT_EQ(hiplayer_->InitAudioDefaultTrackIndex(), Status::ERROR_UNKNOWN);
    EXPECT_EQ(hiplayer_->InitVideoDefaultTrackIndex(), Status::ERROR_UNKNOWN);
    EXPECT_EQ(hiplayer_->InitSubtitleDefaultTrackIndex(), Status::ERROR_UNKNOWN);
    EXPECT_EQ(hiplayer_->Prepare(), MSERR_OK);
}

HWTEST_F(HiplayerImplUnitTest, ReleaseHiPlayerImplHasSubtitleSink_001, TestSize.Level0)
{
    hiplayer_->subtitleSink_ = std::make_shared<SubtitleSinkFilter>("Test_SubtitleSinkFilter",
        FilterType::FILTERTYPE_SSINK);
    hiplayer_->dfxAgent_ = nullptr;
    uint64_t testInstanceId = 0;
    hiplayer_->SetInstancdId(testInstanceId);
    hiplayer_->ReleaseInner();
    EXPECT_EQ(hiplayer_->subtitleSink_, nullptr);
}

HWTEST_F(HiplayerImplUnitTest, ReleaseHiPlayerImplHasNoDemuxer_001, TestSize.Level0)
{
    hiplayer_->demuxer_ = nullptr;
    hiplayer_->ReleaseInner();
    EXPECT_EQ(hiplayer_->demuxer_, nullptr);
}

HWTEST_F(HiplayerImplUnitTest, InitWithNoSyncManager_001, TestSize.Level0)
{
    hiplayer_->syncManager_ = nullptr;
    EXPECT_EQ(hiplayer_->Init(), Status::OK);
}

HWTEST_F(HiplayerImplUnitTest, SetDefaultAudioRenderInfoWithNullptrMeta_001, TestSize.Level0)
{
    std::shared_ptr<Meta> meta = std::make_shared<Meta>();
    std::vector<std::shared_ptr<Meta>> trackInfos{meta};

    hiplayer_->syncManager_ = nullptr;
    EXPECT_EQ(hiplayer_->Init(), Status::OK);
}

HWTEST_F(HiplayerImplUnitTest, ValidSeekTime_001, TestSize.Level0)
{
    hiplayer_->endTimeWithMode_ = 100;
    hiplayer_->startTimeWithMode_  = 0;
    int32_t validSeekTime = 10;
    EXPECT_EQ(hiplayer_->IsInValidSeekTime(validSeekTime), false);
}

HWTEST_F(HiplayerImplUnitTest, InValidSeekTime_001, TestSize.Level0)
{
    hiplayer_->endTimeWithMode_ = 100;
    hiplayer_->startTimeWithMode_  = 0;
    int32_t inValidSeekTime = PLAY_RANGE_DEFAULT_VALUE;
    EXPECT_EQ(hiplayer_->IsInValidSeekTime(inValidSeekTime), true);
}

HWTEST_F(HiplayerImplUnitTest, InValidSeekTime_002, TestSize.Level0)
{
    hiplayer_->endTimeWithMode_ = 100;
    hiplayer_->startTimeWithMode_  = PLAY_RANGE_DEFAULT_VALUE;
    int32_t inValidSeekTime = PLAY_RANGE_DEFAULT_VALUE;
    EXPECT_EQ(hiplayer_->IsInValidSeekTime(inValidSeekTime), false);
}

HWTEST_F(HiplayerImplUnitTest, GetPlayStartTime_001, TestSize.Level0)
{
    int32_t startTime = 100;
    hiplayer_->playRangeStartTime_ = startTime;
    EXPECT_EQ(hiplayer_->GetPlayStartTime(), startTime);
}

HWTEST_F(HiplayerImplUnitTest, GetPlayStartTimeWithValidTime_001, TestSize.Level0)
{
    int32_t defaultStartTime = 0;
    hiplayer_->playRangeStartTime_ = PLAY_RANGE_DEFAULT_VALUE;
    EXPECT_EQ(hiplayer_->GetPlayStartTime(), defaultStartTime);
}

HWTEST_F(HiplayerImplUnitTest, GetPlayStartTimeWithInValidTime_001, TestSize.Level0)
{
    hiplayer_->startTimeWithMode_ = 0;
    hiplayer_->endTimeWithMode_  = 100;
    hiplayer_->playRangeStartTime_ = PLAY_RANGE_DEFAULT_VALUE;
    EXPECT_EQ(hiplayer_->GetPlayStartTime(), hiplayer_->startTimeWithMode_);
}

HWTEST_F(HiplayerImplUnitTest, GetPlayStartTimeWithInValidTime_002, TestSize.Level0)
{
    int32_t defaultStartTime = 0;
    hiplayer_->startTimeWithMode_ = PLAY_RANGE_DEFAULT_VALUE;
    hiplayer_->endTimeWithMode_  = 100;
    hiplayer_->playRangeStartTime_ = PLAY_RANGE_DEFAULT_VALUE;
    EXPECT_EQ(hiplayer_->GetPlayStartTime(), defaultStartTime);
}

HWTEST_F(HiplayerImplUnitTest, GetPlayStartTimeWithInValidTime_003, TestSize.Level0)
{
    int32_t defaultStartTime = 0;
    hiplayer_->startTimeWithMode_ = defaultStartTime;
    hiplayer_->endTimeWithMode_  = PLAY_RANGE_DEFAULT_VALUE;
    hiplayer_->playRangeStartTime_ = PLAY_RANGE_DEFAULT_VALUE;
    EXPECT_EQ(hiplayer_->GetPlayStartTime(), defaultStartTime);
}

HWTEST_F(HiplayerImplUnitTest, SetPlayRangeWithMode_001, TestSize.Level0)
{
    int64_t start = -1;
    int64_t end = -1;
    PlayerSeekMode mode = PlayerSeekMode::SEEK_NEXT_SYNC;
    EXPECT_EQ(hiplayer_->SetPlayRangeWithMode(start, end, mode), MSERR_INVALID_VAL);
}

HWTEST_F(HiplayerImplUnitTest, SetPlayRangeWithMode_002, TestSize.Level0)
{
    int64_t start = 0;
    int64_t end = 100;
    PlayerSeekMode mode = PlayerSeekMode::SEEK_NEXT_SYNC;
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_INITIALIZED;
    EXPECT_EQ(hiplayer_->SetPlayRangeWithMode(start, end, mode), MSERR_OK);
}

HWTEST_F(HiplayerImplUnitTest, SetPlayRangeWithMode_003, TestSize.Level0)
{
    int64_t start = 0;
    int64_t end = 100;
    PlayerSeekMode mode = PlayerSeekMode::SEEK_NEXT_SYNC;
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_STOPPED;
    EXPECT_EQ(hiplayer_->SetPlayRangeWithMode(start, end, mode), MSERR_INVALID_VAL);
}

HWTEST_F(HiplayerImplUnitTest, SetPlayRangeWithMode_004, TestSize.Level0)
{
    int64_t start = 0;
    int64_t end = 100;
    PlayerSeekMode mode = PlayerSeekMode::SEEK_NEXT_SYNC;
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_IDLE;
    EXPECT_EQ(hiplayer_->SetPlayRangeWithMode(start, end, mode), MSERR_INVALID_VAL);
}

HWTEST_F(HiplayerImplUnitTest, SetPlayRangeWithMode_005, TestSize.Level0)
{
    int64_t start = 0;
    int64_t end = 100;
    PlayerSeekMode mode = PlayerSeekMode::SEEK_NEXT_SYNC;
    std::string url = "";
    hiplayer_->DoSetSource(std::make_shared<MediaSource>(url));
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_IDLE;
    EXPECT_EQ(hiplayer_->SetPlayRangeWithMode(start, end, mode), MSERR_INVALID_VAL);
}

HWTEST_F(HiplayerImplUnitTest, PrepareAsync_001, TestSize.Level0)
{
    int32_t ret = hiplayer_->SetSource(VIDEO_FILE1);
    EXPECT_EQ(MSERR_OK, ret);
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_INITIALIZED;
    EXPECT_EQ(MSERR_OK, hiplayer_->PrepareAsync());
}

HWTEST_F(HiplayerImplUnitTest, PrepareAsync_002, TestSize.Level0)
{
    int32_t ret = hiplayer_->SetSource(VIDEO_FILE1);
    EXPECT_EQ(MSERR_OK, ret);
    hiplayer_->pipelineStates_ = PlayerStates::PLAYER_STOPPED;
    EXPECT_EQ(MSERR_OK, hiplayer_->PrepareAsync());
}

HWTEST_F(HiplayerImplUnitTest, StopBufferring_001, TestSize.Level0)
{
    std::string url = VIDEO_FILE1;
    hiplayer_->DoSetSource(std::make_shared<MediaSource>(url));
    EXPECT_EQ(MSERR_OK, hiplayer_->StopBufferring(false));
}

HWTEST_F(HiplayerImplUnitTest, SetAudioEffectMode_001, TestSize.Level0)
{
    EXPECT_EQ(MSERR_OK, hiplayer_->SetAudioEffectMode(0));
}

HWTEST_F(HiplayerImplUnitTest, SetAudioEffectMode_002, TestSize.Level0)
{
    hiplayer_->audioSink_ = FilterFactory::Instance().CreateFilter<AudioSinkFilter>("player.audiosink",
        FilterType::FILTERTYPE_ASINK);
    EXPECT_EQ(MSERR_UNKNOWN, hiplayer_->SetAudioEffectMode(1));
}

HWTEST_F(HiplayerImplUnitTest, GetAudioEffectMode_001, TestSize.Level0)
{
    hiplayer_->audioSink_ = FilterFactory::Instance().CreateFilter<AudioSinkFilter>("player.audiosink",
        FilterType::FILTERTYPE_ASINK);
    int32_t effect = 1;
    EXPECT_EQ(MSERR_UNKNOWN, hiplayer_->GetAudioEffectMode(effect));
}

HWTEST_F(HiplayerImplUnitTest, SetPlaybackSpeed_001, TestSize.Level0)
{
    hiplayer_->subtitleSink_ = FilterFactory::Instance().CreateFilter<SubtitleSinkFilter>("player.subtitlesink",
        FilterType::FILTERTYPE_SSINK);
    hiplayer_->syncManager_ = nullptr;
    EXPECT_EQ(MSERR_OK, hiplayer_->SetPlaybackSpeed(PlaybackRateMode::SPEED_FORWARD_2_00_X));
}

HWTEST_F(HiplayerImplUnitTest, GetSarVideoWidth_001, TestSize.Level0)
{
    double videoSar = 0;
    double width = 1;
    std::shared_ptr<Meta> trackInfo = std::make_shared<Meta>();
    
    trackInfo->SetData(Tag::VIDEO_SAR, videoSar);
    trackInfo->SetData(Tag::VIDEO_WIDTH, width);
    EXPECT_EQ(0, hiplayer_->GetSarVideoWidth(trackInfo));
}

HWTEST_F(HiplayerImplUnitTest, GetSarVideoHeight_001, TestSize.Level0)
{
    std::shared_ptr<Meta> trackInfo = std::make_shared<Meta>();
    double videoSar = 2;
    int32_t height = 2;
    trackInfo->SetData(Tag::VIDEO_SAR, videoSar);
    trackInfo->SetData(Tag::VIDEO_HEIGHT, height);
    EXPECT_EQ(1, hiplayer_->GetSarVideoHeight(trackInfo));
}

} // namespace Media
} // namespace OHOS