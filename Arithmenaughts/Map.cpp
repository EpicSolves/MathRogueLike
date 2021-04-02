#include "Map.h"
#include "Game.h"
#include <fstream>
#include <iostream>
#include "ECS.h"
#include "Components.h"

Map::Map(std::string tid, int ms, int ts) : texId(tid), mapScale(ms), tileSize(ts) {
	scaledSize = ms * ts;
}

Map::~Map() {}

void Map::LoadMap(std::string path, int sizeX, int sizeY, std::string tileset) {
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			AddTile(srcX, srcY, x*scaledSize, y*scaledSize);
			mapFile.ignore();
		}
	}

	mapFile.ignore();
	//mapFile.ignore();
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			if (c == '1') {
				auto& tcol(Game::manager.addEntity());
				tcol.addComponent<ColliderComponent>(tileset, x*scaledSize, y*scaledSize, scaledSize, true);				
				tcol.addGroup(Game::groupMapColliders);
			}
			mapFile.ignore();
		}
	}
	mapFile.close();
}


void Map::AddTile(int srcX, int srcY, int xPos, int yPos) {
	auto& tile(Game::manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texId);
	tile.addGroup(Game::groupMap);
}

void Map::generateRoom(std::string filePath, int width, int height) {
	std::ofstream mapFile;
	mapFile.open(filePath);
	vvs map = vvs(height, vs(width, "base"));

	// Make top and bottom walls
	for (int i = 0; i < width; i++) {

		// Make the top a wall
		map[0][i] = "wall";

		// Make bottom a wall
		map[height - 1][i] = "wall";
	}
	// Make left and right walls
	for (int i = 0; i < height; i++) {

		// Make the left walls
		map[i][0] = "wall";

		// Make the right walls
		map[i][width - 1] = "wall";
	}
	// Convert map to values
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {

			mapFile << tileType[map[row][col]];
			if (col != width - 1)
				mapFile << ",";
			else
				mapFile << "\n";
		}
	}

	// Add a linebreak
	mapFile << "\n";

	// Generate collision
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {

			if (map[row][col] == "wall")
				mapFile << "1";
			else
				mapFile << "0";
			if (col != width - 1)
				mapFile << ",";
			else
				mapFile << "\n";
		}
	}

	// Close the file
	mapFile.close();
}

void Map::generateOverworld(std::string filePath, int width, int height) {
	std::ofstream mapFile;
	mapFile.open(filePath);
	vvs map = vvs(height, vs(width, "base"));

	// Make top and bottom walls
	for (int i = 0; i < width; i++) {

		// Make the top a wall
		map[0][i] = "wall";

		// Make bottom a wall
		map[height - 1][i] = "wall";
	}

	// Make left and right walls
	for (int i = 0; i < height; i++) {

		// Make the left walls
		map[i][0] = "wall";

		// Make the right walls
		map[i][width - 1] = "wall";
	}

	// Add 10 random houses
	for (int i = 0; i < 10; i++) {
		while (true) {
			int x = rand() % (width - 1);
			int y = rand() % (height - 1);
			if (map[y + 0][x + 0] == "base" && map[y + 1][x + 0] == "base" &&
				map[y + 0][x + 1] == "base" && map[y + 1][x + 1] == "base") {
				map[y + 0][x + 0] = "ulhouse";
				map[y + 1][x + 0] = "llhouse";
				map[y + 0][x + 1] = "urhouse";
				map[y + 1][x + 1] = "lrhouse";

				break;
			}
		}
	}

	// Add 10 random caves
	for (int i = 0; i < 10; i++) {
		while (true) {
			int x = rand() % (width - 1);
			int y = rand() % (height - 1);
			if (map[y][x] == "base" &&
				map[y + 1][x] == "base" &&
				map[y][x + 1] == "base" &&
				map[y + 1][x + 1] == "base") {
				map[y][x] = "ulcave";
				map[y + 1][x] = "llcave";
				map[y][x + 1] = "urcave";
				map[y + 1][x + 1] = "lrcave";
				cavePositionsX.push_back(x);
				cavePositionsY.push_back(y);
				break;
			}
		}
	}

	// Convert map to values
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {

			mapFile << tileType[map[row][col]];
			if (col != width - 1)
				mapFile << ",";
			else
				mapFile << "\n";
		}
	}

	// Add a linebreak
	mapFile << "\n";

	// Generate collision
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {

			if (map[row][col] == "wall")
				mapFile << "1";
			else
				mapFile << "0";
			if (col != width - 1)
				mapFile << ",";
			else
				mapFile << "\n";
		}
	}

	// Close the file
	mapFile.close();
}