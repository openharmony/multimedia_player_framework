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

#include "i_engine_factory.h"
#include "media_errors.h"
#include "media_utils.h"
#include "common/log.h"
#include "avmetadatahelper_impl.h"
#ifdef SUPPORT_RECORDER
#include "hirecorder_impl.h"
#endif
#ifdef SUPPORT_TRANSCODER
#include "hitranscoder_impl.h"
#endif
#ifdef SUPPORT_PLAYER
#include "hiplayer_impl.h"
#endif
#ifdef SUPPORT_LPP
#include "hilpp_vstreamer_impl.h"
#include "hilpp_astreamer_impl.h"
#endif

namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, LOG_DOMAIN_SYSTEM_PLAYER, "HstEngineFactory" };
}

namespace OHOS {
namespace Media {
class HstEngineFactory : public IEngineFactory {
public:
    HstEngineFactory() = default;
    ~HstEngineFactory() override = default;

    int32_t Score(Scene scene, const int32_t& appUid, const std::string& uri) override;
#ifdef SUPPORT_PLAYER
    std::unique_ptr<IPlayerEngine> CreatePlayerEngine(int32_t uid = 0, int32_t pid = 0, uint32_t tokenId = 0) override;
#endif
#ifdef SUPPORT_RECORDER
    std::unique_ptr<IRecorderEngine> CreateRecorderEngine(int32_t appUid, int32_t appPid, uint32_t appTokenId,
        uint64_t appFullTokenId) override;
#endif
#ifdef SUPPORT_TRANSCODER
    std::unique_ptr<ITransCoderEngine> CreateTransCoderEngine(int32_t appUid, int32_t appPid, uint32_t appTokenId,
        uint64_t appFullTokenId) override;
#endif
#ifdef SUPPORT_METADATA
    std::unique_ptr<IAVMetadataHelperEngine> CreateAVMetadataHelperEngine(int32_t uid = 0, int32_t pid = 0,
        uint32_t tokenId = 0, std::string appName = "") override;
#endif
#ifdef SUPPORT_LPP
    std::shared_ptr<ILppVideoStreamerEngine> CreateLppVideoStreamerEngine(int32_t appUid, int32_t appPid,
        uint32_t tokenId) override;
    std::shared_ptr<ILppAudioStreamerEngine> CreateLppAudioStreamerEngine(int32_t appUid, int32_t appPid,
        uint32_t tokenId) override;
#endif
};

int32_t HstEngineFactory::Score(Scene scene, const int32_t& appUid, const std::string& uri)
{
    MEDIA_LOG_D("Score in");
    (void)scene;
    (void)uri;
    (void)appUid;

    return MAX_SCORE;
}

#ifdef SUPPORT_RECORDER
std::unique_ptr<IRecorderEngine> HstEngineFactory::CreateRecorderEngine(
    int32_t appUid, int32_t appPid, uint32_t appTokenId, uint64_t appFullTokenId)
{
    MEDIA_LOG_D("CreateRecorderEngine enter.");
    auto recorder = std::unique_ptr<HiRecorderImpl>(new (std::nothrow) HiRecorderImpl(
        appUid, appPid, appTokenId, appFullTokenId));
    if (recorder && recorder->Init() == 0) {
        return recorder;
    }
    MEDIA_LOG_E("create recorder failed or recorder init failed");
    return nullptr;
}
#endif

#ifdef SUPPORT_TRANSCODER
std::unique_ptr<ITransCoderEngine> HstEngineFactory::CreateTransCoderEngine(
    int32_t appUid, int32_t appPid, uint32_t appTokenId, uint64_t appFullTokenId)
{
    MEDIA_LOG_E("CreateTransCoderEngine enter.");
    auto transCoder = std::make_unique<HiTransCoderImpl>(appUid, appPid, appTokenId, appFullTokenId);
    if (transCoder && transCoder->Init() == 0) {
        return transCoder;
    }
    MEDIA_LOG_E("create transCoder failed or transCoder init failed");
    return nullptr;
}
#endif

#ifdef SUPPORT_PLAYER
std::unique_ptr<IPlayerEngine> HstEngineFactory::CreatePlayerEngine(int32_t uid, int32_t pid, uint32_t tokenId)
{
    MEDIA_LOG_D("Hst CreatePlayerEngine enter.");
    auto player = std::unique_ptr<HiPlayerImpl>(new (std::nothrow) HiPlayerImpl(
        uid, pid, tokenId, 0));
    if (player) {
        return player;
    }
    MEDIA_LOG_E("create player failed");
    return nullptr;
}
#endif

#ifdef SUPPORT_METADATA
std::unique_ptr<IAVMetadataHelperEngine> HstEngineFactory::CreateAVMetadataHelperEngine(int32_t uid, int32_t pid,
    uint32_t tokenId, std::string appName)
{
    MEDIA_LOG_D("CreateAVMetadataHelperEngine enter.");
    auto helper = std::make_unique<AVMetadataHelperImpl>(uid, pid, tokenId, appName);
    if (helper == nullptr) {
        MEDIA_LOG_E("create AVMetadataHelperImpl failed");
        return nullptr;
    }
    return helper;
}
#endif

#ifdef SUPPORT_LPP
std::shared_ptr<ILppVideoStreamerEngine> HstEngineFactory::CreateLppVideoStreamerEngine(int32_t appUid, int32_t appPid,
    uint32_t tokenId)
{
    (void)appUid;
    (void)appPid;
    (void)tokenId;
    auto engine = std::make_shared<HiLppVideoStreamerImpl>();
    if (engine == nullptr) {
        MEDIA_LOG_E("create HiLppVideoStreamerEngine failed");
        return nullptr;
    }
    return engine;
    return nullptr;
}
 
std::shared_ptr<ILppAudioStreamerEngine> HstEngineFactory::CreateLppAudioStreamerEngine(int32_t appUid, int32_t appPid,
    uint32_t tokenId)
{
    (void)appUid;
    (void)appPid;
    (void)tokenId;
    auto engine = std::make_shared<HiLppAudioStreamerImpl>();
    if (engine == nullptr) {
        MEDIA_LOG_E("create LppAudioStreamerEngine failed");
        return nullptr;
    }
    return engine;
    return nullptr;
}
#endif
} // namespace Media
} // namespace OHOS

#ifdef __cplusplus
extern "C" {
#endif
__attribute__((visibility("default"))) OHOS::Media::IEngineFactory *CreateEngineFactory()
{
    return new (std::nothrow) OHOS::Media::HstEngineFactory();
}
#ifdef __cplusplus
}
#endif