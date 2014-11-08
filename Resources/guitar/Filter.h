//
//								Filter
//
//	Abstract base class for a filter
//

#define PI	3.14159265358979323846
#define E	2.718281828

class Filter
{
public:
	Filter() {};
	~Filter() {};
	virtual double Process(double val) = 0;
	const double SAMPLERATE = 44100.0;
};

//
//								OnePoleFilter
//
//	High pass and low pass have same processing routine, just different coefficients
//
class OnePoleFilter : public Filter
{
public:
	OnePoleFilter(double cutoff);
	~OnePoleFilter() {};
	double Process(double val)
	{
		double y = _a0 * val + _a1 * _x1 + _b1 * _y1;
		_x1 = val;
		_y1 = y;
		return  y;
	};
protected:
	double _fc;
	double _a0, _a1, _x1, _y1, _b1;
};

//
//								TwoPoleFilter
//
//	High pass and low pass have same processing routine, just different coefficients
//
class TwoPoleFilter : public Filter
{
public:
	TwoPoleFilter(double cutoff);
	~TwoPoleFilter() {};
	double Process(double val)
	{
		double y = (b0 / a0) * val + (b1 / a0) * x1 + (b2 / a0) * x2 - (a1 / a0) * y1 - (a2 / a0) * y2;
		x2 = x1;
		x1 = val;
		y2 = y1;
		y1 = y;
		return y;
	};
protected:
	double _cutoff;
	const double Q = 1.1;
	double x1, x2, y1, y2;
	double a0, a1, a2, b0, b1, b2, w0, alpha;
};

//
//								TwoPoleHighPassFilter
//
class TwoPoleHighPassFilter : public TwoPoleFilter
{
public:
	TwoPoleHighPassFilter(double cutoff);
	~TwoPoleHighPassFilter() {};
};

//
//								TwoPoleLowPassFilter
//
class TwoPoleLowPassFilter : public TwoPoleFilter
{
public:
	TwoPoleLowPassFilter(double cutoff);
	~TwoPoleLowPassFilter() {};
};

//
//								OnePoleHighPassFilter
//
class OnePoleHighPassFilter : public OnePoleFilter
{
public:
	OnePoleHighPassFilter(double cutoff);
	~OnePoleHighPassFilter() {};
};

//
//								OnePoleLowPassFilter
//
class OnePoleLowPassFilter : public OnePoleFilter
{
public:
	OnePoleLowPassFilter(double cutoff);
	~OnePoleLowPassFilter();
};

//
//								Limiter
//
class Limiter : public Filter
{
public:
	Limiter(double releaseMs);
	~Limiter() {};
	double Process(double val);
	void SetThreshold(double thresh);
private:
	double _threshold;
	double _envelope;
	double _release;
	static const int CBSZ = 882; // arbitrarily one period at 50hz 
	float _inq[CBSZ];
	int _fp;
	double _total;
};

//
//								Phaser
//
//	Classic 4 stage phaser, gotta mix it to hear the effect of course
//
class Phaser : public Filter
{
public:
	Phaser();
	~Phaser() {};
	double Process(double val);
	void SetSweep(double rate, double width);
private:
	double _sweepRate;
	double _wp;
	double _minwp;
	double _maxwp;
	double _sweepFactor;
	double _lx1;
	double _ly1;
	double _lx2;
	double _ly2;
	double _lx3;
	double _ly3;
	double _lx4;
	double _ly4;

};

//
//								Chorus
//
//	Simple smooth sweep chorus a la EH
//
class Chorus : public Filter
{
public:
	Chorus();
	~Chorus() {};
	double Process(double val);
	void SetSweep(double rate, double width);
private:
	static const int BSZ = 8192;		// must be about 1/5 of a second at given sample rate
	float _buf[BSZ];
	int _fp;
	double _step;
	double _sweep;
	double _sweepRate;
	double _minSweepSamples;
	double _maxSweepSamples;
	int  _delaySamples;
	int _sweepSamples;
};