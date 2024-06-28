#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

extern "C" {

void primary_color();

const char *primary_color_from_image_url(const char *image_url);

} // extern "C"
