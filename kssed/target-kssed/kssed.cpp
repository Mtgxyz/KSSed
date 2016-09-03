#include "kssed.hpp"
#include <cstdio>
unique_pointer<Video> video;
unique_pointer<Audio> audio;
unique_pointer<Input> input;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  location = {Path::config(), "kssed/", name};
  if(inode::exists(location)) return location;

  directory::create({Path::local(), "kssed/"});
  return {Path::local(), "kssed/", name};
}
#include <nall/main.hpp>
auto nall::main(string_vector args) -> void {
  printf("HI!\n");
}