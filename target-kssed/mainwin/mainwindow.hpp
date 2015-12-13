
struct mainWindow : Window {
  mainWindow();
  FixedLayout layout{this};
    Viewport viewport{&layout, Geometry{0, 0, 1, 1}};
};
extern mainWindow* mainwin;