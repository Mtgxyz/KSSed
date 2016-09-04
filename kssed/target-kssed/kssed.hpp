#include <nall/nall.hpp>
#include <ruby/ruby.hpp>
#include <hiro/hiro.hpp>
#include <nall/stdint.hpp>
using namespace nall;
using namespace ruby;
using namespace hiro;
extern unique_pointer<Video> video;
extern unique_pointer<Audio> audio;
extern unique_pointer<Input> input;

#include "program/program.hpp"
#include "configuration/config.hpp"
#include "ui/mainwindow.hpp"
#include "sfc/rom.hpp"
#include "ksd/room.hpp"
auto locate(string name) -> string;