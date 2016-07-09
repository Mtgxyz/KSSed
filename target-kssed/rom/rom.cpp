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
auto ROM::getWord(int addr) -> unsigned short  {
	//return (unsigned short)((*this)[addr]<<8+(*this)[addr+1]);
	return (unsigned short)(((*this)[addr+1]<<8)+(*this)[addr]);
}
auto ROM::get24bit(int addr) -> unsigned int  {
	//return (unsigned int)(getWord(addr)<<8+(*this)[addr+2]);
	return (unsigned int)((getWord(addr+1)<<8)+(*this)[addr]);
}
auto ROM::getInt(int addr) -> unsigned int {
	//return (unsigned int)(getWord(addr)<<16+getWord(addr+2));
	return (unsigned int)((getWord(addr+2)<<16)+getWord(addr));
}
auto ROM::setWord(int addr,unsigned short val) -> void {
	/*(*this)[addr]=(unsigned char)(val>>8);
	(*this)[addr+1]=(unsigned char)val;*/
	(*this)[addr+1]=(unsigned char)(val>>8);
	(*this)[addr]=(unsigned char)val;
}
auto ROM::set24bit(int addr, unsigned int val) -> void {
	/*setWord(addr, (unsigned short)(val>>8));
	(*this)[addr+2]=(unsigned char)val;*/
	setWord(addr+1, (unsigned short)(val>>8));
	(*this)[addr]=(unsigned char)val;
}
auto ROM::setInt(int addr, unsigned int val) -> void {
	/*setWord(addr, (unsigned short)(val>>16));
	setWord(addr+2, (unsigned short)val);*/
	setWord(addr+2, (unsigned short)(val>>16));
	setWord(addr, (unsigned short)val);
}
RGBA8888::RGBA8888(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha): red(red), green(green), blue(blue), alpha(alpha) {}
RGBA8888::RGBA8888(unsigned int color) {
  alpha=(unsigned char)(color>>24);
  red=(unsigned char)(color>>16);
  green=(unsigned char)(color>>8);
  blue=(unsigned char)color;
}
RGBA8888::operator unsigned int() {
  return (alpha<<24)+(red<<16)+(green<<8)+blue;
}
RGBA8888::operator struct RGB555() {
  return RGB555((unsigned char)(red>>3),(unsigned char)(green>>3),(unsigned char)(blue>>3));
}
RGB555::RGB555(unsigned char red, unsigned char green, unsigned char blue): red(red), green(green), blue(blue) {}
RGB555::RGB555(unsigned color) {
  red=(unsigned char)(color & 0x1F);
  green=(unsigned char)((color>>5)&0x1F);
  blue=(unsigned char)((color>>10)&0x1F);
}
RGB555::operator unsigned short() {
  return red+(green<<5)+(blue<<10);
}
RGB555::operator struct RGBA8888() {
  return RGBA8888(red<<3,green<<3,blue<<3,-1);
}
