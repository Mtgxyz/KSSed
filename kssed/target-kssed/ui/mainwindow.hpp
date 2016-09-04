struct MainWindow : Window {
    MainWindow();
    MenuBar menuBar{this};
        Menu fileMenu{&menuBar};
            MenuItem loadFile{&fileMenu};
            MenuItem quit{&fileMenu};
    
    FixedLayout layout{this};
        Viewport viewport{&layout, Geometry{0,0,1,1}};
    
    StatusBar statusBar{this};
};
extern unique_pointer<MainWindow> mainwindow;