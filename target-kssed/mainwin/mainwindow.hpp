
struct mainWindow : Window {
  mainWindow();
  MenuBar menuBar{this};
    Menu fileMenu{&menuBar};
      MenuItem loadFile{&fileMenu};
      MenuItem saveFile{&fileMenu};
      MenuItem closeFile{&fileMenu};
  FixedLayout layout{this};
    Viewport viewport{&layout, Geometry{0, 0, 1, 1}};
};
extern mainWindow* mainwin;