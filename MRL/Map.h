#pragma once
#include <string>
#include <map>
#include <vector>
#include "Vector2D.h"
#include "SDL.h"

#define vi std::vector<int>
#define vs std::vector<std::string>
#define vvs std::vector<vs>
#define vvi std::vector<vi>

class Map {
public:
	Map();
	Map(std::string id, int ms, int ts);
	~Map();
	int mapWidth;
	int mapHeight;
	void LoadMap(std::string tid, int roomX, int roomY, int sizeX, int sizeY, std::string tileset);
    void AddTile(int srcX, int srcY, int xPos, int yPos);
	void setTileset(std::string path);
	void generateGraph(int numberOfRooms, Vector2D s, Vector2D t);
	void generateRooms(std::string tileSet, int roomX, int roomY);
	void generateOverworld(std::string filePath, int width, int height);
	void generateRoom(std::string filePath, int x, int y, int width, int height, bool doors[4]);
	std::vector<int> cavePositionsX;
	std::vector<int> cavePositionsY;
	struct Node *graphRoot;
	bool explore(struct Node *node, Vector2D t);
	void dfsExplore(struct Node *node);
	std::map<std::pair<int, int>, bool> visited;
	int roomWidth;
	int roomHeight;
	std::string texId;

	// To know where to put animation tiles
	// One map for each room
	// Each map in each room as an animation tile possible at pos x, y
	std::map<std::pair<int, int>, std::map<std::pair<int, int>, std::string>> roomAnimations;
	std::map<std::pair<int, int>, uint8_t> roomGates;
	std::map<std::pair<int, int>, bool> roomCleared;

private:
	
	int mapScale;
	int tileSize;
	int scaledSize;

	std::map<std::string, std::vector<std::string>> tileType = {
		{ "wall",    { "00" } },
		{ "base",    { "01" } },
		{ "lrpath",  { "04" } },
		{ "ulhouse", { "12" } },
		{ "urhouse", { "11" } },
		{ "llhouse", { "15" } },
		{ "lrhouse", { "13" } },
		{ "ulcave",  { "21" } },
		{ "urcave",  { "22" } },
		{ "llcave",  { "23" } },
		{ "lrcave",  { "24" } },
		{ "fire",    { "25" } }
	};
};

struct Node {
	Node *left = NULL;
	Node *right = NULL;
	Node *top = NULL;
	Node *bot = NULL;
	std::string mapName;
	std::string tileSet;
	int roomWidth;
	int roomHeight;
	int x;
	int y;
	Node();
	Node(int x, int y);
};

