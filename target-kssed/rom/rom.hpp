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
  ~ROM();
};
extern ROM* rom;
struct RGBA8888 {
  char red;
  char green;
  char blue;
  char alpha;
  RGBA8888(char red, char green, char blue, char alpha);
  RGBA8888(int color);
  operator int();
  operator struct RGB555();
};
struct RGB555 {
  char red;
  char green;
  char blue;
  RGB555(char red, char green, char blue);
  RGB555(short color);
  operator short();
  operator struct RGBA8888();
};
