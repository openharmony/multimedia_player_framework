/*
 * Copyright (C) 2024-2025 Huawei Device Co., Ltd.
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

#ifndef TRANSCODER_SERVICE_SERVER_H
#define TRANSCODER_SERVICE_SERVER_H

#include "i_transcoder_service.h"
#include "i_transcoder_engine.h"
#include "nocopyable.h"
#include "task_queue.h"
#include "watchdog.h"
#include "uri_helper.h"
#include "hitrace/tracechain.h"

namespace OHOS {
namespace Media {
enum class TransCoderWatchDogStatus : int32_t {
    WATCHDOG_WATCHING = 0,
    WATCHDOG_PAUSE,
};

class TransCoderServer : public ITransCoderService, public ITransCoderEngineObs, public NoCopyable {
public:
    static std::shared_ptr<ITransCoderService> Create();
    TransCoderServer();
    ~TransCoderServer();

    enum RecStatus {
        REC_INITIALIZED = 0,
        REC_CONFIGURED,
        REC_PREPARED,
        REC_TRANSCODERING,
        REC_PAUSED,
        REC_ERROR,
    };

    // ITransCoderService override
    int32_t SetVideoEncoder(VideoCodecFormat encoder) override;
    int32_t SetVideoSize(int32_t width, int32_t height) override;
    int32_t SetVideoEncodingBitRate(int32_t rate) override;
    int32_t SetColorSpace(TranscoderColorSpace colorSpaceFormat) override;
    int32_t SetEnableBFrame(bool enableBFrame) override;
    int32_t SetAudioEncoder(AudioCodecFormat encoder) override;
    int32_t SetAudioEncodingBitRate(int32_t bitRate) override;
    int32_t SetOutputFormat(OutputFormatType format) override;
    int32_t SetInputFile(int32_t fd, int64_t offset, int64_t size) override;
    int32_t SetOutputFile(int32_t fd) override;
    int32_t SetTransCoderCallback(const std::shared_ptr<TransCoderCallback> &callback) override;
    int32_t Prepare() override;
    int32_t Start() override;
    int32_t Pause() override;
    int32_t Resume() override;
    int32_t Cancel() override;
    int32_t Release() override;

    // ITransCoderEngineObs override
    void OnError(TransCoderErrorType errorType, int32_t errorCode) override;
    void OnInfo(TransCoderOnInfoType type, int32_t extra) override;
    void ReleaseInner();
    int32_t DumpInfo(int32_t fd);

private:
    int32_t Init();
    void ChangeStatus(RecStatus status);
    const std::string &GetStatusDescription(OHOS::Media::TransCoderServer::RecStatus status);

    std::unique_ptr<ITransCoderEngine> transCoderEngine_ = nullptr;
    std::shared_ptr<TransCoderCallback> transCoderCb_ = nullptr;
    RecStatus status_ = REC_INITIALIZED;
    std::mutex mutex_;
    std::mutex cbMutex_;
    TaskQueue taskQue_;
    struct ConfigInfo {
        VideoCodecFormat videoCodec = VIDEO_CODEC_FORMAT_BUTT;
        AudioCodecFormat audioCodec = AUDIO_CODEC_FORMAT_BUTT;
        TranscoderColorSpace colorSpaceFormat = TRANSCODER_COLORSPACE_NONE;
        int32_t width = -1;
        int32_t height = -1;
        int32_t videoBitRate = -1;
        int32_t audioBitRate = -1;
        OutputFormatType format = FORMAT_BUTT;
        std::string srcUrl;
        int32_t srcFd = -1;
        int64_t srcFdOffset = -1;
        int64_t srcFdSize = -1;
        int32_t dstUrl = -1;
        bool enableBFrame = false;
    } config_;
    std::string lastErrMsg_;

    std::shared_ptr<UriHelper> uriHelper_;

    std::atomic<bool> watchdogPause_ = false;

    uint64_t instanceId_ = 0;
    bool isReleased_ = false;
};
} // namespace Media
} // namespace OHOS
#endif // TRANSCODER_SERVICE_SERVER_H
