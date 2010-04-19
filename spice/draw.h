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

#ifndef _H_SPICE_DRAW
#define _H_SPICE_DRAW

#include <spice/types.h>

#include <spice/start-packed.h>

#define SPICE_GET_ADDRESS(addr) ((void *)(unsigned long)(addr))
#define SPICE_SET_ADDRESS(addr, val) ((addr) = (unsigned long)(val))

typedef int32_t SPICE_FIXED28_4;
typedef uint64_t SPICE_ADDRESS;

enum {
    SPICE_PATH_BEGIN = (1 << 0),
    SPICE_PATH_END = (1 << 1),
    SPICE_PATH_CLOSE = (1 << 3),
    SPICE_PATH_BEZIER = (1 << 4),
};

enum {
    SPICE_LINE_ATTR_STARTGAP = (1 << 2),
    SPICE_LINE_ATTR_STYLED = (1 << 3),
};

typedef struct SPICE_ATTR_PACKED SpicePointFix {
    SPICE_FIXED28_4 x;
    SPICE_FIXED28_4 y;
} SpicePointFix;

typedef struct SPICE_ATTR_PACKED SpicePoint {
    int32_t x;
    int32_t y;
} SpicePoint;

typedef struct SPICE_ATTR_PACKED SpicePoint16 {
    int16_t x;
    int16_t y;
} SpicePoint16;

typedef struct SPICE_ATTR_PACKED SpiceRect {
    int32_t top;
    int32_t left;
    int32_t bottom;
    int32_t right;
} SpiceRect;

typedef struct SPICE_ATTR_PACKED SpicePathSeg {
    uint32_t flags;
    uint32_t count;
    uint8_t data[0];
} SpicePathSeg;

enum SpiceClipType {
    SPICE_CLIP_TYPE_NONE,
    SPICE_CLIP_TYPE_RECTS,
    SPICE_CLIP_TYPE_PATH,
};

typedef struct SPICE_ATTR_PACKED SpiceClip {
    uint32_t type;
    SPICE_ADDRESS data;
} SpiceClip;

enum SpiceROPDescriptor {
    SPICE_ROPD_INVERS_SRC = (1 << 0),
    SPICE_ROPD_INVERS_BRUSH = (1 << 1),
    SPICE_ROPD_INVERS_DEST = (1 << 2),
    SPICE_ROPD_OP_PUT = (1 << 3),
    SPICE_ROPD_OP_OR = (1 << 4),
    SPICE_ROPD_OP_AND = (1 << 5),
    SPICE_ROPD_OP_XOR = (1 << 6),
    SPICE_ROPD_OP_BLACKNESS = (1 << 7),
    SPICE_ROPD_OP_WHITENESS = (1 << 8),
    SPICE_ROPD_OP_INVERS = (1 << 9),
    SPICE_ROPD_INVERS_RES = (1 << 10),
};

typedef struct SPICE_ATTR_PACKED SpicePattern {
    SPICE_ADDRESS pat;
    SpicePoint pos;
} SpicePattern;

enum {
    SPICE_BRUSH_TYPE_NONE,
    SPICE_BRUSH_TYPE_SOLID,
    SPICE_BRUSH_TYPE_PATTERN,
};

typedef struct SPICE_ATTR_PACKED SpiceBrush {
    uint32_t type;
    union {
        uint32_t color;
        SpicePattern pattern;
    } u;
} SpiceBrush;

enum {
    SPICE_MASK_FLAGS_INVERS = (1 << 0),
};

typedef struct SPICE_ATTR_PACKED SpiceQMask {
    uint8_t flags;
    SpicePoint pos;
    SPICE_ADDRESS bitmap;
} SpiceQMask;

typedef struct SPICE_ATTR_PACKED SpiceFill {
    SpiceBrush brush;
    uint16_t rop_decriptor;
    SpiceQMask mask;
} SpiceFill;

typedef struct SPICE_ATTR_PACKED SpicePalette {
    uint64_t unique;
    uint16_t num_ents;
    uint32_t ents[0];
} SpicePalette;

enum {
    SPICE_SURFACE_FMT_INVALID,
    SPICE_SURFACE_FMT_1_A     = 1,
    SPICE_SURFACE_FMT_8_A     = 8,
    SPICE_SURFACE_FMT_16_555  = 16 + (0 << 6),
    SPICE_SURFACE_FMT_16_565  = 16 + (1 << 6),
    SPICE_SURFACE_FMT_32_xRGB = 32 + (0 << 6),
    SPICE_SURFACE_FMT_32_ARGB = 32 + (1 << 6),
};
#define SPICE_SURFACE_FMT_DEPTH(_d) ((_d) & 0x3f)

enum {
    SPICE_IMAGE_TYPE_BITMAP,
    SPICE_IMAGE_TYPE_QUIC,
    SPICE_IMAGE_TYPE_RESERVED,
    SPICE_IMAGE_TYPE_LZ_PLT = 100,
    SPICE_IMAGE_TYPE_LZ_RGB,
    SPICE_IMAGE_TYPE_GLZ_RGB,
    SPICE_IMAGE_TYPE_FROM_CACHE,
    SPICE_IMAGE_TYPE_SURFACE,
};

enum {
    SPICE_IMAGE_FLAGS_CACHE_ME = (1 << 0),
};

typedef struct SPICE_ATTR_PACKED SpiceImageDescriptor {
    uint64_t id;
    uint8_t type;
    uint8_t flags;
    uint32_t width;
    uint32_t height;
} SpiceImageDescriptor;

enum {
    SPICE_BITMAP_FMT_INVALID,
    SPICE_BITMAP_FMT_1BIT_LE /* unused */,
    SPICE_BITMAP_FMT_1BIT_BE,
    SPICE_BITMAP_FMT_4BIT_LE /* unused */,
    SPICE_BITMAP_FMT_4BIT_BE,
    SPICE_BITMAP_FMT_8BIT /* 8bit indexed mode */,
    SPICE_BITMAP_FMT_16BIT /* 555 format */,
    SPICE_BITMAP_FMT_24BIT /* 3 byte, brg */,
    SPICE_BITMAP_FMT_32BIT /* 4 byte, xrgb in little endian format */,
    SPICE_BITMAP_FMT_RGBA /* 4 byte, argb in little endian format */,
};

enum {
    SPICE_BITMAP_FLAGS_PAL_CACHE_ME = (1 << 0),
    SPICE_BITMAP_FLAGS_PAL_FROM_CACHE = (1 << 1),
    SPICE_BITMAP_FLAGS_TOP_DOWN = (1 << 2),
};

typedef struct SPICE_ATTR_PACKED SpiceBitmap {
    uint8_t format;
    uint8_t flags;
    uint32_t x;
    uint32_t y;
    uint32_t stride;
    SPICE_ADDRESS palette;
    SPICE_ADDRESS data; //data[0] ?
} SpiceBitmap;

typedef struct SPICE_ATTR_PACKED SpiceBitmapImage {
    SpiceImageDescriptor descriptor;
    SpiceBitmap bitmap;
} SpiceBitmapImage;

typedef struct SPICE_ATTR_PACKED SpiceSurface {
    uint32_t surface_id;
} SpiceSurface;

typedef struct SPICE_ATTR_PACKED SpiceSurfaceImage {
    SpiceImageDescriptor descriptor; //?
    SpiceSurface surface;
} SpiceSurfaceImage;

typedef struct SPICE_ATTR_PACKED SpiceQUICData {
    uint32_t data_size;
    uint8_t data[0];
} SpiceQUICData, SpiceLZRGBData;

typedef struct SPICE_ATTR_PACKED SpiceQUICImage {
    SpiceImageDescriptor descriptor;
    SpiceQUICData quic;
} SpiceQUICImage;

typedef struct SPICE_ATTR_PACKED SpiceLZRGBImage {
    SpiceImageDescriptor descriptor;
    SpiceLZRGBData lz_rgb;
} SpiceLZRGBImage;

typedef struct SPICE_ATTR_PACKED SpiceLZPLTData {
    uint8_t flags;
    uint32_t data_size;
    SPICE_ADDRESS palette;
    uint8_t data[0];
} SpiceLZPLTData;

typedef struct SPICE_ATTR_PACKED SpiceLZPLTImage {
    SpiceImageDescriptor descriptor;
    SpiceLZPLTData lz_plt;
} SpiceLZPLTImage;

enum {
    SPICE_IMAGE_SCALE_MODE_INTERPOLATE,
    SPICE_IMAGE_SCALE_MODE_NEAREST,
};

typedef struct SPICE_ATTR_PACKED SpiceOpaque {
    SPICE_ADDRESS src_bitmap;
    SpiceRect src_area;
    SpiceBrush brush;
    uint16_t rop_decriptor;
    uint8_t scale_mode;
    SpiceQMask mask;
} SpiceOpaque;

typedef struct SPICE_ATTR_PACKED SpiceCopy {
    SPICE_ADDRESS src_bitmap;
    SpiceRect src_area;
    uint16_t rop_decriptor;
    uint8_t scale_mode;
    SpiceQMask mask;
} SpiceCopy, SpiceBlend;

typedef struct SPICE_ATTR_PACKED SpiceTransparent {
    SPICE_ADDRESS src_bitmap;
    SpiceRect src_area;
    uint32_t src_color;
    uint32_t true_color;
} SpiceTransparent;

typedef struct SPICE_ATTR_PACKED SpiceAlphaBlnd {
    uint8_t alpha;
    SPICE_ADDRESS src_bitmap;
    SpiceRect src_area;
} SpiceAlphaBlnd;

typedef struct SPICE_ATTR_PACKED SpiceRop3 {
    SPICE_ADDRESS src_bitmap;
    SpiceRect src_area;
    SpiceBrush brush;
    uint8_t rop3;
    uint8_t scale_mode;
    SpiceQMask mask;
} SpiceRop3;

typedef struct SPICE_ATTR_PACKED SpiceBlackness {
    SpiceQMask mask;
} SpiceBlackness, SpiceInvers, SpiceWhiteness;

enum {
    SPICE_LINE_FLAGS_STYLED = (1 << 3),
    SPICE_LINE_FLAGS_START_WITH_GAP = (1 << 2),
};

enum {
    SPICE_LINE_CAP_ROUND,
    SPICE_LINE_CAP_SQUARE,
    SPICE_LINE_CAP_BUTT,
};

enum {
    SPICE_LINE_JOIN_ROUND,
    SPICE_LINE_JOIN_BEVEL,
    SPICE_LINE_JOIN_MITER,
};

typedef struct SPICE_ATTR_PACKED SpiceLineAttr {
    uint8_t flags;
    uint8_t join_style;
    uint8_t end_style;
    uint8_t style_nseg;
    SPICE_FIXED28_4 width;
    SPICE_FIXED28_4 miter_limit;
    SPICE_ADDRESS style; //data[0] ?
} SpiceLineAttr;

typedef struct SPICE_ATTR_PACKED SpiceStroke {
    SPICE_ADDRESS path;
    SpiceLineAttr attr;
    SpiceBrush brush;
    uint16_t fore_mode;
    uint16_t back_mode;
} SpiceStroke;

typedef struct SPICE_ATTR_PACKED SpiceRasterGlyph {
    SpicePoint render_pos;
    SpicePoint glyph_origin;
    uint16_t width;
    uint16_t height;
    uint8_t data[0];
} SpiceRasterGlyph;

typedef struct SPICE_ATTR_PACKED SpiceVectorGlyph {
    SpicePoint render_pos;
    uint32_t data_size;
    uint8_t data[0]; //SpicePathSeg[]
} SpiceVectorGlyph;

enum {
    SPICE_STRING_FLAGS_RASTER_A1 = 1 << 0,
    SPICE_STRING_FLAGS_RASTER_A4 = 1 << 1,
    SPICE_STRING_FLAGS_RASTER_A8 = 1 << 2,
    SPICE_STRING_FLAGS_RASTER_TOP_DOWN = 1 << 3,
};

typedef struct SPICE_ATTR_PACKED SpiceString {
    uint16_t length;
    uint16_t flags;
    uint8_t data[0];
} SpiceString;

typedef struct SPICE_ATTR_PACKED SpiceText {
    SPICE_ADDRESS str;
    SpiceRect back_area;
    SpiceBrush fore_brush;
    SpiceBrush back_brush;
    uint16_t fore_mode;
    uint16_t back_mode;
} SpiceText;

enum {
    SPICE_CURSOR_TYPE_ALPHA,
    SPICE_CURSOR_TYPE_MONO,
    SPICE_CURSOR_TYPE_COLOR4,
    SPICE_CURSOR_TYPE_COLOR8,
    SPICE_CURSOR_TYPE_COLOR16,
    SPICE_CURSOR_TYPE_COLOR24,
    SPICE_CURSOR_TYPE_COLOR32,
};

typedef struct SPICE_ATTR_PACKED SpiceCursorHeader {
    uint64_t unique;
    uint16_t type;
    uint16_t width;
    uint16_t height;
    uint16_t hot_spot_x;
    uint16_t hot_spot_y;
} SpiceCursorHeader;

#include <spice/end-packed.h>

#endif /* _H_SPICE_DRAW */
