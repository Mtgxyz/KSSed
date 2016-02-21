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
#include <target-kssed/kssed.hpp>
#include <compress.h>
#include <iostream>
#include <malloc.h>
#include <fstream>

Room::Room(int roomID): roomID(roomID) {
  if(roomID<0 || roomID>=MAX_ROOM) throw "You are trying to load an invalid map";
  int currAddr,addr;
  //Finding the beginning of the room header
  addr=rom->getWord(0xFF000C); //$FF is a bank consisting out of a huge table. $FF:$0002 points to the beginning of the level pointer table
  std::cout << std::hex << addr << std::endl;
  addr=rom->getWord(0xFF0000+addr); //Level 0 (Green Greens out of Spring Breeze) points incidentally to room 0 (The "demo" of the map)
  std::cout << std::hex << addr << std::endl;
  roomBase=rom->get24bit(0xFF0000+addr+roomID*4);
  std::cout << std::hex << roomBase << std::endl;
  //The room base pointer points directly to the header
  //Here I could copy the room header, if I want to.
  currAddr=roomBase;
  //The room data consists out of 7 sections, each of which is connected to each other by a relative pointer in the beginning
  //The use of some of the sections (3 to be precise) is currently unknown.
  currAddr+=0x3C;
  //Section 1: The door data. This program currently does not handle any door data. But in fact the structure of it is already known
  //TODO handle door data here
  currAddr+=rom->getWord(currAddr-2);
  //Section 2: Switch data. This determines what a switch modifies. Currently unknown structure
  //TODO find out
  currAddr+=rom->getWord(currAddr-2);
  //Section 3: Middle boss data. This determines what minibosses are appearing (because there are only 2 enemies for ALL the minibosses (I am not sure)) Unknown structure
  //TODO find out
  currAddr+=rom->getWord(currAddr-2);
  //Section 4: Variable Tiles. When you use a bomb block or a few switches, some blocks will "explode" into another. Structure known and handled later on
  //TODO handle
  currAddr+=rom->getWord(currAddr-2);
  //Section 5: Unknown Data. Unknown structure.
  //TODO Check if any room has data in this section and find out
  currAddr+=rom->getWord(currAddr-2);
  //Section 6: Unknown Data. Unknown structure.
  //TODO Check if any room has data in this section and find out
  currAddr+=rom->getWord(currAddr-2);
  //Section 7: Main room data. Contains room size, the actual room and the enemies in this room (Last two compressed)
  height=rom->getWord(currAddr);
  currAddr+=2;
  width=rom->getWord(currAddr);
  currAddr+=2;
  std::cout << std::dec << "Size: " << height << "x" << width << std::endl;
  unsigned char buf[65536]; //64 KiB is the absolute maximum the compression method can address (The actual maximum size in this game is about 56 KiB, if not less)
  size_t numenemies=unpack(&((*rom)[currAddr]),buf); // This routine is part of libexhal
  numenemies%=height*width*2; //Each block takes 2 bytes
  numenemies/=6; //Enemie data is 6 bytes long
  std::cout << "Enemy count: " << numenemies << std::endl;
  //Put the room data into vector
  /*vram=(char*)calloc(65536,1);
  cgram=(char*)calloc(512,1);*/
  delete vram;
  //new VRAM(width,height);
  new VRAM(27,27);
  initTiles();
  initPals();
  initTileset();
  /*for(int x=0;x<width;x++) {
    for(int y=0;y<height;y++) {
      vram->setTile(x,y,buf[x+y*width]);
    }
  }*/
  for(int x=0;x<width;x++) {
    for(int y=0;y<height;y++) {
      vram->setTile(x,y,x+y*width);
    }
  }
}
auto Room::initTiles() -> void {
	char buffer[16];
	string str("vram");
	sprintf(buffer, "%d", roomID);
	str.append(buffer);
	str.append(".smc");
	static const int BGoff[2]={0x18, 0x1D};
	uint8_t buf[65536];
	for(int BGN=0;BGN<2; BGN++) {
		int addr=rom->get24bit(0xFF0000+rom->getWord(0xFF0002)+(*rom)[roomBase+BGoff[BGN]]*3); //Gets the beginning address of the CHR pointer of one of the backgrounds
		if(!addr)
			continue;
		size_t size=unpack(&((*rom)[addr]),buf);
		addr=(*rom)[roomBase+0x05];
		addr=0x018482+addr*4+BGN*2;
		addr=0x010000+rom->getWord(addr); //Get DMA data location
		int src, dst, sz;
		sz=rom->getWord(addr+1);
		src=rom->get24bit(addr+3)-0x7E2000;
		dst=rom->getWord(addr+6)*2;
		//Execute DMA
    vector<uint8_t> dmabytes;
    for(auto x : range(sz))
      dmabytes.append(buf[src+x]);
		vram->dma(dmabytes, dst);
	}
	//By default the VRAM gets dumped here for debugging
	std::ofstream file(str.data(), std::ios::out|std::ios::binary);
	if(file.is_open()) {
		//file.write(vram,65536);
		file.close();
	}
}
auto Room::initPals() -> void {
	char buffer[16];
	string str("vram");
	sprintf(buffer, "%d", roomID);
	str.append(buffer);
	str.append(".tpl");
	static const int BGoff[2]={0x19, 0x1E};
	for(int BGN=0; BGN<2; BGN++) {
		int addr=rom->get24bit(0xFF0000+rom->getWord(0xFF0002)+(*rom)[roomBase+BGoff[BGN]]*3);
		if(!addr)
			continue;
		int count, off;
		count=(*rom)[addr++];
		off=(*rom)[addr++];
		for(;count;count--,off++) {
      (*cgram)[off%256]=rom->getWord(addr++);
			addr++;
		}
	}
	//By default the CGRAM gets dumped here for debugging

	std::ofstream file(str.data(), std::ios::out|std::ios::binary);
	if(file.is_open()) {
		file.write("TPL\x02",4);
		//file.write(cgram,512);
		file.close();
	}
}
auto Room::initTileset() -> void {
	uint8_t buf[0x10000];
	int addr=rom->get24bit(0xFF0000+rom->getWord(0xFF0002)+rom->getWord(roomBase+0x1B)*3);
	if(!addr)
		return;
	size_t size = unpack(&((*rom)[addr]),buf); //The tile information is compressed
  for(int tile=0;tile<0x2D0;tile++)
    for(int subtile=0;subtile<9;subtile++)
      vram->addTile(tile,subtile,(short)(buf[(tile*9+subtile)*2]+buf[(tile*9+subtile)*2]<<8));

}
auto Room::draw(int tilenum) -> vector<int> * {
  return new vector<int>();
}
