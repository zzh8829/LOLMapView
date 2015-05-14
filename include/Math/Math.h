#ifndef Z_MATH_H_
#define Z_MATH_H_

#include "Core.h"

template <typename Real>
class Math
{
public:
	// Wrappers to hide implementations of functions.  The ACos and ASin
	// functions clamp the input argument to [-1,1] to avoid NaN issues
	// when the input is slightly larger than 1 or slightly smaller than -1.
	// Other functions have the potential for using a fast and approximate
	// algorithm rather than calling the standard math library functions.
	static Real ACos (Real value);
	static Real ASin (Real value);
	static Real ATan (Real value);
	static Real ATan2 (Real y, Real x);
	static Real Ceil (Real value);
	static Real Cos (Real value);
	static Real Exp (Real value);
	static Real FAbs (Real value);
	static Real Floor (Real value);
	static Real FMod (Real x, Real y);
	static Real InvSqrt (Real value);
	static Real Log (Real value);
	static Real Log2 (Real value);
	static Real Log10 (Real value);
	static Real Pow (Real base, Real exponent);
	static Real Sin (Real value);
	static Real Sqr (Real value);
	static Real Sqrt (Real value);
	static Real Tan (Real value);

	// Return -1 if the input is negative, 0 if the input is zero, and +1
	// if the input is positive.
	static int Sign (int value);
	static Real Sign (Real value);

	// Convert between degrees and radians
	static Real Radians (Real degrees);
	static Real Degrees (Real radians);

	// Generate a random number in [0,1].  The random number generator may
	// be seeded by a first call to UnitRandom with a positive seed.
	static Real UnitRandom (unsigned int seed = 0);

	// Generate a random number in [-1,1].  The random number generator may
	// be seeded by a first call to SymmetricRandom with a positive seed.
	static Real SymmetricRandom (unsigned int seed = 0);

	// Generate a random number in [min,max].  The random number generator may
	// be seeded by a first call to IntervalRandom with a positive seed.
	static Real IntervalRandom (Real min, Real max, unsigned int seed = 0);

	// Clamp the input to the specified interval [min,max].
	static Real Clamp (Real value, Real minValue, Real maxValue);

	// Clamp the input to [0,1].
	static Real Saturate (Real value);

	// Fast evaluation of trigonometric and inverse trigonometric functions
	// using polynomial approximations.  The speed ups vary with processor.

	// The input must be in [0,pi/2].
	static Real FastSin0 (Real angle);
	static Real FastSin1 (Real angle);

	// The input must be in [0,pi/2]
	static Real FastCos0 (Real angle);
	static Real FastCos1 (Real angle);

	// The input must be in [0,pi/4].
	static Real FastTan0 (Real angle);
	static Real FastTan1 (Real angle);

	// The input must be in [0,1].
	static Real FastInvSin0 (Real value);
	static Real FastInvSin1 (Real value);

	// The input must be in [0,1].
	static Real FastInvCos0 (Real value);
	static Real FastInvCos1 (Real value);

	// The input must be in [-1,1]. 
	static Real FastInvTan0 (Real value);
	static Real FastInvTan1 (Real value);

	// Fast approximations to exp(-x).  The input x must be in [0,infinity).
	static Real FastNegExp0 (Real value);
	static Real FastNegExp1 (Real value);
	static Real FastNegExp2 (Real value);
	static Real FastNegExp3 (Real value);

	// Fast approximations to InvSqrt
	static Real FastInvSqrt (Real value);

	// Common constants.
	static const Real EPSILON;
	static const Real MAX_VALUE;
	static const Real MIN_VALUE;
	static const Real PI;
	static const Real TWO_PI;
	static const Real HALF_PI;
	static const Real INV_PI;
	static const Real INV_TWO_PI;
	static const Real DEG_TO_RAD;
	static const Real RAD_TO_DEG;
	static const Real LN_2;
	static const Real LN_10;
	static const Real INV_LN_2;
	static const Real INV_LN_10;
	static const Real SQRT_2;
	static const Real INV_SQRT_2;
	static const Real SQRT_3;
	static const Real INV_SQRT_3;
};

template <typename Real>
Real Math<Real>::ACos (Real value)
{
	if (-(Real)1 < value)
	{
		if (value < (Real)1)
		{
			return acos(value);
		}
		else
		{
			return (Real)0;
		}
	}
	else
	{
		return PI;
	}
}

template <typename Real>
Real Math<Real>::ASin (Real value)
{
	if (-(Real)1 < value)
	{
		if (value < (Real)1)
		{
			return asin(value);
		}
		else
		{
			return HALF_PI;
		}
	}
	else
	{
		return -HALF_PI;
	}
}

template <typename Real>
Real Math<Real>::ATan (Real value)
{
	return atan(value);
}

template <typename Real>
Real Math<Real>::ATan2 (Real y, Real x)
{
	if (x != (Real)0 || y != (Real)0)
	{
		return atan2(y, x);
	}
	else
	{
		// Mathematically, ATan2(0,0) is undefined, but ANSI standards
		// require the function to return 0.
		return (Real)0;
	}
}

template <typename Real>
Real Math<Real>::Ceil (Real value)
{
	return ceil(value);
}

template <typename Real>
Real Math<Real>::Cos (Real value)
{
	return cos(value);
}

template <typename Real>
Real Math<Real>::Exp (Real value)
{
	return exp(value);
}

template <typename Real>
Real Math<Real>::FAbs (Real value)
{
	return fabs(value);
}

template <typename Real>
Real Math<Real>::Floor (Real value)
{
	return floor(value);
}

template <typename Real>
Real Math<Real>::FMod (Real x, Real y)
{
	if (y != (Real)0)
	{
		return fmod(x, y);
	}
	else
	{
		assertion(false, "Zero input to FMod\n");
		return (Real)0;
	}
}

template <typename Real>
Real Math<Real>::InvSqrt (Real value)
{
	if (value != (Real)0)
	{
		return ((Real)1)/sqrt(value);
	}
	else
	{
		assertion(false, "Division by zero in InvSqr\n");
		return (Real)0;
	}
}

template <typename Real>
Real Math<Real>::Log (Real value)
{
	if (value > (Real)0)
	{
		return log(value);
	}
	else
	{
		assertion(false, "Nonpositive input to Log\n");
		return (Real)0;
	}
}

template <typename Real>
Real Math<Real>::Log2 (Real value)
{
	if (value > (Real)0)
	{
		return Math<Real>::INV_LN_2 * log(value);
	}
	else
	{
		assertion(false, "Nonpositive input to Log2\n");
		return (Real)0;
	}
}

template <typename Real>
Real Math<Real>::Log10 (Real value)
{
	if (value > (Real)0)
	{
		return Math<Real>::INV_LN_10 * log(value);
	}
	else
	{
		assertion(false, "Nonpositive input to Log10\n");
		return (Real)0;
	}
}

template <typename Real>
Real Math<Real>::Pow (Real base, Real exponent)
{
	if (base >= (Real)0)
	{
		return pow(base, exponent);
	}
	else
	{
		assertion(false, "Negative base not allowed in Pow\n");
		return Math<Real>::MAX_VALUE;
	}
}

template <typename Real>
Real Math<Real>::Sin (Real value)
{
	return sin(value);
}

template <typename Real>
Real Math<Real>::Sqr (Real value)
{
	return value*value;
}

template <typename Real>
Real Math<Real>::Sqrt (Real value)
{
	if (value >= (Real)0)
	{
		return sqrt(value);
	}
	else
	{
		assertion(false, "Negative input to Sqrt\n");
		return (Real)0;
	}
}

template <typename Real>
Real Math<Real>::Tan (Real value)
{
	return tan(value);
}

template <typename Real>
int Math<Real>::Sign (int value)
{
	if (value > 0)
	{
		return 1;
	}

	if (value < 0)
	{
		return -1;
	}

	return 0;
}

template <typename Real>
Real Math<Real>::Sign (Real value)
{
	if (value > (Real)0)
	{
		return (Real)1;
	}

	if (value < (Real)0)
	{
		return (Real)-1;
	}

	return (Real)0;
}

template <typename Real>
Real Math<Real>::Radians (Real degrees)
{
	return degrees * (PI * (Real)0.00555555555);
}

template <typename Real>
Real Math<Real>::Degrees (Real radians)
{
	return radians * ((Real)180 * INV_PI);
}

template <typename Real>
Real Math<Real>::UnitRandom (unsigned int seed)
{
	if (seed > 0)
	{
		srand(seed);
	}

	Real ratio = ((Real)rand())/((Real)(RAND_MAX));
	return (Real)ratio;
}

template <typename Real>
Real Math<Real>::SymmetricRandom (unsigned int seed)
{
	return ((Real)2)*UnitRandom(seed) - (Real)1;
}

template <typename Real>
Real Math<Real>::IntervalRandom (Real min, Real max, unsigned int seed)
{
	return min + (max - min)*UnitRandom(seed);
}

template <typename Real>
Real Math<Real>::Clamp (Real value, Real minValue, Real maxValue)
{
	if (value <= minValue)
	{
		 return minValue;
	}
	if (value >= maxValue)
	{
		return maxValue;
	}
	return value;
}

template <typename Real>
Real Math<Real>::Saturate (Real value)
{
	if (value <= (Real)0)
	{
		 return (Real)0;
	}
	if (value >= (Real)1)
	{
		return (Real)1;
	}
	return value;
}

template <typename Real>
Real Math<Real>::FastSin0 (Real angle)
{
	Real angleSqr = angle*angle;
	Real result = (Real)7.61e-03;
	result *= angleSqr;
	result -= (Real)1.6605e-01;
	result *= angleSqr;
	result += (Real)1.0;
	result *= angle;
	return result;
}

template <typename Real>
Real Math<Real>::FastSin1 (Real angle)
{
	Real angleSqr = angle*angle;
	Real result = -(Real)2.39e-08;
	result *= angleSqr;
	result += (Real)2.7526e-06;
	result *= angleSqr;
	result -= (Real)1.98409e-04;
	result *= angleSqr;
	result += (Real)8.3333315e-03;
	result *= angleSqr;
	result -= (Real)1.666666664e-01;
	result *= angleSqr;
	result += (Real)1.0;
	result *= angle;
	return result;
}

template <typename Real>
Real Math<Real>::FastCos0 (Real angle)
{
	Real angleSqr = angle*angle;
	Real result = (Real)3.705e-02;
	result *= angleSqr;
	result -= (Real)4.967e-01;
	result *= angleSqr;
	result += (Real)1.0;
	return result;
}

template <typename Real>
Real Math<Real>::FastCos1 (Real angle)
{
	Real angleSqr = angle*angle;
	Real result = -(Real)2.605e-07;
	result *= angleSqr;
	result += (Real)2.47609e-05;
	result *= angleSqr;
	result -= (Real)1.3888397e-03;
	result *= angleSqr;
	result += (Real)4.16666418e-02;
	result *= angleSqr;
	result -= (Real)4.999999963e-01;
	result *= angleSqr;
	result += (Real)1.0;
	return result;
}

template <typename Real>
Real Math<Real>::FastTan0 (Real angle)
{
	Real angleSqr = angle*angle;
	Real result = (Real)2.033e-01;
	result *= angleSqr;
	result += (Real)3.1755e-01;
	result *= angleSqr;
	result += (Real)1.0;
	result *= angle;
	return result;
}

template <typename Real>
Real Math<Real>::FastTan1 (Real angle)
{
	Real angleSqr = angle*angle;
	Real result = (Real)9.5168091e-03;
	result *= angleSqr;
	result += (Real)2.900525e-03;
	result *= angleSqr;
	result += (Real)2.45650893e-02;
	result *= angleSqr;
	result += (Real)5.33740603e-02;
	result *= angleSqr;
	result += (Real)1.333923995e-01;
	result *= angleSqr;
	result += (Real)3.333314036e-01;
	result *= angleSqr;
	result += (Real)1.0;
	result *= angle;
	return result;
}

template <typename Real>
Real Math<Real>::FastInvSin0 (Real value)
{
	Real root = Math<Real>::Sqrt(FAbs((Real)1 - value));
	Real result = -(Real)0.0187293;
	result *= value;
	result += (Real)0.0742610;
	result *= value;
	result -= (Real)0.2121144;
	result *= value;
	result += (Real)1.5707288;
	result = HALF_PI - root*result;
	return result;
}

template <typename Real>
Real Math<Real>::FastInvSin1 (Real value)
{
	Real root = Math<Real>::Sqrt(FAbs((Real)1 - value));
	Real result = -(Real)0.0012624911;
	result *= value;
	result += (Real)0.0066700901;
	result *= value;
	result -= (Real)0.0170881256;
	result *= value;
	result += (Real)0.0308918810;
	result *= value;
	result -= (Real)0.0501743046;
	result *= value;
	result += (Real)0.0889789874;
	result *= value;
	result -= (Real)0.2145988016;
	result *= value;
	result += (Real)1.5707963050;
	result = HALF_PI - root*result;
	return result;
}

template <typename Real>
Real Math<Real>::FastInvCos0 (Real value)
{
	Real root = Math<Real>::Sqrt(FAbs((Real)1 - value));
	Real result = -(Real)0.0187293;
	result *= value;
	result += (Real)0.0742610;
	result *= value;
	result -= (Real)0.2121144;
	result *= value;
	result += (Real)1.5707288;
	result *= root;
	return result;
}

template <typename Real>
Real Math<Real>::FastInvCos1 (Real value)
{
	Real root = Math<Real>::Sqrt(FAbs((Real)1 - value));
	Real result = -(Real)0.0012624911;
	result *= value;
	result += (Real)0.0066700901;
	result *= value;
	result -= (Real)0.0170881256;
	result *= value;
	result += (Real)0.0308918810;
	result *= value;
	result -= (Real)0.0501743046;
	result *= value;
	result += (Real)0.0889789874;
	result *= value;
	result -= (Real)0.2145988016;
	result *= value;
	result += (Real)1.5707963050;
	result *= root;
	return result;
}

template <typename Real>
Real Math<Real>::FastInvTan0 (Real value)
{
	Real valueSqr = value*value;
	Real result = (Real)0.0208351;
	result *= valueSqr;
	result -= (Real)0.085133;
	result *= valueSqr;
	result += (Real)0.180141;
	result *= valueSqr;
	result -= (Real)0.3302995;
	result *= valueSqr;
	result += (Real)0.999866;
	result *= value;
	return result;
}

template <typename Real>
Real Math<Real>::FastInvTan1 (Real value)
{
	Real valueSqr = value*value;
	Real result = (Real)0.0028662257;
	result *= valueSqr;
	result -= (Real)0.0161657367;
	result *= valueSqr;
	result += (Real)0.0429096138;
	result *= valueSqr;
	result -= (Real)0.0752896400;
	result *= valueSqr;
	result += (Real)0.1065626393;
	result *= valueSqr;
	result -= (Real)0.1420889944;
	result *= valueSqr;
	result += (Real)0.1999355085;
	result *= valueSqr;
	result -= (Real)0.3333314528;
	result *= valueSqr;
	result += (Real)1;
	result *= value;
	return result;
}

template <typename Real>
Real Math<Real>::FastNegExp0 (Real value)
{
	Real result = (Real)0.0038278;
	result *= value;
	result += (Real)0.0292732;
	result *= value;
	result += (Real)0.2507213;
	result *= value;
	result += (Real)1;
	result *= result;
	result *= result;
	result = ((Real)1)/result;
	return result;
}

template <typename Real>
Real Math<Real>::FastNegExp1 (Real value)
{
	Real result = (Real)0.00026695;
	result *= value;
	result += (Real)0.00227723;
	result *= value;
	result += (Real)0.03158565;
	result *= value;
	result += (Real)0.24991035;
	result *= value;
	result += (Real)1;
	result *= result;
	result *= result;
	result = ((Real)1)/result;
	return result;
}

template <typename Real>
Real Math<Real>::FastNegExp2 (Real value)
{
	Real result = (Real)0.000014876;
	result *= value;
	result += (Real)0.000127992;
	result *= value;
	result += (Real)0.002673255;
	result *= value;
	result += (Real)0.031198056;
	result *= value;
	result += (Real)0.250010936;
	result *= value;
	result += (Real)1;
	result *= result;
	result *= result;
	result = ((Real)1)/result;
	return result;
}

template <typename Real>
Real Math<Real>::FastNegExp3 (Real value)
{
	Real result = (Real)0.0000006906;
	result *= value;
	result += (Real)0.0000054302;
	result *= value;
	result += (Real)0.0001715620;
	result *= value;
	result += (Real)0.0025913712;
	result *= value;
	result += (Real)0.0312575832;
	result *= value;
	result += (Real)0.2499986842;
	result *= value;
	result += (Real)1;
	result *= result;
	result *= result;
	result = ((Real)1)/result;
	return result;
}

template <typename Real>
Real Math<Real>::FastInvSqrt (Real value)
{
	int32_t i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = value * 0.5f;
	y = value;
	i = * ( int32_t * ) &y;						// massive hack
	i = 0x5f3759df - ( i >> 1 );				// magic constant 0x5f3759df
	y = * ( int32_t * ) &i;
	y = y * ( threehalfs - ( x2 * y * y ) );	// 1st Newton iteration
	y = y * ( threehalfs - ( x2 * y * y ) );	// 2nd Newton iteration

	return y;
}

#endif
