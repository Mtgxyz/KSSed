#include "../kssed.hpp"
Room::Room(int roomID): roomID(roomID) {
  if(roomID<0 || roomID>=MAX_ROOM) throw "You are trying to load an invalid map";
  int currAddr,roomBase,addr;
  //Finding the beginning of the room header
  addr=rom[0xFF0002]+rom[0xFF0003]<<8; //$FF is a bank consisting out of a huge table. $FF:$0002 points to the beginning of the level pointer table
  addr=rom[0xFF0000+addr]+rom[0xFF0001+addr]<<8; //Level 0 (Green Greens out of Spring Breeze) points incidentally to room 0 (The "demo" of the map)
  roomBase=rom[0xFF0000+addr+roomID*4]+rom[0xFF0001+addr+roomID*4]<<8+rom[0xFF0002+addr+roomID*4]<<16;
  //The room base pointer points directly to the header
  //Here I could copy the room header, if I want to.
  currAddr=roomBase;
  //The room data consists out of 8 sections, each of which is connected to each other by a relative pointer in the beginning
  //The use of some of the sections (3 to be precise) is currently unknown.
  currAddr+=0x3C;
  //Section 1: The door data. This program currently does not handle any door data. But in fact the structure of it is already known
  //TODO handle door data here
  currAddr+=rom[currAddr-2]+rom[currAddr-1]<<8;
  //Section 2: Switch data. This determines what a switch modifies. Currently unknown structure
  //TODO find out
  currAddr+=rom[currAddr-2]+rom[currAddr-1]<<8;
  //Section 3: Middle boss data. This determines what minibosses are appearing (because there are only 2 enemies for ALL the minibosses (I am not sure)) Unknown structure
  //TODO find out
  currAddr+=rom[currAddr-2]+rom[currAddr-1]<<8;
  //Section 4: Variable Tiles. When you use a bomb block or a few switches, some blocks will "explode" into another. Structure known and handled later on
  //TODO handle
  currAddr+=rom[currAddr-2]+rom[currAddr-1]<<8;
  //Section 5: Unknown Data. Unknown structure.
  //TODO Check if any room has data in this section and find out
  currAddr+=rom[currAddr-2]+rom[currAddr-1]<<8;
  //Section 6: Unknown Data. Unknown structure.
  //TODO Check if any room has data in this section and find out
  currAddr+=rom[currAddr-2]+rom[currAddr-1]<<8;
  //Section 7: Unknown Data. Unknown structure.
  //TODO Check if any room has data in this section and find out
  currAddr+=rom[currAddr-2]+rom[currAddr-1]<<8;
  //Section 8: Main room data. Contains room size, the actual room and the enemies in this room (Last two compressed)
  //TODO I am feeling lazy now
}