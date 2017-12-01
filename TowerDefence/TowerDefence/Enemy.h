#pragma once
#ifndef _Enemy_H_
#include "Effect.h"
#include "Cel.h"

class Enemy {

private:
	double _HP_Max;
	double _HP;
	double _ratio;
	double _Speed;
	int _Money;
	Effect *_EffectTable[3];

public:
	Enemy();
	Enemy(Enemy*);
	Enemy(int HP_Max, int Speed, int Money);
	void SetEffect(Effect*);
	void ApplyEffect();
	void ChangeSpeed(double);
	void SubHP(double);
	bool CheckHP();
	void ChangeRatio(double);
	int GetMoney();
	double GetSpeed();
	//
	Effect* GetChar();
	double GetHP();
	//
};

struct EnemyPoint {
	Point _xy;
	Enemy *_Enemy;

	EnemyPoint();
	EnemyPoint(Enemy*, Point);
};
#endif
