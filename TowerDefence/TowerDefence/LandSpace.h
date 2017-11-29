#pragma once
#ifndef _LandSpace_H_
#include "../TowerDefence/Effect.h"
#include "../TowerDefence/Enemy.h"
#include "../TowerDefence/Structure.h"
#include "../TowerDefence/Castle.h"
#include <vector>

// Cell Type
#define _Castle 1;
#define _Lair 2;
#define _Field 3;
#define _Road 4;
#define _Forest 5;
#define _Tower 6;
#define _MagicTower 7;
#define _Trap 8;

using namespace std;

class Road : public Cell {
	bool fl;
	Point Next;
};

class LandSpace
{
private:
	Point _size;
	vector < vector < Cell* > > Board;
	vector < Point > TowerTable;
	vector < Point > LairTable;
	vector < EnemyPoint > EnemyTable;
	Point KingCastle;

public:
	LandSpace(Point);
	bool AddTower(Point, Structure*);
	bool AddLair(Point, Lair*);
	void AddEnemy(EnemyPoint);
	void AddCastle(Point, Castle*);
	void DeleteTower(Point);
	void DeleteLair(Point);
	void DeleteCastle();
	void KillEnemy(Point);
	bool CheckBoard();
	bool Process();				// ход врага и выстрел башни
	bool DoStep(EnemyPoint*); 
	bool FindWay();

};

#endif