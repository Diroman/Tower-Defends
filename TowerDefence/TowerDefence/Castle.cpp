#include "stdafx.h"
#include "Castle.h"

using namespace std;


Point::Point()
{
	_x = -1;
	_y = -1;
}

Point::Point(int x, int y) {
	_x = x;
	_y = y;
}

bool operator ==(const Point &l, const Point &r) {
	if ((l._x == r._x) && (l._y == r._y))
		return true;
	return false;
}

EnemyPoint::EnemyPoint() {

	_Enemy = nullptr;
}

EnemyPoint::EnemyPoint(Enemy *en, Point xy) {
	_xy._x = xy._x;
	_xy._y = xy._y;
	_Enemy = en;
}

Castle::Castle(string Name, double HP_Max, int Money) :
	Cell(1), _HP_Max(HP_Max), _Money(Money)
{
	_Name = Name;
}

Castle::Castle(const Castle &st) : Cell(1){
	_Name = st._Name;
	_Money = st._Money;
	_HP_Max = st._HP_Max;
	_HP = _HP_Max;
}

string Castle::GetName() {
	return _Name;
}

double Castle::GetHP() {
	return _HP;
}

int Castle::GetMoney() {
	return _Money;
}

void Castle::AddMoney(int Money) {
	_Money += Money;
}

bool Castle::SubMoney(int Money) {

	if (_Money - Money < 0)
		return false;
	_Money -= Money;
	return true;
}

bool Castle::SubHP(double HP) {

	if (_HP - HP < 0)
		return false;
	_HP -= HP;
	return true;
}