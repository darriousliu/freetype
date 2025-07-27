#ifndef FREETYPE_CINTEROP_H
#define FREETYPE_CINTEROP_H

#include "freetype/freetype.h"
#include "freetype/tttables.h"
#include "freetype/tttags.h"

#ifndef __cplusplus
#include <stdbool.h>
#endif

#endif  // FREETYPE_CINTEROP_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char *ptr;
    int length;
} UCharArray;

long init();

bool doneFreeType(long library);

// Returns an array with [major, minor, patch]
int *libraryVersion(long library);

// ---- Face
long newMemoryFace(long library, char *data, int length, long faceIndex);

bool loadMathTable(long face, char *data, int length);

int faceGetAscender(long face);

int faceGetDescender(long face);

long faceGetFaceFlags(long face);

long faceGetFaceIndex(long face);

char *faceGetFamilyName(long face);

int faceGetHeight(long face);

int faceGetMaxAdvanceHeight(long face);

int faceGetMaxAdvanceWidth(long face);

long faceGetNumFaces(long face);

long faceGetNumGlyphs(long face);

long faceGetStyleFlags(long face);

const char *faceGetStyleName(long face);

int faceGetUnderlinePosition(long face);

int faceGetUnderlineThickness(long face);

int faceGetUnitsPerEM(long face);

long faceGetGlyph(long face); // Pointer to FT_GlyphSlot
long faceGetSize(long face); // Pointer to FT_Size
long getTrackKerning(long face, int pointSize, int degree);

// Returns an array with [x, y] coordinates
long *getKerning(long face, int left, int right, int mode);

bool doneFace(long face);

bool referenceFace(long face);

bool hasKerning(long face);

const char *getPostscriptName(long face);

bool selectCharMap(long face, int encoding);

bool setCharMap(long face, long charMap); // Pointer to FT_CharMap
bool faceCheckTrueTypePatents(long face);

bool faceSetUnpatentedHinting(long face, bool value);

unsigned long *getFirstChar(long face);

// Returns an array with [charcode, glyphIndex]
unsigned int getNextChar(long face, long charcode);

unsigned int getCharIndex(long face, int code);

unsigned int getNameIndex(long face, const char *name);

unsigned short getFSTypeFlags(long face);

bool selectSize(long face, int strikeIndex);

bool loadChar(long face, int c, int flags);

bool requestSize(long face, int width, int height,
                 int horizResolution, int vertResolution, int type);

bool setPixelSizes(long face, int width, int height);

bool loadGlyph(long face, int glyphIndex, int loadFlags);

bool setCharSize(long face, int charWidth, int charHeight, int horizResolution,
                 int vertResolution);

// ---- Size
long sizeGetMetrics(long size); // Pointer to FT_Size_Metrics

// ---- Size Metrics
long sizeMetricsGetAscender(long sizeMetrics);

long sizeMetricsGetDescender(long sizeMetrics);

long sizeMetricsGetHeight(long sizeMetrics);

long sizeMetricsGetMaxAdvance(long sizeMetrics);

int sizeMetricsGetXPPEM(long sizeMetrics);

long sizeMetricsGetXScale(long sizeMetrics);

long sizeMetricsGetYPPEM(long sizeMetrics);

long sizeMetricsGetYScale(long sizeMetrics);

// ---- GlyphSlot
long glyphSlotGetLinearHoriAdvance(long glyphSlot);

long glyphSlotGetLinearVertAdvance(long glyphSlot);

long *glyphSlotGetAdvance(long glyphSlot);

// Returns an array with [x, y] coordinates
int glyphSlotGetFormat(long glyphSlot);

int glyphSlotGetBitmapLeft(long glyphSlot);

int glyphSlotGetBitmapTop(long glyphSlot);

long glyphSlotGetBitmap(long glyphSlot); // Pointer to FT_Bitmap
long glyphSlotGetMetrics(long glyphSlot); // Pointer to FT_Glyph_Metrics
bool renderGlyph(long glyphSlot, int renderMode);

// ---- GlyphMetrics
long glyphMetricsGetWidth(long glyphMetrics);

long glyphMetricsGetHeight(long glyphMetrics);

long glyphMetricsGetHoriAdvance(long glyphMetrics);

long glyphMetricsGetVertAdvance(long glyphMetrics);

long glyphMetricsGetHoriBearingX(long glyphMetrics);

long glyphMetricsGetHoriBearingY(long glyphMetrics);

long glyphMetricsGetVertBearingX(long glyphMetrics);

long glyphMetricsGetVertBearingY(long glyphMetrics);

// ---- Bitmap
unsigned int bitmapGetWidth(long bitmap);

unsigned int bitmapGetRows(long bitmap);

int bitmapGetPitch(long bitmap);

unsigned short bitmapGetNumGrays(long bitmap);

unsigned char bitmapGetPaletteMode(long bitmap);

unsigned char bitmapGetPixelMode(long bitmap);

UCharArray bitmapGetBuffer(long bitmap); // Returns a pointer to the buffer

// ---- Charmap
int getCharMapIndex(long charMap);


#ifdef __cplusplus
}
#endif
