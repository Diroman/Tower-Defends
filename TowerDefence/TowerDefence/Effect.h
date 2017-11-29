#pragma once
#ifndef _Effect_H_
//#include "SDL.h"
#include <ctime>
class Enemy;

// Effect Type
#define EfFreeze 1;
#define EfVenom 2;
#define EfWeakness 3;


class Effect {

protected:
	time_t _StartTime;
	time_t _ActionTime;
	double _Value;
	int _Type;

public:
	Effect(double, time_t);  //constructor
	virtual Effect* Clone() = 0;
	bool CheckTime();						// true - ef has time
	virtual bool MakeDamage(Enemy*) = 0;			
	//
	int GetType();
	time_t GetTime();
	double GetValue();
	//
};

class Freeze : public Effect {

private:
	bool status;

public:
	Freeze(double, time_t);
	Freeze(Freeze*);
	virtual Freeze* Clone();
	bool MakeDamage(Enemy*);
	
};

class Venom : public Effect {

public:
	Venom(double, time_t);
	Venom(Venom*);
	virtual Venom* Clone();
	bool MakeDamage(Enemy*);
};

class Weakness : public Effect {

private:
	bool status;

public:
	Weakness(double, time_t);
	Weakness(Weakness*);
	virtual Weakness* Clone();
	bool MakeDamage(Enemy*);
};

#endif