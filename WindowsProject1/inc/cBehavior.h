//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------------------------
#include "../inc/misc.h"

//--------------------------------------------------------------------------------------------------
class cRule {

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ctor / dtor
public:
	cRule																					(void);
	~cRule																					(void);
	void									set												(float r, float f);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ public variables
	float																					radius, force;
};
//--------------------------------------------------------------------------------------------------
inline cRule::cRule() {
	radius = 0;
	force = 0;
}
//--------------------------------------------------------------------------------------------------
inline cRule::~cRule(void) {
	//
}
//--------------------------------------------------------------------------------------------------
inline void cRule::set(float r, float f) {
	radius = r;
	force = f;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
class cBehavior {

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ctor / dtor
public:
	cBehavior(void);
	~cBehavior(void);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ public variables
	cRule attract;
	cRule repel;
};
//--------------------------------------------------------------------------------------------------
inline cBehavior::cBehavior() {
	attract.set(0, 0);
	repel.set(0, 0);
}
//--------------------------------------------------------------------------------------------------
inline cBehavior::~cBehavior(void) {
	//
}
//--------------------------------------------------------------------------------------------------