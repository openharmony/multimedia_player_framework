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

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "aw_common.h"
#include "string_ex.h"
#include "media_errors.h"
#include "directory_ex.h"
#include "screen_capture.h"
#include "screencaptureaudiosourcetype_fuzzer.h"

using namespace std;
using namespace OHOS;
using namespace Media;

namespace OHOS {
namespace Media {
ScreenCaptureAudioSourceTypeFuzzer::ScreenCaptureAudioSourceTypeFuzzer()
{
}

ScreenCaptureAudioSourceTypeFuzzer::~ScreenCaptureAudioSourceTypeFuzzer()
{
}

void SetConfig(AVScreenCaptureConfig &config)
{
    AudioCaptureInfo miccapinfo = {
        .audioSampleRate = 48000,
        .audioChannels = 2,
        .audioSource = SOURCE_DEFAULT
    };

    VideoCaptureInfo videocapinfo = {
        .videoFrameWidth = 720,
        .videoFrameHeight = 1280,
        .videoSource = VIDEO_SOURCE_SURFACE_RGBA
    };

    AudioInfo audioinfo = {
        .micCapInfo = miccapinfo,
    };

    VideoInfo videoinfo = {
        .videoCapInfo = videocapinfo
    };

    config = {
        .captureMode = CAPTURE_HOME_SCREEN,
        .dataType = ORIGINAL_STREAM,
        .audioInfo = audioinfo,
        .videoInfo = videoinfo,
    };
}

bool ScreenCaptureAudioSourceTypeFuzzer::FuzzScreenCaptureAudioSourceType(uint8_t *data, size_t size)
{
    if (data == nullptr || size < sizeof(AudioCaptureSourceType)) {
        return false;
    }
    bool retFlags = TestScreenCapture::CreateScreenCapture();
    RETURN_IF(retFlags, false);

    AVScreenCaptureConfig config;
    SetConfig(config);
    constexpr int32_t audioSourceTypesList = 5;
    constexpr uint32_t recorderTime = 3;
    const AudioCaptureSourceType audioSourceType[audioSourceTypesList] {
        SOURCE_INVALID,
        SOURCE_DEFAULT,
        MIC,
        ALL_PLAYBACK,
        APP_PLAYBACK,
    };
    int32_t asourcesubscript = abs(*reinterpret_cast<int32_t *>(data) % (audioSourceTypesList));
    config.audioInfo.micCapInfo.audioSource = audioSourceType[asourcesubscript];

    std::shared_ptr<TestScreenCaptureCallbackTest> callbackobj
        = std::make_shared<TestScreenCaptureCallbackTest>();
    TestScreenCapture::SetMicrophoneEnabled(true);
    TestScreenCapture::SetCanvasRotation(true);
    TestScreenCapture::SetScreenCaptureCallback(callbackobj);
    TestScreenCapture::Init(config);
    TestScreenCapture::StartScreenCapture();
    sleep(recorderTime);
    TestScreenCapture::StopScreenCapture();
    TestScreenCapture::Release();
    return true;
}
} // namespace Media

bool FuzzTestScreenCaptureAudioSourceType(uint8_t *data, size_t size)
{
    if (data == nullptr) {
        return true;
    }

    if (size < sizeof(AudioCaptureSourceType)) {
        return true;
    }
    ScreenCaptureAudioSourceTypeFuzzer testScreenCapture;
    return testScreenCapture.FuzzScreenCaptureAudioSourceType(data, size);
}
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::FuzzTestScreenCaptureAudioSourceType(data, size);
    return 0;
}