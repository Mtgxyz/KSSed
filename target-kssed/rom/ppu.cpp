#include "../kssed.hpp"
#include <iostream>
CGRAM *cgram=nullptr;
VRAM *vram=nullptr;
CGRAM::CGRAM() {
  for(int i=0;i<256; i++)
    colors.append(RGBA8888(0,0,0,0));
  cgram=this;
}
CGRAM::CGRAM(vector<RGB555> colors): CGRAM() {
  int i;
  for(i=0; i<(colors.size()<256?colors.size():256);i++)
    this->colors.append(colors[i]);
  for(; i<256; i++)
    this->colors.append(RGB555(0,0,0));
}
CGRAM::CGRAM(vector<uint8_t> colors): CGRAM() {
  for(int i=0;i<((colors.size()/2)<256?colors.size()/2:256);i++) {
    this->colors.append(RGB555((short)(colors[i*2]+colors[i*2+1]<<8)));
  }
}
auto CGRAM::operator[](int color) -> RGBA8888 & {
  return colors[color];
}
auto CGRAM::getPalette(int palNum) -> vector<RGBA8888> * {
  vector<RGBA8888> * pal=new vector<RGBA8888>();
  for(int i=palNum*16;i<palNum*16+16;i++) {
    pal->append(colors[i]);
  }
  return pal;
}
auto VRAM::character::chara::render(int pal) -> vector<vector<RGBA8888>> * {
  if(!cgram)
    return nullptr;
  vector<vector<RGBA8888>>* tile=new vector<vector<RGBA8888>>();
  vector<RGBA8888> *tmp=cgram->getPalette(pal);
  vector<RGBA8888> palette=*tmp;
  delete tmp;
  for(int x=0;x<8;x++) {
    vector<RGBA8888> tmp;
    for(int y=0;y<8;y++) {
      uint64_t bit=1<<(x+y*8);
      uint8_t color=(bp0&bit)==bit;
      color|=((bp1&bit)==bit)<<1;
      color|=((bp2&bit)==bit)<<2;
      color|=((bp3&bit)==bit)<<3;
      tmp.append(palette[color]);
    }
    tile->append(tmp);
  }
  return tile;
}

VRAM::VRAM(int width, int height) {
  mapentry entry;
  entry.entry=0;
  for(int x=0;x<width*3;x++) {
    vector<mapentry> tmp;
    for(int y=0;y<height*3;y++)
      tmp.append(entry);
    tilemap.append(tmp);
  }
  for(int i=0;i<1024;i++) {
    vram.append(character());
  }
  for(int i=0;i<0x2D0;i++) {
    vector<mapentry> tmp;
    for(int j=0;j<9;j++)
      tmp.append(entry);
    tiles.append(tmp);
  }
  ::vram=this;
}
VRAM::VRAM(vector<uint8_t> chr, int width, int height): VRAM(width, height) {
  dma(chr, 0);
}
auto VRAM::dma(vector<uint8_t> from, unsigned short to) -> void {
  if((to+from.size()>65536) || (from.size()>65536))
    return;
  for(int i=0;i<(from.size());i+=32) {
    character chara;
    for(int j=0;j<32;j++) {
      chara.input[j]=from[i+j];
    }
    vram[(to+i)/32]=chara;
  }
}
auto VRAM::addTile(int index, int subindex, uint16_t mapent) -> void {
  mapentry entry;
  entry.entry=mapent;
  tiles[index][subindex]=entry;
}
auto VRAM::setTile(int x, int y, uint16_t tile) -> void {
  for(int i=0;i<3;i++) {
    for(int j=0;j<3;j++) {
      tilemap[x*3+i][y*3+j] = tiles[tile][i+j*3];
    }
  }
}
auto VRAM::render() -> vector<vector<RGBA8888>> * {
  vector<vector<RGBA8888>>* output = new vector<vector<RGBA8888>>;
  vector<vector<vector<vector<RGBA8888>>*>> renderedTiles;
  for(int x=0;x<tilemap.size();x++) {
    vector<vector<vector<RGBA8888>>*> tmp;
    for (int y=0;y<tilemap[x].size();y++) {
      mapentry entr = tilemap[x][y];
      tmp.append(vram[entr.entry&0x3FF].cha.render((entr.entry/1024)&0x7));
    }
    renderedTiles.append(tmp);
  }
  for(int tx=0;tx<renderedTiles.size();tx++) {
    for(int x=0;x<8;x++) {
      vector<RGBA8888> tmp;
      for(int ty=0; ty<renderedTiles[tx].size();ty++) {
        for(int y=0; y<8; y++) {
          tmp.append((*(renderedTiles[tx][ty]))[x][y]);
        }
      }
      output->append(tmp);
    }
  }
  for(int x=0;x<renderedTiles.size();x++)
    for(int y=0;y<renderedTiles[x].size();y++) {
      delete renderedTiles[x][y];
    }

  return output;
}
