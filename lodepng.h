#ifndef LODEPNG_H
#define LODEPNG_H
#include <string.h> /*for size_t*/
#ifdef __cplusplus
#include <vector>
#include <string>
#endif /*__cplusplus*/
#define LODEPNG_VERSION_STRING "20141130"

#ifndef LODEPNG_NO_COMPILE_ZLIB
#define LODEPNG_COMPILE_ZLIB
#endif

#ifndef LODEPNG_NO_COMPILE_PNG
#define LODEPNG_COMPILE_PNG
#endif

#ifndef LODEPNG_NO_COMPILE_DECODER
#define LODEPNG_COMPILE_DECODER
#endif

#ifndef LODEPNG_NO_COMPILE_ENCODER
#define LODEPNG_COMPILE_ENCODER
#endif

#ifndef LODEPNG_NO_COMPILE_DISK
#define LODEPNG_COMPILE_DISK
#endif

#ifndef LODEPNG_NO_COMPILE_ANCILLARY_CHUNKS
#define LODEPNG_COMPILE_ANCILLARY_CHUNKS
#endif

#ifdef __cplusplus
#ifndef LODEPNG_NO_COMPILE_CPP
#define LODEPNG_COMPILE_CPP
#endif
#endif

#ifdef LODEPNG_COMPILE_PNG

typedef enum LodePNGColorType
{
LCT_GREY = 0, /*grayscale*/
LCT_RGB = 2, /*RGB*/
LCT_PALETTE = 3, /*palette*/
LCT_GREY_ALPHA = 4, /*grayscale+alpha*/
LCT_RGBA = 6 /*RGB+alpha*/
} LodePNGColorType;

#ifdef LODEPNG_COMPILE_DECODER

unsigned lodepng_decode_memory(unsigned char** out, unsigned* w, unsigned* h,
                               const unsigned char* in, size_t insize,
                               LodePNGColorType colortype, unsigned bitdepth);
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_CPP

namespace lodepng
{
#ifdef LODEPNG_COMPILE_DECODER
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
                const std::vector<unsigned char>& in,
                LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
#endif
}

#endif /*LODEPNG_COMPILE_CPP*/
#endif /*LODEPNG_COMPILE_PNG*/

const char* lodepng_error_text(unsigned code);

#ifdef LODEPNG_COMPILE_DECODER
typedef struct LodePNGDecompressSettings LodePNGDecompressSettings;
struct LodePNGDecompressSettings
{
unsigned ignore_adler32;
};

extern const LodePNGDecompressSettings lodepng_default_decompress_settings;
void lodepng_decompress_settings_init(LodePNGDecompressSettings* settings);
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER
typedef struct LodePNGCompressSettings LodePNGCompressSettings;
struct LodePNGCompressSettings
{
unsigned btype; /* 2 */
unsigned minmatch; /* 3 = best, 0 = default */
unsigned nicematch; /* 258 = best, 128 = default */
unsigned lazymatching; /* true */
unsigned (*custom_deflate)(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGCompressSettings*);
const void* custom_context;
};

extern const LodePNGCompressSettings lodepng_default_compress_settings;
void lodepng_compress_settings_init(LodePNGCompressSettings* settings);
#endif /*LODEPNG_COMPILE_ENCODER*/

#ifdef LODEPNG_COMPILE_PNG
typedef struct LodePNGColorMode
{
LodePNGColorType colortype;
unsigned bitdepth;
unsigned char* palette;
size_t palettesize;
unsigned key_defined;
unsigned key_r;
unsigned key_g;
unsigned key_b;
} LodePNGColorMode;

void lodepng_color_mode_init(LodePNGColorMode* info);
void lodepng_color_mode_cleanup(LodePNGColorMode* info);
unsigned lodepng_color_mode_copy(LodePNGColorMode* dest, const LodePNGColorMode* source);
unsigned lodepng_palette_add(LodePNGColorMode* info, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void lodepng_palette_clear(LodePNGColorMode* info);
unsigned lodepng_is_greyscale_type(const LodePNGColorMode* info);
unsigned lodepng_is_alpha_type(const LodePNGColorMode* info);
unsigned lodepng_has_palette_alpha(const LodePNGColorMode* info);

size_t lodepng_get_raw_size(unsigned w, unsigned h, const LodePNGColorMode* color);

typedef struct LodePNGInfo
{
unsigned compression_method;
unsigned filter_method;
unsigned interlace_method;
LodePNGColorMode color;

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
unsigned background_defined;
unsigned background_r;
unsigned background_g;
unsigned background_b;
size_t text_num;
char** text_keys;
char** text_strings;

size_t itext_num;
char** itext_keys;
char** itext_langtags;
char** itext_transkeys;
char** itext_strings;

unsigned char* unknown_chunks_data[3];
size_t unknown_chunks_size[3];
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
} LodePNGInfo;

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
unsigned lodepng_add_text(LodePNGInfo* info, const char* key, const char* str);
unsigned lodepng_add_itext(LodePNGInfo* info, const char* key, const char* langtag, const char* transkey, const char* str);
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

unsigned lodepng_convert(unsigned char* out, const unsigned char* in, LodePNGColorMode* mode_out, const LodePNGColorMode* mode_in, unsigned w, unsigned h);
#ifdef LODEPNG_COMPILE_DECODER

typedef struct LodePNGDecoderSettings
{
LodePNGDecompressSettings zlibsettings;
unsigned ignore_crc;
unsigned color_convert;

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
unsigned read_text_chunks;
unsigned remember_unknown_chunks;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/
} LodePNGDecoderSettings;
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER

// filtering
typedef enum LodePNGFilterStrategy
{
LFS_ZERO = 0,
LFS_SUB = 1,
LFS_UP = 2,
LFS_AVG = 3,
LFS_PAETH = 4,
LFS_BRUTE_FORCE = 5,
LFS_PREDEFINED = 6,
LFS_MINSUM_ORG = 7,
LFS_DISTINCT_BYTES = 9,
LFS_MINSUM = 10,
LFS_INCREMENTAL = 11,
LFS_BRUTE_FORCE2 = 12,
LFS_INCREMENTAL2 = 13
} LodePNGFilterStrategy;

// palette sorting: priority
typedef enum LodePNGPalettePriorityStrategy
{
LPPS_POPULARITY = 0,
LPPS_RGB = 2,
LPPS_YUV = 4,
LPPS_MSB = 1
} LodePNGPalettePriorityStrategy;


typedef enum LodePNGPaletteDirectionStrategy
{
  /*Sort in ascending direction*/
  LPDS_ASCENDING = 0,
  /*Sort in descending direction*/
  LPDS_DESCENDING = 1
} LodePNGPaletteDirectionStrategy;

// palette sorting: transparency
typedef enum LodePNGPaletteTransparencyStrategy
{
LPTS_IGNORE = 1,
LPTS_SORT = 2,
LPTS_FIRST = 0
} LodePNGPaletteTransparencyStrategy;

// palette sorting: order
typedef enum LodePNGPaletteOrderStrategy
{
LPOS_NONE = -1,
LPOS_GLOBAL = 0,
LPOS_NEAREST_NONE = 2,
LPOS_NEAREST = 3,
LPOS_NEAREST_WEIGHT = 4
} LodePNGPaletteOrderStrategy;

typedef struct LodePNGPaletteSettings{
LodePNGPalettePriorityStrategy priority;
  LodePNGPaletteDirectionStrategy direction;
LodePNGPaletteTransparencyStrategy trans;
LodePNGPaletteOrderStrategy order;
} LodePNGPaletteSettings;

typedef struct LodePNGColorProfile
{
unsigned colored;
unsigned key;
unsigned short key_r;
unsigned short key_g;
unsigned short key_b;
unsigned alpha;
unsigned numcolors;
unsigned char palette[1024];
unsigned bits;
} LodePNGColorProfile;

void lodepng_color_profile_init(LodePNGColorProfile* profile);
unsigned lodepng_get_color_profile(LodePNGColorProfile* profile,
const unsigned char* image, unsigned w, unsigned h,
const LodePNGColorMode* mode_in);
typedef struct LodePNGEncoderSettings
{
LodePNGCompressSettings zlibsettings;
unsigned auto_convert;
unsigned clean_alpha;
LodePNGFilterStrategy filter_strategy;
const unsigned char* predefined_filters;
unsigned force_palette;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
unsigned text_compression;
#endif /*LODEPNG_COMPILE_ANCILLARY_CHUNKS*/

unsigned short filter_style;
} LodePNGEncoderSettings;

void lodepng_encoder_settings_init(LodePNGEncoderSettings* settings);
#endif /*LODEPNG_COMPILE_ENCODER*/

#if defined(LODEPNG_COMPILE_DECODER) || defined(LODEPNG_COMPILE_ENCODER)
typedef struct LodePNGState
{
#ifdef LODEPNG_COMPILE_DECODER
LodePNGDecoderSettings decoder;
#endif /*LODEPNG_COMPILE_DECODER*/
#ifdef LODEPNG_COMPILE_ENCODER
LodePNGEncoderSettings encoder;
#endif /*LODEPNG_COMPILE_ENCODER*/
LodePNGColorMode info_raw;
LodePNGInfo info_png;
LodePNGColorMode out_mode;
unsigned error;
#ifdef LODEPNG_COMPILE_CPP
virtual ~LodePNGState(){}
#endif
} LodePNGState;

void lodepng_state_init(LodePNGState* state);
void lodepng_state_cleanup(LodePNGState* state);
void lodepng_state_copy(LodePNGState* dest, const LodePNGState* source);
#endif /* defined(LODEPNG_COMPILE_DECODER) || defined(LODEPNG_COMPILE_ENCODER) */

#ifdef LODEPNG_COMPILE_DECODER
unsigned lodepng_decode(unsigned char** out, unsigned* w, unsigned* h,
LodePNGState* state,
const unsigned char* in, size_t insize);

unsigned lodepng_inspect(unsigned* w, unsigned* h,
 LodePNGState* state,
 const unsigned char* in, size_t insize);
#endif /*LODEPNG_COMPILE_DECODER*/

unsigned lodepng_chunk_length(const unsigned char* chunk);
void lodepng_chunk_type(char type[5], const unsigned char* chunk);
unsigned char lodepng_chunk_type_equals(const unsigned char* chunk, const char* type);
unsigned char lodepng_chunk_ancillary(const unsigned char* chunk);
const unsigned char* lodepng_chunk_data_const(const unsigned char* chunk);
unsigned lodepng_chunk_check_crc(const unsigned char* chunk);
void lodepng_chunk_generate_crc(unsigned char* chunk);
unsigned char* lodepng_chunk_next(unsigned char* chunk);
const unsigned char* lodepng_chunk_next_const(const unsigned char* chunk);
unsigned lodepng_chunk_append(unsigned char** out, size_t* outlength, const unsigned char* chunk);
unsigned lodepng_chunk_create(unsigned char** out, size_t* outlength, unsigned length,
const char* type, const unsigned char* data);
#endif /*LODEPNG_COMPILE_PNG*/


#ifdef LODEPNG_COMPILE_DECODER
unsigned lodepng_zlib_decompress(unsigned char** out, size_t* outsize,
const unsigned char* in, size_t insize,
const LodePNGDecompressSettings* settings);
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_CPP
namespace lodepng
{
#ifdef LODEPNG_COMPILE_PNG
class State : public LodePNGState
{
public:
State();
virtual ~State();
};

#ifdef LODEPNG_COMPILE_DECODER
unsigned decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h, State& state, const std::vector<unsigned char>& in);
#endif /*LODEPNG_COMPILE_DECODER*/

#ifdef LODEPNG_COMPILE_ENCODER
unsigned encode(std::vector<unsigned char>& out, std::vector<unsigned char>& in, unsigned w, unsigned h, State& state, LodePNGPaletteSettings p);
unsigned encode2(std::vector<unsigned char>& out, unsigned char* in, unsigned w, unsigned h, State& state, LodePNGPaletteSettings p);
#endif /*LODEPNG_COMPILE_ENCODER*/

#ifdef LODEPNG_COMPILE_DISK
void load_file(std::vector<unsigned char>& buffer, const std::string& filename);
void save_file(const std::vector<unsigned char>& buffer, const std::string& filename);
void save_file2(const std::vector<unsigned char>& buffer, const std::string& filename);
#endif
#endif
}
#endif /*LODEPNG_COMPILE_CPP*/
#endif /*LODEPNG_H inclusion guard*/