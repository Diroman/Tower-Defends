#include "stdafx.h"
#include "Effect.h"
#include "Enemy.h"

Effect::Effect(double value, time_t tm) {
	_StartTime = time(NULL);//SDL_GetTicks();
	_ActionTime = tm;
	_Value = value;
}

int Effect::GetType() {
	return _Type;
}

bool Effect::CheckTime() {
	return /*SDL_GetTicks()*/(int)time(NULL) - _StartTime < _ActionTime ? true : false;
}

time_t Effect::GetTime() {
	return _ActionTime;
}

double Effect::GetValue() {
	return _Value;
}


Freeze::Freeze(double Value, time_t tm) :Effect(Value, tm) {
	status = false;
	_Type = EfFreeze;
}

Freeze::Freeze(Freeze *ef) :Effect(this->_Value, this->_ActionTime) {
	_StartTime = time(NULL);
	_ActionTime = ef->_ActionTime;
	_Value = ef->_Value;
	_Type = ef->_Type;
}

Freeze* Freeze::Clone() {
	return new Freeze(this);
}

bool Freeze::MakeDamage(Enemy *en) {

	int math;
	if (!CheckTime()) {
		math = en->GetSpeed() / (1 - _Value);
		en->ChangeSpeed(_Value);
		return false;
	}
	if (!status) {
		math = en->GetSpeed() - en->GetSpeed() * _Value;
		en->ChangeSpeed(math);
		status = true;
	}
	return true;
}


Venom::Venom(double Value, time_t tm) :Effect(Value, tm) {
	_Value = Value;
	_Type = EfVenom;
}

Venom::Venom(Venom *ef) : Effect(this->_Value, this->_ActionTime) {
	_StartTime = time(NULL);
	_ActionTime = ef->_ActionTime;
	_Value = ef->_Value;
	_Type = ef->_Type;
}

Venom* Venom::Clone() {
	return new Venom(this);
}

bool Venom::MakeDamage(Enemy *en) {

	if (!CheckTime())
		return false;
	en->SubHP(_Value);
	return true;
}


Weakness::Weakness(double Value, time_t tm) :Effect(Value, tm) {
	status = false;
	_Value = Value;
	_Type = EfWeakness;
}

Weakness::Weakness(Weakness *ef) :Effect(this->_Value, this->_ActionTime) {
	_StartTime = time(NULL);
	_ActionTime = ef->_ActionTime;
	_Value = ef->_Value;
	_Type = ef->_Type;
}

Weakness* Weakness::Clone() {
	return new Weakness(this);
}

bool Weakness::MakeDamage(Enemy *en) {

	if (!CheckTime()) {
		en->ChangeRatio(1);
		return false;
	}
	if (!status)
		en->ChangeRatio(_Value);
}
