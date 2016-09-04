#define MAX_ROOM 319
#define MAX_TILES 0x2D0
#define ROOM_HDR_SIZE 0x3A
struct Room {
    int roomID, roomBase;
    int height, width;
    vector<vector<short>> room;
    vector<uint16_t> tilemaps;
    Room(){};
    auto initTiles() -> void;
    auto initPals() -> void;
    auto initTileset() -> void;
    Room(int roomID);
};
extern unique_pointer<Room> room;