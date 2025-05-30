/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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
#ifndef SOUNDPOOL_MOCK_H
#define SOUNDPOOL_MOCK_H

#include <fcntl.h>
#include <cstdio>
#include <mutex>
#include <cstdlib>
#include <thread>
#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <iostream>
#include "gtest/gtest.h"
#include "unittest_log.h"
#include "media_errors.h"
#include "nocopyable.h"
#include "isoundpool.h"

namespace OHOS {
namespace Media {
using namespace std;
using namespace AudioStandard;
class SoundPoolMock {
public:
    SoundPoolMock() = default;
    ~SoundPoolMock() = default;
    bool CreateSoundPool(int maxStreams, AudioStandard::AudioRendererInfo audioRenderInfo);
    int32_t Load(std::string url);
    int32_t Load(int32_t fd, int64_t offset, int64_t length);
    int32_t Play(int32_t soundID, PlayParams playParameters);
    int32_t Stop(int32_t streamID);
    int32_t SetLoop(int32_t streamID, int32_t loop);
    int32_t SetPriority(int32_t streamID, int32_t priority);
    int32_t SetRate(int32_t streamID, AudioStandard::AudioRendererRate renderRate);
    int32_t SetVolume(int32_t streamID, float leftVolume, float rigthVolume);
    int32_t Unload(int32_t soundID);
    int32_t Release();
    int32_t SetSoundPoolCallback(const std::shared_ptr<ISoundPoolCallback> &soundPoolCallback);
    size_t GetFileSize(const std::string& fileName);
private:
    std::shared_ptr<ISoundPool> soundPool_ = nullptr;
    std::atomic<bool> isExit_ { false };
};

class SoundPoolParallelMock {
public:
    SoundPoolParallelMock() = default;
    ~SoundPoolParallelMock() = default;
    bool CreateParallelSoundPool(int maxStreams, AudioStandard::AudioRendererInfo audioRenderInfo);
    int32_t Load(std::string url);
    int32_t Load(int32_t fd, int64_t offset, int64_t length);
    int32_t Play(int32_t soundID, PlayParams playParameters);
    int32_t Stop(int32_t streamID);
    int32_t SetLoop(int32_t streamID, int32_t loop);
    int32_t SetPriority(int32_t streamID, int32_t priority);
    int32_t SetRate(int32_t streamID, AudioStandard::AudioRendererRate renderRate);
    int32_t SetVolume(int32_t streamID, float leftVolume, float rigthVolume);
    int32_t Unload(int32_t soundID);
    int32_t Release();
    int32_t SetSoundPoolCallback(const std::shared_ptr<ISoundPoolCallback> &soundPoolCallback);
    size_t GetFileSize(const std::string& fileName);
private:
    std::shared_ptr<ISoundPool> soundPoolParallel_ = nullptr;
};

class NumberCounter {
public:
    void Increment()
    {
        std::unique_lock<mutex> lock(numberMtx_);
        number_++;
        if (number_ >= targetNumber_) {
            isNumReached_ = true;
            numberCv_.notify_all();
        }
    }
    void Reset()
    {
        std::unique_lock<mutex> lock(numberMtx_);
        number_ = 0;
    }
    int32_t GetNumber()
    {
        std::unique_lock<mutex> lock(numberMtx_);
        return number_;
    }
    bool WaitForCounter(int32_t target, std::chrono::seconds timeoutSec)
    {
        targetNumber_ = target;
        std::unique_lock<mutex> lock(numberMtx_);
        return numberCv_.wait_for(lock, timeoutSec, [this, target] {
            return isNumReached_ || number_ >= target;
        });
    }
private:
    std::mutex numberMtx_;
    std::condition_variable numberCv_;
    int32_t number_ = 0;
    int32_t targetNumber_ = INT32_MAX;
    bool isNumReached_ = false;
};

class SoundPoolCallbackTest : public ISoundPoolCallback, public NoCopyable {
public:
    static const int32_t DEFAULT_MAX_WAIT_SECONDS = 20;
    SoundPoolCallbackTest(std::shared_ptr<SoundPoolMock> soundPool)
    {
        soundPool_ = soundPool;
    }
    SoundPoolCallbackTest(std::shared_ptr<SoundPoolParallelMock> soundPoolParallel)
    {
        soundPoolParallel_ = soundPoolParallel;
    }
    ~SoundPoolCallbackTest()
    {
        soundPool_ = nullptr;
        soundPoolParallel_ = nullptr;
    }
    bool WaitLoadedSoundNum(int32_t target, int32_t seconds = DEFAULT_MAX_WAIT_SECONDS)
    {
        return soundCounter_.WaitForCounter(target, std::chrono::seconds(seconds));
    }
    int32_t GetHaveLoadedSoundNum()
    {
        return soundCounter_.GetNumber();
    }
    void ResetHaveLoadedSoundNum()
    {
        cout << "Before ResetHaveLoadedSoundNum :" << GetHaveLoadedSoundNum() << endl;
        soundCounter_.Reset();
    }

    int32_t GetHavePlayedSoundNum()
    {
        cout << "GetHavePlayedSoundNum havePlayedSoundNumInner_:" << havePlayedSoundNumInner_ << endl;
        return havePlayedSoundNumInner_;
    }
    void ResetHavePlayedSoundNum()
    {
        cout << "Before ResetHavePlayedSoundNum havePlayedSoundNumInner_:" << havePlayedSoundNumInner_ << endl;
        havePlayedSoundNumInner_ = 0;
        cout << "After ResetHavePlayedSoundNum havePlayedSoundNumInner_:" << havePlayedSoundNumInner_ << endl;
    }

    std::vector<int32_t> GetHavePlayedStreamID()
    {
        return vector_;
    }

    void ResetHavePlayedStreamID()
    {
        vector_.clear();
    }
    std::shared_ptr<SoundPoolMock> soundPool_ = nullptr;
    std::shared_ptr<SoundPoolParallelMock> soundPoolParallel_ = nullptr;
    NumberCounter soundCounter_;
    void OnLoadCompleted(int32_t soundId) override;
    void OnPlayFinished(int32_t streamID) override;
    void OnError(int32_t errorCode) override;

private:
    int32_t havePlayedSoundNumInner_ = 0;
    std::vector<int32_t> vector_;
};

class SoundPoolFrameWriteCallbackTest : public ISoundPoolFrameWriteCallback {
    void OnFirstAudioFrameWritingCallback(uint64_t &latency) override
    {}
};

} // namespace Media
} // namespace OHOS
#endif