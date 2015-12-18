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
#include <stdint.h>
#include <iostream>
mainWindow* mainwin=nullptr;
int resolv(int addr) {
  if(addr > 0xFFFFFF)
    return -1;
  if((addr < 0xC00000)&&!(addr&0x8000))
    return -1;
  if((addr < 0x800000)&&(addr >= 0x400000))
    return -1;
  unsigned char bank=(unsigned char)(addr>>16);
  if((bank<0xC0)&&(bank>0x7F))
    bank-=0x40;
  std::cout<<std::hex<<addr<<"->"<<std::hex<<((addr&0x7FFF)+(bank<<15))<<" or "<<std::hex<<(addr-0xC0000)<<std::endl;
  if(bank<0x80)
    return (addr&0x7FFF)+(bank<<15);
  else
    return addr-0xC00000;  
}
mainWindow::mainWindow() {
  mainwin=this;
  fileMenu.setText("File");
  loadFile.setText("Open...").onActivate([=] {
    auto location = BrowserDialog()
    .setTitle("Load Kirby Super Star/Kirby's Fun Pak/Kirby Super Deluxe ROM")
    .setPath("/")
    .setFilters("Unheadered SNES ROM|*.sfc")
    .openFile();
    try {
      new ROM(location,&resolv);
      //Checking if ROM is a valid Kirby ROM
      char name[]="KIRBY SUPER DELUXE  ";
      //The name is same for all 4 revisions of the game (NTSC J, NTSC J 2, NTSC U, PAL)
      for(int i=0xFFC0;i<0xFFD4;i++) {
	if((char)((*rom)[i])!=name[i-0xFFC0])
	  throw (string{"This is not a valid ROM for this program! (Wanted ",name[i-0xFFC0]," got ", (char)((*rom)[i]),"!"}).data();
      }
      mainwin->loadFile.setEnabled(false);
      mainwin->saveFile.setEnabled(true);
      mainwin->closeFile.setEnabled(true);
      Room r(0);
    } catch(const char* msg){if(rom) { delete rom; rom=nullptr;} 
      std::cerr<<msg<<std::endl;}
  });
  saveFile.setText("Save").onActivate([] {rom->save();});
  closeFile.setText("Close").onActivate([] {
    delete rom;
    rom=nullptr;
    mainwin->loadFile.setEnabled(true);
    mainwin->saveFile.setEnabled(false);
    mainwin->closeFile.setEnabled(false);
  });
  mainwin->saveFile.setEnabled(false);
  mainwin->closeFile.setEnabled(false);
  setGeometry({32,32,640,480});
  setResizable(false);
  setBackgroundColor({0,0,0});
  viewport.setGeometry({0,0,640,480});
  setCentered();
  onClose([&] { program->quit(); });
}