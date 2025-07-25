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

#ifndef AUDIO_HAPTIC_PLAYER_IMPL_H
#define AUDIO_HAPTIC_PLAYER_IMPL_H

#include "audio_haptic_player.h"
#include "audio_haptic_sound.h"
#include "audio_haptic_vibrator.h"

namespace OHOS {
namespace Media {
class AudioHapticSoundCallbackImpl;

class AudioHapticPlayerImpl : public AudioHapticPlayer, public std::enable_shared_from_this<AudioHapticPlayerImpl> {
public:
    AudioHapticPlayerImpl();
    ~AudioHapticPlayerImpl();

    // AudioHapticPlayer override
    bool IsMuted(const AudioHapticType &audioHapticType) const override;
    int32_t Prepare() override;
    int32_t Start() override;
    int32_t Stop() override;
    int32_t Release() override;
    int32_t SetVolume(float volume) override;
    int32_t SetHapticIntensity(float intensity) override;
    int32_t SetLoop(bool loop) override;
    int32_t SetAudioHapticPlayerCallback(const std::shared_ptr<AudioHapticPlayerCallback> &playerCallback) override;
    int32_t GetAudioCurrentTime() override;
    HapticsMode GetHapticsMode() const override;
    void SetHapticsMode(HapticsMode hapticsMode) override;
    int32_t EnableHapticsInSilentMode(bool enable) override;
    bool IsHapticsIntensityAdjustmentSupported() override;
    bool IsHapticsRampSupported() override;
    int32_t SetHapticsRamp(int32_t duration, float startIntensity, float endIntensity) override;
    int32_t SetHapticsFeature(const HapticsFeature &feature) override;

    void SetPlayerParam(const AudioHapticPlayerParam &param);
    void LoadPlayer();
    void NotifyStartVibrate(const uint64_t &latency);
    void NotifyInterruptEvent(const AudioStandard::InterruptEvent &interruptEvent);
    void NotifyEndOfStreamEvent();
    void NotifyErrorEvent(int32_t errCode);
    static void SendHapticPlayerEvent(const int32_t &errorCode, const std::string &strEvent);
    void NotifyFirstFrame(const uint64_t &latency);

private:
    // func for sound
    void ReleaseSound();
    static void HandleEndOfStreamEventThreadFunc(std::weak_ptr<AudioHapticPlayerImpl> player);
    void HandleEndOfStreamEvent();
    // func for vibration
    int32_t StartVibrate();
    void StopVibrate();
    void ResetVibrateState();
    void ReleaseVibrator();
    int32_t GetDelayTime(int32_t playedTimes);

    // var for all
    AudioLatencyMode latencyMode_;
    AudioStandard::StreamUsage streamUsage_ = AudioStandard::STREAM_USAGE_UNKNOWN;
    bool muteAudio_;
    bool muteHaptic_;
    bool parallelPlayFlag_ = false;
    AudioSource audioSource_;
    HapticSource hapticSource_;
    float volume_ = 1.0f;
    std::atomic<bool> loop_ = false;
    AudioHapticPlayerState playerState_ = AudioHapticPlayerState::STATE_INVALID;
    std::mutex audioHapticPlayerLock_;
    HapticsMode hapticsMode_ = HapticsMode::HAPTICS_MODE_INVALID;
    std::atomic<int32_t> audioHapticSyncId_ = 0;
    bool isSupportDSPSync_ = false;

    // var for callback
    std::weak_ptr<AudioHapticPlayerCallback> audioHapticPlayerCallback_;
    uint64_t audioLatency_ = 0;
    std::shared_ptr<AudioHapticSoundCallback> soundCallback_ = nullptr;

    // var for vibrate
    std::shared_ptr<AudioHapticVibrator> audioHapticVibrator_ = nullptr;
    std::shared_ptr<std::thread> vibrateThread_;
    std::mutex waitStartVibrateMutex_;
    std::condition_variable condStartVibrate_;
    bool canStartVibrate_ = false;
    std::atomic<bool> isVibrationStopped_ = false;
    std::atomic_bool isVibrationRunning_{false};
    std::atomic<bool> isGentle_ = false;
    std::atomic<bool> isRamp_ = false;

    // var for audio
    std::shared_ptr<AudioHapticSound> audioHapticSound_ = nullptr;
};

class AudioHapticSoundCallbackImpl : public AudioHapticSoundCallback {
public:
    explicit AudioHapticSoundCallbackImpl(std::shared_ptr<AudioHapticPlayerImpl> audioHapticPlayerImpl);
    virtual ~AudioHapticSoundCallbackImpl() = default;

    void OnEndOfStream() override;
    void OnError(int32_t errorCode) override;
    void OnFirstFrameWriting(uint64_t latency) override;
    void OnInterrupt(const AudioStandard::InterruptEvent &interruptEvent) override;

private:
    std::weak_ptr<AudioHapticPlayerImpl> audioHapticPlayerImpl_;
};
} // namespace Media
} // namespace OHOS
#endif // AUDIO_HAPTIC_PLAYER_IMPL_H
