#include "../kssed.hpp"
#include <stdint.h>
mainWindow* mainwin=nullptr;
mainWindow::mainWindow() {
  mainwin=this;
  setGeometry({32,32,640,480});
  setResizable(false);
  setBackgroundColor({0,0,0});
  viewport.setGeometry({0,0,640,480});
  setCentered();
  onClose([&] { program->quit(); });
}