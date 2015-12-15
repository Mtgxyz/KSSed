#define MAX_ROOM 319
#define MAX_TILES 0x2D0
#define ROOM_HDR_SIZE 0x3A
class Room {
private:
  int roomID;
  int height, width;
  vector<vector<short>> room;
  Room(){};
public:
  Room(int roomID);
};