#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

enum class HexOrRgb {
  Hex,
  Rgb,
};

extern "C" {

void test_fn(HexOrRgb hex_or_rgb);

const char *primary_color_from_image_url(const char *image_url, HexOrRgb hex_or_rgb);

const char *primary_color_from_base64(const char *base64, HexOrRgb hex_or_rgb);

} // extern "C"
