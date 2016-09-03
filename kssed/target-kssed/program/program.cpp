#include "../kssed.hpp"
#include <stdio.h>
unique_pointer<Program> program;

Program::Program(string_vector args) {
    program=this;
    Application::onMain({&Program::main, this});
}
auto Program::main() -> void {
    printf("HI!\n");
}

auto Program::quit() -> void {
    Application::quit();
}