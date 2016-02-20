#include "ppu.hpp"
CGRAM::CGRAM() {
  for(int i=0;i<256; i++)
    colors.append(RGB555(0,0,0));
}
