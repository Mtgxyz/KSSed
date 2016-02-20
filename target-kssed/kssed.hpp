#include <libintl.h>
#include <locale.h>
#define _(x) gettext((x))
#include <nall/nall.hpp>
#include <ruby/ruby.hpp>
#include <hiro/hiro.hpp>
using namespace nall;
using namespace ruby;
using namespace hiro;
#include "configuration/config.hpp"
#include "mainwin/mainwindow.hpp"
#include "rom/rom.hpp"
#include "ksd/room.hpp"
extern Video* video;
extern Audio* audio;
extern Input* input;
struct Program {
  Program(lstring args);
  auto main() -> void;
  auto quit() -> void;
};
extern Program* program;
auto locate(string pathname, string filename) -> string;
