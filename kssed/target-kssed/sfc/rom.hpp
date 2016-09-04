struct decompress {
    size_t uncompressed_sz;
    uint8_t *uncompressed;
};
struct compress {
    size_t compressed_sz;
    size_t uncompressed_sz;
    uint8_t *uncompressed;
    uint8_t *compressed;
};
struct ROM {
    uint8_t rom[4*1024*1024];
    string &fname;
    ROM(string &file);
    auto operator[](int32_t addr) -> uint8_t &;
    auto get8(int32_t addr) -> uint8_t;
    auto get16(int32_t addr) -> uint16_t;
    auto get24(int32_t addr) -> int32_t;
    auto get32(int32_t addr) -> uint32_t;
    auto set(int32_t addr, uint8_t val) -> void; //8-bit write
    auto set(int32_t addr, uint16_t val) -> void; //16-bit write
    auto set(int32_t addr, int32_t val) -> void; //24-bit write
    auto set(int32_t addr, uint32_t val) -> void; //32-bit write
    auto decompress(int32_t addr) -> ::decompress;
    auto compress(::compress &data) -> void;
    auto writeRange(int32_t addr, uint8_t *data, int32_t size) -> void;
    auto readRange(int32_t addr, int32_t size) -> uint8_t*;
    auto write() -> void;
};
extern unique_pointer<ROM> rom;