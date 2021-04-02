#pragma once
#include <string>
#include <map>
#include <vector>
#include "Vector2D.h"

#define vs std::vector<std::string>
#define vvs std::vector<vs>

class Map {
public:
	Map(std::string id, int ms, int ts);
	~Map();
	static int mapWidth;
	static int mapHeight;
	void LoadMap(std::string tid, int sizeX, int sizeY, std::string tileset);
    void AddTile(int srcX, int srcY, int xPos, int yPos);
	void generateOverworld(std::string filePath, int width, int height);
	void generateRoom(std::string filePath, int width, int height);
	std::vector<int> cavePositionsX;
	std::vector<int> cavePositionsY;

private:
	std::string texId;
	int mapScale;
	int tileSize;
	int scaledSize;
	std::map<std::string, std::string> tileType = {
		{ "wall",    "00" },
		{ "base",    "01" },
		{ "lrpath",  "04" },
		{ "ulhouse", "12" },
		{ "urhouse", "11" },
		{ "llhouse", "15" },
		{ "lrhouse", "13" },
		{ "ulcave",  "21" },
		{ "urcave",  "22" },
		{ "llcave",  "23" },
		{ "lrcave",  "24" }, };
};

