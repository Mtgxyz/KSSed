/*   KSSed - A level editor for Kirby Super Star/Kirby's Fun Pak and Kirby of the Stars Super Deluxe
 *   Copyright (C) 2015  Morten Delenk
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../kssed.hpp"
#include <iostream>
#include <fstream>
ROM* rom=nullptr;
ROM::ROM(string filename, int(*addrResolv)(int)): filename(filename),addrResolv(addrResolv) {
  std::ifstream file(filename.data(),std::ios::in|std::ios::binary);
  ::rom=this;
  if(file.is_open()) {
    file.read((char *)rom,sizeof(rom));
    file.close();
  } else {
    throw "Cannot read file!"; //TODO
  }
}
ROM::~ROM() {
}
auto ROM::save() -> void {
  std::ofstream file(filename.data(),std::ios::out|std::ios::binary);
  if(file.is_open()) {
    file.write((char *)rom,sizeof(rom));
    file.close();
  } else {
    throw "Cannot write file!"; //TODO
  }
}
auto ROM::operator[](int addr)->unsigned char & {
  int offset=addrResolv(addr);
  if(offset==-1)
    return freeMemory;
  else
    return rom[offset];
}
RGBA8888::RGBA8888(char red, char green, char blue, char alpha): red(red), green(green), blue(blue), alpha(alpha) {}
RGBA8888::RGBA8888(int color) {
  alpha=(char)(color>>24);
  red=(char)(color>>16);
  green=(char)(color>>8);
  blue=(char)color;
}
RGBA8888::operator int() {
  return (alpha<<24)+(red<<16)+(green<<8)+blue;
}
RGBA8888::operator struct RGB555() {
  return RGB555((char)(red>>3),(char)(green>>3),(char)(blue>>3));
}
RGB555::RGB555(char red, char green, char blue): red(red), green(green), blue(blue) {}
RGB555::RGB555(short color) {
  red=(char)(color & 0x1F);
  green=(char)((color>>5)&0x1F);
  blue=(char)((color>>10)&0x1F);
}
RGB555::operator short() {
  return red+(green<<5)+(blue<<10);
}
RGB555::operator struct RGBA8888() {
  return RGBA8888(red<<3,green<<3,blue<<3,-1);
}
