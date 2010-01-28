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


#ifndef _H_QXL_DEV
#define _H_QXL_DEV

#include <spice/types.h>
#include <spice/barrier.h>

#include "ipc_ring.h"
#include "draw.h"

#include <spice/start-packed.h>

#define REDHAT_PCI_VENDOR_ID 0x1b36
#define QXL_DEVICE_ID 0x0100 /* 0x100-0x11f reserved for spice */
#define QXL_REVISION 0x03

#define QXL_ROM_MAGIC (*(uint32_t*)"QXRO")
#define QXL_RAM_MAGIC (*(uint32_t*)"QXRA")

enum {
    QXL_RAM_RANGE_INDEX,
    QXL_VRAM_RANGE_INDEX,
    QXL_ROM_RANGE_INDEX,
    QXL_IO_RANGE_INDEX,

    QXL_PCI_RANGES
};

enum {
    QXL_IO_NOTIFY_CMD,
    QXL_IO_NOTIFY_CURSOR,
    QXL_IO_UPDATE_AREA,
    QXL_IO_UPDATE_IRQ,
    QXL_IO_NOTIFY_OOM,
    QXL_IO_RESET,
    QXL_IO_LOG,
    QXL_IO_MEMSLOT_ADD,
    QXL_IO_MEMSLOT_DEL,
    QXL_IO_DETACH_PRIMARY,
    QXL_IO_ATTACH_PRIMARY,
    QXL_IO_CREATE_PRIMARY,
    QXL_IO_DESTROY_PRIMARY,
    QXL_IO_DESTROY_SURFACE_WAIT,

    QXL_IO_RANGE_SIZE
};

typedef struct SPICE_ATTR_PACKED QXLRom {
    uint32_t magic;
    uint32_t id;
    uint32_t update_id;
    uint32_t compression_level;
    uint32_t log_level;
    uint32_t modes_offset;
    uint32_t num_pages;
    uint32_t surface0_area_size;
    uint32_t ram_header_offset;
    uint32_t mm_clock;
    uint64_t flags;
    uint8_t slots_start;
    uint8_t slots_end;
    uint8_t slot_gen_bits;
    uint8_t slot_id_bits;
    uint8_t slot_generation;
} QXLRom;

typedef struct SPICE_ATTR_PACKED QXLMode {
    uint32_t id;
    uint32_t x_res;
    uint32_t y_res;
    uint32_t bits;
    uint32_t stride;
    uint32_t x_mili;
    uint32_t y_mili;
    uint32_t orientation;
} QXLMode;

typedef struct SPICE_ATTR_PACKED QXLModes {
    uint32_t n_modes;
    QXLMode modes[0];
} QXLModes;

typedef uint64_t PHYSICAL;
typedef uint32_t QXLFIXED; //fixed 28.4

enum QXLCmdType {
    QXL_CMD_NOP,
    QXL_CMD_DRAW,
    QXL_CMD_UPDATE,
    QXL_CMD_CURSOR,
    QXL_CMD_MESSAGE,
};

typedef struct SPICE_ATTR_PACKED QXLCommand {
    PHYSICAL data;
    uint32_t type;
    uint32_t ped;
} QXLCommand;

typedef struct SPICE_ATTR_PACKED QXLCommandExt {
    QXLCommand cmd;
    uint32_t group_id;
} QXLCommandExt;

typedef struct SPICE_ATTR_PACKED QXLMemSlot {
    uint64_t mem_start;
    uint64_t mem_end;
} QXLMemSlot;

#define QXL_SURF_TYPE_PRIMARY 0

typedef struct SPICE_ATTR_PACKED QXLSurfaceCreate {
    uint32_t width;
    uint32_t height;
    int32_t stride;
    uint32_t depth;
    uint32_t position;
    uint32_t mouse_mode;
    uint32_t flags;
    uint32_t type;
    PHYSICAL mem;
} QXLSurfaceCreate;

RING_DECLARE(QXLCommandRing, QXLCommand, 32);
RING_DECLARE(QXLCursorRing, QXLCommand, 32);

RING_DECLARE(QXLReleaseRing, uint64_t, 8);

#define QXL_LOG_BUF_SIZE 4096

#define QXL_INTERRUPT_DISPLAY (1 << 0)
#define QXL_INTERRUPT_CURSOR (1 << 1)

typedef struct SPICE_ATTR_PACKED QXLRam {
    uint32_t magic;
    uint32_t int_pending;
    uint32_t int_mask;
    uint8_t log_buf[QXL_LOG_BUF_SIZE];
    QXLCommandRing cmd_ring;
    QXLCursorRing cursor_ring;
    QXLReleaseRing release_ring;
    Rect update_area;
    QXLMemSlot mem_slot;
    QXLSurfaceCreate create_surface;
    uint64_t flags;
} QXLRam;

typedef union QXLReleaseInfo {
    uint64_t id;      // in
    uint64_t next;    // out
} QXLReleaseInfo;

typedef struct QXLReleaseInfoExt {
    QXLReleaseInfo *info;
    uint32_t group_id;
} QXLReleaseInfoExt;

typedef struct  SPICE_ATTR_PACKED QXLDataChunk {
    uint32_t data_size;
    PHYSICAL prev_chunk;
    PHYSICAL next_chunk;
    uint8_t data[0];
} QXLDataChunk;

typedef struct SPICE_ATTR_PACKED QXLMessage {
    QXLReleaseInfo release_info;
    uint8_t data[0];
} QXLMessage;

typedef struct SPICE_ATTR_PACKED QXLUpdateCmd {
    QXLReleaseInfo release_info;
    Rect area;
    uint32_t update_id;
} QXLUpdateCmd;

typedef struct SPICE_ATTR_PACKED QXLCursor {
    CursorHeader header;
    uint32_t data_size;
    QXLDataChunk chunk;
} QXLCursor;

enum {
    QXL_CURSOR_SET,
    QXL_CURSOR_MOVE,
    QXL_CURSOR_HIDE,
    QXL_CURSOR_TRAIL,
};

#define QXL_CURSUR_DEVICE_DATA_SIZE 128

typedef struct SPICE_ATTR_PACKED QXLCursorCmd {
    QXLReleaseInfo release_info;
    uint8_t type;
    union {
        struct SPICE_ATTR_PACKED {
            Point16 position;
            uint8_t visible;
            PHYSICAL shape;
        } set;
        struct SPICE_ATTR_PACKED {
            uint16_t length;
            uint16_t frequency;
        } trail;
        Point16 position;
    } u;
    uint8_t device_data[QXL_CURSUR_DEVICE_DATA_SIZE]; //todo: dynamic size from rom
} QXLCursorCmd;

enum {
    QXL_DRAW_NOP,
    QXL_DRAW_FILL,
    QXL_DRAW_OPAQUE,
    QXL_DRAW_COPY,
    QXL_COPY_BITS,
    QXL_DRAW_BLEND,
    QXL_DRAW_BLACKNESS,
    QXL_DRAW_WHITENESS,
    QXL_DRAW_INVERS,
    QXL_DRAW_ROP3,
    QXL_DRAW_STROKE,
    QXL_DRAW_TEXT,
    QXL_DRAW_TRANSPARENT,
    QXL_DRAW_ALPHA_BLEND,
};

typedef struct SPICE_ATTR_PACKED QXLString {
    uint32_t data_size;
    uint16_t length;
    uint16_t flags;
    QXLDataChunk chunk;
} QXLString;

typedef struct SPICE_ATTR_PACKED QXLCopyBits {
    Point src_pos;
} QXLCopyBits;

#define QXL_EFFECT_BLEND 0
#define QXL_EFFECT_OPAQUE 1
#define QXL_EFFECT_REVERT_ON_DUP 2
#define QXL_EFFECT_BLACKNESS_ON_DUP 3
#define QXL_EFFECT_WHITENESS_ON_DUP 4
#define QXL_EFFECT_NOP_ON_DUP 5
#define QXL_EFFECT_NOP 6
#define QXL_EFFECT_OPAQUE_BRUSH 7

typedef struct SPICE_ATTR_PACKED QXLDrawable {
    QXLReleaseInfo release_info;
    uint8_t effect;
    uint8_t type;
    uint8_t self_bitmap;
    Rect self_bitmap_area;
    Rect bbox;
    Clip clip;
    uint32_t mm_time;
    union {
        Fill fill;
        Opaque opaque;
        Copy copy;
        Transparent transparent;
        AlphaBlnd alpha_blend;
        QXLCopyBits copy_bits;
        Blend blend;
        Rop3 rop3;
        Stroke stroke;
        Text text;
        Blackness blackness;
        Invers invers;
        Whiteness whiteness;
    } u;
} QXLDrawable;

typedef struct SPICE_ATTR_PACKED QXLClipRects {
    uint32_t num_rects;
    QXLDataChunk chunk;
} QXLClipRects;

enum {
    QXL_PATH_BEGIN = (1 << 0),
    QXL_PATH_END = (1 << 1),
    QXL_PATH_CLOSE = (1 << 3),
    QXL_PATH_BEZIER = (1 << 4),
};

typedef struct SPICE_ATTR_PACKED QXLPath {
    uint32_t data_size;
    QXLDataChunk chunk;
} QXLPath;

enum {
    QXL_IMAGE_GROUP_DRIVER,
    QXL_IMAGE_GROUP_DEVICE,
    QXL_IMAGE_GROUP_RED,
    QXL_IMAGE_GROUP_DRIVER_DONT_CACHE,
};

typedef struct SPICE_ATTR_PACKED QXLImageID {
    uint32_t group;
    uint32_t unique;
} QXLImageID;

enum {
    QXL_IMAGE_CACHE = (1 << 0),
};

enum {
    QXL_BITMAP_DIRECT = (1 << 0),
    QXL_BITMAP_UNSTABLE = (1 << 1),
    QXL_BITMAP_TOP_DOWN = (1 << 2), // == BITMAP_TOP_DOWN
};

#define QXL_SET_IMAGE_ID(image, _group, _unique) {              \
    uint64_t* id_ptr = &(image)->descriptor.id;                   \
    QXLImageID *image_id = (QXLImageID *)id_ptr;                \
    image_id->group = _group;                                   \
    image_id->unique = _unique;                                 \
}

typedef struct SPICE_ATTR_PACKED QXLImage {
    ImageDescriptor descriptor;
    union { // variable length
        Bitmap bitmap;
        QUICData quic;
    };
} QXLImage;

#include <spice/end-packed.h>

#endif
