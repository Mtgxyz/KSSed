#include <functional>
class ROM {
private:
  string filename;
  unsigned char rom[4*1024*1024];
  unsigned char freeMemory;
  int (*addrResolv)(int);
  ROM(){}
public:
  ROM(string filename, int(*addrResolv)(int));
  auto save() -> void;
  auto setAddrResolver(int (*addrResolv)(int)) -> void {
    this->addrResolv=addrResolv;
  }
  auto operator[](int addr)->unsigned char &;
  auto getWord(int addr) -> unsigned short;
  auto get24bit(int addr) -> unsigned int;
  auto getInt(int addr) -> unsigned int;
  auto setWord(int addr,unsigned short val) -> void;
  auto set24bit(int addr, unsigned int val) -> void;
  auto setInt(int addr, unsigned int val) -> void;
  ~ROM();
};
extern ROM* rom;
struct RGBA8888 {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
  RGBA8888(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
  RGBA8888(unsigned int color);
  operator unsigned int();
  operator struct RGB555();
};
struct RGB555 {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  RGB555(unsigned char red, unsigned char green, unsigned char blue);
  RGB555(unsigned short color);
  operator unsigned short();
  operator struct RGBA8888();
};
