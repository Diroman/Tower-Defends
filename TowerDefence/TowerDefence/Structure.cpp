#include "stdafx.h"
#include "Enemy.h"
#include "Structure.h"


Lair::Lair(time_t Time, Enemy *Pattern, int NumberOfEnemy) :Cell() {
	_Time = Time;
	_Pattern = new Enemy(Pattern);
	_NumberOfEnemy = NumberOfEnemy;
	_StartTime = time(NULL);
}

bool Lair::CheckTime() {
	return /*SDL_GetTicks()*/(int)time(NULL) - _StartTime > _Time ? true : false;
}

bool Lair::CheckNumberOfEnemy() {
	return	_NumberOfEnemy > 0 ? true : false;
}

void Lair::SetStartPosition(Point xy) {
	_xy = xy;
}

EnemyPoint* Lair::ReleaseEnemy() {
	if (CheckTime() && (_NumberOfEnemy > 0)) {
		_StartTime = time(NULL);
		_NumberOfEnemy--;
		return new EnemyPoint(_Pattern, _xy);
	}
	return nullptr;
}

int Lair::GetType() {
	return 2;
}


Structure::Structure(int table[3], time_t Time, int type) : Cell(){
	_StartTime = time(NULL);
	_level = 1;
	_Speed = Time;
	for (int i = 0; i < 3; i++)
		_Char_Table[i] = table[i];
}

bool Structure::CheckTime() {
	return time(NULL)/*SDL_GetTicks()*/ - _StartTime > _Char_Table[3] ? true : false;
}

void Structure::UpLevel() {
	_level++;
}

int Structure::GetType() {
	return _Type;
}

int Structure::GetLevel() {
	return _level;
}

int Structure::GetCost() {
	return _Char_Table[0];
}

int Structure::GetRadius() {
	return _Char_Table[1];
}

void Structure::SetStrategy(int str) {
	Strategy = str;
}

int Structure::GetStrategy() {
	return Strategy;
}


SimpleTower::SimpleTower(int tab[3], time_t tm) : Structure(tab, tm, 6) {
	_Type = 6;
}

void SimpleTower::ApplyDamage(Enemy *enemy) {
	enemy->SubHP(_Char_Table[2]);
}

int SimpleTower::GetType() {
	return 6;
}


MagicTower::MagicTower(int tb[3], time_t tm, Effect *ef) : Structure(tb, tm, 7) {
	_TowerEffect = ef;
	_Type = 7;
}

void MagicTower::ApplyDamage(Enemy *enemy) {
	enemy->SetEffect(_TowerEffect->Clone());
}

int MagicTower::GetType() {
	return 7;
}


Trap::Trap(int tb[3], time_t tm, Effect *ef) : Structure(tb, tm, 8) {
	_TrapEffect = ef;
	_Type = 8;
}

void Trap::ApplyDamage(Enemy *enemy) {
	enemy->SetEffect(_TrapEffect->Clone());
}

int Trap::GetType() {
	return 8;
}

//
int* Structure::GetChar() {
	return _Char_Table;
}

time_t  Structure::GetSpeed() {
	return _Speed;
}
//