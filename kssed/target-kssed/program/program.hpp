struct Program {
    //program.cpp
    Program(string_vector args);
    auto main() -> void;
    auto quit() -> void;
};
extern unique_pointer<Program> program;