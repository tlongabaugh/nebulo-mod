#include "CurveApprox.h"
#include "math.h"
#include "string.h"

extern void outputDebugInt(char *msg, int val);
extern void outputDebugDouble(char *msg, double val);

//
//								CurveApprox
//
CurveApprox::CurveApprox(int points, double *inputs, double *outputs, char *name)
{
	_points = points;
	_inputs = new double[points];
	_outputs = new double[points];
	_lastInput = FP_NAN;
	_lastOutput = 0.0;
	for (int i = 0; i < points; i++)
	{
		_inputs[i] = inputs[i];
		_outputs[i] = outputs[i];
	}
	strcpy(_name, name);
}

//
//                              Map
//
//  Maps the input to an output in the piece-wise approx.  Caches the last value
//  so repeated lookups of the same thing are fast.
//
double CurveApprox::Map(double input)
{
	// see if we already did this
	if (input == _lastInput)
	{
		return _lastOutput;
	}

	// remember this input
	_lastInput = input;
	double sign = 1.0;
	if (input < 0)
	{
		sign = -1.0;
		input = -input;
	}

	// check for out-of-bounds
	if (input < _inputs[0])
	{
		outputDebugDouble("underflow:", input);
		return _lastOutput = _outputs[0];
	}

	if (input >= _inputs[_points - 1])
	{
		outputDebugDouble("overflow:", input);
		return _lastOutput = _outputs[_points - 1];
	}

	// scan and interpolate
	for (int i = 0; i < _points - 1; i++)
	{
		if (input >= _inputs[i] && input <= _inputs[i + 1])
		{
			// got a pair to map to
			double i1 = _inputs[i];
			double i2 = _inputs[i + 1];
			double iDelta = i2 - i1;
			double iPrime = input - i1;

			// see fractionally how far "in" we are
			double frac = iPrime / iDelta;

			// get mapped output points
			double o1 = _outputs[i];
			double o2 = _outputs[i + 1];
			double oDelta = o2 - o1;

			// now map to output
			_lastOutput = sign * (frac * oDelta + o1);
			return _lastOutput;
		}
	}

	// should never get to here
	throw;
}
