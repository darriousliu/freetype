#include "freetype_cinterop.h"
#include "freetype/freetype.h"

#include "freetype/tttables.h"
#include "freetype/tttags.h"
#include "freetype/internal/ftobjs.h"


extern "C" long initLibrary() {
  FT_Library library;
  if (FT_Init_FreeType(&library)) {
    return 0; // Error initializing FreeType
  }
  return reinterpret_cast<long>(library);
}

extern "C" bool doneFreeType(const long library) {
  if (library == 0) {
    return false; // Invalid library handle
  }
  return FT_Done_FreeType(reinterpret_cast<FT_Library>(library));
}

extern "C" int *libraryVersion(const long library) {
  if (library == 0) {
    return nullptr; // Invalid library handle
  }
  int major, minor, patch;
  const auto libraryVersion = new int[3];
  const auto ftLibrary = reinterpret_cast<FT_Library>(library);
  FT_Library_Version(ftLibrary, &major, &minor, &patch);
  libraryVersion[0] = major;
  libraryVersion[1] = minor;
  libraryVersion[2] = patch;
  return libraryVersion;
}

extern "C" long newMemoryFace(const long library, char *data,
                              const int length, const long faceIndex) {
  if (library == 0 || data == nullptr || length <= 0) {
    // 输出原因
    printf("Invalid parameters");
    return 0; // Invalid parameters
  }
  const auto ftLibrary = reinterpret_cast<FT_Library>(library);
  const auto *header = reinterpret_cast<const FT_Byte *>(data);

  printf("Font signature: %c%c%c%c\n", header[0], header[1], header[2],
         header[3]);

  FT_Face face;
  const FT_Error error =
      FT_New_Memory_Face(ftLibrary, header, length, faceIndex, &face);

  printf("FT_New_Memory_Face returned: %d\n", error);

  printf("Face loaded successfully: %s - %s\n",
         face->family_name ? face->family_name : "Unknown",
         face->style_name ? face->style_name : "Unknown");

  return reinterpret_cast<long>(face);
}

extern "C" bool loadMathTable(const long face, char *data, const int length) {
  FT_ULong t_length = length;
  const auto result = FT_Load_Sfnt_Table(reinterpret_cast<FT_Face>(face),
                                         TTAG_MATH, 0,
                                         reinterpret_cast<FT_Byte *>(data),
                                         &t_length);
  return result;
}

int faceGetAscender(const long face) {
  return reinterpret_cast<FT_Face>(face)->ascender;
}

int faceGetDescender(const long face) {
  return reinterpret_cast<FT_Face>(face)->descender;
}

long faceGetFaceFlags(const long face) {
  return reinterpret_cast<FT_Face>(face)->face_flags;
}

long faceGetFaceIndex(const long face) {
  return reinterpret_cast<FT_Face>(face)->face_index;
}

char *faceGetFamilyName(const long face) {
  const auto familyName = reinterpret_cast<FT_Face>(face)->family_name;
  return familyName;
}

int faceGetHeight(const long face) {
  return reinterpret_cast<FT_Face>(face)->height;
}

int faceGetMaxAdvanceHeight(const long face) {
  return reinterpret_cast<FT_Face>(face)->max_advance_height;
}

int faceGetMaxAdvanceWidth(const long face) {
  return reinterpret_cast<FT_Face>(face)->max_advance_width;
}

long faceGetNumFaces(const long face) {
  return reinterpret_cast<FT_Face>(face)->num_faces;
}

long faceGetNumGlyphs(const long face) {
  return reinterpret_cast<FT_Face>(face)->num_glyphs;
}

long faceGetStyleFlags(const long face) {
  return reinterpret_cast<FT_Face>(face)->style_flags;
}

const char *faceGetStyleName(const long face) {
  const auto styleName = reinterpret_cast<FT_Face>(face)->style_name;
  return styleName;
}

int faceGetUnderlinePosition(const long face) {
  return reinterpret_cast<FT_Face>(face)->underline_position;
}

int faceGetUnderlineThickness(const long face) {
  return reinterpret_cast<FT_Face>(face)->underline_thickness;
}

int faceGetUnitsPerEM(const long face) {
  return reinterpret_cast<FT_Face>(face)->units_per_EM;
}

// Pointer to FT_GlyphSlot
long faceGetGlyph(const long face) {
  return reinterpret_cast<long>(reinterpret_cast<FT_Face>(face)->glyph);
}

// Pointer to FT_Size
long faceGetSize(const long face) {
  return reinterpret_cast<long>(reinterpret_cast<FT_Face>(face)->size);
}

long getTrackKerning(const long face, const int pointSize, const int degree) {
  long kern;
  if (FT_Get_Track_Kerning(reinterpret_cast<FT_Face>(face), pointSize, degree,
                           &kern) != 0)
    return 0;
  return kern;
}

long *getKerning(const long face, const int left, const int right,
                 const int mode) {
  FT_Vector vector;
  long x = 0;
  long y = 0;
  if (!FT_Get_Kerning(reinterpret_cast<FT_Face>(face), left, right, mode,
                      &vector)) {
    x = vector.x;
    y = vector.y;
  }
  const auto kerning = new long[2];
  kerning[0] = x;
  kerning[1] = y;
  return kerning;
}

bool doneFace(const long face) {
  return FT_Done_Face(reinterpret_cast<FT_Face>(face));
}

bool referenceFace(const long face) {
  return FT_Reference_Face(reinterpret_cast<FT_Face>(face));
}

bool hasKerning(const long face) {
  return FT_HAS_KERNING(reinterpret_cast<FT_Face>(face));
}

const char *getPostscriptName(const long face) {
  return FT_Get_Postscript_Name(reinterpret_cast<FT_Face>(face));
}

bool selectCharMap(const long face, int encoding) {
  return FT_Select_Charmap(reinterpret_cast<FT_Face>(face),
                           static_cast<FT_Encoding>(encoding));

}

// Pointer to FT_CharMap
bool setCharMap(const long face, const long charMap) {
  return FT_Set_Charmap(reinterpret_cast<FT_Face>(face),
                        reinterpret_cast<FT_CharMap>(charMap));

}

bool faceCheckTrueTypePatents(const long face) {
  return FT_Face_CheckTrueTypePatents(reinterpret_cast<FT_Face>(face));

}

bool faceSetUnpatentedHinting(const long face, const bool value) {
  return FT_Face_SetUnpatentedHinting(reinterpret_cast<FT_Face>(face), value);

}

// Returns an array with [charcode, glyphIndex]
unsigned long *getFirstChar(const long face) {
  FT_UInt gindex;
  const FT_ULong charcode = FT_Get_First_Char(reinterpret_cast<FT_Face>(face),
                                        &gindex);

  const auto result = new unsigned long[2];
  result[0] = charcode;
  result[1] = gindex;

  return result;
}

unsigned int getNextChar(const long face, const long charcode) {
  FT_UInt gindex;
  FT_Get_Next_Char(reinterpret_cast<FT_Face>(face), charcode, &gindex);
  return gindex;
}

unsigned int getCharIndex(const long face, const int code) {
  return FT_Get_Char_Index(reinterpret_cast<FT_Face>(face), code);
}

unsigned int getNameIndex(const long face, const char *name) {
  const auto glyphIndex = FT_Get_Name_Index(reinterpret_cast<FT_Face>(face), name);
  return glyphIndex;
}

unsigned short getFSTypeFlags(const long face) {
  return FT_Get_FSType_Flags(reinterpret_cast<FT_Face>(face));
}

bool selectSize(const long face, const int strikeIndex) {
  return FT_Select_Size(reinterpret_cast<FT_Face>(face), strikeIndex);
}

bool loadChar(const long face, const int c, const int flags) {
  return FT_Load_Char(reinterpret_cast<FT_Face>(face), c, flags);
}

bool requestSize(const long face, const int width, const int height,
                 const int horizResolution, const int vertResolution,
                 int type) {

  FT_Size_RequestRec req = {};
  req.height = height;
  req.width = width;
  req.horiResolution = horizResolution;
  req.vertResolution = vertResolution;
  req.type = static_cast<FT_Size_Request_Type>(type);

  return FT_Request_Size(reinterpret_cast<FT_Face>(face), &req);
}

bool setPixelSizes(const long face, const int width, const int height) {
  return FT_Set_Pixel_Sizes(reinterpret_cast<FT_Face>(face), width, height);
}

bool loadGlyph(const long face, const int glyphIndex, const int loadFlags) {
  return FT_Load_Glyph(reinterpret_cast<FT_Face>(face), glyphIndex, loadFlags);
}

bool setCharSize(const long face, const int charWidth, const int charHeight,
                 const int horizResolution,
                 const int vertResolution) {
  return FT_Set_Char_Size(reinterpret_cast<FT_Face>(face), charWidth,
                          charHeight,
                          horizResolution,
                          vertResolution);
}

long sizeGetMetrics(const long size) {
  return reinterpret_cast<long>(&(reinterpret_cast<FT_Size>(size)->metrics));
}

long sizeMetricsGetAscender(const long sizeMetrics) {
  return reinterpret_cast<FT_Size_Metrics *>(sizeMetrics)->ascender;
}

long sizeMetricsGetDescender(const long sizeMetrics) {
  return reinterpret_cast<FT_Size_Metrics *>(sizeMetrics)->descender;
}

long sizeMetricsGetHeight(const long sizeMetrics) {
  return reinterpret_cast<FT_Size_Metrics *>(sizeMetrics)->height;
}

long sizeMetricsGetMaxAdvance(const long sizeMetrics) {
  return reinterpret_cast<FT_Size_Metrics *>(sizeMetrics)->max_advance;
}

int sizeMetricsGetXPPEM(const long sizeMetrics) {
  return reinterpret_cast<FT_Size_Metrics *>(sizeMetrics)->x_ppem;
}

long sizeMetricsGetXScale(const long sizeMetrics) {
  return reinterpret_cast<FT_Size_Metrics *>(sizeMetrics)->x_scale;
}

long sizeMetricsGetYPPEM(const long sizeMetrics) {
  return reinterpret_cast<FT_Size_Metrics *>(sizeMetrics)->y_ppem;
}

long sizeMetricsGetYScale(const long sizeMetrics) {
  return reinterpret_cast<FT_Size_Metrics *>(sizeMetrics)->y_scale;
}

long glyphSlotGetLinearHoriAdvance(const long glyphSlot) {
  return reinterpret_cast<FT_GlyphSlot>(glyphSlot)->linearHoriAdvance;
}

long glyphSlotGetLinearVertAdvance(const long glyphSlot) {
  return reinterpret_cast<FT_GlyphSlot>(glyphSlot)->linearVertAdvance;
}

long *glyphSlotGetAdvance(const long glyphSlot) {
  const auto advance = reinterpret_cast<FT_GlyphSlot>(glyphSlot)->advance;
  auto *result = new long[2];
  result[0] = advance.x;
  result[1] = advance.y;
  return result;
}

int glyphSlotGetFormat(const long glyphSlot) {
  return reinterpret_cast<FT_GlyphSlot>(glyphSlot)->format;
}

int glyphSlotGetBitmapLeft(const long glyphSlot) {
  return reinterpret_cast<FT_GlyphSlot>(glyphSlot)->bitmap_left;
}

int glyphSlotGetBitmapTop(const long glyphSlot) {
  return reinterpret_cast<FT_GlyphSlot>(glyphSlot)->bitmap_top;
}

// Pointer to FT_Bitmap
long glyphSlotGetBitmap(const long glyphSlot) {
  return reinterpret_cast<long>(&reinterpret_cast<FT_GlyphSlot>(glyphSlot)->
    bitmap);
}

// Pointer to FT_Glyph_Metrics
long glyphSlotGetMetrics(const long glyphSlot) {
  return reinterpret_cast<long>(&reinterpret_cast<FT_GlyphSlot>(glyphSlot)->
    metrics);
}

bool renderGlyph(const long glyphSlot, int renderMode) {
  return FT_Render_Glyph(reinterpret_cast<FT_GlyphSlot>(glyphSlot),
                         static_cast<FT_Render_Mode>(renderMode));
}

long glyphMetricsGetWidth(const long glyphMetrics) {
  return reinterpret_cast<FT_Glyph_Metrics *>(glyphMetrics)->width;
}

long glyphMetricsGetHeight(const long glyphMetrics) {
  return reinterpret_cast<FT_Glyph_Metrics *>(glyphMetrics)->height;
}

long glyphMetricsGetHoriAdvance(const long glyphMetrics) {
  return reinterpret_cast<FT_Glyph_Metrics *>(glyphMetrics)->horiAdvance;
}

long glyphMetricsGetVertAdvance(const long glyphMetrics) {
  return reinterpret_cast<FT_Glyph_Metrics *>(glyphMetrics)->vertAdvance;
}

long glyphMetricsGetHoriBearingX(const long glyphMetrics) {
  return reinterpret_cast<FT_Glyph_Metrics *>(glyphMetrics)->horiBearingX;
}

long glyphMetricsGetHoriBearingY(const long glyphMetrics) {
  return reinterpret_cast<FT_Glyph_Metrics *>(glyphMetrics)->horiBearingY;
}

long glyphMetricsGetVertBearingX(const long glyphMetrics) {
  return reinterpret_cast<FT_Glyph_Metrics *>(glyphMetrics)->vertBearingX;
}

long glyphMetricsGetVertBearingY(const long glyphMetrics) {
  return reinterpret_cast<FT_Glyph_Metrics *>(glyphMetrics)->vertBearingY;
}

unsigned int bitmapGetWidth(const long bitmap) {
  return reinterpret_cast<FT_Bitmap *>(bitmap)->width;
}

unsigned int bitmapGetRows(const long bitmap) {
  return reinterpret_cast<FT_Bitmap *>(bitmap)->rows;
}

int bitmapGetPitch(const long bitmap) {
  return reinterpret_cast<FT_Bitmap *>(bitmap)->pitch;
}

unsigned short bitmapGetNumGrays(const long bitmap) {
  return reinterpret_cast<FT_Bitmap *>(bitmap)->num_grays;
}

unsigned char bitmapGetPaletteMode(const long bitmap) {
  return reinterpret_cast<FT_Bitmap *>(bitmap)->palette_mode;
}

unsigned char bitmapGetPixelMode(const long bitmap) {
  return reinterpret_cast<FT_Bitmap *>(bitmap)->pixel_mode;
}

// Returns a pointer to the buffer
UCharArray bitmapGetBuffer(const long bitmap) {
  const auto *bmp = reinterpret_cast<FT_Bitmap *>(bitmap);
  const auto size = static_cast<int>(bmp->rows * abs(bmp->pitch));
  return UCharArray{reinterpret_cast<char *>(bmp->buffer), size};
}

int getCharMapIndex(const long charMap) {
  return FT_Get_Charmap_Index(reinterpret_cast<FT_CharMap>(charMap));
}