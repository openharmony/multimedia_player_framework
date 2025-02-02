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
 
#define HST_LOG_TAG "DraggingPlayerAgent"

#include <dlfcn.h>
 
#include "common/log.h"
#include "dragging_player_agent.h"
#include "osal/task/pipeline_threadpool.h"
 
namespace {
const std::string REFERENCE_LIB_PATH = std::string(DRAGGING_PLAYER_PATH);
const std::string FILESEPARATOR = "/";
const std::string REFERENCE_LIB_NAME = "libvideo_dragging_player.z.so";
const std::string REFENCE_LIB_ABSOLUTE_PATH = REFERENCE_LIB_PATH + FILESEPARATOR + REFERENCE_LIB_NAME;
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, LOG_DOMAIN_PLAYER, "DraggingPlayerAgent"};
}
 
namespace OHOS {
namespace Media {
 
void *DraggingPlayerAgent::handler_ = nullptr;
DraggingPlayerAgent::CreateFunc DraggingPlayerAgent::createFunc_ = nullptr;
DraggingPlayerAgent::DestroyFunc DraggingPlayerAgent::destroyFunc_ = nullptr;
std::mutex DraggingPlayerAgent::mtx_;
 
class VideoStreamReadyCallbackImpl : public VideoStreamReadyCallback {
public:
    explicit VideoStreamReadyCallbackImpl(const std::shared_ptr<DraggingPlayerAgent> draggingPlayerAgent)
        : draggingPlayerAgent_(draggingPlayerAgent) {}
    bool IsVideoStreamDiscardable(const std::shared_ptr<AVBuffer> buffer) override
    {
        auto draggingPlayerAgent = draggingPlayerAgent_.lock();
        if (draggingPlayerAgent != nullptr && buffer != nullptr) {
            return draggingPlayerAgent->IsVideoStreamDiscardable(buffer);
        }
        return false;
    }
private:
    std::weak_ptr<DraggingPlayerAgent> draggingPlayerAgent_;
};
 
class VideoFrameReadyCallbackImpl : public VideoFrameReadyCallback {
public:
    explicit VideoFrameReadyCallbackImpl(const std::shared_ptr<DraggingPlayerAgent> draggingPlayerAgent)
        : draggingPlayerAgent_(draggingPlayerAgent) {}
    void ConsumeVideoFrame(const std::shared_ptr<AVBuffer> buffer, uint32_t bufferIndex) override
    {
        auto draggingPlayerAgent = draggingPlayerAgent_.lock();
        if (draggingPlayerAgent != nullptr || buffer != nullptr) {
            return draggingPlayerAgent->ConsumeVideoFrame(buffer, bufferIndex);
        }
    }
private:
    std::weak_ptr<DraggingPlayerAgent> draggingPlayerAgent_;
};
 
shared_ptr<DraggingPlayerAgent> DraggingPlayerAgent::Create()
{
    shared_ptr<DraggingPlayerAgent> agent = make_shared<DraggingPlayerAgent>();
    if (!agent->LoadSymbol()) {
        return nullptr;
    }
    agent->draggingPlayer_ = agent->createFunc_();
    if (agent->draggingPlayer_ == nullptr) {
        MEDIA_LOG_E("createFunc_ fail");
        return nullptr;
    }
 
    return agent;
}
 
DraggingPlayerAgent::~DraggingPlayerAgent()
{
    if (!isReleased_) {
        Release();
    }
    PipeLineThreadPool::GetInstance().DestroyThread(threadName_);
    if (draggingPlayer_ != nullptr) {
        destroyFunc_(draggingPlayer_);
        draggingPlayer_ = nullptr;
    }
}
 
Status DraggingPlayerAgent::Init(const shared_ptr<DemuxerFilter> &demuxer,
    const shared_ptr<DecoderSurfaceFilter> &decoder, std::string playerId)
{
    FALSE_RETURN_V_MSG_E(demuxer != nullptr && decoder != nullptr,
        Status::ERROR_INVALID_PARAMETER, "Invalid demuxer filter instance.");
    demuxer_ = demuxer;
    decoder_ = decoder;
    Status ret = draggingPlayer_->Init(demuxer, decoder);
    if (ret != Status::OK) {
        MEDIA_LOG_E("liyudebug DraggingPlayerAgent::Init failed");
        return ret;
    }
    MEDIA_LOG_I("DraggingPlayerAgent::Init register");
    videoStreamReadyCb_ = std::make_shared<VideoStreamReadyCallbackImpl>(shared_from_this());
    demuxer->RegisterVideoStreamReadyCallback(videoStreamReadyCb_);
    videoFrameReadyCb_ = std::make_shared<VideoFrameReadyCallbackImpl>(shared_from_this());
    decoder->RegisterVideoFrameReadyCallback(videoFrameReadyCb_);
    threadName_ = "DraggingTask_" + playerId;
    task_ = std::make_unique<Task>("draggingThread", threadName_, TaskType::GLOBAL, TaskPriority::NORMAL, false);
    task_->Start();
    return Status::OK;
}
 
bool DraggingPlayerAgent::IsVideoStreamDiscardable(const std::shared_ptr<AVBuffer> avBuffer)
{
    FALSE_RETURN_V_MSG_E(draggingPlayer_ != nullptr, false, "Invalid draggingPlayer_ instance.");
    return draggingPlayer_->IsVideoStreamDiscardable(avBuffer);
}
 
void DraggingPlayerAgent::ConsumeVideoFrame(const std::shared_ptr<AVBuffer> avBuffer, uint32_t bufferIndex)
{
    FALSE_RETURN(draggingPlayer_ != nullptr);
    draggingPlayer_->ConsumeVideoFrame(avBuffer, bufferIndex);
}
 
void DraggingPlayerAgent::UpdateSeekPos(int64_t seekMs)
{
    std::unique_lock<std::mutex> lock(draggingMutex_);
    FALSE_RETURN(draggingPlayer_ != nullptr);
    seekCnt_.fetch_add(1);
    draggingPlayer_->UpdateSeekPos(seekMs);
    if (task_) {
        int64_t seekCnt = seekCnt_.load();
        lock.unlock();
        task_->SubmitJob([this, seekCnt]() { StopDragging(seekCnt); }, 33333); // 33333 means 33333us, 33ms
    }
}

void DraggingPlayerAgent::StopDragging(int64_t seekCnt)
{
    std::unique_lock<std::mutex> lock(draggingMutex_);
    FALSE_RETURN(!isReleased_);
    FALSE_RETURN(draggingPlayer_ != nullptr);
    if (seekCnt_.load() != seekCnt) {
        return;
    }
    draggingPlayer_->StopDragging();
}
 
void DraggingPlayerAgent::Release()
{
    if (task_) {
        task_->Stop();
    }
    std::unique_lock<std::mutex> lock(draggingMutex_);
    if (draggingPlayer_ != nullptr) {
        draggingPlayer_->Release();
    }
    if (draggingPlayer_ != nullptr) {
        auto res = demuxer_->PauseDragging();
        FALSE_LOG_MSG(res == Status::OK, "PauseDragging failed");
    }
    if (demuxer_ != nullptr) {
        demuxer_->DeregisterVideoStreamReadyCallback();
    }
    if (decoder_ != nullptr) {
        decoder_->DeregisterVideoFrameReadyCallback();
    }
    isReleased_ = true;
}
 
void *DraggingPlayerAgent::LoadLibrary()
{
    char path[PATH_MAX] = {0x00};
    const char *inputPath = REFENCE_LIB_ABSOLUTE_PATH.c_str();
    if (strlen(inputPath) > PATH_MAX || realpath(inputPath, path) == nullptr) {
        MEDIA_LOG_E("dlopen failed due to Invalid path");
        return nullptr;
    }
    auto ptr = ::dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (ptr == nullptr) {
        MEDIA_LOG_E("dlopen failed due to %{public}s", ::dlerror());
    }
    handler_ = ptr;
    return ptr;
}
 
bool DraggingPlayerAgent::CheckSymbol(void *handler)
{
    if (handler) {
        std::string createFuncName = "CreateDraggingPlayer";
        std::string destroyFuncName = "DestroyDraggingPlayer";
        CreateFunc createFunc = nullptr;
        DestroyFunc destroyFunc = nullptr;
        createFunc = (CreateFunc)(::dlsym(handler, createFuncName.c_str()));
        destroyFunc = (DestroyFunc)(::dlsym(handler, destroyFuncName.c_str()));
        if (createFunc && destroyFunc) {
            MEDIA_LOG_D("CheckSymbol:  createFuncName %{public}s", createFuncName.c_str());
            MEDIA_LOG_D("CheckSymbol:  destroyFuncName %{public}s", destroyFuncName.c_str());
            createFunc_ = createFunc;
            destroyFunc_ = destroyFunc;
            return true;
        }
    }
    return false;
}
 
bool DraggingPlayerAgent::LoadSymbol()
{
    lock_guard<mutex> lock(mtx_);
    if (handler_ == nullptr) {
        if (!CheckSymbol(LoadLibrary())) {
            MEDIA_LOG_E("Load Reference parser so fail");
            return false;
        }
    }
    return true;
}
}  // namespace Media
}  // namespace OHOS