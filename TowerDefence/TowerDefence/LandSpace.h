#pragma once
#ifndef _LandSpace_H_
#include "Cel.h"
#include "Enemy.h"
#include "Effect.h"
#include "Structure.h"
#include <vector>


using namespace std;

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
	bool ChangeSize(int, int);
	bool AddTower(Point, Structure*);
	bool AddLair(Point, Lair*);
	void AddEnemy(EnemyPoint);
	bool AddRoad(Point);
	bool AddCastle(Point, Castle*);
	void DeleteTower(Point);
	void DeleteLair(Point);
	void DeleteCastle();
	void KillEnemy(Point);
	bool CheckBoard();
	bool Process();							// ход 
	void DoStep(EnemyPoint*);				// перемещение врага
	void CauseDamage(Structure*, Point);	// удар башен
	bool FindWay();							// путь до замка
};
#endif