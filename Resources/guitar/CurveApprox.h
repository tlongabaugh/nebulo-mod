class CurveApprox
{
public:
	CurveApprox(int points, double *inputs, double *outputs, char *name);
	~CurveApprox()
	{
		delete _inputs;
		delete _outputs;
	};
	double Map(double input);
private:
	char _name[32];
	int _points;
	double *_inputs;
	double *_outputs;
	double _lastInput;
	double _lastOutput;
};