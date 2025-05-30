/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#ifndef RECORDERPROFILES_IMPL_H
#define RECORDERPROFILES_IMPL_H

#include <mutex>
#include "nocopyable.h"
#include "i_recorder_profiles_service.h"

namespace OHOS {
namespace Media {
class RecorderProfilesImpl : public RecorderProfiles, public NoCopyable {
public:
    ~RecorderProfilesImpl();
    bool IsAudioRecorderConfigSupported(const AudioRecorderProfile &profile) override;
    bool HasVideoRecorderProfile(int32_t sourceId, int32_t qualityLevel) override;
    std::vector<std::shared_ptr<AudioRecorderCaps>> GetAudioRecorderCaps() override;
    std::vector<std::shared_ptr<VideoRecorderCaps>> GetVideoRecorderCaps() override;
    std::shared_ptr<VideoRecorderProfile> GetVideoRecorderProfile(int32_t sourceId, int32_t qualityLevel) override;
    static RecorderProfiles& GetInstance();

private:
    std::shared_ptr<IRecorderProfilesService> recorderProfilesService_ = nullptr;
    RecorderProfilesImpl();
    int32_t Init();
    std::mutex mutex_;
};
}  // namespace Media
}  // namespace OHOS
#endif  // RECORDERPROFILES_IMPL_H

