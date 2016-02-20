#ifndef _PPU_HPP
#define _PPU_HPP
#include "../kssed.hpp"
class CGRAM {
private:
  vector<RGB555> colors;
public:
  CGRAM();
  CGRAM(vector<RGB555>);
  CGRAM(vector<uint8_t>);
  auto operator[](int color) -> RGB555 &;
  auto getPalette(int palNum) -> vector<RGB555>;
};
extern CGRAM* cgram;
//For rendering graphics up to 512*512 tiles 24x24 tiles (Tilemap of up to 1536x1536)
class VRAM {
private:
  struct character {
    uint8_t r0bp1;
    uint8_t r0bp2;
    uint8_t r1bp1;
    uint8_t r1bp2;
    uint8_t r2bp1;
    uint8_t r2bp2;
    uint8_t r3bp1;
    uint8_t r3bp2;
    uint8_t r4bp1;
    uint8_t r4bp2;
    uint8_t r5bp1;
    uint8_t r5bp2;
    uint8_t r6bp1;
    uint8_t r6bp2;
    uint8_t r7bp1;
    uint8_t r7bp2;
    uint8_t r0bp1;
    uint8_t r0bp2;
    uint8_t r1bp3;
    uint8_t r1bp4;
    uint8_t r2bp3;
    uint8_t r2bp4;
    uint8_t r3bp3;
    uint8_t r3bp4;
    uint8_t r4bp3;
    uint8_t r4bp4;
    uint8_t r5bp3;
    uint8_t r5bp4;
    uint8_t r6bp3;
    uint8_t r6bp4;
    uint8_t r7bp3;
    uint8_t r7bp4;
    auto render() -> vector<vector<int>>;
  }__attribute__((packed));
  struct mapentry {
    uint16_t chr : 10;
    uint8_t pal : 3;
    uint8_t prio : 1;
    uint8_t verticalFlip : 1;
    uint8_t horizontalFlip : 1;
  }__attribute__((packed));
  vector<character> VRAM;
  vector<vector<mapentry>> tilemap;
  vector<vector<mapentry>> tiles;
  VRAM() {};
public:
  VRAM(int width, int height);
  VRAM(vector<uint8_t> chr, vector<uint16_t> tiles, int width, int height);
  auto dma(vector<uint8_t> from, short to) -> void;
  auto addTile(int index, int subindex, uint16_t chr, uint8_t pal, bool vFlip, bool hFlip) -> void; //Add tile to usable tiles
  auto setTile(int x, int y, uint16_t tile) -> void; //Sets a specific tile in tilemap
  auto render(int x, int y) -> vector<vector<int>>;
};
extern VRAM* vram;
#endif
