#include "Map.h"
#include "Game.h"
#include <fstream>
#include <iostream>
#include "ECS.h"
#include "Components.h"
#include <algorithm>

Map::Map() {}

Map::Map(std::string tid, int ms, int ts) : texId(tid), mapScale(ms), tileSize(ts) {
	scaledSize = ms * ts;
}

Map::~Map() {}

void Map::LoadMap(std::string path, int sizeX, int sizeY, std::string tileset) {
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;
	mapWidth = this->tileSize*this->mapScale*sizeX;
	mapHeight = this->tileSize*this->mapScale*sizeY;

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

void Map::generateRoom(std::string filePath, int width, int height, bool doors[4]) {
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

	// Add right door if exists
	if (doors[0])
		map[height / 2][width - 1] = "base";

	// Add left door if exists
	if (doors[1])
		map[height / 2][0] = "base";

	// Add top door if exists
	if (doors[2])
		map[0][width/2] = "base";

	// Add bot door if exists
	if (doors[3])
		map[height-1][width / 2] = "base";

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

void Map::setTileset(std::string path) {
	texId = path;
}

void Map::generateGraph(int numberOfRooms, Vector2D s, Vector2D t) {
	graphRoot = new Node(s.x, s.y);
	float rE = pow(pow(abs(graphRoot->x + 1 - t.x), 2) + std::pow(abs(graphRoot->y + 0 - t.y), 2), 1.0f);
	float lE = pow(pow(abs(graphRoot->x - 1 - t.x), 2) + std::pow(abs(graphRoot->y + 0 - t.y), 2), 1.0f);
	float tE = pow(pow(abs(graphRoot->x + 0 - t.x), 2) + std::pow(abs(graphRoot->y - 1 - t.y), 2), 1.0f);
	float bE = pow(pow(abs(graphRoot->x + 0 - t.x), 2) + std::pow(abs(graphRoot->y + 1 - t.y), 2), 1.0f);
	float sE = rE + lE + bE + tE;
	float de = 4 * sE - rE - lE - tE - bE;
	float rP = (sE - rE) / de;
	float lP = (sE - lE) / de;
	float tP = (sE - tE) / de;
	float bP = (sE - bE) / de;
	
	printf("Right: %.2f Left: %.2f Top: %.2f Bottom: %.2f\n", rP, lP, tP, bP);
	std::vector<std::pair<float, std::string>> v;
	v.push_back(std::make_pair(rP, "right"));
	v.push_back(std::make_pair(lP, "left"));
	v.push_back(std::make_pair(tP, "top"));
	v.push_back(std::make_pair(bP, "bottom"));
	std::sort(v.begin(), v.end());
	v[0] = std::make_pair(0.70, v[3].second);
	v[1] = std::make_pair(0.60, v[2].second);
	v[2] = std::make_pair(0.40, v[1].second);
	v[3] = std::make_pair(0.20, v[0].second);

	// Based on probabilities, choose neighbor
	while (v.size() > 0) {
		for (int i = 0; i < v.size(); i++) {
			float r = rand() % 100;
			printf("%.2f\n", r);
			if (r < 100.0f*v[i].first) {
				// If not visited, explore r.second
				if (v[i].second == "right") {
					printf("Selected Right\n");
					if (graphRoot->right == NULL) {
						Node *n = new Node(s.x + 1, s.y);
						graphRoot->right = n;
						n->left = graphRoot;
					}
					if (explore(graphRoot->right, t)) {
						return;
					}
				} else if (v[i].second == "left") {
					printf("Selected Left\n");
					if (graphRoot->left == NULL) {
						Node *n = new Node(s.x - 1, s.y);
						graphRoot->left = n;
						n->right = graphRoot;
					}
					if (explore(graphRoot->left, t)) {
						return;
					}
				} else if (v[i].second == "top") {
					printf("Selected Top\n");
					if (graphRoot->top == NULL) {
						Node *n = new Node(s.x, s.y-1);
						graphRoot->top = n;
						n->bot = graphRoot;
					}
					if (explore(graphRoot->top, t)) {
						return;
					}
				} else if (v[i].second == "bottom") {
					printf("Selected Bot\n");
					if (graphRoot->bot == NULL) {
						Node *n = new Node(s.x, s.y + 1);
						graphRoot->bot = n;
						n->top = graphRoot;
					}
					if (explore(graphRoot->bot, t)) {
						return;
					}
				}

				// Remove p from v
				v.erase(v.begin() + i);
				break;
			}
		}
	}
}
bool Map::explore(Node *node, Vector2D t) {
	std::cout << "Exploring (" << node->x << ", " << node->y << ")" << std::endl;
	if (t.x == node->x && t.y == node->y) {
		return true;
	}
	if (node->x < 0 || node->y < 0 || node->x > 10 || node->y > 10)
		return false;
	float rE = pow(pow(abs(node->x + 1 - t.x), 2) + std::pow(abs(node->y + 0 - t.y), 2), 6.0f);
	float lE = pow(pow(abs(node->x - 1 - t.x), 2) + std::pow(abs(node->y + 0 - t.y), 2), 6.0f);
	float tE = pow(pow(abs(node->x + 0 - t.x), 2) + std::pow(abs(node->y - 1 - t.y), 2), 6.0f);
	float bE = pow(pow(abs(node->x + 0 - t.x), 2) + std::pow(abs(node->y + 1 - t.y), 2), 6.0f);
	float sE = rE + lE + bE + tE;
	float de = 4 * sE - rE - lE - tE - bE;
	float rP = (sE - rE) / de;
	float lP = (sE - lE) / de;
	float tP = (sE - tE) / de;
	float bP = (sE - bE) / de;

	printf("Right: %.2f Left: %.2f Top: %.2f Bottom: %.2f\n", rP, lP, tP, bP);
	std::vector<std::pair<float, std::string>> v;
	v.push_back(std::make_pair(rP, "right"));
	v.push_back(std::make_pair(lP, "left"));
	v.push_back(std::make_pair(tP, "top"));
	v.push_back(std::make_pair(bP, "bottom"));
	std::sort(v.begin(), v.end());
	v[0] = std::make_pair(0.70, v[3].second);
	v[1] = std::make_pair(0.60, v[2].second);
	v[2] = std::make_pair(0.40, v[1].second);
	v[3] = std::make_pair(0.20, v[0].second);

	// Based on probabilities, choose neighbor
	while (v.size() > 0) {
		for (int i = 0; i < v.size(); i++) {
			float r = rand() % 100;
			printf("%.2f\n", r);
			if (r < 100.0f*v[i].first) {
				// If not visited, explore r.second
				if (v[i].second == "right") {
					printf("Selected right\n");
					if (node->right == NULL) {
						Node *n = new Node(node->x + 1, node->y);
						node->right = n;
						n->left = node;
					}
					if (explore(node->right, t)) {
						return true;
					}
				}
				else if (v[i].second == "left") {
					printf("Selected left\n");
					if (node->left == NULL) {
						Node *n = new Node(node->x - 1, node->y);
						node->left = n;
						n->right = node;
					}
					if (explore(node->left, t)) {
						return true;
					}
				}
				else if (v[i].second == "top") {
					printf("Selected top\n");
					if (node->top == NULL) {
						Node *n = new Node( node->x, node->y - 1);
						node->top = n;
						n->bot = node;
					}
					if (explore(node->top, t)) {
						return true;
					}
				}
				else if (v[i].second == "bottom") {
					printf("Selected bottom\n");
					if (node->bot == NULL) {
						Node *n = new Node(node->x, node->y + 1);
						node->bot = n;
						n->top = node;
					}
					if (explore(node->bot, t)) {
						return true;
					}
				}

				// Remove p from v
				v.erase(v.begin() + i);
				break;
			}
		}
	}
	return false;
}

void Map::generateRooms(std::string tileSet, int width, int height) {
	texId = tileSet;
	roomWidth = width;
	roomHeight = height;

	// Run DFS on graph, generating rooms
	visited = std::map<std::pair<int, int>, bool>();

	bool doors[4];
	doors[0] = graphRoot->right != NULL;
	doors[1] = graphRoot->left != NULL;
	doors[2] = graphRoot->top != NULL;
	doors[3] = graphRoot->bot != NULL;
	
	generateRoom(graphRoot->mapName, width, height, doors);
	visited[{graphRoot->x, graphRoot->y}] = true;
	if (graphRoot->right != NULL && visited.find({ graphRoot->x + 1, graphRoot->y }) == visited.end()) {
		dfsExplore(graphRoot->right);
	}
	if (graphRoot->left != NULL && visited.find({ graphRoot->x - 1, graphRoot->y }) == visited.end()) {
		dfsExplore(graphRoot->left);
	}
	if (graphRoot->top != NULL && visited.find({ graphRoot->x, graphRoot->y - 1 }) == visited.end()) {
		dfsExplore(graphRoot->top);
	}
	if (graphRoot->bot != NULL && visited.find({ graphRoot->x, graphRoot->y + 1 }) == visited.end()) {
		dfsExplore(graphRoot->bot);
	}
}
void Map::dfsExplore(Node *node) {
	visited[{node->x, node->y}] = true;

	bool doors[4];
	doors[0] = node->right != NULL;
	doors[1] = node->left != NULL;
	doors[2] = node->top != NULL;
	doors[3] = node->bot != NULL;

	generateRoom(node->mapName, roomWidth, roomHeight, doors);
	if (node->right != NULL && visited.find({ node->x + 1, node->y }) == visited.end()) {
		dfsExplore(node->right);
	}
	if (node->left != NULL && visited.find({ node->x - 1, node->y }) == visited.end()) {
		dfsExplore(node->left);
	}
	if (node->top != NULL && visited.find({ node->x, node->y - 1 }) == visited.end()) {
		dfsExplore(node->top);
	}
	if (node->bot != NULL && visited.find({ node->x, node->y + 1 }) == visited.end()) {
		dfsExplore(node->bot);
	}
}

Node::Node() {}
Node::Node(int x, int y) {
	this->x = x;
	this->y = y;
	this->mapName = "Assets/Maps/Room" + std::to_string(x) + "."
		+ std::to_string(y) + ".map";
}