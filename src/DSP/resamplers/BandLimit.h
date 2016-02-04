
#ifndef PDSP_RESAMPLERS_BANDLIMIT_H_INCLUDED
#define PDSP_RESAMPLERS_BANDLIMIT_H_INCLUDED

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/

class CAllPassFilter
{
public:

	CAllPassFilter(const double coefficient);
	~CAllPassFilter();
	double process(double input);

private:
	double a;

	double x0;
	double x1;
	double x2;

	double y0;
	double y1;
	double y2;
};


class CAllPassFilterCascade
{
public:
	CAllPassFilterCascade(const double* coefficients, int N);
	~CAllPassFilterCascade();

	double process(double input);

private:
	CAllPassFilter** allpassfilter;
	int numfilters;
};


class CHalfBandFilter
{
public:
	CHalfBandFilter(const int order, const bool steep);
	~CHalfBandFilter();

	double process(const double input);

private:
	CAllPassFilterCascade* filter_a;
	CAllPassFilterCascade* filter_b;
	double oldout;
};

/*!
    @endcond
*/

}//END NAMESPACE

#endif  // PDSP_RESAMPLERS_BANDLIMIT_H_INCLUDED
