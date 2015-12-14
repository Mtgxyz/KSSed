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
  string location{programpath(), filename};
  if(file_system_object::exists(location)) return location;
  return {pathname, filename};
}
auto nall::main(lstring args) -> void {
  Application::setName("kssed");
  new Program(args);
  Application::run();
}
Program::Program(lstring args) {
  program=this;
  directory::create({configpath(), "kssed/"});
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
  if(video->lock(output, length, 640, 480)) {
    for(auto y : range(480)) {
      uint32_t* dp = output + y * (length >> 2);
      for(auto x : range(640)) *dp++ = 0xFF00FF00;
    }
    video->unlock();
    video->refresh();	
  }
}
auto Program::main() -> void {
}
auto Program::quit() -> void {
  delete video;
  Application::quit();
}