#pragma once
#ifndef _Game_H_
#include <string>
#include "../TowerDefence/Enemy.h"
class Enemy;

using namespace std;

struct Point {
	int _x;
	int _y;

	Point();
};

struct EnemyPoint {
	Point _xy;
	Enemy *_Enemy;

	EnemyPoint();
	EnemyPoint(Enemy*, Point);
};

class Castle : public Cell {

private:
	string _Name;
	double _HP_Max;
	double _HP = _HP_Max;
	int _Money;

public:
	Castle(string, double, double); // HP  Money
	Castle(const Castle &st);
	string GetName();
	double GetHP();
	int GetMoney();
	void AddMoney(int);
	bool SubMoney(int);		//	true - enought money
	bool SubHP(double);		//  true - castle have HP
};

#endif