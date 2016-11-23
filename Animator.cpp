// 
// 
// 

#include "Animator.h"



bool Animator::EqColor(CRGB c1, CRGB c2) {
	return c1 == c2;
}

void Animator::Clear(CLEDController *controller, CRGB color) {

	CRGB* d = controller->leds();

	for (int i = 0; i < controller->size(); i++) {
		d[i] = color;
	}
}

void Animator::DoSwap(CLEDController *controller, int index, CRGB c1, CRGB c2) {
	CRGB* d = controller->leds();

	d[index] = EqColor(d[index], c1) ? c2 : c1;
}

bool Animator::HasFlag(PositionFlags value, PositionFlags testValue) {
	return (value & testValue) == testValue;
}