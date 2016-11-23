// 
// 
// 

#include "SwapAnimator.h"



SwapAnimator::SwapAnimator(CRGB forwardColor, CRGB backwardColor, CRGB leftColor, CRGB rightColor, uint32_t period) {
	_period = period;
	_forwardColor = forwardColor;
	_backwardColor = backwardColor;
	_leftColor = leftColor;
	_rightColor = rightColor;
	_epoch = 0;
	_nextMicros = 0;
}


bool SwapAnimator::Execute(uint32_t micros, bool modeChanged, CLEDController ** controllers, PositionFlags flags[], uint32_t numControllers) {

	if (modeChanged)
	{
		_epoch = 0;
		_nextMicros = 0;
	}

	if (micros < _nextMicros)
	{
		return false;
	}

	uint8_t maxStripLength;

	for (uint8_t i = 0; i < numControllers; i++) {
		maxStripLength = max(maxStripLength, controllers[i]->size());
	}

	uint8_t idx = _epoch % maxStripLength;

	for (uint32_t i = 0; i < numControllers; i++) {
		CRGB color = GetColor(flags[i]);

		CRGB * buffer = controllers[i]->leds();
		int numLeds = controllers[i]->size();

		bool reverse = HasFlag(flags[i], OutsideIn);

		if (reverse) {
			idx = maxStripLength - idx;
		}

		if (idx < numLeds) {
			buffer[idx] = buffer[idx] == color ? CRGB::White : color;
		}
	}

	_epoch++;
	_nextMicros = micros + _period;
	return true;
}


CRGB SwapAnimator::GetColor(PositionFlags flags) {
	if (HasFlag(flags, (PositionFlags)(Forward | Right | Diagonal))) {
		return _forwardColor.lerp8(_rightColor, 128);
	}
	if (HasFlag(flags, (PositionFlags)(Forward | Left | Diagonal))) {
		return _forwardColor.lerp8(_leftColor, 128);
	}
	if (HasFlag(flags, (PositionFlags)(Backward | Right | Diagonal))) {
		return _backwardColor.lerp8(_rightColor, 128);
	}
	if (HasFlag(flags, (PositionFlags)(Backward | Left | Diagonal))) {
		return _backwardColor.lerp8(_leftColor, 128);
	}

	if (HasFlag(flags, Forward)) {
		return _forwardColor;
	}

	if (HasFlag(flags, Backward))
	{
		return _backwardColor;
	}

	if (HasFlag(flags, Left)) {
		return _leftColor;
	}

	if (HasFlag(flags, Right)) {
		return _rightColor;
	}

	return CRGB::Azure;

}