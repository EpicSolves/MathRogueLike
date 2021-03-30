#pragma once

struct Wave {
	int leftEnemies;
	int rightEnemies;
	int topEnemies;
	int bottomEnemies;
	int scale;
	float health;
	bool canShoot;
	Wave(int l, int r, int t, int b, int s, float h, bool cs) {
		leftEnemies = l;
		rightEnemies = r;
		topEnemies = t;
		bottomEnemies = b;
		scale = s;
		health = h;
		canShoot = cs;
	}
};