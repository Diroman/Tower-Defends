#include "stdafx.h"
#include "Cel.h"


Point::Point(int x, int y) {
	_x = x;
	_y = y;
}

bool operator ==(const Point &l, const Point &r) {
	if ((l._x == r._x) && (l._y == r._y))
		return true;
	return false;
}

Cell::Cell() {

}

int Cell::GetType() {
	return 4;
}


Castle::Castle(const string &Name, double HP_Max, int Money) :
	Cell(), _HP_Max(HP_Max), _Money(Money)
{
	_Name = Name;
}

Castle::Castle(const Castle &st) : Cell() {
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

int Castle::GetType() {
	return 1;
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


int Road::GetType() {
	return 4;
}

void Road::SetNext(Point next) {
	_Next = next;
}

Point Road::GetNext() {
	return _Next;
}

void Road::SetWeight(int weight) {
	_Weight = weight;
}

int Road::GetWeight() {
	return _Weight;
}
