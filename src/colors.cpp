
#include "../headers/colors.h"

Color color_from_hex(const char* s) {
  Color c = {0, 0, 0, 255};
  char dump;
  sscanf(s, "%c%02hhx%02hhx%02hhx", &dump, &c.r, &c.g, &c.b);
  return c;
}
