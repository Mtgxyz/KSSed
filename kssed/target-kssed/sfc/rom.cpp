#include "../kssed.hpp"
#include <exhal/compress.h>
#include <cstdio>
auto addrResolv = [] (int32_t addr) -> int32_t {
    if(addr>=0x1000000)
        return -1;
    uint8_t bank=(uint8_t)(addr>>16);
    uint16_t bankAddr=(uint16_t) addr;
    if(bank<0x40) {
        return (bankAddr&0x7FFF)+(((int32_t)bank)<<15);
    } else if(bank<0x80) {
        return -1;
    } else if(bank<0xC0) {
        bank-=0x40;
        return (bankAddr&0x7FFF)+(((int32_t)bank)<<15);
    } else {
        return addr-0xC00000;
    }
};
unique_pointer<ROM> rom;
ROM::ROM(string &fname):fname(fname) {
    ::rom=this;
    printf("Hi!\n");
    file::read(fname, rom, 4*1024*1024);
}
auto ROM::operator[](int32_t addr) -> uint8_t & {
    int32_t res=addrResolv(addr);
    printf("%x -> %x: %x (%c)\n", addr, res, rom[res], rom[res]);
    if(res==-1)
        throw nullptr;
    return rom[res];
}
auto ROM::get8(int32_t addr) -> uint8_t {
    return (*this)[addr];
}
auto ROM::get16(int32_t addr) -> uint16_t {
    uint16_t val=get8(addr+1);
    val<<=8;
    val|=get8(addr);
    return val;
}
auto ROM::get24(int32_t addr) -> int32_t {
    int32_t val=(int32_t)((uint32_t)get16(addr+1));
    val<<=8;
    val|=(int32_t)((uint32_t)get8(addr));
    return val;
}
auto ROM::get32(int32_t addr) -> uint32_t {
    uint32_t val=get16(addr+2);
    val<<=16;
    val|=get16(addr);
    return val;
}
auto ROM::set(int32_t addr, uint8_t val) -> void { //8-bit write
    (*this)[addr]=val;
}
auto ROM::set(int32_t addr, uint16_t val) -> void { //16-bit write
    set(addr+1,(uint8_t)(val>>8));
    set(addr++,(uint8_t)(val));
}
auto ROM::set(int32_t addr, int32_t val) -> void { //24-bit write
    set(addr+1, (uint16_t)(val>>8));
    set(addr++, (uint8_t)val);
}
auto ROM::set(int32_t addr, uint32_t val) -> void { //32-bit write
    set(addr+2, (uint16_t)(val>>16));
    set(addr++, (uint16_t)val);
}
auto ROM::decompress(int32_t addr) -> ::decompress {
    ::decompress blob;
    uint8_t buf[65536];
    blob.uncompressed_sz=unpack(&((*this)[addr]),buf);
    blob.uncompressed = new uint8_t[blob.uncompressed_sz];
    memcpy(blob.uncompressed, buf, blob.uncompressed_sz);
    return blob;
}
auto ROM::compress(::compress &data) -> void {
    uint8_t buf[65536];
    data.compressed_sz=pack(data.uncompressed, data.uncompressed_sz, buf, 0);
    data.compressed = new uint8_t[data.compressed_sz];
    memcpy(data.compressed, buf, data.compressed_sz);
}
auto ROM::writeRange(int32_t addr, uint8_t *data, int32_t size) -> void {
    for(int32_t i=0;i<size;i++) {
        (*this)[addr+i]=data[i];
    }
}
auto ROM::readRange(int32_t addr, int32_t size) -> uint8_t* {
    uint8_t *buf=new uint8_t[size];
    for(int32_t i=0;i<size; i++) {
        buf[i]=(*this)[addr+i];
    }
    return buf;
}
auto ROM::write() -> void {
    file::write(fname, rom, 4*1024*1024);
}