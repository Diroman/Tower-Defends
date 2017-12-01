#pragma once
#ifndef _Structure_H_
#include "Cel.h"
#include "Enemy.h"
#include "Effect.h"
#include <ctime>

// Strategy
#define NearTower 1
#define NearCastle 2
#define Weak 3
#define Strong 4
#define Fast 5


class Lair : public Cell{

private:
	Point _xy;
	time_t _StartTime;
	time_t _Time;
	int _NumberOfEnemy;
	Enemy *_Pattern;

public:
	Lair(time_t, Enemy*, int);
	void SetStartPosition(Point);
	bool CheckNumberOfEnemy();
	bool CheckTime();
	int GetType();
	EnemyPoint* ReleaseEnemy();
};

class Structure : public Cell {
protected:
	int _level;
	int _StartTime;
	time_t _Speed;
	int _Type;
	int Strategy;
	int _Char_Table[3]; 	// Cost, radius, damage

public:
	Structure(int[3], time_t, int);
	bool CheckTime();
	void UpLevel();
	virtual void ApplyDamage(Enemy*) = 0;
	int GetCost();
	int GetRadius();
	int GetStrategy();
	void SetStrategy(int);
	//
	int GetLevel();
	int GetType();
	int* GetChar();
	time_t GetSpeed();
	//
};

class SimpleTower : public Structure {

public:
	SimpleTower(int[3], time_t);
	void ApplyDamage(Enemy*);
	int GetType();
};

class MagicTower : public Structure {

private:
	Effect *_TowerEffect;
public:
	MagicTower(int[3], time_t, Effect*);
	void ApplyDamage(Enemy*);
	int GetType();
};

class Trap : public Structure {


private:
	Effect *_TrapEffect;
public:
	Trap(int[3], time_t, Effect*);
	void ApplyDamage(Enemy*);
	int GetType();
};

#endif