#include "stdafx.h"
#include "LandSpace.h"

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
		CauseDamage(tower);
	}

	return true;
}

void LandSpace::DoStep(EnemyPoint *enemy) {
	
}

void LandSpace::CauseDamage(Structure *tower) {

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
			if (Board[p._x][p._y]->GetNext()._x != -1)
				break;
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
