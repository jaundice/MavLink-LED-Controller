// PositionFlags.h

#ifndef _POSITIONFLAGS_h
#define _POSITIONFLAGS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

enum PositionFlags : uint32_t {
	Top = 1,
	Bottom = Top <<1,
	Forward = Bottom << 1,
	Backward = Forward << 1,
	Left = Backward <<1,
	Right = Left << 1,
	Diagonal = Right << 1,
	OutsideIn = Diagonal << 1,
	InsideOut = OutsideIn << 1
};

#endif

