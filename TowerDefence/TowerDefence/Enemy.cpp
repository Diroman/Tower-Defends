#include "stdafx.h"
#include "Enemy.h"
#include "Castle.h"


Cell::Cell(int type) {
	_Type = type;
	next._x = -1;
	next._y = -1;
}

Cell::~Cell() {

}

void Cell::SetNext(Point p) {
	next = p;
}

void Cell::SetX(int x) {
	next._x = x;
}

void Cell::SetY(int y) {
	next._y = y;
}

Point Cell::GetNext() {
	return next;
}

void Cell::SetType(int type) {
	_Type = type;
}

int Cell::GetType() {
	return _Type;
}


Enemy::Enemy() : 
	 _HP_Max(100), _Speed(10), _Money(20), _ratio(1) {
	_HP = _HP_Max;
	for (int i = 0; i < 2; i++)
		_EffectTable[i] = nullptr;
}

Enemy::Enemy(Enemy *en) {

	_HP_Max = en->_HP_Max;
	_HP = _HP_Max;
	_Speed = en->_Speed;
	_Money = en->_Money;
	for (int i = 0; i < 3; i++) {
		if (en->_EffectTable[i])
			this->_EffectTable[i] = en->_EffectTable[i]->Clone();
		else
			this->_EffectTable[i] = nullptr;
	}

}

Enemy::Enemy(int HP_Max, int Speed, int Money) :
	 _HP_Max(HP_Max), _Speed(Speed), _Money(Money) {

}

void Enemy::ChangeSpeed(double Speed) {
	_Speed -= Speed;
}

void Enemy::SubHP(double HP) {
	
	_HP -= HP * _ratio;
}

bool Enemy::CheckHP() {
	return _HP <= 0 ? false : true;
}

void Enemy::SetEffect(Effect *ef) {

	int i = 0;
	bool f = true;
	while (i++ < 3)
		if (_EffectTable[i]->GetType() == ef->GetType())
			return;
	i = 0;
	while (f && (i++ < 3))
		if (!_EffectTable[i]) {
			_EffectTable[i] = ef;
			f = false;
		}
	if (f)
		delete ef;
}

void Enemy::ApplyEffect() {

	int i = 0;
	bool fl;
	while (_EffectTable[i] || i++ < 3) {
		fl = _EffectTable[i]->MakeDamage(this);		// true - эффект ещё действует
		if (!fl) {
			delete _EffectTable[i];
			_EffectTable[i] = nullptr;
		}
	}
}

void Enemy::ChangeRatio(double ratio) {
	_ratio = ratio;
}

int Enemy::GetMoney() {
	return _Money;
}

double Enemy::GetSpeed() {
	return _Speed;
}

//
double Enemy::GetHP() {
	return _HP;
}

Effect* Enemy::GetChar() {
	return _EffectTable[3];
}
//