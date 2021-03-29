#pragma once
#include <string>

class Map {
public:
	Map(std::string id, int ms, int ts);
	~Map();
	static int mapWidth;
	static int mapHeight;
	void LoadMap(std::string tid, int sizeX, int sizeY, std::string tileset);
    void AddTile(int srcX, int srcY, int xPos, int yPos);

private:
	std::string texId;
	int mapScale;
	int tileSize;
	int scaledSize;
};

