// Animator.h

#ifndef _ANIMATOR_h
#define _ANIMATOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FastLED.h>
#include "PositionFlags.h"

class Animator  {
public:
	virtual bool Execute(uint32_t micros, bool modeChanged, CLEDController ** controllers, PositionFlags * flags, uint32_t numControllers) = 0;
	~Animator() {};
	bool EqColor(CRGB c1, CRGB c2);
	void Clear(CLEDController *controller, CRGB color);
	void DoSwap(CLEDController *controller, int index, CRGB c1, CRGB c2);
	bool HasFlag(PositionFlags value, PositionFlags testValue);
};



#endif

