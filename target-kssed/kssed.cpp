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
#include "kssed.hpp"
#include <iostream>
Video* video = nullptr;
Audio* audio = nullptr;
Input* input = nullptr;
#include <nall/main.hpp>
Program* program=nullptr;
//if file already exists in the same path as the binary; use it (portable mode)
//if not, use default requested path (*nix/user mode)
auto locate(string pathname, string filename) -> string {
  string location{Path::program(), filename};
  if(inode::exists(location)) return location;
  return {pathname, filename};
}
auto nall::main(lstring args) -> void {
  Application::setName("kssed");
  new Program(args);
  Application::run();
}
Program::Program(lstring args) {
  program=this;
  directory::create({Path::config(), "kssed/"});
  Application::onMain({&Program::main, this});
  new ConfigurationManager;
  new mainWindow;
  mainwin->setVisible();
  mainwin->setFocused();
  mainwin->setTitle("KSSed");
  video = Video::create(config->video.driver);
  video->set(Video::Handle, mainwin->viewport.handle());
  std::clog<<config->video.driver.data()<<std::endl;
  if(!video->init()) {
    std::cerr<<"Could not create video!"<<std::endl;
    delete video;
    video=Video::create("None");
  }
  uint32_t* output;
  unsigned length;
  if(video->lock(output, length, 1024, 786)) {
    for(auto y : range(786)) {
      uint32_t* dp = output + y * (length >> 2);
      for(auto x : range(1024)) *dp++ = 0xFF00FF00;
    }
    video->unlock();
    video->refresh();
  }
  new CGRAM();
  new VRAM(1,1);
}
auto Program::main() -> void {
  if(!room)
    return;
  uint32_t* output;
  unsigned length;
  vector<vector<RGBA8888>> *screen = vram->render();
  if(video->lock(output, length, screen->size(), (*screen)[0].size())) {
    try{
      for(int x=0; x<screen->size(); x++) {
        for(int y=0; y<(*screen)[x].size(); y++) {
          output[x+y*(length>>2)] = (int)((*screen)[x][y]);
        }
      }
    } catch(...){};
    delete screen;
    video->unlock();
    video->refresh();
  }
}
auto Program::quit() -> void {
  delete video;
  Application::quit();
}
