/*
 * Spindle.cpp
 *
 *  Created on: Mar 21, 2018
 *      Author: Christian
 */

#include "Spindle.h"

bool Spindle::SetPins(LogicalPin lpf, LogicalPin lpr, bool invert)
{
	const bool ok1 = spindleForwardPort.Set(lpf, PinAccess::pwm, invert);
	if (lpr == NoLogicalPin)
	{
		spindleReversePort.Clear();
		return ok1;
	}
	const bool ok2 = spindleReversePort.Set(lpr, PinAccess::pwm, invert);
	return ok1 && ok2;
}

void Spindle::GetPins(LogicalPin& lpf, LogicalPin& lpr, bool& invert) const
{
	lpf = spindleForwardPort.GetLogicalPin(invert);
	lpr = spindleReversePort.GetLogicalPin();
}

void Spindle::SetPwmFrequency(float freq)
{
	spindleReversePort.SetFrequency(freq);
	spindleForwardPort.SetFrequency(freq);
}

void Spindle::SetRpm(float rpm)
{
	configuredRpm = rpm;
}

void Spindle::TurnOn()
{
	const float correctedRpm = coefs.a*powf(configuredRpm, 3.0)
		+ coefs.b*powf(configuredRpm, 2.0)
		+ coefs.c*configuredRpm + coefs.d;
	float pwm = abs(correctedRpm / maxRpm);
	pwm = pwm < 1.0 ? pwm : 1.0; // Just in case, need to think about this.
	if (configuredRpm >= 0.0)
	{
		spindleReversePort.WriteAnalog(0.0);
		spindleForwardPort.WriteAnalog(pwm);
	}
	else
	{
		spindleReversePort.WriteAnalog(pwm);
		spindleForwardPort.WriteAnalog(0.0);
	}
	currentRpm = configuredRpm;
}

void Spindle::TurnOff()
{
	spindleReversePort.WriteAnalog(0.0);
	spindleForwardPort.WriteAnalog(0.0);
	currentRpm = 0.0;
}



// End
