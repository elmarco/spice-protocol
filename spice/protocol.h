/*
   Copyright (C) 2009 Red Hat, Inc.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in
         the documentation and/or other materials provided with the
         distribution.
       * Neither the name of the copyright holder nor the names of its
         contributors may be used to endorse or promote products derived
         from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS
   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _H_SPICE_PROTOCOL
#define _H_SPICE_PROTOCOL

#include <spice/types.h>
#include <spice/draw.h>
#include <spice/start-packed.h>

#define SPICE_MAGIC (*(uint32_t*)"REDQ")
#define SPICE_VERSION_MAJOR (~(uint32_t)0 - 1)
#define SPICE_VERSION_MINOR 2

// Encryption & Ticketing Parameters
#define SPICE_MAX_PASSWORD_LENGTH 60
#define SPICE_TICKET_KEY_PAIR_LENGTH 1024
#define SPICE_TICKET_PUBKEY_BYTES (SPICE_TICKET_KEY_PAIR_LENGTH / 8 + 34)

enum {
    SPICE_CHANNEL_MAIN = 1,
    SPICE_CHANNEL_DISPLAY,
    SPICE_CHANNEL_INPUTS,
    SPICE_CHANNEL_CURSOR,
    SPICE_CHANNEL_PLAYBACK,
    SPICE_CHANNEL_RECORD,
    SPICE_CHANNEL_TUNNEL,

    SPICE_END_CHANNEL
};

enum {
    SPICE_LINK_ERR_OK,
    SPICE_LINK_ERR_ERROR,
    SPICE_LINK_ERR_INVALID_MAGIC,
    SPICE_LINK_ERR_INVALID_DATA,
    SPICE_LINK_ERR_VERSION_MISMATCH,
    SPICE_LINK_ERR_NEED_SECURED,
    SPICE_LINK_ERR_NEED_UNSECURED,
    SPICE_LINK_ERR_PERMISSION_DENIED,
    SPICE_LINK_ERR_BAD_CONNECTION_ID,
    SPICE_LINK_ERR_CHANNEL_NOT_AVAILABLE,
};

enum {
    SPICE_WARN_GENERAL,
};

enum {
    SPICE_INFO_GENERAL,
};

typedef struct SPICE_ATTR_PACKED SpiceLinkHeader {
    uint32_t magic;
    uint32_t major_version;
    uint32_t minor_version;
    uint32_t size;
} SpiceLinkHeader;

typedef struct SPICE_ATTR_PACKED SpiceLinkMess {
    uint32_t connection_id;
    uint8_t channel_type;
    uint8_t channel_id;
    uint32_t num_common_caps;
    uint32_t num_channel_caps;
    uint32_t caps_offset;
} SpiceLinkMess;

typedef struct SPICE_ATTR_PACKED SpiceLinkReply {
    uint32_t error;
    uint8_t pub_key[SPICE_TICKET_PUBKEY_BYTES];
    uint32_t num_common_caps;
    uint32_t num_channel_caps;
    uint32_t caps_offset;
} SpiceLinkReply;

typedef struct SPICE_ATTR_PACKED SpiceLinkEncryptedTicket {
    uint8_t encrypted_data[SPICE_TICKET_KEY_PAIR_LENGTH / 8];
} SpiceLinkEncryptedTicket;

typedef struct SPICE_ATTR_PACKED SpiceDataHeader {
    uint64_t serial;
    uint16_t type;
    uint32_t size;
    uint32_t sub_list; //offset to SpiceSubMessageList[]
} SpiceDataHeader;

typedef struct SPICE_ATTR_PACKED SpicedSubMessage {
    uint16_t type;
    uint32_t size;
} SpicedSubMessage;

typedef struct SPICE_ATTR_PACKED SpiceSubMessageList {
    uint16_t size;
    uint32_t sub_messages[0]; //offsets to SpicedSubMessage
} SpiceSubMessageList;

enum {
    SPICE_MSG_MIGRATE = 1,
    SPICE_MSG_MIGRATE_DATA,
    SPICE_MSG_SET_ACK,
    SPICE_MSG_PING,
    SPICE_MSG_WAIT_FOR_CHANNELS,
    SPICE_MSG_DISCONNECTING,
    SPICE_MSG_NOTIFY,

    SPICE_MSG_FIRST_AVAIL = 101
};

enum {
    SPICE_MSGC_ACK_SYNC = 1,
    SPICE_MSGC_ACK,
    SPICE_MSGC_PONG,
    SPICE_MSGC_MIGRATE_FLUSH_MARK,
    SPICE_MSGC_MIGRATE_DATA,
    SPICE_MSGC_DISCONNECTING,

    SPICE_MSGC_FIRST_AVAIL = 101,
};

enum {
    SPICE_MSG_MAIN_MIGRATE_BEGIN = SPICE_MSG_FIRST_AVAIL,
    SPICE_MSG_MAIN_MIGRATE_CANCEL,
    SPICE_MSG_MAIN_INIT,
    SPICE_MSG_MAIN_CHANNELS_LIST,
    SPICE_MSG_MAIN_MOUSE_MODE,
    SPICE_MSG_MAIN_MULTI_MEDIA_TIME,

    SPICE_MSG_MAIN_AGENT_CONNECTED,
    SPICE_MSG_MAIN_AGENT_DISCONNECTED,
    SPICE_MSG_MAIN_AGENT_DATA,
    SPICE_MSG_MAIN_AGENT_TOKEN,

    SPICE_MSG_MAIN_MIGRATE_SWITCH_HOST,

    SPICE_MSG_END_MAIN,
};

enum {
    SPICE_MSGC_MAIN_CLIENT_INFO = SPICE_MSGC_FIRST_AVAIL,
    SPICE_MSGC_MAIN_MIGRATE_CONNECTED,
    SPICE_MSGC_MAIN_MIGRATE_CONNECT_ERROR,
    SPICE_MSGC_MAIN_ATTACH_CHANNELS,
    SPICE_MSGC_MAIN_MOUSE_MODE_REQUEST,

    SPICE_MSGC_MAIN_AGENT_START,
    SPICE_MSGC_MAIN_AGENT_DATA,
    SPICE_MSGC_MAIN_AGENT_TOKEN,
};

#define SPICE_INPUT_MOTION_ACK_BUNCH 4

enum {
    SPICE_MSG_INPUTS_INIT = SPICE_MSG_FIRST_AVAIL,
    SPICE_MSG_INPUTS_KEY_MODIFIERS,

    SPICE_MSG_INPUTS_MOUSE_MOTION_ACK = SPICE_MSG_FIRST_AVAIL + 10,

    SPICE_MSG_END_INPUTS,
};

#define SPICE_SCROLL_LOCK_MODIFIER (1 << 0)
#define SPICE_NUM_LOCK_MODIFIER (1 << 1)
#define SPICE_CAPS_LOCK_MODIFIER (1 << 2)

typedef struct SPICE_ATTR_PACKED SpiceMsgInputsInit {
    uint32_t keyboard_modifiers;
} SpiceMsgInputsInit;

typedef struct SPICE_ATTR_PACKED SpiceMsgInputsKeyModifiers {
    uint32_t modifiers;
} SpiceMsgInputsKeyModifiers;

typedef struct SPICE_ATTR_PACKED SpiceMsgMainMultiMediaTime {
    uint32_t time;
} SpiceMsgMainMultiMediaTime;

enum {
    SPICE_PUBKEY_TYPE_INVALID,
    SPICE_PUBKEY_TYPE_RSA,
    SPICE_PUBKEY_TYPE_RSA2,
    SPICE_PUBKEY_TYPE_DSA,
    SPICE_PUBKEY_TYPE_DSA1,
    SPICE_PUBKEY_TYPE_DSA2,
    SPICE_PUBKEY_TYPE_DSA3,
    SPICE_PUBKEY_TYPE_DSA4,
    SPICE_PUBKEY_TYPE_DH,
    SPICE_PUBKEY_TYPE_EC,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgMainMigrationBegin {
    uint16_t port;
    uint16_t sport;
    uint32_t host_offset;
    uint32_t host_size;
    uint16_t pub_key_type;
    uint32_t pub_key_offset;
    uint32_t pub_key_size;
} SpiceMsgMainMigrationBegin;

typedef struct SPICE_ATTR_PACKED SpiceMsgMainMigrationSwitchHost {
    uint16_t port;
    uint16_t sport;
    uint32_t host_offset;
    uint32_t host_size;
    uint32_t cert_subject_offset;
    uint32_t cert_subject_size;
} SpiceMsgMainMigrationSwitchHost;

enum {
    SPICE_MIGRATE_NEED_FLUSH = (1 << 0),
    SPICE_MIGRATE_NEED_DATA_TRANSFER = (1 << 1),
};

typedef struct SPICE_ATTR_PACKED SpiceMsgMigrate {
    uint32_t flags;
} SpiceMsgMigrate;

enum {
    SPICE_RES_TYPE_INVALID,
    SPICE_RES_TYPE_PIXMAP,
};

typedef struct SPICE_ATTR_PACKED SpiceResorceID {
    uint8_t type;
    uint64_t id;
} SpiceResorceID;

typedef struct SPICE_ATTR_PACKED SpiceResorceList {
    uint16_t count;
    SpiceResorceID resorces[0];
} SpiceResorceList;

typedef struct SPICE_ATTR_PACKED SpiceMsgSetAck {
    uint32_t generation;
    uint32_t window;
} SpiceMsgSetAck;

typedef struct SPICE_ATTR_PACKED SpiceWaitForChannel {
    uint8_t channel_type;
    uint8_t channel_id;
    uint64_t message_serial;
} SpiceWaitForChannel;

typedef struct SPICE_ATTR_PACKED SpiceMsgWaitForChannels {
    uint8_t wait_count;
    SpiceWaitForChannel wait_list[0];
} SpiceMsgWaitForChannels;

typedef struct SPICE_ATTR_PACKED SpiceChannelId {
    uint8_t type;
    uint8_t id;
} SpiceChannelId;

typedef struct SPICE_ATTR_PACKED SpiceMsgMainInit {
    uint32_t session_id;
    uint32_t display_channels_hint;
    uint32_t supported_mouse_modes;
    uint32_t current_mouse_mode;
    uint32_t agent_connected;
    uint32_t agent_tokens;
    uint32_t multi_media_time;
    uint32_t ram_hint;
} SpiceMsgMainInit;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisconnect {
    uint64_t time_stamp;
    uint32_t reason; // SPICE_ERR_?
} SpiceMsgDisconnect;

enum {
    SPICE_NOTIFY_SEVERITY_INFO,
    SPICE_NOTIFY_SEVERITY_WARN,
    SPICE_NOTIFY_SEVERITY_ERROR,
};

enum {
    SPICE_NOTIFY_VISIBILITY_LOW,
    SPICE_NOTIFY_VISIBILITY_MEDIUM,
    SPICE_NOTIFY_VISIBILITY_HIGH,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgNotify {
    uint64_t time_stamp;
    uint32_t severty;
    uint32_t visibilty;
    uint32_t what;
    uint32_t message_len;
    uint8_t message[0];
} SpiceMsgNotify;

typedef struct SPICE_ATTR_PACKED SpiceMsgChannels {
    uint32_t num_of_channels;
    SpiceChannelId channels[0];
} SpiceMsgChannels;

typedef struct SPICE_ATTR_PACKED SpiceMsgMainMouseMode {
    uint32_t supported_modes;
    uint32_t current_mode;
} SpiceMsgMainMouseMode;

typedef struct SPICE_ATTR_PACKED SpiceMsgPing {
    uint32_t id;
    uint64_t timestamp;
} SpiceMsgPing;

typedef struct SPICE_ATTR_PACKED SpiceMsgMainAgentDisconnect {
    uint32_t error_code; // SPICE_ERR_?
} SpiceMsgMainAgentDisconnect;

#define SPICE_AGENT_MAX_DATA_SIZE 2048

typedef struct SPICE_ATTR_PACKED SpiceMsgMainAgentTokens {
    uint32_t num_tokens;
} SpiceMsgMainAgentTokens, SpiceMsgcMainAgentTokens, SpiceMsgcMainAgentStart;

typedef struct SPICE_ATTR_PACKED SpiceMsgcClientInfo {
    uint64_t cache_size;
} SpiceMsgcClientInfo;

typedef struct SPICE_ATTR_PACKED SpiceMsgcMainMouseModeRequest {
    uint32_t mode;
} SpiceMsgcMainMouseModeRequest;

enum {
    SPICE_MSG_DISPLAY_MODE = SPICE_MSG_FIRST_AVAIL,
    SPICE_MSG_DISPLAY_MARK,
    SPICE_MSG_DISPLAY_RESET,
    SPICE_MSG_DISPLAY_COPY_BITS,

    SPICE_MSG_DISPLAY_INVAL_LIST,
    SPICE_MSG_DISPLAY_INVAL_ALL_PIXMAPS,
    SPICE_MSG_DISPLAY_INVAL_PALETTE,
    SPICE_MSG_DISPLAY_INVAL_ALL_PALETTES,

    SPICE_MSG_DISPLAY_STREAM_CREATE = SPICE_MSG_FIRST_AVAIL + 21,
    SPICE_MSG_DISPLAY_STREAM_DATA,
    SPICE_MSG_DISPLAY_STREAM_CLIP,
    SPICE_MSG_DISPLAY_STREAM_DESTROY,
    SPICE_MSG_DISPLAY_STREAM_DESTROY_ALL,

    SPICE_MSG_DISPLAY_DRAW_FILL = SPICE_MSG_FIRST_AVAIL + 201,
    SPICE_MSG_DISPLAY_DRAW_OPAQUE,
    SPICE_MSG_DISPLAY_DRAW_COPY,
    SPICE_MSG_DISPLAY_DRAW_BLEND,
    SPICE_MSG_DISPLAY_DRAW_BLACKNESS,
    SPICE_MSG_DISPLAY_DRAW_WHITENESS,
    SPICE_MSG_DISPLAY_DRAW_INVERS,
    SPICE_MSG_DISPLAY_DRAW_ROP3,
    SPICE_MSG_DISPLAY_DRAW_STROKE,
    SPICE_MSG_DISPLAY_DRAW_TEXT,
    SPICE_MSG_DISPLAY_DRAW_TRANSPARENT,
    SPICE_MSG_DISPLAY_DRAW_ALPHA_BLEND,

    SPICE_MSG_END_DISPLAY,
};

enum {
    SPICE_CURSOR_FLAGS_NONE = (1 << 0),
    SPICE_CURSOR_FLAGS_CACHE_ME = (1 << 1),
    SPICE_CURSOR_FLAGS_FROM_CACHE = (1 << 2),
};

typedef struct SPICE_ATTR_PACKED SpiceCursor {
    uint32_t flags;
    SpiceCursorHeader header;
    uint8_t data[0];
} SpiceCursor;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayMode {
    uint32_t x_res;
    uint32_t y_res;
    uint32_t bits;
} SpiceMsgDisplayMode;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayBase {
    SpiceRect box;
    SpiceClip clip;
} SpiceMsgDisplayBase;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawFill {
    SpiceMsgDisplayBase base;
    SpiceFill data;
} SpiceMsgDisplayDrawFill;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawOpaque {
    SpiceMsgDisplayBase base;
    SpiceOpaque data;
} SpiceMsgDisplayDrawOpaque;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawCopy {
    SpiceMsgDisplayBase base;
    SpiceCopy data;
} SpiceMsgDisplayDrawCopy;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawTransparent {
    SpiceMsgDisplayBase base;
    SpiceTransparent data;
} SpiceMsgDisplayDrawTransparent;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawAlphaBlend {
    SpiceMsgDisplayBase base;
    SpiceAlphaBlnd data;
} SpiceMsgDisplayDrawAlphaBlend;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayCopyBits {
    SpiceMsgDisplayBase base;
    SpicePoint src_pos;
} SpiceMsgDisplayCopyBits;

typedef SpiceMsgDisplayDrawCopy SpiceMsgDisplayDrawBlend;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawRop3 {
    SpiceMsgDisplayBase base;
    SpiceRop3 data;
} SpiceMsgDisplayDrawRop3;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawBlackness {
    SpiceMsgDisplayBase base;
    SpiceBlackness data;
} SpiceMsgDisplayDrawBlackness;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawWhiteness {
    SpiceMsgDisplayBase base;
    SpiceWhiteness data;
} SpiceMsgDisplayDrawWhiteness;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawInvers {
    SpiceMsgDisplayBase base;
    SpiceInvers data;
} SpiceMsgDisplayDrawInvers;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawStroke {
    SpiceMsgDisplayBase base;
    SpiceStroke data;
} SpiceMsgDisplayDrawStroke;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayDrawText {
    SpiceMsgDisplayBase base;
    SpiceText data;
} SpiceMsgDisplayDrawText;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayInvalOne {
    uint64_t id;
} SpiceMsgDisplayInvalOne;

enum {
    SPICE_VIDEO_CODEC_TYPE_MJPEG = 1,
};

enum {
    SPICE_STREAM_FLAGS_TOP_DOWN = (1 << 0),
};

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayStreamCreate {
    uint32_t id;
    uint32_t flags;
    uint32_t codec_type;
    uint64_t stamp;
    uint32_t stream_width;
    uint32_t stream_height;
    uint32_t src_width;
    uint32_t src_height;
    SpiceRect dest;
    SpiceClip clip;
} SpiceMsgDisplayStreamCreate;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayStreamData {
    uint32_t id;
    uint32_t multi_media_time;
    uint32_t data_size;
    uint32_t ped_size;
    uint8_t data[0];
} SpiceMsgDisplayStreamData;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayStreamClip {
    uint32_t id;
    SpiceClip clip;
} SpiceMsgDisplayStreamClip;

typedef struct SPICE_ATTR_PACKED SpiceMsgDisplayStreamDestroy {
    uint32_t id;
} SpiceMsgDisplayStreamDestroy;

enum {
    SPICE_MSG_CURSOR_INIT = SPICE_MSG_FIRST_AVAIL,
    SPICE_MSG_CURSOR_RESET,
    SPICE_MSG_CURSOR_SET,
    SPICE_MSG_CURSOR_MOVE,
    SPICE_MSG_CURSOR_HIDE,
    SPICE_MSG_CURSOR_TRAIL,
    SPICE_MSG_CURSOR_INVAL_ONE,
    SPICE_MSG_CURSOR_INVAL_ALL,

    SPICE_MSG_END_CURSOR,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgCursorInit {
    SpicePoint16 position;
    uint16_t trail_length;
    uint16_t trail_frequency;
    uint8_t visible;
    SpiceCursor cursor;
} SpiceMsgCursorInit;

typedef struct SPICE_ATTR_PACKED SpiceMsgCursorSet {
    SpicePoint16 postition;
    uint8_t visible;
    SpiceCursor cursor;
} SpiceMsgCursorSet;

typedef struct SPICE_ATTR_PACKED SpiceMsgCursorMove {
    SpicePoint16 postition;
} SpiceMsgCursorMove;

typedef struct SPICE_ATTR_PACKED SpiceMsgCursorTrail {
    uint16_t length;
    uint16_t frequency;
} SpiceMsgCursorTrail;

enum {
    SPICE_MSGC_DISPLAY_INIT = SPICE_MSGC_FIRST_AVAIL,

    SPICE_MSGC_END_DISPLAY,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgcDisplayInit {
    uint8_t pixmap_cache_id;
    int64_t pixmap_cache_size; //in pixels
    uint8_t glz_dictionary_id;
    int32_t glz_dictionary_window_size;       // in pixels
} SpiceMsgcDisplayInit;

enum {
    SPICE_MSGC_INPUTS_KEY_DOWN = SPICE_MSGC_FIRST_AVAIL,
    SPICE_MSGC_INPUTS_KEY_UP,
    SPICE_MSGC_INPUTS_KEY_MODIFIERS,

    SPICE_MSGC_INPUTS_MOUSE_MOTION = SPICE_MSGC_FIRST_AVAIL + 10,
    SPICE_MSGC_INPUTS_MOUSE_POSITION,
    SPICE_MSGC_INPUTS_MOUSE_PRESS,
    SPICE_MSGC_INPUTS_MOUSE_RELEASE,

    SPICE_MSGC_END_INPUTS_MESSGES,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgcKeyDown {
    uint32_t code;
} SpiceMsgcKeyDown;

typedef struct SPICE_ATTR_PACKED SpiceMsgcKeyUp {
    uint32_t code;
} SpiceMsgcKeyUp;

enum {
    SPICE_MOUSE_MODE_SERVER = (1 << 0),
    SPICE_MOUSE_MODE_CLIENT = (1 << 1),
};

typedef struct SPICE_ATTR_PACKED SpiceMsgcKeyModifiers {
    uint32_t modifiers;
} SpiceMsgcKeyModifiers;

enum SpiceMouseButton {
    SPICE_MOUSE_BUTTON_INVALID,
    SPICE_MOUSE_BUTTON_LEFT,
    SPICE_MOUSE_BUTTON_MIDDLE,
    SPICE_MOUSE_BUTTON_RIGHT,
    SPICE_MOUSE_BUTTON_UP,
    SPICE_MOUSE_BUTTON_DOWN,
};

#define SPICE_MOUSE_BUTTON_MASK_LEFT (1 << (SPICE_MOUSE_BUTTON_LEFT - 1))
#define SPICE_MOUSE_BUTTON_MASK_MIDDLE (1 << (SPICE_MOUSE_BUTTON_MIDDLE - 1))
#define SPICE_MOUSE_BUTTON_MASK_RIGHT (1 << (SPICE_MOUSE_BUTTON_RIGHT - 1))

typedef struct SPICE_ATTR_PACKED SpiceMsgcMouseMotion {
    int32_t dx;
    int32_t dy;
    uint32_t buttons_state;
} SpiceMsgcMouseMotion;

typedef struct SPICE_ATTR_PACKED SpiceMsgcMousePosition {
    uint32_t x;
    uint32_t y;
    uint32_t buttons_state;
    uint8_t display_id;
} SpiceMsgcMousePosition;

typedef struct SPICE_ATTR_PACKED SpiceMsgcMousePress {
    int32_t button;
    int32_t buttons_state;
} SpiceMsgcMousePress;

typedef struct SPICE_ATTR_PACKED SpiceMsgcMouseRelease {
    int32_t button;
    int32_t buttons_state;
} SpiceMsgcMouseRelease;

enum {
    SPICE_AUDIO_FMT_INVALD,
    SPICE_AUDIO_FMT_S16,
};

enum {
    SPICE_AUDIO_DATA_MODE_INVALD,
    SPICE_AUDIO_DATA_MODE_RAW,
    SPICE_AUDIO_DATA_MODE_CELT_0_5_1,
};

enum {
    SPICE_MSG_PLAYBACK_DATA = SPICE_MSG_FIRST_AVAIL,
    SPICE_MSG_PLAYBACK_MODE,
    SPICE_MSG_PLAYBACK_START,
    SPICE_MSG_PLAYBACK_STOP,

    SPICE_MSG_END_PLAYBACK,
};

enum {
    SPICE_PLAYBACK_CAP_CELT_0_5_1,
};

enum {
    SPICE_MSG_RECORD_START = SPICE_MSG_FIRST_AVAIL,
    SPICE_MSG_RECORD_STOP,

    SPICE_MSG_END_RECORD,
};

enum {
    SPICE_MSGC_RECORD_DATA = SPICE_MSG_FIRST_AVAIL,
    SPICE_MSGC_RECORD_MODE,
    SPICE_MSGC_RECORD_START_MARK,

    SPICE_MSGC_END_RECORD,
};

enum {
    SPICE_RECORD_CAP_CELT_0_5_1,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgPlaybackMode {
    uint32_t time;
    uint32_t mode; //SPICE_AUDIO_DATA_MODE_?
    uint8_t data[0];
} SpiceMsgPlaybackMode, SpiceMsgcRecordMode;

typedef struct SPICE_ATTR_PACKED SpiceMsgPlaybackStart {
    uint32_t channels;
    uint32_t format; //SPICE_AUDIO_FMT_?
    uint32_t frequency;
    uint32_t time;
} SpiceMsgPlaybackStart;

typedef struct SPICE_ATTR_PACKED SpiceMsgPlaybackPacket {
    uint32_t time;
    uint8_t data[0];
} SpiceMsgPlaybackPacket, SpiceMsgcRecordPacket;

typedef struct SPICE_ATTR_PACKED SpiceMsgRecordStart {
    uint32_t channels;
    uint32_t format; //SPICE_AUDIO_FMT_?
    uint32_t frequency;
} SpiceMsgRecordStart;

typedef struct SPICE_ATTR_PACKED SpiceMsgcRecordStartMark {
    uint32_t time;
} SpiceMsgcRecordStartMark;

enum {
    SPICE_TUNNEL_SERVICE_TYPE_INVALID,
    SPICE_TUNNEL_SERVICE_TYPE_GENERIC,
    SPICE_TUNNEL_SERVICE_TYPE_IPP,
};

enum {
    SPICE_MSG_TUNNEL_INIT = SPICE_MSG_FIRST_AVAIL,
    SPICE_MSG_TUNNEL_SERVICE_IP_MAP,
    SPICE_MSG_TUNNEL_SOCKET_OPEN,
    SPICE_MSG_TUNNEL_SOCKET_FIN,
    SPICE_MSG_TUNNEL_SOCKET_CLOSE,
    SPICE_MSG_TUNNEL_SOCKET_DATA,
    SPICE_MSG_TUNNEL_SOCKET_CLOSED_ACK,
    SPICE_MSG_TUNNEL_SOCKET_TOKEN,

    SPICE_MSG_END_TUNNEL,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelInit {
    uint16_t max_num_of_sockets;
    uint32_t max_socket_data_size;
} SpiceMsgTunnelInit;

enum {
    SPICE_TUNNEL_IP_TYPE_INVALID,
    SPICE_TUNNEL_IP_TYPE_IPv4,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelIpInfo {
    uint16_t type;
    uint8_t data[0];
} SpiceMsgTunnelIpInfo;

typedef uint8_t SpiceTunnelIPv4[4];

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelServiceIpMap {
    uint32_t service_id;
    SpiceMsgTunnelIpInfo virtual_ip;
} SpiceMsgTunnelServiceIpMap;

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelSocketOpen {
    uint16_t connection_id;
    uint32_t service_id;
    uint32_t tokens;
} SpiceMsgTunnelSocketOpen;

/* connection id must be the first field in msgs directed to a specific connection */

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelSocketFin {
    uint16_t connection_id;
} SpiceMsgTunnelSocketFin;

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelSocketClose {
    uint16_t connection_id;
} SpiceMsgTunnelSocketClose;

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelSocketData {
    uint16_t connection_id;
    uint8_t data[0];
} SpiceMsgTunnelSocketData;

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelSocketTokens {
    uint16_t connection_id;
    uint32_t num_tokens;
} SpiceMsgTunnelSocketTokens;

typedef struct SPICE_ATTR_PACKED SpiceMsgTunnelSocketClosedAck {
    uint16_t connection_id;
} SpiceMsgTunnelSocketClosedAck;

enum {
    SPICE_MSGC_TUNNEL_SERVICE_ADD = SPICE_MSGC_FIRST_AVAIL,
    SPICE_MSGC_TUNNEL_SERVICE_REMOVE,
    SPICE_MSGC_TUNNEL_SOCKET_OPEN_ACK,
    SPICE_MSGC_TUNNEL_SOCKET_OPEN_NACK,
    SPICE_MSGC_TUNNEL_SOCKET_FIN,
    SPICE_MSGC_TUNNEL_SOCKET_CLOSED,
    SPICE_MSGC_TUNNEL_SOCKET_CLOSED_ACK,
    SPICE_MSGC_TUNNEL_SOCKET_DATA,

    SPICE_MSGC_TUNNEL_SOCKET_TOKEN,

    SPICE_MSGC_END_TUNNEL,
};

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelAddGenericService {
    uint32_t type;
    uint32_t id;
    uint32_t group;
    uint32_t port;
    uint32_t name;
    uint32_t description;
} SpiceMsgcTunnelAddGenericService;

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelAddPrintService {
    SpiceMsgcTunnelAddGenericService base;
    SpiceMsgTunnelIpInfo ip;
} SpiceMsgcTunnelAddPrintService;

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelRemoveService {
    uint32_t id;
} SpiceMsgcTunnelRemoveService;

/* connection id must be the first field in msgs directed to a specific connection */

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelSocketOpenAck {
    uint16_t connection_id;
    uint32_t tokens;
} SpiceMsgcTunnelSocketOpenAck;

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelSocketOpenNack {
    uint16_t connection_id;
} SpiceMsgcTunnelSocketOpenNack;

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelSocketData {
    uint16_t connection_id;
    uint8_t data[0];
} SpiceMsgcTunnelSocketData;

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelSocketFin {
    uint16_t connection_id;
} SpiceMsgcTunnelSocketFin;

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelSocketClosed {
    uint16_t connection_id;
} SpiceMsgcTunnelSocketClosed;

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelSocketClosedAck {
    uint16_t connection_id;
} SpiceMsgcTunnelSocketClosedAck;

typedef struct SPICE_ATTR_PACKED SpiceMsgcTunnelSocketTokens {
    uint16_t connection_id;
    uint32_t num_tokens;
} SpiceMsgcTunnelSocketTokens;

#include <spice/end-packed.h>

#endif /* _H_SPICE_PROTOCOL */
