#include "../kssed.hpp"

Settings settings;
Settings::Settings() {
    Markup::Node::operator=(BML::unserialize(string::read(locate("settings.bml"))));
    
    auto set = [&](const string& name, const string& value) {
        //create node and set to default value only if it does not already exist
        if(!operator[](name)) operator()(name).setValue(value);
    };
    set("Video/Driver", ruby::Video::optimalDriver());
    set("Audio/Driver", ruby::Audio::optimalDriver());
    set("Input/Driver", ruby::Input::optimalDriver());
}
auto Settings::quit() -> void {
    file::write(locate("settings.bml"), BML::serialize(*this));
}