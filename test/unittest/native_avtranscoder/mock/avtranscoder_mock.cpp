/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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

#include "avtranscoder_mock.h"

using namespace OHOS;
using namespace OHOS::Media;

static const int32_t INVALID_TYPE = -1;
static const int32_t TEST_EXTRA = 0;

std::shared_ptr<TransCoder> TransCoderFactory::CreateTransCoder()
{
    std::shared_ptr<MockAVTransCoder> transcoder = std::make_shared<MockAVTransCoder>();
    return transcoder;
}

int32_t MockAVTransCoder::SetOutputFormat(OutputFormatType format)
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetVideoEncoder(VideoCodecFormat encoder)
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetVideoEncodingBitRate(int32_t rate)
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetVideoSize(int32_t videoFrameWidth, int32_t videoFrameHeight)
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetColorSpace(TranscoderColorSpace colorSpaceFormat)
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetEnableBFrame(bool enableBFrame)
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetAudioEncoder(AudioCodecFormat encoder)
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetAudioEncodingBitRate(int32_t bitRate)
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetInputFile(int32_t fd, int64_t offset, int64_t size)
{
    if (fd < 0 && callback_ != nullptr) {
        callback_->OnError(MSERR_SUPER_RESOLUTION_NOT_ENABLED, "mock");
        return MSERR_INVALID_VAL;
    }
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetOutputFile(int32_t fd)
{
    if (fd < 0 && callback_ != nullptr) {
        callback_->OnError(MSERR_INVALID_OPERATION, "mock");
        return MSERR_INVALID_VAL;
    }
    return MSERR_OK;
}

int32_t MockAVTransCoder::SetTransCoderCallback(const std::shared_ptr<TransCoderCallback> &callback)
{
    callback_ = callback;
    return MSERR_OK;
}

int32_t MockAVTransCoder::Prepare()
{
    if (callback_) {
        callback_->OnInfo(TransCoderOnInfoType::INFO_TYPE_PROGRESS_UPDATE, TEST_EXTRA);
    }
    return MSERR_OK;
}

int32_t MockAVTransCoder::Start()
{
    if (callback_) {
        callback_->OnInfo(TransCoderOnInfoType::INFO_TYPE_TRANSCODER_COMPLETED, TEST_EXTRA);
    }
    return MSERR_OK;
}

int32_t MockAVTransCoder::Pause()
{
    if (callback_) {
        callback_->OnInfo(INVALID_TYPE, TEST_EXTRA);
    }
    return MSERR_OK;
}

int32_t MockAVTransCoder::Resume()
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::Cancel()
{
    return MSERR_OK;
}

int32_t MockAVTransCoder::Release()
{
    return MSERR_OK;
}