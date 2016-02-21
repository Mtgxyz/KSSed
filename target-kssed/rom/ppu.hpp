#ifndef _PPU_HPP
#define _PPU_HPP
class CGRAM {
private:
  vector<RGB555> colors;
public:
  CGRAM();
  CGRAM(vector<RGB555>);
  CGRAM(vector<uint8_t>);
  auto operator[](int color) -> RGB555 &;
  auto getPalette(int palNum) -> vector<RGB555> *;
};
extern CGRAM* cgram;
//For rendering graphics up to 512*512 tiles 24x24 tiles (Tilemap of up to 1536x1536)
class VRAM {
private:
  union character {
    struct chara {
      uint64_t bp0;
      uint64_t bp1;
      uint64_t bp2;
      uint64_t bp3;
      auto render(int pal) -> vector<vector<RGBA8888>> *;
    }__attribute__((packed)) cha;
    uint8_t input[32];
  };
  union mapentry {
    struct map {
      uint16_t chr : 10;
      uint8_t pal : 3;
      uint8_t prio : 1;
      uint8_t verticalFlip : 1;
      uint8_t horizontalFlip : 1;
    }__attribute__((packed));
    uint16_t entry;
  };
  vector<character> vram;
  vector<vector<mapentry>> tilemap;
  vector<vector<mapentry>> tiles;
  VRAM() {};
public:
  VRAM(int width, int height);
  VRAM(vector<uint8_t> chr, int width, int height);
  auto dma(vector<uint8_t> from, unsigned short to) -> void;
  auto addTile(int index, int subindex, uint16_t) -> void; //Add tile to usable tiles
  auto setTile(int x, int y, uint16_t tile) -> void; //Sets a specific tile in tilemap
  auto render() -> vector<vector<RGBA8888>> *;
};
extern VRAM* vram;
#endif
