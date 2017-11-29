#pragma once
#ifndef _Structure_H_
#include "Enemy.h"
#include "Effect.h"
#include <ctime>
#include "Castle.h"


class Lair : public Cell{

private:
	Point _xy;
	time_t _StartTime;
	time_t _Time;
	int _NumberOfEnemy;
	Enemy *_Pattern;

public:
	Lair(time_t, Enemy*, int);
	bool CheckNumberOfEnemy();
	bool CheckTime();
	EnemyPoint* ReleaseEnemy();
};

class Structure : public Cell {
protected:
	int _level;
	int _StartTime;
	time_t _Speed;
	int _Char_Table[3]; 	// Cost, radius, damage

public:
	Structure(int[3], time_t, int);
	bool CheckTime();
	void UpLevel();
	virtual void ApplyDamage(Enemy*) = 0;
	//
	int GetLevel();
	int* GetChar();
	time_t GetSpeed();
	//
};

class SimpleTower : public Structure {

public:
	SimpleTower(int[3], time_t);

	void ApplyDamage(Enemy*);
};

class MagicTower : public Structure {

private:
	Effect *_TowerEffect;
public:
	MagicTower(int[3], time_t, Effect*);
	void ApplyDamage(Enemy*);
};

class Trap : public Structure {


private:
	Effect *_TrapEffect;
public:
	Trap(int[3], time_t, Effect*);
	void ApplyDamage(Enemy*);
};

#endif