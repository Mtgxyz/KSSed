#include "../kssed.hpp"
ConfigurationManager* config = nullptr;
ConfigurationManager::ConfigurationManager() {
  config=this;
  video.append(video.driver, "Driver");
  append(video, "Video");
  if(!video.driver) video.driver = ruby::Video::safestDriver();
  save(locate({configpath(), "kssed/"}, "settings.bml"));
}
auto ConfigurationManager::quit() ->void {
  save(locate({configpath(),"kssed/"},"settings.bml"));
}