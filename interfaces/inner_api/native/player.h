/*
 * Copyright (C) 2021-2025 Huawei Device Co., Ltd.
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

#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include "media_core.h"
#ifndef SUPPORT_AUDIO_ONLY
#include "surface.h"
#endif
#include "meta/format.h"
#include "media_data_source.h"
#include "loading_request.h"
#include "media_source.h"

namespace OHOS {
namespace DrmStandard {
class IMediaKeySessionService;
}
}

namespace OHOS {
namespace Media {
struct AVPlayStrategy;

namespace DrmConstant {
constexpr uint32_t DRM_MAX_M3U8_DRM_PSSH_LEN = 2048;
constexpr uint32_t DRM_MAX_M3U8_DRM_UUID_LEN = 16;
constexpr uint32_t DRM_MAX_DRM_INFO_COUNT = 200;
}

namespace AVPlayStrategyConstant {
constexpr double BUFFER_DURATION_FOR_PLAYING_SECONDS = 2; // flv live set default BufferDurationForPlaying 2s
constexpr double START_QUICK_PLAY_THRESHOLD_SECONDS = 5; // flv live set default thresholdForAutoQuickPlay 5s
}

struct AVPlayStrategy {
    uint32_t preferredWidth = 0;
    uint32_t preferredHeight = 0;
    uint32_t preferredBufferDuration = 0;
    double preferredBufferDurationForPlaying = 0;
    double thresholdForAutoQuickPlay = -1;
    bool preferredHdr = false;
    bool showFirstFrameOnPrepare = false;
    bool enableSuperResolution = false;
    bool enableCameraPostprocessing = false;
    OHOS::Media::MediaType mutedMediaType = OHOS::Media::MediaType::MEDIA_TYPE_MAX_COUNT;
    std::string preferredAudioLanguage = "";
    std::string preferredSubtitleLanguage = "";
    bool keepDecodingOnMute = false;
};

struct AVPlayMediaStream {
    std::string url = "";
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t bitrate = 0;
};

struct DrmInfoItem {
    uint8_t uuid[DrmConstant::DRM_MAX_M3U8_DRM_UUID_LEN];
    uint8_t pssh[DrmConstant::DRM_MAX_M3U8_DRM_PSSH_LEN];
    uint32_t psshLen;
};

class AVMediaSource {
public:
    AVMediaSource(std::string sourceUrl, std::map<std::string, std::string> sourceHeader)
        : url(sourceUrl), header(sourceHeader)
    {
    }
    ~AVMediaSource()
    {
        header.clear();
    }
    void SetMimeType(const std::string& mimeType)
    {
        mimeType_ = mimeType;
    }
    std::string GetMimeType() const
    {
        return mimeType_;
    }

    const std::vector<AVPlayMediaStream>& GetAVPlayMediaStreamList()
    {
        return mediaStreamVec_;
    }
 
    void AddMediaStream(AVPlayMediaStream& mediaStream)
    {
        mediaStreamVec_.push_back(mediaStream);
    }
    std::string url {};
    std::string mimeType_ {};
    std::map<std::string, std::string> header;
    std::shared_ptr<LoaderCallback> mediaSourceLoaderCb_ {nullptr};
    std::shared_ptr<Plugins::IMediaSourceLoader> sourceLoader_ {nullptr};
private:
    std::vector<AVPlayMediaStream> mediaStreamVec_;
};

class PlayerKeys {
public:
    static constexpr std::string_view PLAYER_MESSAGE_TYPE = "message_type";
    static constexpr std::string_view PLAYER_IS_LIVE_STREAM = "is_live_stream";
    static constexpr std::string_view PLAYER_SEEK_POSITION = "seek_done";
    static constexpr std::string_view PLAYER_PLAYBACK_SPEED = "speed_done";
    static constexpr std::string_view PLAYER_PLAYBACK_RATE = "rate_done";
    static constexpr std::string_view PLAYER_BITRATE_DONE = "bitrate_done";
    static constexpr std::string_view PLAYER_CURRENT_POSITION = "current_position";
    static constexpr std::string_view PLAYER_DURATION = "duration";
    static constexpr std::string_view PLAYER_STATE_CHANGE = "player_state_change";
    static constexpr std::string_view PLAYER_STATE_CHANGED_REASON = "state_changed_reason";
    static constexpr std::string_view PLAYER_VOLUME_LEVEL = "volume_level";
    static constexpr std::string_view PLAYER_TRACK_INDEX = "track_index";
    static constexpr std::string_view PLAYER_TRACK_TYPE = "track_type";
    static constexpr std::string_view PLAYER_TRACK_INFO = "track_info";
    static constexpr std::string_view PLAYER_WIDTH = "width";
    static constexpr std::string_view PLAYER_HEIGHT = "height";
    static constexpr std::string_view PLAYER_MIME = "codec_mime";
    static constexpr std::string_view PLAYER_BITRATE = "bitrate";
    static constexpr std::string_view PLAYER_FRAMERATE = "frame_rate";
    static constexpr std::string_view PLAYER_LANGUGAE = "language_code";
    static constexpr std::string_view PLAYER_SAMPLE_RATE = "sample_rate";
    static constexpr std::string_view PLAYER_CHANNELS = "channel_count";
    static constexpr std::string_view PLAYER_BUFFERING_TYPE = "buffering_type";
    static constexpr std::string_view PLAYER_BUFFERING_VALUE = "buffering_value";
    static constexpr std::string_view PLAYER_BUFFERING_START = "buffering_start";
    static constexpr std::string_view PLAYER_BUFFERING_END = "buffering_end";
    static constexpr std::string_view PLAYER_BUFFERING_PERCENT = "buffering_percent";
    static constexpr std::string_view PLAYER_CACHED_DURATION = "cached_duration";
    static constexpr std::string_view PLAYER_IS_SELECT = "track_is_select";
    static constexpr std::string_view PLAYER_ERROR_TYPE = "error_type";
    static constexpr std::string_view PLAYER_ERROR_MSG = "error_msg";
    static constexpr std::string_view CONTENT_TYPE = "content_type";
    static constexpr std::string_view STREAM_USAGE = "stream_usage";
    static constexpr std::string_view VOLUME_MODE = "volume_mode";
    static constexpr std::string_view RENDERER_FLAG = "renderer_flag";
    static constexpr std::string_view VIDEO_SCALE_TYPE = "video_scale_type";
    static constexpr std::string_view AUDIO_INTERRUPT_MODE = "audio_interrupt_mode";
    static constexpr std::string_view AUDIO_INTERRUPT_TYPE = "audio_interrupt_type";
    static constexpr std::string_view AUDIO_INTERRUPT_FORCE = "audio_interrupt_force";
    static constexpr std::string_view AUDIO_INTERRUPT_HINT = "audio_interrupt_hint";
    static constexpr std::string_view AUDIO_FIRST_FRAME = "audio_first_frame";
    static constexpr std::string_view AUDIO_EFFECT_MODE = "audio_effect_mode";
    static constexpr std::string_view AUDIO_DEVICE_CHANGE = "audio_device_change";
    static constexpr std::string_view AUDIO_DEVICE_CHANGE_REASON = "audio_device_change_reason";
    static constexpr std::string_view SUBTITLE_TEXT = "subtitle_text";
    static constexpr std::string_view SUBTITLE_PTS = "subtitle_pts";
    static constexpr std::string_view SUBTITLE_DURATION = "subtitle_duration";
    static constexpr std::string_view PLAYER_DRM_INFO_ADDR = "drm_info_addr";
    static constexpr std::string_view PLAYER_DRM_INFO_COUNT = "drm_info_count";
    static constexpr std::string_view PLAYER_AVAILABLE_BITRATES = "available_bitRates";
    static constexpr std::string_view AUDIO_MAX_AMPLITUDE = "max_amplitude";
    static constexpr std::string_view SEI_PLAYBACK_POSITION = "sei_playbackPosition";
    static constexpr std::string_view SUPER_RESOLUTION_ENABLED = "super_resolution_enabled";
    static constexpr std::string_view PLAYER_AUDIO_HAPTICS_SYNC_ID = "audio_haptic_sync_id";
};

class PlaybackInfoKey {
public:
    static constexpr std::string_view SERVER_IP_ADDRESS = "server_ip_address";
    static constexpr std::string_view AVG_DOWNLOAD_RATE = "average_download_rate";
    static constexpr std::string_view DOWNLOAD_RATE = "download_rate";
    static constexpr std::string_view IS_DOWNLOADING = "is_downloading";
    static constexpr std::string_view BUFFER_DURATION = "buffer_duration";
};

enum PlayerErrorType : int32_t {
    /* Valid error, error code reference defined in media_errors.h */
    PLAYER_ERROR,
    /* Unknown error */
    PLAYER_ERROR_UNKNOWN,
    /* extend error type start,The extension error type agreed upon by the plug-in and
       the application will be transparently transmitted by the service. */
    PLAYER_ERROR_EXTEND_START = 0X10000,
};

enum PlayerMessageType : int32_t {
    /* unknown info */
    PLAYER_INFO_UNKNOWN = 0,
    /* first video frame start to render. */
    PLAYER_INFO_VIDEO_RENDERING_START,
    /* network bandwidth, uint is KB and passed by "extra"(arg 2). */
    PLAYER_INFO_NETWORK_BANDWIDTH,
    /* not fatal errors accured, errorcode see "media_errors.h" and passed by "extra"(arg 2). */
    PLAYER_INFO_WARNING,
    /* system new info type should be added here.
       extend start. App and plugins or PlayerEngine extended info type start. */
    PLAYER_INFO_EXTEND_START = 0X1000,
};

enum PlayerOnSystemOperationType : int32_t {
    OPERATION_TYPE_PLAY = 1,
    OPERATION_TYPE_PAUSE,
    OPERATION_TYPE_CHECK_LIVE_DELAY,
};

enum PlayerOperationReason : int32_t {
    OPERATION_REASON_AUDIO_INTERRUPT = 1,
    OPERATION_REASON_USER_BACKGROUND,
    OPERATION_REASON_CHECK_LIVE_DELAY_TIME,
};

enum PlayerOnInfoType : int32_t {
    /* return the message when seeking done. */
    INFO_TYPE_SEEKDONE = 1,
    /* return the message when speeding done. */
    INFO_TYPE_SPEEDDONE,
    /* return the message when speeding done. */
    INFO_TYPE_RATEDONE,
    /* return the message when select bitrate done */
    INFO_TYPE_BITRATEDONE,
    /* return the message when playback is end of steam. */
    INFO_TYPE_EOS,
    /* return the message when PlayerStates changed. */
    INFO_TYPE_STATE_CHANGE,
    /* return the current posion of playback automatically. */
    INFO_TYPE_POSITION_UPDATE,
    /* return the playback message. */
    INFO_TYPE_MESSAGE,
    /* return the message when volume changed. */
    INFO_TYPE_VOLUME_CHANGE,
    /* return the message when video size is first known or updated. */
    INFO_TYPE_RESOLUTION_CHANGE,
    /* return multiqueue buffering time. */
    INFO_TYPE_BUFFERING_UPDATE,
    /* return hls bitrate.
       Bitrate is to convert data into uint8_t array storage,
       which needs to be forcibly converted to uint32_t through offset access. */
    INFO_TYPE_BITRATE_COLLECT,
    /* return the message when audio focus changed. */
    INFO_TYPE_INTERRUPT_EVENT,
    /* return the message when PlayerStates changed by audio. */
    INFO_TYPE_STATE_CHANGE_BY_AUDIO,
    /* return the message with extra information in format. */
    INFO_TYPE_EXTRA_FORMAT,
    /* return the duration of playback. */
    INFO_TYPE_DURATION_UPDATE,
    /* return the playback is live stream. */
    INFO_TYPE_IS_LIVE_STREAM,
    /* return the message when track changes. */
    INFO_TYPE_TRACKCHANGE,
    /* return the default audio track. */
    INFO_TYPE_DEFAULTTRACK,
    /* Return to the end of track processing. */
    INFO_TYPE_TRACK_DONE,
    /* Return error message to prompt the user. */
    INFO_TYPE_ERROR_MSG,
    /* return the message when subtitle track num updated. */
    INFO_TYPE_TRACK_NUM_UPDATE,
    /* return the message when subtitle track info updated. */
    INFO_TYPE_TRACK_INFO_UPDATE,
    /* return the subtitle of playback. */
    INFO_TYPE_SUBTITLE_UPDATE,
    /* return to the end of adding subtitle processing. */
    INFO_TYPE_ADD_SUBTITLE_DONE,
    /* return the message with drminfo. */
    INFO_TYPE_DRM_INFO_UPDATED,
    /* return set decrypt done message. */
    INFO_TYPE_SET_DECRYPT_CONFIG_DONE,
    /* return the audio latency when the first frame is writing. */
    INFO_TYPE_AUDIO_FIRST_FRAME,
    /* audio device change. */
    INFO_TYPE_AUDIO_DEVICE_CHANGE,
    /* return the subtitle info */
    INFO_TYPE_SUBTITLE_UPDATE_INFO,
    /* return audio uv value */
    INFO_TYPE_MAX_AMPLITUDE_COLLECT,
    /* return the sei info */
    INFO_TYPE_SEI_UPDATE_INFO,
    /* return the message when super resolution is changed*/
    INFO_TYPE_SUPER_RESOLUTION_CHANGED,
    /* return the auto select flv live bitrate. internal info type, not open to northbound interfaces. */
    INFO_TYPE_FLV_AUTO_SELECT_BITRATE,
};

enum PlayerStates : int32_t {
    /* error states */
    PLAYER_STATE_ERROR = 0,
    /* idle states */
    PLAYER_IDLE = 1,
    /* initialized states(Internal states) */
    PLAYER_INITIALIZED = 2,
    /* preparing states(Internal states) */
    PLAYER_PREPARING = 3,
    /* prepared states */
    PLAYER_PREPARED = 4,
    /* started states */
    PLAYER_STARTED = 5,
    /* paused states */
    PLAYER_PAUSED = 6,
    /* stopped states */
    PLAYER_STOPPED = 7,
    /* Play to the end states */
    PLAYER_PLAYBACK_COMPLETE = 8,
    /* released states */
    PLAYER_RELEASED = 9,
    PLAYER_FROZEN = 10,
};

enum PlaybackRateMode : int32_t {
    /* Video playback at 0.75x normal speed */
    SPEED_FORWARD_0_75_X = 0,
    /* Video playback at normal speed */
    SPEED_FORWARD_1_00_X = 1,
    /* Video playback at 1.25x normal speed */
    SPEED_FORWARD_1_25_X = 2,
    /* Video playback at 1.75x normal speed */
    SPEED_FORWARD_1_75_X = 3,
    /* Video playback at 2.0x normal speed */
    SPEED_FORWARD_2_00_X = 4,
    /* Video playback at 0.5x normal speed */
    SPEED_FORWARD_0_50_X = 5,
    /* Video playback at 1.5x normal speed */
    SPEED_FORWARD_1_50_X = 6,
    /* Video playback at 3.0x normal speed */
    SPEED_FORWARD_3_00_X = 7,
    /* Video playback at 0.25x normal speed */
    SPEED_FORWARD_0_25_X = 8,
    /* Video playback at 0.125x normal speed */
    SPEED_FORWARD_0_125_X = 9,
    /* Video playback at 4.00x normal speed */
    SPEED_FORWARD_4_00_X = 10,
    /* Video playback at 1.20x normal speed */
    SPEED_FORWARD_1_20_X = 100, // flv live quick play, internal value, not open to northbound
};

enum PlayerProducer : int32_t {
    INNER = 0,
    CAPI,
    NAPI
};

class PlayerCallback {
public:
    virtual ~PlayerCallback() = default;
    /**
     * Called when a player message or alarm is received.
     *
     * @param type Indicates the information type. For details, see {@link PlayerOnInfoType}.
     * @param extra Indicates other information, for example, the start time position of a playing file.
     * @param infoBody According to the info type, the information carrier passed.Is an optional parameter.
     */
    virtual void OnInfo(PlayerOnInfoType type, int32_t extra, const Format &infoBody) = 0;

    /**
     * Called when an error occurred for versions above api9
     *
     * @param errorCode Error code.
     * @param errorMsg Error message.
     */
    virtual void OnError(int32_t errorCode, const std::string &errorMsg) = 0;

    virtual void SetFreezeFlag(bool isFrozen)
    {
        (void)isFrozen;
    }

    virtual void SetInterruptListenerFlag(bool isRegistered)
    {
        (void)isRegistered;
    }
};

class Player {
public:
    virtual ~Player() = default;

    /**
     * @brief Sets the playback source for the player. The corresponding source can be http url
     *
     * @param url Indicates the playback source.
     * @return Returns {@link MSERR_OK} if the url is set successfully; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetSource(const std::string &url) = 0;

    /**
     * @brief Sets the playback media data source for the player.
     *
     * @param dataSrc Indicates the media data source. in {@link media_data_source.h}
     * @return Returns {@link MSERR_OK} if the mediadatasource is set successfully; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetSource(const std::shared_ptr<IMediaDataSource> &dataSrc) = 0;

    /**
     * @brief Sets the playback media file descriptor source for the player.
     *
     * @param fd Indicates the file descriptor of media source.
     * @param offset Indicates the offset of media source in file descriptor.
     * @param size Indicates the size of media source.
     * @return Returns {@link MSERR_OK} if the fd source is set successfully; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetSource(int32_t fd, int64_t offset = 0, int64_t size = 0) = 0;

    /**
     * @brief Add a subtitle source for the player. The corresponding source can be http url.
     *
     * @param url Indicates the subtitle source.
     * @return Returns {@link MSERR_OK} if the url is set successfully; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t AddSubSource(const std::string &url) = 0;

    /**
     * @brief Add a playback subtitle file descriptor source for the player.
     *
     * @param fd Indicates the file descriptor of subtitle source.
     * @param offset Indicates the offset of subtitle source in file descriptor.
     * @param size Indicates the size of subtitle source.
     * @return Returns {@link MSERR_OK} if the fd source is set successfully; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t AddSubSource(int32_t fd, int64_t offset = 0, int64_t size = 0) = 0;

    /**
     * @brief Start playback.
     *
     * This function must be called after {@link Prepare}. If the player state is <b>Prepared</b>,
     * this function is called to start playback.
     *
     * @return Returns {@link MSERR_OK} if the playback is started; otherwise returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t Play() = 0;

    /**
     * @brief Prepares the playback environment and buffers media data asynchronous.
     *
     * This function must be called after {@link SetSource}.
     *
     * @return Returns {@link MSERR_OK} if {@link Prepare} is successfully added to the task queue;
     * returns an error code defined in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    __attribute__((deprecated)) virtual int32_t Prepare() = 0;

    /**
     * @brief Prepares the playback environment and buffers media data asynchronous.
     *
     * This function must be called after {@link SetSource}.
     *
     * @return Returns {@link MSERR_OK} if {@link PrepareAsync} is successfully added to the task queue;
     * returns an error code defined in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t PrepareAsync() = 0;

    /**
     * @brief Pauses playback.
     *
     * @return Returns {@link MSERR_OK} if {@link Pause} is successfully added to the task queue;
     * returns an error code defined in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t Pause() = 0;

    /**
     * @brief Stop playback.
     *
     * @return Returns {@link MSERR_OK} if {@link Stop} is successfully added to the task queue;
     * returns an error code defined in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t Stop() = 0;

    /**
     * @brief Restores the player to the initial state.
     *
     * After the function is called, add a playback source by calling {@link SetSource},
     * call {@link Play} to start playback again after {@link Prepare} is called.
     *
     * @return Returns {@link MSERR_OK} if {@link Reset} is successfully added to the task queue;
     * returns an error code defined in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t Reset() = 0;

    /**
     * @brief Releases player resources async
     *
     *  Asynchronous release guarantees the performance
     *  but cannot ensure whether the surfacebuffer is released.
     *  The caller needs to ensure the life cycle security of the sufrace
     *
     * @return Returns {@link MSERR_OK} if {@link Release} is successfully added to the task queue;
     * returns an error code defined in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t Release() = 0;

    /**
     * @brief Sets the volume of the player.
     *
     * This function can be used during playback or pause. The value <b>0</b> indicates no sound,
     * and <b>1</b> indicates the original volume. If no audio device is started or no audio
     * stream exists, the value <b>-1</b> is returned.
     *
     * @param leftVolume Indicates the target volume of the left audio channel to set,
     *        ranging from 0 to 1. each step is 0.01.
     * @param rightVolume Indicates the target volume of the right audio channel to set,
     *        ranging from 0 to 1. each step is 0.01.
     * @return Returns {@link MSERR_OK} if the volume is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetVolume(float leftVolume, float rightVolume) = 0;

    virtual int32_t SetVolumeMode(int32_t mode);

    virtual int32_t SetMediaSource(const std::shared_ptr<AVMediaSource> &mediaSource, AVPlayStrategy strategy) = 0;

    /**
     * @brief Changes the playback position.
     *
     * This function can be used during play or pause.
     *
     * @param mSeconds Indicates the target playback position, accurate to milliseconds.
     * @param mode Indicates the player seek mode. For details, see {@link PlayerSeekMode}.
     * @return Returns {@link MSERR_OK} if the seek is done; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
    */
    virtual int32_t Seek(int32_t mSeconds, PlayerSeekMode mode) = 0;

    /**
     * @brief Obtains the playback position, accurate to millisecond.
     *
     * @param currentTime Indicates the playback position.
     * @return Returns {@link MSERR_OK} if the current position is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetCurrentTime(int32_t &currentTime) = 0;

    /**
     * @brief Obtains the video track info, contains mimeType, bitRate, width, height, frameRata.
     *
     * @param video track info vec.
     * @return Returns {@link MSERR_OK} if the track info is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetVideoTrackInfo(std::vector<Format> &videoTrack) = 0;

    /**
     * @brief Obtains the playbackInfo, contains server_ip_address, average_download_rate,
     * download_rate, is_downloading, buffer_duration.
     *
     * @param playbackInfo.
     * @return Returns {@link MSERR_OK} if the track info is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetPlaybackInfo(Format &playbackInfo) = 0;

    /**
     * @brief Obtains the audio track info, contains mimeType, bitRate, sampleRate, channels, language.
     *
     * @param audio track info vec.
     * @return Returns {@link MSERR_OK} if the track info is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetAudioTrackInfo(std::vector<Format> &audioTrack) = 0;

    /**
     * @brief get the video width.
     *
     * @return Returns width if success; else returns 0
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetVideoWidth() = 0;

    /**
     * @brief get the video height.
     *
     * @return Returns height if success; else returns 0
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetVideoHeight() = 0;

    /**
     * @brief Obtains the total duration of media files, accurate to milliseconds.
     *
     * @param duration Indicates the total duration of media files.
     * @return Returns {@link MSERR_OK} if the current duration is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetDuration(int32_t &duration) = 0;

    /**
     * @brief set the player playback rate
     *
     * @param mode the rate mode {@link PlaybackRateMode} which can set.
     * @return Returns {@link MSERR_OK} if the playback rate is set successful; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetPlaybackSpeed(PlaybackRateMode mode) = 0;

    /**
     * @brief set the player playback rate
     *
     * @param rate the rate which can set.
     * @return Returns {@link MSERR_OK} if the playback rate is set successful; returns an error code defined
     * in {@link media_errors.h} otherwise.
     */
    virtual int32_t SetPlaybackRate(float rate) = 0;

    /**
     * @brief get the current player playback rate
     *
     * @param mode the rate mode {@link PlaybackRateMode} which can get.
     * @return Returns {@link MSERR_OK} if the current player playback rate is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetPlaybackSpeed(PlaybackRateMode &mode) = 0;

    /**
     * @brief set the bit rate use for hls player
     * the playback bitrate expressed in bits per second, expressed in bits per second,
     * which is only valid for HLS protocol network flow. By default,
     * the player will select the appropriate bit rate and speed according to the network connection.
     * report the effective bit rate linked list by "INFO_TYPE_BITRATE_COLLECT"
     * set and select the specified bit rate, and select the bit rate that is less than and closest
     * to the specified bit rate for playback. When ready, read it to query the currently selected bit rate.
     * @param bitRate the bit rate, The unit is bps.
     * @return Returns {@link MSERR_OK} if the bit rate is set successfully; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SelectBitRate(uint32_t bitRate) = 0;

    /**
     * @brief set the playback strategy
     * the playback strategy includes five fileds:
     * preferredWidth: Preferred width, which is of the int type, for example, 1080.
     * preferredHeight: Preferred height, which is of the int type, for example, 1920.
     * preferredBufferDuration: Preferred buffer duration, in seconds. The value ranges from 1 to 20.
     * preferredHdr: Whether HDR is preferred. The value true means that HDR is preferred, and false means the opposite.
     * mutedMediaType: The mediaType to be muted before play, which is of the MediaType type,
     * enableSuperResolution: Whether super resolution is enabled. Must be set before prepare.
     * for example, MediaType::MEDIA_TYPE_AUD.
     * @param playbackStrategy the playback strategy.
     * @return Returns {@link MSERR_OK} if the playback strategy is set successfully; returns an error code defined
    * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetPlaybackStrategy(AVPlayStrategy playbackStrategy)
    {
        (void)playbackStrategy;
        return 0;
    }

    virtual int32_t SetMediaMuted(OHOS::Media::MediaType type, bool isMuted)
    {
        (void)type;
        (void)isMuted;
        return 0;
    }

#ifdef SUPPORT_AUDIO_ONLY
#else
    /**
     * @brief Method to set the surface.
     *
     * @param surface pointer of the surface.
     * @return Returns {@link MSERR_OK} if the surface is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetVideoSurface(sptr<Surface> surface) = 0;
#endif

    /**
     * @brief Checks whether the player is playing.
     *
     * @return Returns true if the playback is playing; false otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual bool IsPlaying() = 0;

    /**
     * @brief Returns the value whether single looping is enabled or not .
     *
     * @return Returns true if the playback is single looping; false otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual bool IsLooping() = 0;

    /**
     * @brief Enables single looping of the media playback.
     *
     * @return Returns {@link MSERR_OK} if the single looping is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetLooping(bool loop) = 0;

    /**
     * @brief Method to set player callback.
     *
     * @param callback object pointer.
     * @return Returns {@link MSERR_OK} if the playercallback is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetPlayerCallback(const std::shared_ptr<PlayerCallback> &callback) = 0;

    /**
     * @brief Sets an extended parameter for player
     *
     * @param format Indicates the string key and value. For details, see {@link Format}
     * @return Returns {@link MSERR_OK} if the parameters are set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetParameter(const Format &param) = 0;

    /**
     * @brief Releases player resources sync
     *
     * Synchronous release ensures effective release of surfacebuffer
     * but this interface will take a long time (when the engine is not idle state)
     * requiring the caller to design an asynchronous mechanism by itself
     *
     * @return Returns {@link MSERR_OK} if the playback is released; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t ReleaseSync() = 0;

    /**
     * @brief Select audio or subtitle track.
     * By default, the first audio stream with data is played, and the subtitle track is not played.
     * After the settings take effect, the original track will become invalid. Please set subtitles
     * in prepared/playing/paused/completed state and set audio tracks in prepared state.
     *
     * @param index Track index, reference {@link #GetAudioTrackInfo} and {@link #GetVideoTrackInfo}.
     * @return Returns {@link MSERR_OK} if selected successfully; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
    */
    virtual int32_t SelectTrack(int32_t index, PlayerSwitchMode mode = PlayerSwitchMode::SWITCH_SMOOTH) = 0;

    /**
     * @brief Deselect the current audio or subtitle track.
     * After audio is deselected, the default track will be played, and after subtitles are deselected,
     * they will not be played. Please set subtitles in prepared/playing/paused/completed state and set
     * audio tracks in prepared state.
     *
     * @param index Track index, reference {@link #GetAudioTrackInfo} and {@link #GetVideoTrackInfo}.
     * @return Returns {@link MSERR_OK} if selected successfully; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
    */
    virtual int32_t DeselectTrack(int32_t index) = 0;

    /**
     * @brief Obtain the currently effective track index.
     * Please get it in the prepared/playing/paused/completed state.
     *
     * @param trackType Media type.
     * @param index Track index, reference {@link #GetAudioTrackInfo} and {@link #GetVideoTrackInfo}.
     * @return Returns {@link MSERR_OK} if the track index is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetCurrentTrack(int32_t trackType, int32_t &index) = 0;

    /**
     * @brief Obtains the subtitle track info, contains mimeType, type, language.
     *
     * @param subtitle track info vec.
     * @return Returns {@link MSERR_OK} if the track info is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetSubtitleTrackInfo(std::vector<Format> &subtitleTrack) = 0;

    virtual int32_t SetDecryptConfig(const sptr<DrmStandard::IMediaKeySessionService> &keySessionProxy,
        bool svp)
    {
        (void)keySessionProxy;
        (void)svp;
        return 0;
    }

    virtual void ReleaseClientListener() = 0;

    /**
     * @brief Enables render video first frame of the media playback.
     *
     * @return Returns {@link MSERR_OK} if the single display is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetRenderFirstFrame(bool display)
    {
        (void)display;
        return 0;
    }

    /**
     * @brief Specify the start and end time to play
     * This function must be called after {@link SetSource}.
     * This function is called to set start and end time
     *
     * @return Returns {@link MSERR_OK} if the single display is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetPlayRange(int64_t start, int64_t end)
    {
        (void)start;
        (void)end;
        return 0;
    }

    /**
     * @brief set get max amplitude callback status.
     *
     * @return Returns {@link MSERR_OK} if the single display is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetMaxAmplitudeCbStatus(bool status)
    {
        (void)status;
        return 0;
    }

    /**
     * @brief Set playback start position and end position.
     * Use the specified seek mode to jump to the playback start position,
     * currently support SEEK_PREVIOUS_SYNC and SEEK_CLOSEST,
     * other values are invalid, the default value is SEEK_PREVIOUS_SYNC.
     * This function must be called after {@link SetSource}.
     *
     * @return Returns {@link MSERR_OK} if the single display is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetPlayRangeWithMode(int64_t start, int64_t end, PlayerSeekMode mode = SEEK_PREVIOUS_SYNC)
    {
        (void)start;
        (void)end;
        (void)mode;
        return 0;
    }

    /**
     * @brief Set playback start position and end position.
     * Use the specified seek mode to jump to the playback start position,
     * currently support SEEK_PREVIOUS_SYNC and SEEK_CLOSEST,
     * other values are invalid, the default value is SEEK_PREVIOUS_SYNC.
     * This function must be called after {@link SetSource}.
     *
     * @return Returns {@link MSERR_OK} if the single display is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 6.0
     * @version 6.0
     */
    virtual int32_t SetPlayRangeUsWithMode(int64_t start, int64_t end, PlayerSeekMode mode = SEEK_PREVIOUS_SYNC)
    {
        (void)start;
        (void)end;
        (void)mode;
        return 0;
    }
    
    /**
     * @brief set get device change callback status.
     *
     * @return Returns {@link MSERR_OK} if the single display is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetDeviceChangeCbStatus(bool status)
    {
        (void)status;
        return 0;
    }

    /**
     * @brief Obtain the api version of application.
     *
     * @return Returns {@link MSERR_OK} if the current api version is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetApiVersion(int32_t &apiVersion)
    {
        (void)apiVersion;
        return 0;
    }

    /**
     * @brief Checks whether the player supports SeekContinuous.
     *
     * @return Returns true if the player supports SeekContinuous; false otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual bool IsSeekContinuousSupported()
    {
        return false;
    }

     /**
     * @brief Obtains the playback position, accurate to millisecond.
     *
     * @param playbackPosition Indicates the playback position.
     * @return Returns {@link MSERR_OK} if the current position is get; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t GetPlaybackPosition(int32_t &playbackPosition)
    {
        playbackPosition = 0;
        return 0;
    }
 
    /**
     * @brief set get sei message callback status.
     *
     * @return Returns {@link MSERR_OK} if the single display is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetSeiMessageCbStatus(bool status, const std::vector<int32_t> &payloadTypes)
    {
        (void)status;
        (void)payloadTypes;
        return 0;
    }

    /**
     * @brief Enable or disable super-resolution dynamically.
     *
     * @return Returns {@link MSERR_OK} if super resolution is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetSuperResolution(bool enabled)
    {
        (void)enabled;
        return 0;
    }

    /**
     * @brief Set video window size for super-resolution.
     *
     * @return Returns {@link MSERR_OK} if video window size is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetVideoWindowSize(int32_t width, int32_t height)
    {
        (void)width;
        (void)height;
        return 0;
    }

    /**
     * @brief Enables or disables the report of media progress.
     *
     * @param enable Indicates whether to enable the report of media progress.
     * @return Returns {@link MSERR_OK} if the report of media progress is enabled or disabled; returns an error code
     * defined in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t EnableReportMediaProgress(bool enable)
    {
        (void)enable;
        return 0;
    }
    
    /**
     * @brief Enables or disables the report of audio interrupt during frozen state.
     *
     * @param enable Indicates whether to enable the report of audio interrupt during frozen state.
     * @return Returns {@link MSERR_OK} if the report of audio interrupt is enabled or disabled; returns an error code
     * defined in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t EnableReportAudioInterrupt(bool enable)
    {
        (void)enable;
        return 0;
    }

    /**
     * @brief Set Start Frame Rate Opt Enabled.
     *
     * @return Returns {@link MSERR_OK} if enabled is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetStartFrameRateOptEnabled(bool enabled)
    {
        (void)enabled;
        return 0;
    }

    /**
     * @brief Set video reopen fd.
     *
     * @return Returns {@link MSERR_OK} if video reopen fd is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t SetReopenFd(int32_t fd)
    {
        (void)fd;
        return 0;
    }
 
    /**
     * @brief Enable or disable camera post processor.
     *
     * @return Returns {@link MSERR_OK} if enable camera post processor is set; returns an error code defined
     * in {@link media_errors.h} otherwise.
     * @since 1.0
     * @version 1.0
     */
    virtual int32_t EnableCameraPostprocessing()
    {
        return 0;
    }

    virtual int32_t SetCameraPostprocessing(bool isOpen)
    {
        (void)isOpen;
        return 0;
    }

    virtual int32_t ForceLoadVideo(bool /* status */)
    {
        return 0;
    }
};

class __attribute__((visibility("default"))) PlayerFactory {
public:
#ifdef UNSUPPORT_PLAYER
    static std::shared_ptr<Player> CreatePlayer()
    {
        return nullptr;
    }

    static std::shared_ptr<Player> CreatePlayer(const PlayerProducer producer)
    {
        return nullptr;
    }
#else
    static std::shared_ptr<Player> CreatePlayer();

    static std::shared_ptr<Player> CreatePlayer(const PlayerProducer producer);
#endif
    static std::vector<pid_t> GetPlayerPids();
private:
    PlayerFactory() = default;
    ~PlayerFactory() = default;
};
} // namespace Media
} // namespace OHOS
#endif // PLAYER_H
