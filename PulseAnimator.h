// PulseAnimator.h

#ifndef _PULSEANIMATOR_h
#define _PULSEANIMATOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Animator.h"

class PulseAnimator : public  Animator {
public:
	virtual bool Execute(uint32_t micros, bool modeChanged, CLEDController ** controllers, PositionFlags * flags, uint32_t numControllers);
	PulseAnimator(CRGB forwardColor, CRGB backwardColor, CRGB leftColor, CRGB rightColor, uint32_t period);
	~PulseAnimator() {};
	CRGB GetColor(PositionFlags flags);

private:
	uint32_t _nextMicros;
	uint32_t _period;
	CRGB _forwardColor;
	CRGB _backwardColor;
	CRGB _leftColor;
	CRGB _rightColor;
	uint32_t _epoch;
};

#endif

