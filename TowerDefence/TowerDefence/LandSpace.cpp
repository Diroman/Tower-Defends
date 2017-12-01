#include "stdafx.h"
#include "LandSpace.h"
#include <cmath>

// Cell Type
#define _Castle 1
#define _Lair 2
#define _Field 3
#define _Road 4
#define _Tower 6
#define _MagicTower 7
#define _Trap 8


LandSpace::LandSpace(Point size)
{
	//Board.resize();
	for (int i = 0; i < _size._x; i++)
		for (int j = 0; j < _size._y; j++)
			Board[i][j] = new Cell();
	KingCastle._x = -1;
	KingCastle._y = -1;
}

bool LandSpace::ChangeSize(int w, int h) {

	if ((w < 3) || (h < 3) || (w > 20) || (h > 20))
		return false;

	_size._x = w;
	_size._y = h;

	for (int i = 0; i < _size._x; i++)
		for (int j = 0; j < _size._y; j++)
			delete Board[i][j];

	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			Board[i][j] = new Cell();
	return true;
}

bool LandSpace::AddTower(Point xy, Structure *tower) {
	if (Board[xy._x][xy._y]->GetType() != _Field)
		return false;
	TowerTable.push_back(xy);
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = tower;
	return true;
}

bool LandSpace::AddLair(Point xy, Lair *lair) {
	if (Board[xy._x][xy._y]->GetType() != _Field)
		return false;
	LairTable.push_back(xy);
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = lair;
	return true;
}

void LandSpace::AddEnemy(EnemyPoint EnPoint) {
	EnemyTable.push_back(EnPoint);
}

bool LandSpace::AddRoad(Point xy) {

	if (Board[xy._x][xy._y]->GetType() != _Field)	 return false;
	if (Board[xy._x - 1][xy._y]->GetType() == _Road) return false;
	if (Board[xy._x + 1][xy._y]->GetType() == _Road) return false;
	if (Board[xy._x][xy._y - 1]->GetType() == _Road) return false;
	if (Board[xy._x][xy._y + 1]->GetType() == _Road) return false;

	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = new Cell();
	return true;
}

bool LandSpace::AddCastle(Point xy, Castle *castle) {
	if (Board[xy._x][xy._y]->GetType() != _Field)
		return false;
	KingCastle = xy;
	delete Board[xy._x][xy._y];
	Board[xy._x][xy._y] = castle;
	return true;
}

void LandSpace::DeleteTower(Point xy){
	for (int i = 0; i < TowerTable.size(); i++) 
		if (TowerTable[i] == xy) {
			delete Board[xy._x][xy._y];
			Board[xy._x][xy._y] = new Cell();
			TowerTable.erase(TowerTable.begin() + i);
		}
}

void LandSpace::DeleteLair(Point xy) {
	for (int i = 0; i < LairTable.size(); i++)
		if (LairTable[i] == xy) {
			delete Board[xy._x][xy._y];
			Board[xy._x][xy._y] = new Cell();
LairTable.erase(TowerTable.begin() + i);
		}
}

void LandSpace::DeleteCastle() {
	delete Board[KingCastle._x][KingCastle._y];
	Board[KingCastle._x][KingCastle._y] = new Cell();
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

	for (i = 0; i < TowerTable.size(); i++) {
		tower = dynamic_cast <Structure*> (Board[LairTable[i]._x][LairTable[i]._y]);
		CauseDamage(tower, TowerTable[i]);
	}

	for (i = 0; i < EnemyTable.size(); i++) {
		DoStep(&EnemyTable[i]);
		if (!EnemyTable[i]._Enemy->CheckHP())
			KillEnemy(EnemyTable[i]._xy);
	}

	return true;
}

void LandSpace::DoStep(EnemyPoint *enemy) {

	Point step;
	Castle *castle;
	Road *road;

	road = dynamic_cast<Road*>(Board[enemy->_xy._x][enemy->_xy._y]);
	step = road->GetNext();
	//
	enemy->_xy = step;
	// изменится для плавного перемещения
	if (step == KingCastle) {
		castle = dynamic_cast<Castle*>(Board[KingCastle._x][KingCastle._y]);
		castle->SubHP(enemy->_Enemy->GetHP());
		enemy->_Enemy->SubHP(enemy->_Enemy->GetHP());
	}
}

void LandSpace::CauseDamage(Structure *tower, Point xy) {		// возможео тоже работает

	Point enemy;
	Enemy *kill;
	vector <EnemyPoint> EnPoint;			// нужно ли очищать
	int TowerRadius = tower->GetRadius();
	int Distance;
	int r = 10000;

	for (int i = 0; i < EnemyTable.size(); i++) {
		enemy = EnemyTable[i]._xy;
		Distance = sqrt(pow(enemy._x - xy._x, 2) + pow(enemy._y - xy._y, 2));
		if (Distance < TowerRadius)
			EnPoint.push_back(EnemyTable[i]);
	}

	if (EnPoint.size() == 0)
		return;

	switch (tower->GetStrategy())
	{
	case NearTower:
		for (int i = 0; i < EnPoint.size(); i++) {
			enemy = EnPoint[i]._xy;
			Distance = sqrt(pow(enemy._x - xy._x, 2) + pow(enemy._y - xy._y, 2));
			if (Distance < r) {
				r = Distance;
				kill = EnPoint[i]._Enemy;
			}
		}
		tower->ApplyDamage(kill);
		break;

	case NearCastle:
		for (int i = 0; i < EnPoint.size(); i++) {
			enemy = EnPoint[i]._xy;
			Distance = dynamic_cast<Road*>(Board[enemy._x][enemy._y])->GetWeight();
			if (Distance < r) {
				r = Distance;
				kill = EnPoint[i]._Enemy;
			}
		}
		tower->ApplyDamage(kill);
		break;

	case Weak:
		for (int i = 0; i < EnPoint.size(); i++) {
			Distance = EnPoint[i]._Enemy->GetHP();
			if (Distance < r) {
				r = Distance;
				kill = EnPoint[i]._Enemy;
			}
		}
		tower->ApplyDamage(kill);
		break;

	case Strong:
		r = 0;
		for (int i = 0; i < EnPoint.size(); i++) {
			Distance = EnPoint[i]._Enemy->GetHP();
			if (Distance > r) {
				r = Distance;
				kill = EnPoint[i]._Enemy;
			}
		}
		tower->ApplyDamage(kill);
		break;

	case Fast:
		r = 0;
		for (int i = 0; i < EnPoint.size(); i++) {
			Distance = EnPoint[i]._Enemy->GetSpeed();
			if (Distance > r) {
				r = Distance;
				kill = EnPoint[i]._Enemy;
			}
		}
		tower->ApplyDamage(kill);
		break;

	default:
		break;
	}
}

bool LandSpace::FindWay() {		// возможно работает
	Point p;
	Lair *lair;
	Road *road;
	int weight = 0;
	bool f = false;

	for (int i = 0; i < LairTable.size(); i++) {
		p = LairTable[i];
		lair = dynamic_cast<Lair*>(Board[p._x][p._y]);

		if (p._x && (Board[p._x - 1][p._y]->GetType() == _Road)) {
			road = dynamic_cast<Road*>(Board[p._x - 1][p._y]);
			lair->SetStartPosition(Point(p._x - 1, p._y));
			road->SetWeight(1);
			p._x = p._x - 1;
			f = true;
		}
		if (!f && (p._x < _size._x) && (Board[p._x + 1][p._y]->GetType() == _Road)) {
			road = dynamic_cast<Road*>(Board[p._x + 1][p._y]);
			lair->SetStartPosition(Point(p._x + 1, p._y));
			road->SetWeight(1);
			p._x = p._x + 1;
			f = true;
		}
		if (!f && p._y && (Board[p._x][p._y - 1]->GetType() == _Road)) {
			road = dynamic_cast<Road*>(Board[p._x][p._y - 1]);
			lair->SetStartPosition(Point(p._x, p._y - 1));
			road->SetWeight(1);
			p._y = p._y - 1;
			f = true;
		}
		if (!f && (p._y < _size._y) && (Board[p._x][p._y + 1]->GetType() == _Road)) {
			road = dynamic_cast<Road*>(Board[p._x][p._y + 1]);
			lair->SetStartPosition(Point(p._x, p._y + 1));
			road->SetWeight(1);
			p._y = p._y + 1;
			f = true;
		}
		
		if (!f) return false;
		f = false;
		weight++;

		while (!(p == KingCastle) && !f) {

			f = true;
			if (dynamic_cast<Road*>(Board[p._x][p._y])->GetNext()._x != -1) {	// для нескольких логов, с пересекающимися дорогами 
				p = KingCastle;
				f = false;
				break;
			}

			if (p._x && (Board[p._x - 1][p._y]->GetType() == _Road) && (Board[p._x - 1][p._y]->GetType() == _Castle)) {
				if (Board[p._x - 1][p._y]->GetType() == _Castle) {
					p = KingCastle;
					road->SetNext(p);
					f = false;
				}
				else {
					road = dynamic_cast<Road*>(Board[p._x][p._y]);
					if (!(road->GetNext() == Point(p._x - 1, p._y))) {
						road->SetNext(Point(p._x - 1, p._y));
						road->SetWeight(weight++);
						p._x = p._x - 1;
						f = false;
					}
				}
			}
			if (f && (p._x < _size._x) && (Board[p._x + 1][p._y]->GetType() == _Road) && (Board[p._x + 1][p._y]->GetType() == _Castle)) {
				if (Board[p._x + 1][p._y]->GetType() == _Castle) {
					p = KingCastle;
					road->SetNext(p);
					f = false;
				}
				else {
					road = dynamic_cast<Road*>(Board[p._x + 1][p._y]);
					if (!(road->GetNext() == Point(p._x + 1, p._y))) {
						road->SetNext(Point(p._x + 1, p._y));
						road->SetWeight(weight++);
						p._x = p._x + 1;
						f = false;
					}
				}
			}
			if (f && p._y && (Board[p._x][p._y - 1]->GetType() == _Road) && (Board[p._x][p._y - 1]->GetType() == _Castle)) {
				if (Board[p._x][p._y - 1]->GetType() == _Castle) {
					p = KingCastle;
					road->SetNext(p);
					f = false;
				}
				else {
					road = dynamic_cast<Road*>(Board[p._x][p._y - 1]);
					if (!(road->GetNext() == Point(p._x, p._y - 1))) {
						road->SetNext(Point(p._x, p._y - 1));
						road->SetWeight(weight++);
						p._y = p._y - 1;
						f = false;
					}
				}
			}
			if (f && (p._y < _size._y) && (Board[p._x][p._y + 1]->GetType() == _Road) && (Board[p._x][p._y + 1]->GetType() == _Castle)) {
				if (Board[p._x][p._y + 1]->GetType() == _Castle) {
					p = KingCastle;
					road->SetNext(p);
					f = false;
				}
				else {
					road = dynamic_cast<Road*>(Board[p._x][p._y + 1]);
					if (!(road->GetNext() == Point(p._x, p._y + 1))) {
						road->SetNext(Point(p._x, p._y + 1));
						road->SetWeight(weight++);
						p._y = p._y + 1;
						f = false;
					}
				}
			}
		}

		if (f)
			return false;
	}
	return true;
}
