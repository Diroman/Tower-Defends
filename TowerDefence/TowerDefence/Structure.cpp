#include "stdafx.h"
#include "Structure.h"
#include "Enemy.h"

// Strategy
#define NearTower 1;
#define NearCastle 2;
#define Weak 3;
#define Strong 4;


Lair::Lair(time_t Time, Enemy *Pattern, int NumberOfEnemy) :Cell(2) {
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


Structure::Structure(int table[3], time_t Time, int type) : Cell(type){
	_StartTime = time(NULL)/*SDL_GetTicks()*/;
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

int Structure::GetLevel() {
	return _level;
}


SimpleTower::SimpleTower(int tab[3], time_t tm) : Structure(tab, tm, 6) {

}

void SimpleTower::ApplyDamage(Enemy *enemy) {
	enemy->SubHP(_Char_Table[2]);
}


MagicTower::MagicTower(int tb[3], time_t tm, Effect *ef) : Structure(tb, tm, 7) {
	_TowerEffect = ef;
}

void MagicTower::ApplyDamage(Enemy *enemy) {
	enemy->SetEffect(_TowerEffect->Clone());
}


Trap::Trap(int tb[3], time_t tm, Effect *ef) : Structure(tb, tm, 8) {
	_TrapEffect = ef;
}

void Trap::ApplyDamage(Enemy *enemy) {
	enemy->SetEffect(_TrapEffect->Clone());
}

//
int* Structure::GetChar() {
	return _Char_Table;
}

time_t  Structure::GetSpeed() {
	return _Speed;
}
//