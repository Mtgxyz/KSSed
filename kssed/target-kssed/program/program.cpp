#include "../kssed.hpp"
#include <stdio.h>
unique_pointer<Program> program;
extern unique_pointer<Video> video;
Program::Program(string_vector args) {
    program=this;
    Application::onMain({&Program::main, this});
    
    new Settings;
    
    new MainWindow;
    mainwindow->setVisible();
    
    video = Video::create(settings["Video/Driver"].text());
    video->set(Video::Handle, mainwindow->viewport.handle());
    if(!video->init()) video = Video::create("None");
    
    mainwindow->setFocused();
    
}
auto Program::main() -> void {
}

auto Program::quit() -> void {
    Application::quit();
    settings.quit();
}