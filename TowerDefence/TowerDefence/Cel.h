#pragma once
#ifndef _Cel_H_

#include <string>

using namespace std;

struct Point {
	int _x;
	int _y;

	Point() {};
	Point(int, int);
	friend bool operator == (const Point&, const Point&);
};

class Cell {

public:
	Cell();
	virtual int GetType();
};

class Road : public Cell {
private:
	Point _Next;
	int _Weight;

public:
	void SetNext(Point);
	Point GetNext();
	void SetWeight(int);
	int GetWeight();
	int GetType();
};

class Castle : public Cell {

private:
	string _Name;
	double _HP_Max;
	double _HP = _HP_Max;
	int _Money;

public:
	Castle(const string &Name, double HP_Max, int Money);
	Castle(const Castle &st);
	string GetName();
	double GetHP();
	int GetMoney();
	int GetType();
	void AddMoney(int);
	bool SubMoney(int);		//	true - enought money
	bool SubHP(double);		//  true - castle have HP
};
#endif