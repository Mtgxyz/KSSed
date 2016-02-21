#define MAX_ROOM 319
#define MAX_TILES 0x2D0
#define ROOM_HDR_SIZE 0x3A
struct Tile {
	int tile;
	bool mirrorX;
	bool mirrorY;
	int pal;
};
class Room {
private:
  int roomID, roomBase;
  int height, width;
  vector<vector<short>> room;
	vector<uint16_t> tilemaps;
  //char* vram, *cgram;
  Room(){};
  auto initTiles() -> void;
  auto initPals() -> void;
  auto initTileset() -> void;
public:
  Room(int roomID);
  auto draw(int tileNumber) -> vector<int> *;
};
extern Room* room;
