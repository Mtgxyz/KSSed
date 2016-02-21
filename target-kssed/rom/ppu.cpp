#include "ppu.hpp"
CGRAM::CGRAM() {
  for(int i=0;i<256; i++)
    colors.append(RGB555(0,0,0));
}
CGRAM::CGRAM(vector<RGB555> colors) {
  int i;
  for(i=0; i<(colors.size()<256?colors.size():256);i++)
    this->colors.append(colors[i]);
  for(; i<256; i++)
    this->colors.append(RGB555(0,0,0));
}
