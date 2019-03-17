/*
 * Spindle.h
 *
 *  Created on: Mar 21, 2018
 *      Author: Christian
 */

#ifndef SPINDLE_H
#define SPINDLE_H

#include "RepRapFirmware.h"
#include "IoPorts.h"

class Spindle
{
public:
	struct Coefficents {
		// y = ax^3 + bx^2 + cx + d
		float a;
		float b;
		float c;
		float d;
	};
private:
	PwmPort spindleForwardPort, spindleReversePort;
	bool inverted;
	float currentRpm, configuredRpm, maxRpm;
	int toolNumber;


	Coefficents coefs = { 0.0, 0.0, 1.0, 0.0 };

public:
	Spindle() : inverted(false), currentRpm(0.0), configuredRpm(0.0), maxRpm(DefaultMaxSpindleRpm), toolNumber(-1) { }

	bool SetPins(LogicalPin lpr, LogicalPin lpf, bool invert);
	void GetPins(LogicalPin& lpf, LogicalPin& lpr, bool& invert) const;

	int GetToolNumber() const { return toolNumber; }
	void SetToolNumber(int tool) { toolNumber = tool; }

	void SetPwmFrequency(float freq);
	void SetMaxRpm(float max) { maxRpm = max; }
	void SetCorrectionCoefs( Coefficents newCoefs ) { coefs = newCoefs; }

	float GetCurrentRpm() const { return currentRpm; }
	float GetRpm() const { return configuredRpm; }
	void SetRpm(float rpm);

	void TurnOn();
	void TurnOff();

};

#endif
