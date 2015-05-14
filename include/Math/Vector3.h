#ifndef Z_VECTOR3_H_
#define Z_VECTOR3_H_

#include "Core.h"
#include "Math/Math.h"

template <typename Real>
class Vector3
{
public:

	union 
	{
		struct 
		{
			Real x,y,z;
		};
		Real v[3];
	};

	Vector3 ();
	Vector3 (const Vector3& vec);
	Vector3 (Real x, Real y, Real z);

	Vector3& operator= (const Vector3& vec);

	inline Vector3 operator+ (const Vector3& vec) const;
	inline Vector3 operator- (const Vector3& vec) const;
	inline Vector3 operator* (Real scalar) const;
	inline Vector3 operator/ (Real scalar) const;
	inline Vector3 operator- () const;

	inline Vector3& operator+= (const Vector3& vec);
	inline Vector3& operator-= (const Vector3& vec);
	inline Vector3& operator*= (Real scalar);
	inline Vector3& operator/= (Real scalar);

	inline Real& operator[] (size_t index);
	inline Real operator[] (size_t index) const;

	inline Real Length () const;
	inline Real SquaredLength () const;
	inline Real Dot (const Vector3& vec) const;
	inline Real Normalize (const Real epsilon = Math<Real>::EPSILON);
	inline Real Distance (const Vector3& vec) const;

	// Compute the axis-aligned bounding box of the points.
	static void ComputeAABB (int numVectors, const Vector3* vectors,
		Vector3& vmin, Vector3& vmax);

	// The cross products are computed using the right-handed rule.  Be aware
	// that some graphics APIs use a left-handed rule.  If you have to compute
	// a cross product with these functions and send the result to the API
	// that expects left-handed, you will need to change sign on the vector
	// (replace each component value c by -c).
	Vector3 Cross (const Vector3& vec) const;
	Vector3 UnitCross (const Vector3& vec) const;
	Vector3 Normalized () const;
	Vector3 Perpendicular () const;
	Vector3 Reflect (const Vector3<Real>& normal) const;


	// Special vectors.
	static const Vector3 ZERO;		// (0,0,0)
	static const Vector3 UNIT_X;	// (1,0,0)
	static const Vector3 UNIT_Y;	// (0,1,0)
	static const Vector3 UNIT_Z;	// (0,0,1)
	static const Vector3 NEGATIVE_UNIT_X;	// (-1,0,0)
	static const Vector3 NEGATIVE_UNIT_Y;	// (0,-1,0)
	static const Vector3 NEGATIVE_UNIT_Z;	// (0,0,-1)
	static const Vector3 ONE;		// (1,1,1)

};

// Arithmetic operations.
template <typename Real>
inline Vector3<Real> operator* (Real scalar, const Vector3<Real>& vec);

// Debugging output.
template <typename Real>
std::ostream& operator<< (std::ostream& outStream, const Vector3<Real>& vec);

// implementations

template <typename Real>
Vector3<Real>::Vector3 ()
{
	// Uninitialized for performance in array construction.
}

template <typename Real>
Vector3<Real>::Vector3 (const Vector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

template <typename Real>
Vector3<Real>::Vector3 (Real x, Real y, Real z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template <typename Real>
Vector3<Real>& Vector3<Real>::operator= (const Vector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

template <typename Real>
inline Vector3<Real> Vector3<Real>::operator+ (const Vector3& vec) const
{
	return Vector3
	(
		x + vec.x,
		y + vec.y,
		z + vec.z
	);
}

template <typename Real>
inline Vector3<Real> Vector3<Real>::operator- (const Vector3& vec) const
{
	return Vector3
	(
		x - vec.x,
		y - vec.y,
		z - vec.z
	);
}

template <typename Real>
inline Vector3<Real> Vector3<Real>::operator* (Real scalar) const
{
	return Vector3
	(
		scalar * x,
		scalar * y,
		scalar * z
	);
}

template <typename Real>
inline Vector3<Real> Vector3<Real>::operator/ (Real scalar) const
{
	Vector3 result;

	if (scalar != (Real)0)
	{
		Real invScalar = ((Real)1)/scalar;
		result.x = invScalar * x;
		result.y = invScalar * y;
		result.z = invScalar * z;
	}
	else
	{
		result.x = Math<Real>::MAX_VALUE;
		result.y = Math<Real>::MAX_VALUE;
		result.z = Math<Real>::MAX_VALUE;
	}

	return result;
}

template <typename Real>
inline Vector3<Real> Vector3<Real>::operator- () const
{
	return Vector3
	(
		-x,
		-y,
		-z
	);
}

template <typename Real>
inline Vector3<Real>& Vector3<Real>::operator+= (const Vector3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

template <typename Real>
inline Vector3<Real>& Vector3<Real>::operator-= (const Vector3& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

template <typename Real>
inline Vector3<Real>& Vector3<Real>::operator*= (Real scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

template <typename Real>
inline Vector3<Real>& Vector3<Real>::operator/= (Real scalar)
{
	if (scalar != (Real)0)
	{
		Real invScalar = ((Real)1)/scalar;
		x *= invScalar;
		y *= invScalar;
		z *= invScalar;
	}
	else
	{
		x *= Math<Real>::MAX_VALUE;
		y *= Math<Real>::MAX_VALUE;
		z *= Math<Real>::MAX_VALUE;
	}

	return *this;
}

template <typename Real>
inline Real& Vector3<Real>::operator[] (size_t index)
{
	assertion(index >= 0 && index < 3, "Vector3 index out of range\n");
	return v[index];
}

template <typename Real>
inline Real Vector3<Real>::operator[] (size_t index) const
{
	assertion(index >= 0 && index < 3, "Vector3 index out of range.\n");
	return v[index];
}

template <typename Real>
inline Real Vector3<Real>::Length () const
{
	return Math<Real>::Sqrt
	(
		x * x +
		y * y +
		z * z
	);
}

template <typename Real>
inline Real Vector3<Real>::SquaredLength () const
{
	return
		x * x +
		y * y +
		z * z;
}

template <typename Real>
inline Real Vector3<Real>::Dot (const Vector3& vec) const
{
	return
		this->x*vec.x +
		this->y*vec.y +
		this->z*vec.z;
}

template <typename Real>
inline Real Vector3<Real>::Normalize (const Real epsilon)
{
	Real length = Length();

	if (length > epsilon)
	{
		Real invLength = ((Real)1)/length;
		this->x *= invLength;
		this->y *= invLength;
		this->z *= invLength;
	}
	else
	{
		length = (Real)0;
		this->x = (Real)0;
		this->y = (Real)0;
		this->z = (Real)0;
	}

	return length;
}

template <typename Real>
Vector3<Real> Vector3<Real>::Cross (const Vector3& vec) const
{
	return Vector3
	(
		y*vec.z - z*vec.y,
		z*vec.x - x*vec.z,
		x*vec.y - y*vec.x
	);
}

template <typename Real>
Vector3<Real> Vector3<Real>::UnitCross (const Vector3& vec) const
{
	Vector3 cross
	(
		y*vec.z - z*vec.y,
		z*vec.x - x*vec.z,
		x*vec.y - y*vec.x
	);
	cross.Normalize();
	return cross;
}

template <typename Real>
Vector3<Real> Vector3<Real>::Normalized () const
{
	Vector3 vec(x,y,z);
	vec.Normalize();
	return vec;
}

template <typename Real>
Vector3<Real> Vector3<Real>::Perpendicular () const
{
	static const Real squareZero = (Real)(1e-06 * 1e-06);

	Vector3 perp = this->Cross(Vector3::UNIT_X );

	// Check length
	if( perp.SquaredLength() < squareZero )
	{
		// This vector is the Y axis multiplied by a scalar, so we have
		// to use another axis.
		perp = this->Cross(Vector3::UNIT_Y );
	}
	perp.normalize();

	return perp;
}

template <typename Real>
Vector3<Real> Vector3<Real>::Reflect (const Vector3<Real>& normal) const
{
	return Vector3( *this - ( 2 * (this->Dot(normal)) * normal ) );
}

template <typename Real>
void Vector3<Real>::ComputeAABB (int numVectors, const Vector3* vectors,
	Vector3& vmin, Vector3& vmax)
{
	assertion(numVectors > 0 && vectors,
		"Invalid inputs to ComputeExtremes\n");

	vmin = vectors[0];
	vmax = vmin;
	for (int j = 1; j < numVectors; ++j)
	{
		const Vector3& vec = vectors[j];
		for (int i = 0; i < 3; ++i)
		{
			if (vec[i] < vmin[i])
			{
				vmin[i] = vec[i];
			}
			else if (vec[i] > vmax[i])
			{
				vmax[i] = vec[i];
			}
		}
	}
}

template <typename Real>
inline Vector3<Real> operator* (Real scalar, const Vector3<Real>& vec)
{
	return Vector3<Real>
	(
		scalar * vec.x,
		scalar * vec.y,
		scalar * vec.z
	);
}

template <typename Real>
std::ostream& operator<< (std::ostream& outStream, const Vector3<Real>& vec)
{
	 return outStream << vec.x << ' ' << vec.y << ' ' << vec.z;
}

typedef Vector3<float> Vector3f;

#endif