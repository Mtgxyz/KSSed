struct ConfigurationManager : Configuration::Document {
  ConfigurationManager();
  auto quit() -> void;
  struct Video : Configuration::Node {
    string driver;
  } video;
};
extern ConfigurationManager* config;