#include "stdafx.h"
#include "LandSpace.h"
#include <cmath>

// Cell Type
#define _Castle 1
#define _Lair 2
#define _Field 3
#define _Road 4
#define _Forest 5
#define _Tower 6
#define _MagicTower 7
#define _Trap 8

LandSpace::LandSpace(Point size)
{
	//Board.resize();
	for (int i = 0; i < _size._x; i++)
		for (int j = 0; j < _size._y; j++)
			Board[i][j] = new Cell(3);
	KingCastle._x = -1;
	KingCastle._y = -1;
}

bool LandSpace::ChangeSize(int w, int h) {

	for (int i = 0; i < _size._x; i++)
		for (int j = 0; j < _size._y; j++)
			delete Board[i][j];

	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			Board[i][j] = new Cell(3);
}

bool LandSpace::AddTower(Point xy, Structure *tower) {
	if (Board[xy._x][xy._y]->GetType() != 3)
		return false;
	TowerTable.push_back(xy);
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = tower;
	Board[xy._x][xy._y]->SetType(tower->GetType());
	return true;
}

bool LandSpace::AddLair(Point xy, Lair *lair) {
	if (Board[xy._x][xy._y]->GetType() != 3)
		return false;
	LairTable.push_back(xy);
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = lair;
	Board[xy._x][xy._y]->SetType(_Lair);
	return true;
}

void LandSpace::AddEnemy(EnemyPoint EnPoint) {
	EnemyTable.push_back(EnPoint);
}

bool LandSpace::AddRoad(Point xy) {
	if (Board[xy._x][xy._y]->GetType() != 3)
		return false;
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = new Cell(_Road);
	return true;
}

bool LandSpace::AddCastle(Point xy, Castle *castle) {
	if (Board[xy._x][xy._y]->GetType() != 3)
		return false;
	KingCastle = xy;
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = castle;
	Board[xy._x][xy._y]->SetType(_Castle);
	return true;
}

void LandSpace::DeleteTower(Point xy){
	for (int i = 0; i < TowerTable.size(); i++) 
		if (TowerTable[i] == xy) {
			delete Board[xy._x][xy._y];
			Board[xy._x][xy._y] = new Cell(3);
			TowerTable.erase(TowerTable.begin() + i);
		}
}

void LandSpace::DeleteLair(Point xy) {
	for (int i = 0; i < LairTable.size(); i++)
		if (LairTable[i] == xy) {
			delete Board[xy._x][xy._y];
			Board[xy._x][xy._y] = new Cell(3);
			LairTable.erase(TowerTable.begin() + i);
		}
}

void LandSpace::DeleteCastle() {
	delete Board[KingCastle._x][KingCastle._y]; 
	Board[KingCastle._x][KingCastle._y] = new Cell(3);
	KingCastle._x = -1;
	KingCastle._y = -1;
}

void LandSpace::KillEnemy(Point xy) {
	for (int i = 0; i < EnemyTable.size(); i++)
		if (EnemyTable[i]._xy == xy) {
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
	Lair *lair;
	Castle *castle;
	Structure *tower;
	EnemyPoint *newEnemy;
	
	castle = dynamic_cast<Castle*>(Board[KingCastle._x][KingCastle._y]);
	if (castle->GetHP() <= 0)
		return false;
	
	for (; i < LairTable.size(); i++) {
		lair = dynamic_cast <Lair*> (Board[LairTable[i]._x][LairTable[i]._y]);
		newEnemy = lair->ReleaseEnemy();
		if (!newEnemy) {
			AddEnemy(EnemyPoint(newEnemy->_Enemy, newEnemy->_xy));	// возможно перегружу присваивание
			delete newEnemy;
		}
	}
	
	for (i = 0; i < EnemyTable.size(); i++) {
		DoStep(&EnemyTable[i]);
	}

	for (i = 0; i < TowerTable.size(); i++) {
		tower = dynamic_cast <Structure*> (Board[LairTable[i]._x][LairTable[i]._y]);
		CauseDamage(tower, TowerTable[i]);
	}

	return true;
}

void LandSpace::DoStep(EnemyPoint *enemy) {
	
	Point step;
	Castle *castle;

	step = Board[enemy->_xy._x][enemy->_xy._y]->GetNext();
	//
	enemy->_xy = step;
	// изменится для плавного перемещения
	if (step == KingCastle) {
		castle = dynamic_cast<Castle*>(Board[KingCastle._x][KingCastle._y]);
		castle->SubHP(enemy->_Enemy->GetHP());
	}
}

void LandSpace::CauseDamage(Structure *tower, Point xy) {

	Point enemy;
	vector <EnemyPoint> EnPoint;
	int radius = tower->GetRadius();

	for (int i = 0; i < EnemyTable.size(); i++) {
		enemy = EnemyTable[i]._xy;
		if (pow(enemy._x - xy._x, 2) + pow(enemy._y - xy._y, 2) < radius) {
			EnPoint.push_back(EnemyTable[i]);
		}
	}
}

bool LandSpace::FindWay() {
	Point p;
	Lair *lair;
	bool f = false;

	for (int i = 0; i < LairTable.size(); i++) {
		p = LairTable[i];
		lair = dynamic_cast<Lair*>(Board[p._x][p._y]);

		if (p._x && (Board[p._x - 1][p._y]->GetType() == _Road) && !(Board[p._x - 1][p._y]->GetNext() == p)) {
			lair->SetStartPosition(Point(p._x - 1, p._y));
			p._x = p._x - 1;
			f = true;
		}
		if (!f && (p._x < _size._x) && (Board[p._x + 1][p._y]->GetType() == _Road) && !(Board[p._x + 1][p._y]->GetNext() == p)) {
			lair->SetStartPosition(Point(p._x + 1, p._y));
			p._x = p._x + 1;
			f = true;
		}
		if (!f && p._y && (Board[p._x][p._y - 1]->GetType() == _Road) && !(Board[p._x][p._y - 1]->GetNext() == p)) {
			lair->SetStartPosition(Point(p._x, p._y - 1));
			p._y = p._y - 1;
			f = true;
		}
		if (!f && (p._y < _size._y) && (Board[p._x + 1][p._y]->GetType() == _Road) && !(Board[p._x + 1][p._y]->GetNext() == p)) {
			lair->SetStartPosition(Point(p._x, p._y + 1));
			p._y = p._y + 1;
			f = true;
		}
		
		if (!f) return false;
		f = false;

		while (!(p == KingCastle) && !f) {
			f = true;
			if (Board[p._x][p._y]->GetNext()._x != -1) {
				p = KingCastle;
				f = false;
				break;
			}

			if (p._x && (Board[p._x - 1][p._y]->GetType() == _Road) && !(Board[p._x - 1][p._y]->GetNext() == p)) {
				Board[p._x][p._y]->SetX(p._x - 1);
				Board[p._x][p._y]->SetY(p._y);
				p._x = p._x - 1;
				f = false;
			}
			if (f && (p._x < _size._x) && (Board[p._x + 1][p._y]->GetType() == _Road) && !(Board[p._x + 1][p._y]->GetNext() == p)) {
				Board[p._x][p._y]->SetX(p._x + 1);
				Board[p._x][p._y]->SetY(p._y);
				p._x = p._x + 1;
				f = false;
			}
			if (f && p._y && (Board[p._x][p._y - 1]->GetType() == _Road) && !(Board[p._x][p._y - 1]->GetNext() == p)) {
				Board[p._x][p._y]->SetX(p._x);
				Board[p._x][p._y]->SetY(p._y - 1);
				p._y = p._y - 1;
				f = false;
			}
			if (f && (p._y < _size._y) && (Board[p._x + 1][p._y]->GetType() == _Road) && !(Board[p._x + 1][p._y]->GetNext() == p)) {
				Board[p._x][p._y]->SetX(p._x);
				Board[p._x][p._y]->SetY(p._y + 1);
				p._y = p._y - 1;
				f = false;
			}
			if (!f) {
				p._x = Board[p._x][p._y]->GetNext()._x;
				p._y = Board[p._x][p._y]->GetNext()._y;
			}
		}

		if (f)
			return false;
	}
	return true;
}
