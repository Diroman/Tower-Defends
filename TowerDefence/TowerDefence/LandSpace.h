#pragma once
#ifndef _LandSpace_H_
#include "../TowerDefence/Effect.h"
#include "../TowerDefence/Enemy.h"
#include "../TowerDefence/Structure.h"
#include "../TowerDefence/Castle.h"
#include <vector>


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
/*	bool IncreaseSize(int, int);
	bool DecreaseSize(int, int);
*/	bool AddTower(Point, Structure*);
	bool AddLair(Point, Lair*);
	void AddEnemy(EnemyPoint);
	void AddCastle(Point, Castle*);
	void DeleteTower(Point);
	void DeleteLair(Point);
	void DeleteCastle();
	void KillEnemy(Point);
	bool CheckBoard();
	bool Process();				// ход 
	void DoStep(EnemyPoint*);	// перемещение врага
	void CauseDamage(Structure*);			// удар башен
	bool FindWay();		// путь до замка
};
#endif