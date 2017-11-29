#include "stdafx.h"
#include "LandSpace.h"


LandSpace::LandSpace(Point size)
{
	//Board.resize();
	for (int i = 0; i < _size._x; i++)
		for (int j = 0; j < _size._y; j++)
			Board[i][j] = new Cell(3);
	KingCastle._x = -1;
	KingCastle._y = -1;
}

bool LandSpace::AddTower(Point xy, Structure *tower) {
	if (Board[xy._x][xy._y]->GetType() != 3)
		return false;
	TowerTable.push_back(xy);
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = tower;
	return true;
}

bool LandSpace::AddLair(Point xy, Lair *lair) {
	if (Board[xy._x][xy._y]->GetType() != 3)
		return false;
	LairTable.push_back(xy);
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = lair;
	return true;
}

void LandSpace::AddEnemy(EnemyPoint EnPoint) {
	EnemyTable.push_back(EnPoint);
}

void LandSpace::AddCastle(Point xy, Castle *castle) {
	KingCastle = xy;
	Board[xy._x][xy._y] = castle;
}

void LandSpace::DeleteTower(Point xy){
	for (int i = 0; i < TowerTable.size(); i++) 
		if ((TowerTable[i]._x == xy._x) && (TowerTable[i]._y == xy._y)) {
			delete Board[xy._x][xy._y];
			Board[xy._x][xy._y] = new Cell(3);
			TowerTable.erase(TowerTable.begin() + i);
		}
}

void LandSpace::DeleteLair(Point xy) {
	for (int i = 0; i < LairTable.size(); i++)
		if ((LairTable[i]._x == xy._x) && (LairTable[i]._y == xy._y)) {
			delete Board[xy._x][xy._y];
			Board[xy._x][xy._y] = new Cell(3);
			LairTable.erase(TowerTable.begin() + i);
		}
}

void LandSpace::DeleteCastle() {
	delete Board[KingCastle._x][KingCastle._y]; 
	KingCastle._x = -1;
	KingCastle._y = -1;
}

void LandSpace::KillEnemy(Point xy) {
	for (int i = 0; i < EnemyTable.size(); i++)
		if ((EnemyTable[i]._xy._x == xy._x) && (EnemyTable[i]._xy._y == xy._y)) {
			delete EnemyTable[i]._Enemy;
			EnemyTable.erase(EnemyTable.begin() + i);
		}
}

bool LandSpace::CheckBoard() {
	if (KingCastle._x == -1)
		return false;			// exception
	if (LairTable.size() == 0)
		return false;			// exception
	if (FindWay())
		return false;			// exception
}

bool LandSpace::Process() {
	int i = 0;
	EnemyPoint *newEnemy;
	Lair *lair;
	for (; i < LairTable.size(); i++) {
		lair = dynamic_cast <Lair*> (Board[LairTable[i]._x][LairTable[i]._y]);
		newEnemy = lair->ReleaseEnemy();
		if (!newEnemy) {
			AddEnemy(EnemyPoint(newEnemy->_Enemy, newEnemy->_xy));
			delete newEnemy;
		}
	}
	for (i = 0; i < EnemyTable.size(); i++) {
		DoStep(&EnemyTable[i]);
	}
}

bool LandSpace::DoStep(EnemyPoint *enemy) {
	
	return true;
}

bool LandSpace::FindWay() {
	return true;
}
