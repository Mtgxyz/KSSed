#include "../kssed.hpp"
#include <cstdio>
unique_pointer<Room> room;
Room::Room(int roomID) : roomID(roomID) {
    ::room=this;
    if(roomID<0 || roomID>=MAX_ROOM) throw "You are trying to load an invalid map";
    int currAddr,addr;
    //Finding the beginning of the room header
    addr=rom->get16(0xFF000C); //$FF is a bank consisting out of a huge table. $FF:$0002 points to the beginning of the level pointer table
    addr=rom->get16(0xFF0000+addr); //Level 0 (Green Greens out of Spring Breeze) points incidentally to room 0 (The "demo" of the map)
    roomBase=rom->get24(0xFF0000+addr+roomID*4);
    //The room base pointer points directly to the header
    //Here I could copy the room header, if I want to.
    currAddr=roomBase;
    //The room data consists out of 7 sections, each of which is connected to each other by a relative pointer in the beginning
    //The use of some of the sections (3 to be precise) is currently unknown.
    currAddr+=0x3C;
    //Section 1: The door data. This program currently does not handle any door data. But in fact the structure of it is already known
    //TODO handle door data here
    currAddr+=rom->get16(currAddr-2);
    //Section 2: Switch data. This determines what a switch modifies. Currently unknown structure
    //TODO find out
    currAddr+=rom->get16(currAddr-2);
    //Section 3: Middle boss data. This determines what minibosses are appearing (because there are only 2 enemies for ALL the minibosses (I am not sure)) Unknown structure
    //TODO find out
    currAddr+=rom->get16(currAddr-2);
    //Section 4: Variable Tiles. When you use a bomb block or a few switches, some blocks will "explode" into another. Structure known and handled later on
    //TODO handle
    currAddr+=rom->get16(currAddr-2);
    //Section 5: Unknown Data. Unknown structure.
    //TODO Check if any room has data in this section and find out
    currAddr+=rom->get16(currAddr-2);
    //Section 6: Unknown Data. Unknown structure.
    //TODO Check if any room has data in this section and find out
    currAddr+=rom->get16(currAddr-2);
    //Section 7: Main room data. Contains room size, the actual room and the enemies in this room (Last two compressed)
    height=rom->get16(currAddr);
    currAddr+=2;
    width=rom->get16(currAddr);
    currAddr+=2;
    printf("%xx%x\n",height,width);
    decompress blob=rom->decompress(currAddr);
//    numenemies=blob.decompressed_sz;

}