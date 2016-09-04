#include "../kssed.hpp"
#include <cstdio>
unique_pointer<MainWindow> mainwindow;
MainWindow::MainWindow() {
    mainwindow=this;
    fileMenu.setText("File");
    loadFile.setText("Open...");
    loadFile.onActivate([&] {
        auto location = BrowserDialog().setTitle("Load Kirby Super Star/Kirby's Fun Pak/Kirby Super Deluxe ROM")
                                       .setPath("/")
                                       .setFilters("Unheadered SNES ROM|*.sfc")
                                       .openFile();
        try {
            new ROM(location);
            //Checking if ROM is a valid Kirby ROM
            char name[]="KIRBY SUPER DELUXE  ";
            char tmp[21];
            tmp[20]='\0';
            for(int i=0;i<20;i++) {
                tmp[i]=rom->get8(0xFFC0+i);
            }
            if(strcmp(name, tmp)) {
                printf("Expected %s, got %s!\n",name,tmp);
                throw nullptr;
            }
            new Room(0);
        } catch(...) {
            if(rom) { rom=nullptr;}
        }
    });
    quit.setText("Quit");
    quit.onActivate([&] { doClose(); });
    onClose([&] { program->quit(); });
    setTitle("KSSed");
    setResizable(false);
    setBackgroundColor({0,0,0});
    setSize({640,480});
    viewport.setGeometry({0,0,640,480});
    setCentered();
#if defined(PLATFORM_MACOSX)
    about.setVisible(false);
    Application::Cocoa::onActivate([&] { setFocused(); });
    Application::Cocoa::onQuit([&] { doClose(); });
#endif
      
}