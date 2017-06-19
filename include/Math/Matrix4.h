#ifndef Z_MATRIX4_H_
#define Z_MATRIX4_H_

#include "Core.h"
#include "Math/Math.h"
#include "Math/Vector3.h"

template <typename Real>
class Matrix4
{
public:
	union
	{
		struct
		{
			Real m00, m01, m02, m03;
			Real m10, m11, m12, m13;
			Real m20, m21, m22, m23;
			Real m30, m31, m32, m33;
		};
		Real m[4][4];
		Real _m[16];
	};

	// Default
	Matrix4 ();

	// Copy constructor.
	Matrix4 (const Matrix4& mat);

	// Input mrc is in row r, column c.
	Matrix4 (
		Real m00, Real m01, Real m02, Real m03,
		Real m10, Real m11, Real m12, Real m13,
		Real m20, Real m21, Real m22, Real m23,
		Real m30, Real m31, Real m32, Real m33);

	// Create a matrix from an array of numbers.  The input array is
	// interpreted based on the bool input as
	//   true:  mat[0..15]={m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,
	//						m23,m30,m31,m32,m33} [row major]
	//   false: mat[0..15]={m00,m10,m20,m30,m01,m11,m21,m31,m02,m12,m22,
	//						m32,m03,m13,m23,m33} [col major]
	Matrix4 (const Real mat[16], bool rowMajor);

	// Assignment.
	Matrix4& operator= (const Matrix4& mat);

	// Arithmetic operations.
	inline Matrix4 operator+ (const Matrix4& mat) const;
	inline Matrix4 operator- (const Matrix4& mat) const;
	inline Matrix4 operator* (Real scalar) const;
	inline Matrix4 operator/ (Real scalar) const;
	inline Matrix4 operator- () const;

	// Arithmetic updates.
	inline Matrix4& operator+= (const Matrix4& mat);
	inline Matrix4& operator-= (const Matrix4& mat);
	inline Matrix4& operator*= (Real scalar);
	inline Matrix4& operator/= (Real scalar);

	inline Real* operator[] (size_t index);
	inline const Real* operator[] (size_t index) const;

	// M^T
	Matrix4 Transpose () const;

	// M*mat
	Matrix4 operator* (const Matrix4& mat) const;

	// M^T*mat
	Matrix4 TransposeTimes (const Matrix4& mat) const;

	// M*mat^T
	Matrix4 TimesTranspose (const Matrix4& mat) const;

	// M^T*mat^T
	Matrix4 TransposeTimesTranspose (const Matrix4& mat) const;

	// Other operations.
	Matrix4 Inverse (const Real epsilon = Math<Real>::EPSILON) const;
	Matrix4 Adjoint () const;
	Real Determinant () const;

	static Matrix4 CreatePerspective (
		Real const & fovy, Real const & aspect,
		Real const & zNear, Real const & zFar);

	static Matrix4 CreateLookAt (const Vector3<Real>& eye,
		const Vector3<Real>& center, const Vector3<Real>& up);

	static Matrix4 CreateOrthographic (
		Real const & left, Real const & right, Real const & top, Real const & bottom,
		Real const & near, Real const & far);

	// Special matrices.
	static const Matrix4 ZERO;
	static const Matrix4 IDENTITY;
	static const Matrix4 ZEROAFFINE;
};

// c * M
template <typename Real>
inline Matrix4<Real> operator* (Real scalar, const Matrix4<Real>& mat);

template <typename Real>
std::ostream& operator<< (std::ostream& outStream, const Matrix4<Real>& mat);

// implementations

template <typename Real>
Matrix4<Real>::Matrix4 ()
{
}

template <typename Real>
Matrix4<Real>::Matrix4 (const Matrix4& mat)
{
	memcpy(_m,mat._m,sizeof(_m));
}

template <typename Real>
Matrix4<Real>::Matrix4 (Real m00, Real m01, Real m02, Real m03, Real m10,
	Real m11, Real m12, Real m13, Real m20, Real m21, Real m22, Real m23,
	Real m30, Real m31, Real m32, Real m33)
{
	_m[ 0] = m00;
	_m[ 1] = m01;
	_m[ 2] = m02;
	_m[ 3] = m03;
	_m[ 4] = m10;
	_m[ 5] = m11;
	_m[ 6] = m12;
	_m[ 7] = m13;
	_m[ 8] = m20;
	_m[ 9] = m21;
	_m[10] = m22;
	_m[11] = m23;
	_m[12] = m30;
	_m[13] = m31;
	_m[14] = m32;
	_m[15] = m33;
}

template <typename Real>
Matrix4<Real>::Matrix4 (const Real mat[16], bool rowMajor)
{
	if (rowMajor)
	{
		memcpy(_m,mat,sizeof(_m));
	}
	else
	{
		_m[ 0] = mat[ 0];
		_m[ 1] = mat[ 4];
		_m[ 2] = mat[ 8];
		_m[ 3] = mat[12];
		_m[ 4] = mat[ 1];
		_m[ 5] = mat[ 5];
		_m[ 6] = mat[ 9];
		_m[ 7] = mat[13];
		_m[ 8] = mat[ 2];
		_m[ 9] = mat[ 6];
		_m[10] = mat[10];
		_m[11] = mat[14];
		_m[12] = mat[ 3];
		_m[13] = mat[ 7];
		_m[14] = mat[11];
		_m[15] = mat[15];
	}
}

template <typename Real>
Matrix4<Real>& Matrix4<Real>::operator= (const Matrix4& mat)
{
	memcpy(_m,mat._m,sizeof(_m));
	return *this;
}

template <class Real>
inline Matrix4<Real> Matrix4<Real>::operator+ (const Matrix4& mat) const
{
	return Matrix4<Real>
	(
		_m[ 0] + mat._m[ 0],
		_m[ 1] + mat._m[ 1],
		_m[ 2] + mat._m[ 2],
		_m[ 3] + mat._m[ 3],
		_m[ 4] + mat._m[ 4],
		_m[ 5] + mat._m[ 5],
		_m[ 6] + mat._m[ 6],
		_m[ 7] + mat._m[ 7],
		_m[ 8] + mat._m[ 8],
		_m[ 9] + mat._m[ 9],
		_m[10] + mat._m[10],
		_m[11] + mat._m[11],
		_m[12] + mat._m[12],
		_m[13] + mat._m[13],
		_m[14] + mat._m[14],
		_m[15] + mat._m[15]
	);
}

template <class Real>
inline Matrix4<Real> Matrix4<Real>::operator- (const Matrix4& mat) const
{
	return Matrix4<Real>
	(
		_m[ 0] - mat._m[ 0],
		_m[ 1] - mat._m[ 1],
		_m[ 2] - mat._m[ 2],
		_m[ 3] - mat._m[ 3],
		_m[ 4] - mat._m[ 4],
		_m[ 5] - mat._m[ 5],
		_m[ 6] - mat._m[ 6],
		_m[ 7] - mat._m[ 7],
		_m[ 8] - mat._m[ 8],
		_m[ 9] - mat._m[ 9],
		_m[10] - mat._m[10],
		_m[11] - mat._m[11],
		_m[12] - mat._m[12],
		_m[13] - mat._m[13],
		_m[14] - mat._m[14],
		_m[15] - mat._m[15]
	);
}

template <class Real>
inline Matrix4<Real> Matrix4<Real>::operator* (Real scalar) const
{
	return Matrix4<Real>
	(
		scalar*_m[ 0],
		scalar*_m[ 1],
		scalar*_m[ 2],
		scalar*_m[ 3],
		scalar*_m[ 4],
		scalar*_m[ 5],
		scalar*_m[ 6],
		scalar*_m[ 7],
		scalar*_m[ 8],
		scalar*_m[ 9],
		scalar*_m[10],
		scalar*_m[11],
		scalar*_m[12],
		scalar*_m[13],
		scalar*_m[14],
		scalar*_m[15]
	);
}

template <class Real>
inline Matrix4<Real> Matrix4<Real>::operator/ (Real scalar) const
{
	if (scalar != (Real)0)
	{
		Real invScalar = ((Real)1)/scalar;
		return Matrix4<Real>
		(
			invScalar*_m[ 0],
			invScalar*_m[ 1],
			invScalar*_m[ 2],
			invScalar*_m[ 3],
			invScalar*_m[ 4],
			invScalar*_m[ 5],
			invScalar*_m[ 6],
			invScalar*_m[ 7],
			invScalar*_m[ 8],
			invScalar*_m[ 9],
			invScalar*_m[10],
			invScalar*_m[11],
			invScalar*_m[12],
			invScalar*_m[13],
			invScalar*_m[14],
			invScalar*_m[15]
		);
	}
	else
	{
		return Matrix4<Real>
		(
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE,
			Math<Real>::MAX_VALUE
		);
	}
}

template <class Real>
inline Matrix4<Real> Matrix4<Real>::operator- () const
{
	return Matrix4<Real>
	(
		-_m[ 0],
		-_m[ 1],
		-_m[ 2],
		-_m[ 3],
		-_m[ 4],
		-_m[ 5],
		-_m[ 6],
		-_m[ 7],
		-_m[ 8],
		-_m[ 9],
		-_m[10],
		-_m[11],
		-_m[12],
		-_m[13],
		-_m[14],
		-_m[15]
	);
}

template <class Real>
inline Matrix4<Real>& Matrix4<Real>::operator+= (const Matrix4& mat)
{
	_m[ 0] += mat._m[ 0];
	_m[ 1] += mat._m[ 1];
	_m[ 2] += mat._m[ 2];
	_m[ 3] += mat._m[ 3];
	_m[ 4] += mat._m[ 4];
	_m[ 5] += mat._m[ 5];
	_m[ 6] += mat._m[ 6];
	_m[ 7] += mat._m[ 7];
	_m[ 8] += mat._m[ 8];
	_m[ 9] += mat._m[ 9];
	_m[10] += mat._m[10];
	_m[11] += mat._m[11];
	_m[12] += mat._m[12];
	_m[13] += mat._m[13];
	_m[14] += mat._m[14];
	_m[15] += mat._m[15];
	return *this;
}

template <class Real>
inline Matrix4<Real>& Matrix4<Real>::operator-= (const Matrix4& mat)
{
	_m[ 0] -= mat._m[ 0];
	_m[ 1] -= mat._m[ 1];
	_m[ 2] -= mat._m[ 2];
	_m[ 3] -= mat._m[ 3];
	_m[ 4] -= mat._m[ 4];
	_m[ 5] -= mat._m[ 5];
	_m[ 6] -= mat._m[ 6];
	_m[ 7] -= mat._m[ 7];
	_m[ 8] -= mat._m[ 8];
	_m[ 9] -= mat._m[ 9];
	_m[10] -= mat._m[10];
	_m[11] -= mat._m[11];
	_m[12] -= mat._m[12];
	_m[13] -= mat._m[13];
	_m[14] -= mat._m[14];
	_m[15] -= mat._m[15];
	return *this;
}

template <class Real>
inline Matrix4<Real>& Matrix4<Real>::operator*= (Real scalar)
{
	_m[ 0] *= scalar;
	_m[ 1] *= scalar;
	_m[ 2] *= scalar;
	_m[ 3] *= scalar;
	_m[ 4] *= scalar;
	_m[ 5] *= scalar;
	_m[ 6] *= scalar;
	_m[ 7] *= scalar;
	_m[ 8] *= scalar;
	_m[ 9] *= scalar;
	_m[10] *= scalar;
	_m[11] *= scalar;
	_m[12] *= scalar;
	_m[13] *= scalar;
	_m[14] *= scalar;
	_m[15] *= scalar;
	return *this;
}

template <class Real>
inline Matrix4<Real>& Matrix4<Real>::operator/= (Real scalar)
{
	if (scalar != (Real)0)
	{
		Real invScalar = ((Real)1)/scalar;
		_m[ 0] *= invScalar;
		_m[ 1] *= invScalar;
		_m[ 2] *= invScalar;
		_m[ 3] *= invScalar;
		_m[ 4] *= invScalar;
		_m[ 5] *= invScalar;
		_m[ 6] *= invScalar;
		_m[ 7] *= invScalar;
		_m[ 8] *= invScalar;
		_m[ 9] *= invScalar;
		_m[10] *= invScalar;
		_m[11] *= invScalar;
		_m[12] *= invScalar;
		_m[13] *= invScalar;
		_m[14] *= invScalar;
		_m[15] *= invScalar;
	}
	else
	{
		_m[ 0] = Math<Real>::MAX_VALUE;
		_m[ 1] = Math<Real>::MAX_VALUE;
		_m[ 2] = Math<Real>::MAX_VALUE;
		_m[ 3] = Math<Real>::MAX_VALUE;
		_m[ 4] = Math<Real>::MAX_VALUE;
		_m[ 5] = Math<Real>::MAX_VALUE;
		_m[ 6] = Math<Real>::MAX_VALUE;
		_m[ 7] = Math<Real>::MAX_VALUE;
		_m[ 8] = Math<Real>::MAX_VALUE;
		_m[ 9] = Math<Real>::MAX_VALUE;
		_m[10] = Math<Real>::MAX_VALUE;
		_m[11] = Math<Real>::MAX_VALUE;
		_m[12] = Math<Real>::MAX_VALUE;
		_m[13] = Math<Real>::MAX_VALUE;
		_m[14] = Math<Real>::MAX_VALUE;
		_m[15] = Math<Real>::MAX_VALUE;
	}

	return *this;
}

template <class Real>
Real* Matrix4<Real>::operator[] (size_t index)
{
	assertion(index >= 0 && index < 4,"Matrix index out of range\n");
	return m[index];
}

template <class Real>
const Real* Matrix4<Real>::operator[] (size_t index) const
{
	assertion(index >= 0 && index < 4,"Matrix index out of range\n");
	return m[index];
}

template <class Real>
Matrix4<Real> Matrix4<Real>::Transpose () const
{
	return Matrix4<Real>
	(
		_m[ 0],
		_m[ 4],
		_m[ 8],
		_m[12],
		_m[ 1],
		_m[ 5],
		_m[ 9],
		_m[13],
		_m[ 2],
		_m[ 6],
		_m[10],
		_m[14],
		_m[ 3],
		_m[ 7],
		_m[11],
		_m[15]
	);
}

template <class Real>
inline Matrix4<Real> Matrix4<Real>::operator* (const Matrix4& mat) const
{
	// A*B
	return Matrix4<Real>
	(
		_m[ 0]*mat._m[ 0] +
		_m[ 1]*mat._m[ 4] +
		_m[ 2]*mat._m[ 8] +
		_m[ 3]*mat._m[12],

		_m[ 0]*mat._m[ 1] +
		_m[ 1]*mat._m[ 5] +
		_m[ 2]*mat._m[ 9] +
		_m[ 3]*mat._m[13],

		_m[ 0]*mat._m[ 2] +
		_m[ 1]*mat._m[ 6] +
		_m[ 2]*mat._m[10] +
		_m[ 3]*mat._m[14],

		_m[ 0]*mat._m[ 3] +
		_m[ 1]*mat._m[ 7] +
		_m[ 2]*mat._m[11] +
		_m[ 3]*mat._m[15],

		_m[ 4]*mat._m[ 0] +
		_m[ 5]*mat._m[ 4] +
		_m[ 6]*mat._m[ 8] +
		_m[ 7]*mat._m[12],

		_m[ 4]*mat._m[ 1] +
		_m[ 5]*mat._m[ 5] +
		_m[ 6]*mat._m[ 9] +
		_m[ 7]*mat._m[13],

		_m[ 4]*mat._m[ 2] +
		_m[ 5]*mat._m[ 6] +
		_m[ 6]*mat._m[10] +
		_m[ 7]*mat._m[14],

		_m[ 4]*mat._m[ 3] +
		_m[ 5]*mat._m[ 7] +
		_m[ 6]*mat._m[11] +
		_m[ 7]*mat._m[15],

		_m[ 8]*mat._m[ 0] +
		_m[ 9]*mat._m[ 4] +
		_m[10]*mat._m[ 8] +
		_m[11]*mat._m[12],

		_m[ 8]*mat._m[ 1] +
		_m[ 9]*mat._m[ 5] +
		_m[10]*mat._m[ 9] +
		_m[11]*mat._m[13],

		_m[ 8]*mat._m[ 2] +
		_m[ 9]*mat._m[ 6] +
		_m[10]*mat._m[10] +
		_m[11]*mat._m[14],

		_m[ 8]*mat._m[ 3] +
		_m[ 9]*mat._m[ 7] +
		_m[10]*mat._m[11] +
		_m[11]*mat._m[15],

		_m[12]*mat._m[ 0] +
		_m[13]*mat._m[ 4] +
		_m[14]*mat._m[ 8] +
		_m[15]*mat._m[12],

		_m[12]*mat._m[ 1] +
		_m[13]*mat._m[ 5] +
		_m[14]*mat._m[ 9] +
		_m[15]*mat._m[13],

		_m[12]*mat._m[ 2] +
		_m[13]*mat._m[ 6] +
		_m[14]*mat._m[10] +
		_m[15]*mat._m[14],

		_m[12]*mat._m[ 3] +
		_m[13]*mat._m[ 7] +
		_m[14]*mat._m[11] +
		_m[15]*mat._m[15]
	);
}

template <class Real>
Matrix4<Real> Matrix4<Real>::TransposeTimes (const Matrix4& mat) const
{
	// A^T*B
	return Matrix4<Real>
	(
		_m[ 0]*mat._m[ 0] +
		_m[ 4]*mat._m[ 4] +
		_m[ 8]*mat._m[ 8] +
		_m[12]*mat._m[12],

		_m[ 0]*mat._m[ 1] +
		_m[ 4]*mat._m[ 5] +
		_m[ 8]*mat._m[ 9] +
		_m[12]*mat._m[13],

		_m[ 0]*mat._m[ 2] +
		_m[ 4]*mat._m[ 6] +
		_m[ 8]*mat._m[10] +
		_m[12]*mat._m[14],

		_m[ 0]*mat._m[ 3] +
		_m[ 4]*mat._m[ 7] +
		_m[ 8]*mat._m[11] +
		_m[12]*mat._m[15],

		_m[ 1]*mat._m[ 0] +
		_m[ 5]*mat._m[ 4] +
		_m[ 9]*mat._m[ 8] +
		_m[13]*mat._m[12],

		_m[ 1]*mat._m[ 1] +
		_m[ 5]*mat._m[ 5] +
		_m[ 9]*mat._m[ 9] +
		_m[13]*mat._m[13],

		_m[ 1]*mat._m[ 2] +
		_m[ 5]*mat._m[ 6] +
		_m[ 9]*mat._m[10] +
		_m[13]*mat._m[14],

		_m[ 1]*mat._m[ 3] +
		_m[ 5]*mat._m[ 7] +
		_m[ 9]*mat._m[11] +
		_m[13]*mat._m[15],

		_m[ 2]*mat._m[ 0] +
		_m[ 6]*mat._m[ 4] +
		_m[10]*mat._m[ 8] +
		_m[14]*mat._m[12],

		_m[ 2]*mat._m[ 1] +
		_m[ 6]*mat._m[ 5] +
		_m[10]*mat._m[ 9] +
		_m[14]*mat._m[13],

		_m[ 2]*mat._m[ 2] +
		_m[ 6]*mat._m[ 6] +
		_m[10]*mat._m[10] +
		_m[14]*mat._m[14],

		_m[ 2]*mat._m[ 3] +
		_m[ 6]*mat._m[ 7] +
		_m[10]*mat._m[11] +
		_m[14]*mat._m[15],

		_m[ 3]*mat._m[ 0] +
		_m[ 7]*mat._m[ 4] +
		_m[11]*mat._m[ 8] +
		_m[15]*mat._m[12],

		_m[ 3]*mat._m[ 1] +
		_m[ 7]*mat._m[ 5] +
		_m[11]*mat._m[ 9] +
		_m[15]*mat._m[13],

		_m[ 3]*mat._m[ 2] +
		_m[ 7]*mat._m[ 6] +
		_m[11]*mat._m[10] +
		_m[15]*mat._m[14],

		_m[ 3]*mat._m[ 3] +
		_m[ 7]*mat._m[ 7] +
		_m[11]*mat._m[11] +
		_m[15]*mat._m[15]
	);
}

template <class Real>
Matrix4<Real> Matrix4<Real>::TimesTranspose (const Matrix4& mat) const
{
	// A*B^T
	return Matrix4<Real>
	(
		_m[ 0]*mat._m[ 0] +
		_m[ 1]*mat._m[ 1] +
		_m[ 2]*mat._m[ 2] +
		_m[ 3]*mat._m[ 3],

		_m[ 0]*mat._m[ 4] +
		_m[ 1]*mat._m[ 5] +
		_m[ 2]*mat._m[ 6] +
		_m[ 3]*mat._m[ 7],

		_m[ 0]*mat._m[ 8] +
		_m[ 1]*mat._m[ 9] +
		_m[ 2]*mat._m[10] +
		_m[ 3]*mat._m[11],

		_m[ 0]*mat._m[12] +
		_m[ 1]*mat._m[13] +
		_m[ 2]*mat._m[14] +
		_m[ 3]*mat._m[15],

		_m[ 4]*mat._m[ 0] +
		_m[ 5]*mat._m[ 1] +
		_m[ 6]*mat._m[ 2] +
		_m[ 7]*mat._m[ 3],

		_m[ 4]*mat._m[ 4] +
		_m[ 5]*mat._m[ 5] +
		_m[ 6]*mat._m[ 6] +
		_m[ 7]*mat._m[ 7],

		_m[ 4]*mat._m[ 8] +
		_m[ 5]*mat._m[ 9] +
		_m[ 6]*mat._m[10] +
		_m[ 7]*mat._m[11],

		_m[ 4]*mat._m[12] +
		_m[ 5]*mat._m[13] +
		_m[ 6]*mat._m[14] +
		_m[ 7]*mat._m[15],

		_m[ 8]*mat._m[ 0] +
		_m[ 9]*mat._m[ 1] +
		_m[10]*mat._m[ 2] +
		_m[11]*mat._m[ 3],

		_m[ 8]*mat._m[ 4] +
		_m[ 9]*mat._m[ 5] +
		_m[10]*mat._m[ 6] +
		_m[11]*mat._m[ 7],

		_m[ 8]*mat._m[ 8] +
		_m[ 9]*mat._m[ 9] +
		_m[10]*mat._m[10] +
		_m[11]*mat._m[11],

		_m[ 8]*mat._m[12] +
		_m[ 9]*mat._m[13] +
		_m[10]*mat._m[14] +
		_m[11]*mat._m[15],

		_m[12]*mat._m[ 0] +
		_m[13]*mat._m[ 1] +
		_m[14]*mat._m[ 2] +
		_m[15]*mat._m[ 3],

		_m[12]*mat._m[ 4] +
		_m[13]*mat._m[ 5] +
		_m[14]*mat._m[ 6] +
		_m[15]*mat._m[ 7],

		_m[12]*mat._m[ 8] +
		_m[13]*mat._m[ 9] +
		_m[14]*mat._m[10] +
		_m[15]*mat._m[11],

		_m[12]*mat._m[12] +
		_m[13]*mat._m[13] +
		_m[14]*mat._m[14] +
		_m[15]*mat._m[15]
	);
}

template <class Real>
Matrix4<Real> Matrix4<Real>::TransposeTimesTranspose (const Matrix4& mat)
	const
{
	// A^T*B^T
	return Matrix4<Real>
	(
		_m[ 0]*mat._m[ 0] +
		_m[ 4]*mat._m[ 1] +
		_m[ 8]*mat._m[ 2] +
		_m[12]*mat._m[ 3],

		_m[ 0]*mat._m[ 4] +
		_m[ 4]*mat._m[ 5] +
		_m[ 8]*mat._m[ 6] +
		_m[12]*mat._m[ 7],

		_m[ 0]*mat._m[ 8] +
		_m[ 4]*mat._m[ 9] +
		_m[ 8]*mat._m[10] +
		_m[12]*mat._m[11],

		_m[ 0]*mat._m[12] +
		_m[ 4]*mat._m[13] +
		_m[ 8]*mat._m[14] +
		_m[12]*mat._m[15],

		_m[ 1]*mat._m[ 0] +
		_m[ 5]*mat._m[ 1] +
		_m[ 9]*mat._m[ 2] +
		_m[13]*mat._m[ 3],

		_m[ 1]*mat._m[ 4] +
		_m[ 5]*mat._m[ 5] +
		_m[ 9]*mat._m[ 6] +
		_m[13]*mat._m[ 7],

		_m[ 1]*mat._m[ 8] +
		_m[ 5]*mat._m[ 9] +
		_m[ 9]*mat._m[10] +
		_m[13]*mat._m[11],

		_m[ 1]*mat._m[12] +
		_m[ 5]*mat._m[13] +
		_m[ 9]*mat._m[14] +
		_m[13]*mat._m[15],

		_m[ 2]*mat._m[ 0] +
		_m[ 6]*mat._m[ 1] +
		_m[10]*mat._m[ 2] +
		_m[14]*mat._m[ 3],

		_m[ 2]*mat._m[ 4] +
		_m[ 6]*mat._m[ 5] +
		_m[10]*mat._m[ 6] +
		_m[14]*mat._m[ 7],

		_m[ 2]*mat._m[ 8] +
		_m[ 6]*mat._m[ 9] +
		_m[10]*mat._m[10] +
		_m[14]*mat._m[11],

		_m[ 2]*mat._m[12] +
		_m[ 6]*mat._m[13] +
		_m[10]*mat._m[14] +
		_m[14]*mat._m[15],

		_m[ 3]*mat._m[ 0] +
		_m[ 7]*mat._m[ 1] +
		_m[11]*mat._m[ 2] +
		_m[15]*mat._m[ 3],

		_m[ 3]*mat._m[ 4] +
		_m[ 7]*mat._m[ 5] +
		_m[11]*mat._m[ 6] +
		_m[15]*mat._m[ 7],

		_m[ 3]*mat._m[ 8] +
		_m[ 7]*mat._m[ 9] +
		_m[11]*mat._m[10] +
		_m[15]*mat._m[11],

		_m[ 3]*mat._m[12] +
		_m[ 7]*mat._m[13] +
		_m[11]*mat._m[14] +
		_m[15]*mat._m[15]
	);
}

template <typename Real>
Matrix4<Real> Matrix4<Real>::Inverse (const Real epsilon) const
{
	Real a0 = _m[ 0]*_m[ 5] - _m[ 1]*_m[ 4];
	Real a1 = _m[ 0]*_m[ 6] - _m[ 2]*_m[ 4];
	Real a2 = _m[ 0]*_m[ 7] - _m[ 3]*_m[ 4];
	Real a3 = _m[ 1]*_m[ 6] - _m[ 2]*_m[ 5];
	Real a4 = _m[ 1]*_m[ 7] - _m[ 3]*_m[ 5];
	Real a5 = _m[ 2]*_m[ 7] - _m[ 3]*_m[ 6];
	Real b0 = _m[ 8]*_m[13] - _m[ 9]*_m[12];
	Real b1 = _m[ 8]*_m[14] - _m[10]*_m[12];
	Real b2 = _m[ 8]*_m[15] - _m[11]*_m[12];
	Real b3 = _m[ 9]*_m[14] - _m[10]*_m[13];
	Real b4 = _m[ 9]*_m[15] - _m[11]*_m[13];
	Real b5 = _m[10]*_m[15] - _m[11]*_m[14];

	Real det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
	if (Math<Real>::FAbs(det) > epsilon)
	{
		Matrix4 inverse;
		inverse._m[ 0] = + _m[ 5]*b5 - _m[ 6]*b4 + _m[ 7]*b3;
		inverse._m[ 4] = - _m[ 4]*b5 + _m[ 6]*b2 - _m[ 7]*b1;
		inverse._m[ 8] = + _m[ 4]*b4 - _m[ 5]*b2 + _m[ 7]*b0;
		inverse._m[12] = - _m[ 4]*b3 + _m[ 5]*b1 - _m[ 6]*b0;
		inverse._m[ 1] = - _m[ 1]*b5 + _m[ 2]*b4 - _m[ 3]*b3;
		inverse._m[ 5] = + _m[ 0]*b5 - _m[ 2]*b2 + _m[ 3]*b1;
		inverse._m[ 9] = - _m[ 0]*b4 + _m[ 1]*b2 - _m[ 3]*b0;
		inverse._m[13] = + _m[ 0]*b3 - _m[ 1]*b1 + _m[ 2]*b0;
		inverse._m[ 2] = + _m[13]*a5 - _m[14]*a4 + _m[15]*a3;
		inverse._m[ 6] = - _m[12]*a5 + _m[14]*a2 - _m[15]*a1;
		inverse._m[10] = + _m[12]*a4 - _m[13]*a2 + _m[15]*a0;
		inverse._m[14] = - _m[12]*a3 + _m[13]*a1 - _m[14]*a0;
		inverse._m[ 3] = - _m[ 9]*a5 + _m[10]*a4 - _m[11]*a3;
		inverse._m[ 7] = + _m[ 8]*a5 - _m[10]*a2 + _m[11]*a1;
		inverse._m[11] = - _m[ 8]*a4 + _m[ 9]*a2 - _m[11]*a0;
		inverse._m[15] = + _m[ 8]*a3 - _m[ 9]*a1 + _m[10]*a0;

		Real invDet = ((Real)1)/det;
		inverse._m[ 0] *= invDet;
		inverse._m[ 1] *= invDet;
		inverse._m[ 2] *= invDet;
		inverse._m[ 3] *= invDet;
		inverse._m[ 4] *= invDet;
		inverse._m[ 5] *= invDet;
		inverse._m[ 6] *= invDet;
		inverse._m[ 7] *= invDet;
		inverse._m[ 8] *= invDet;
		inverse._m[ 9] *= invDet;
		inverse._m[10] *= invDet;
		inverse._m[11] *= invDet;
		inverse._m[12] *= invDet;
		inverse._m[13] *= invDet;
		inverse._m[14] *= invDet;
		inverse._m[15] *= invDet;

		return inverse;
	}

	return ZERO;
}

template <typename Real>
Matrix4<Real> Matrix4<Real>::Adjoint () const
{
	Real a0 = _m[ 0]*_m[ 5] - _m[ 1]*_m[ 4];
	Real a1 = _m[ 0]*_m[ 6] - _m[ 2]*_m[ 4];
	Real a2 = _m[ 0]*_m[ 7] - _m[ 3]*_m[ 4];
	Real a3 = _m[ 1]*_m[ 6] - _m[ 2]*_m[ 5];
	Real a4 = _m[ 1]*_m[ 7] - _m[ 3]*_m[ 5];
	Real a5 = _m[ 2]*_m[ 7] - _m[ 3]*_m[ 6];
	Real b0 = _m[ 8]*_m[13] - _m[ 9]*_m[12];
	Real b1 = _m[ 8]*_m[14] - _m[10]*_m[12];
	Real b2 = _m[ 8]*_m[15] - _m[11]*_m[12];
	Real b3 = _m[ 9]*_m[14] - _m[10]*_m[13];
	Real b4 = _m[ 9]*_m[15] - _m[11]*_m[13];
	Real b5 = _m[10]*_m[15] - _m[11]*_m[14];

	return Matrix4<Real>
	(
		+ _m[ 5]*b5 - _m[ 6]*b4 + _m[ 7]*b3,
		- _m[ 1]*b5 + _m[ 2]*b4 - _m[ 3]*b3,
		+ _m[13]*a5 - _m[14]*a4 + _m[15]*a3,
		- _m[ 9]*a5 + _m[10]*a4 - _m[11]*a3,
		- _m[ 4]*b5 + _m[ 6]*b2 - _m[ 7]*b1,
		+ _m[ 0]*b5 - _m[ 2]*b2 + _m[ 3]*b1,
		- _m[12]*a5 + _m[14]*a2 - _m[15]*a1,
		+ _m[ 8]*a5 - _m[10]*a2 + _m[11]*a1,
		+ _m[ 4]*b4 - _m[ 5]*b2 + _m[ 7]*b0,
		- _m[ 0]*b4 + _m[ 1]*b2 - _m[ 3]*b0,
		+ _m[12]*a4 - _m[13]*a2 + _m[15]*a0,
		- _m[ 8]*a4 + _m[ 9]*a2 - _m[11]*a0,
		- _m[ 4]*b3 + _m[ 5]*b1 - _m[ 6]*b0,
		+ _m[ 0]*b3 - _m[ 1]*b1 + _m[ 2]*b0,
		- _m[12]*a3 + _m[13]*a1 - _m[14]*a0,
		+ _m[ 8]*a3 - _m[ 9]*a1 + _m[10]*a0
	);
}

template <typename Real>
Real Matrix4<Real>::Determinant () const
{
	Real a0 = _m[ 0]*_m[ 5] - _m[ 1]*_m[ 4];
	Real a1 = _m[ 0]*_m[ 6] - _m[ 2]*_m[ 4];
	Real a2 = _m[ 0]*_m[ 7] - _m[ 3]*_m[ 4];
	Real a3 = _m[ 1]*_m[ 6] - _m[ 2]*_m[ 5];
	Real a4 = _m[ 1]*_m[ 7] - _m[ 3]*_m[ 5];
	Real a5 = _m[ 2]*_m[ 7] - _m[ 3]*_m[ 6];
	Real b0 = _m[ 8]*_m[13] - _m[ 9]*_m[12];
	Real b1 = _m[ 8]*_m[14] - _m[10]*_m[12];
	Real b2 = _m[ 8]*_m[15] - _m[11]*_m[12];
	Real b3 = _m[ 9]*_m[14] - _m[10]*_m[13];
	Real b4 = _m[ 9]*_m[15] - _m[11]*_m[13];
	Real b5 = _m[10]*_m[15] - _m[11]*_m[14];
	Real det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
	return det;
}

template <typename Real>
Matrix4<Real> Matrix4<Real>::CreatePerspective (
	Real const & fovy, Real const & aspect,
	Real const & zNear, Real const & zFar)
{
	Real range = tan(Math<Real>::Radians(fovy / (Real)2)) * zNear;
	Real left = -range * aspect;
	Real right = range * aspect;
	Real bottom = -range;
	Real top = range;

	Matrix4<Real> mat = ZERO;
	mat[0][0] = ((Real)2 * zNear) / (right - left);
	mat[1][1] = ((Real)2 * zNear) / (top - bottom);
	mat[2][2] = - (zFar + zNear) / (zFar - zNear);
	mat[3][2] = - (Real)1;
	mat[2][3] = - ((Real)2 * zFar * zNear) / (zFar - zNear);
	return mat;
}

template <typename Real>
Matrix4<Real> Matrix4<Real>::CreateOrthographic (
	Real const & left, Real const & right, Real const & bottom, Real const & top,
	Real const & zNear, Real const & zFar)
{
	Matrix4<Real> mat = IDENTITY;
	mat[0][0] = Real(2) / (right - left);
	mat[1][1] = Real(2) / (top - bottom);
	mat[2][2] = - Real(2) / (zFar - zNear);
	mat[0][3] = - (right + left) / (right - left);
	mat[1][3] = - (top + bottom) / (top - bottom);
	mat[2][3] = - (zFar + zNear) / (zFar - zNear);
	return mat;
}

template <typename Real>
Matrix4<Real> Matrix4<Real>::CreateLookAt (const Vector3<Real>& eye,
	const Vector3<Real>& center, const Vector3<Real>& up)
{
	Vector3<Real> f = (center - eye).Normalized();
	Vector3<Real> u = up.Normalized();
	Vector3<Real> s = f.Cross(u).Normalized();
	u = s.Cross(f);

	Matrix4<Real> mat = IDENTITY;
	mat[0][0] = s.x;
	mat[0][1] = s.y;
	mat[0][2] = s.z;
	mat[1][0] = u.x;
	mat[1][1] = u.y;
	mat[1][2] = u.z;
	mat[2][0] =-f.x;
	mat[2][1] =-f.y;
	mat[2][2] =-f.z;
	mat[0][3] =-s.Dot(eye);
	mat[1][3] =-u.Dot(eye);
	mat[2][3] = f.Dot(eye);
	return mat;
}

template <typename Real>
inline Matrix4<Real> operator* (Real scalar, const Matrix4<Real>& mat)
{
	return mat*scalar;
}

template <typename Real>
std::ostream& operator<< (std::ostream& outStream, const Matrix4<Real>& mat)
{
	for (size_t i = 0; i < 4; ++i)
	{
		for(size_t j = 0; j < 4; ++j)
		{
			outStream << mat.m[i][j] << " ";
		}
		outStream << "\n";
	}
	return outStream;
}

typedef Matrix4<float> Matrix4f;

#endif

/*
	template <typename T>
	detail::tmat4x4<T> translate(
		detail::tmat4x4<T> const & m,
		detail::tvec3<T> const & v);

	/// Builds a rotation 4 * 4 matrix created from an axis vector and an angle.
	///
	/// @param m Input matrix multiplied by this rotation matrix.
	/// @param angle Rotation angle expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param axis Rotation axis, recommanded to be normalized.
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	/// @see gtx_transform
	/// @see - rotate(T angle, T x, T y, T z)
	/// @see - rotate(detail::tmat4x4<T> const & m, T angle, T x, T y, T z)
	/// @see - rotate(T angle, detail::tvec3<T> const & v)
	template <typename T>
	detail::tmat4x4<T> rotate(
		detail::tmat4x4<T> const & m,
		T const & angle,
		detail::tvec3<T> const & axis);

	/// Builds a scale 4 * 4 matrix created from 3 scalars.
	///
	/// @param m Input matrix multiplied by this scale matrix.
	/// @param v Ratio of scaling for each axis.
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	/// @see gtx_transform
	/// @see - scale(T x, T y, T z) scale(T const & x, T const & y, T const & z)
	/// @see - scale(detail::tmat4x4<T> const & m, T x, T y, T z)
	/// @see - scale(detail::tvec3<T> const & v)
	template <typename T>
	detail::tmat4x4<T> scale(
		detail::tmat4x4<T> const & m,
		detail::tvec3<T> const & v);

	/// Creates a matrix for an orthographic parallel viewing volume.
	///
	/// @param left
	/// @param right
	/// @param bottom
	/// @param top
	/// @param zNear
	/// @param zFar
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	/// @see - glm::ortho(T const & left, T const & right, T const & bottom, T const & top)
	template <typename T>
	detail::tmat4x4<T> ortho(
		T const & left,
		T const & right,
		T const & bottom,
		T const & top,
		T const & zNear,
		T const & zFar);

	/// Creates a matrix for projecting two-dimensional coordinates onto the screen.
	///
	/// @param left
	/// @param right
	/// @param bottom
	/// @param top
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
    /// @see - glm::ortho(T const & left, T const & right, T const & bottom, T const & top, T const & zNear, T const & zFar)
	template <typename T>
	detail::tmat4x4<T> ortho(
		T const & left,
		T const & right,
		T const & bottom,
		T const & top);

	/// Creates a frustum matrix.
	///
	/// @param left
	/// @param right
	/// @param bottom
	/// @param top
	/// @param near
	/// @param far
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	template <typename T>
	detail::tmat4x4<T> frustum(
		T const & left,
		T const & right,
		T const & bottom,
		T const & top,
		T const & near,
		T const & far);

	/// Creates a matrix for a symetric perspective-view frustum.
	///
	/// @param fovy Expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param aspect
	/// @param near
	/// @param far
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	template <typename T>
	detail::tmat4x4<T> perspective(
		T const & fovy,
		T const & aspect,
		T const & near,
		T const & far);

	/// Builds a perspective projection matrix based on a field of view.
	///
	/// @param fov Expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param width
	/// @param height
	/// @param near
	/// @param far
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
	template <typename Real>
	detail::tmat4x4<Real> perspectiveFov(
		Real const & fov,
		Real const & width,
		Real const & height,
		Real const & near,
		Real const & far);

	/// Creates a matrix for a symmetric perspective-view frustum with far plane at infinite.
	///
	/// @param fovy Expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param aspect
	/// @param near
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
    template <typename T>
	detail::tmat4x4<T> infinitePerspective(
		T fovy, T aspect, T near);

	/// Creates a matrix for a symmetric perspective-view frustum with far plane at infinite for graphics hardware that doesn't support depth clamping.
	///
	/// @param fovy Expressed in radians if GLM_FORCE_RADIANS is define or degrees otherwise.
	/// @param aspect
	/// @param near
	/// @tparam T Value type used to build the matrix. Currently supported: half (not recommanded), float or double.
	/// @see gtc_matrix_transform
    template <typename T>
	detail::tmat4x4<T> tweakedInfinitePerspective(
		T fovy, T aspect, T near);

	/// Map the specified object coordinates (obj.x, obj.y, obj.z) into window coordinates.
	///
	/// @param obj
	/// @param model
	/// @param proj
	/// @param viewport
	/// @tparam T Native type used for the computation. Currently supported: half (not recommanded), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	/// @see gtc_matrix_transform
	template <typename T, typename U>
	detail::tvec3<T> project(
		detail::tvec3<T> const & obj,
		detail::tmat4x4<T> const & model,
		detail::tmat4x4<T> const & proj,
		detail::tvec4<U> const & viewport);

	/// Map the specified window coordinates (win.x, win.y, win.z) into object coordinates.
	///
	/// @param win
	/// @param model
	/// @param proj
	/// @param viewport
	/// @tparam T Native type used for the computation. Currently supported: half (not recommanded), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	/// @see gtc_matrix_transform
	template <typename T, typename U>
	detail::tvec3<T> unProject(
		detail::tvec3<T> const & win,
		detail::tmat4x4<T> const & model,
		detail::tmat4x4<T> const & proj,
		detail::tvec4<U> const & viewport);

	/// Define a picking region
	///
	/// @param center
	/// @param delta
	/// @param viewport
	/// @tparam T Native type used for the computation. Currently supported: half (not recommanded), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	/// @see gtc_matrix_transform
	template <typename T, typename U>
	detail::tmat4x4<T> pickMatrix(
		detail::tvec2<T> const & center,
		detail::tvec2<T> const & delta,
		detail::tvec4<U> const & viewport);

	/// Build a look at view matrix.
	///
	/// @param eye Position of the camera
	/// @param center Position where the camera is looking at
	/// @param up Normalized up vector, how the camera is oriented. Typically (0, 0, 1)
	/// @see gtc_matrix_transform
	/// @see - frustum(T const & left, T const & right, T const & bottom, T const & top, T const & nearVal, T const & farVal) frustum(T const & left, T const & right, T const & bottom, T const & top, T const & nearVal, T const & farVal)
	template <typename T>
	detail::tmat4x4<T> lookAt(
		detail::tvec3<T> const & eye,
		detail::tvec3<T> const & center,
		detail::tvec3<T> const & up);

	/// @}
}//namespace glm

#include "matrix_transform.inl"

#endif//GLM_GTC_matrix_transform


///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Mathematics (glm.g-truc.net)
///
/// Copyright (c) 2005 - 2012 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref gtc_matrix_transform
/// @file glm/gtc/matrix_transform.inl
/// @date 2009-04-29 / 2011-06-15
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace glm
{
	template <typename T>
	GLM_FUNC_QUALIFIER detail::tmat4x4<T> translate
	(
		detail::tmat4x4<T> const & m,
		detail::tvec3<T> const & v
	)
	{
		detail::tmat4x4<T> Result(m);
		Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		return Result;
	}

	template <typename T>
	GLM_FUNC_QUALIFIER detail::tmat4x4<T> rotate
	(
		detail::tmat4x4<T> const & m,
		T const & angle,
		detail::tvec3<T> const & v
	)
	{
#ifdef GLM_FORCE_RADIANS
		T a = angle;
#else
		T a = radians(angle);
#endif
		T c = cos(a);
		T s = sin(a);

		detail::tvec3<T> axis = normalize(v);

		detail::tvec3<T> temp = (T(1) - c) * axis;

		detail::tmat4x4<T> Rotate(detail::tmat4x4<T>::null);
		Rotate[0][0] = c + temp[0] * axis[0];
		Rotate[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
		Rotate[0][2] = 0 + temp[0] * axis[2] - s * axis[1];

		Rotate[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
		Rotate[1][1] = c + temp[1] * axis[1];
		Rotate[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

		Rotate[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
		Rotate[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
		Rotate[2][2] = c + temp[2] * axis[2];

		detail::tmat4x4<T> Result(detail::tmat4x4<T>::null);
		Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
		Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
		Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
		Result[3] = m[3];
		return Result;
	}

	template <typename T>
	GLM_FUNC_QUALIFIER detail::tmat4x4<T> scale
	(
		detail::tmat4x4<T> const & m,
		detail::tvec3<T> const & v
	)
	{
		detail::tmat4x4<T> Result(detail::tmat4x4<T>::null);
		Result[0] = m[0] * v[0];
		Result[1] = m[1] * v[1];
		Result[2] = m[2] * v[2];
		Result[3] = m[3];
		return Result;
	}

	template <typename T>
	GLM_FUNC_QUALIFIER detail::tmat4x4<T> translate_slow
	(
		detail::tmat4x4<T> const & m,
		detail::tvec3<T> const & v
	)
	{
		detail::tmat4x4<T> Result(T(1));
		Result[3] = detail::tvec4<T>(v, T(1));
		return m * Result;

		//detail::tmat4x4<Real> Result(m);
		Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		//Result[3][0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0];
		//Result[3][1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1];
		//Result[3][2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2];
		//Result[3][3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3];
		//return Result;
	}

	template <typename T>
	GLM_FUNC_QUALIFIER detail::tmat4x4<T> rotate_slow
	(
		detail::tmat4x4<T> const & m,
		T const & angle,
		detail::tvec3<T> const & v
	)
	{
#ifdef GLM_FORCE_RADIANS
		T const a = angle;
#else
		T const a = radians(angle);
#endif
		T c = cos(a);
		T s = sin(a);
		detail::tmat4x4<T> Result;

		detail::tvec3<T> axis = normalize(v);

		Result[0][0] = c + (1 - c)      * axis.x     * axis.x;
		Result[0][1] = (1 - c) * axis.x * axis.y + s * axis.z;
		Result[0][2] = (1 - c) * axis.x * axis.z - s * axis.y;
		Result[0][3] = 0;

		Result[1][0] = (1 - c) * axis.y * axis.x - s * axis.z;
		Result[1][1] = c + (1 - c) * axis.y * axis.y;
		Result[1][2] = (1 - c) * axis.y * axis.z + s * axis.x;
		Result[1][3] = 0;

		Result[2][0] = (1 - c) * axis.z * axis.x + s * axis.y;
		Result[2][1] = (1 - c) * axis.z * axis.y - s * axis.x;
		Result[2][2] = c + (1 - c) * axis.z * axis.z;
		Result[2][3] = 0;

		Result[3] = detail::tvec4<T>(0, 0, 0, 1);
		return m * Result;
	}

	template <typename T>
	GLM_FUNC_QUALIFIER detail::tmat4x4<T> scale_slow
	(
		detail::tmat4x4<T> const & m,
		detail::tvec3<T> const & v
	)
	{
		detail::tmat4x4<T> Result(T(1));
		Result[0][0] = v.x;
		Result[1][1] = v.y;
		Result[2][2] = v.z;
		return m * Result;
	}

	template <typename Real>
	GLM_FUNC_QUALIFIER detail::tmat4x4<Real> ortho
	(
		Real const & left,
		Real const & right,
		Real const & bottom,
		Real const & top,
		Real const & zNear,
		Real const & zFar
	)
	{
		detail::tmat4x4<Real> Result(1);
		Result[0][0] = Real(2) / (right - left);
		Result[1][1] = Real(2) / (top - bottom);
		Result[2][2] = - Real(2) / (zFar - zNear);
		Result[3][0] = - (right + left) / (right - left);
		Result[3][1] = - (top + bottom) / (top - bottom);
		Result[3][2] = - (zFar + zNear) / (zFar - zNear);
		return Result;
	}

	template <typename Real>
	GLM_FUNC_QUALIFIER detail::tmat4x4<Real> ortho(
		Real const & left,
		Real const & right,
		Real const & bottom,
		Real const & top)
	{
		detail::tmat4x4<Real> Result(1);
		Result[0][0] = Real(2) / (right - left);
		Result[1][1] = Real(2) / (top - bottom);
		Result[2][2] = - Real(1);
		Result[3][0] = - (right + left) / (right - left);
		Result[3][1] = - (top + bottom) / (top - bottom);
		return Result;
	}

	template <typename Real>
	GLM_FUNC_QUALIFIER detail::tmat4x4<Real> frustum
	(
		Real const & left,
		Real const & right,
		Real const & bottom,
		Real const & top,
		Real const & nearVal,
		Real const & farVal
	)
	{
		detail::tmat4x4<Real> Result(0);
		Result[0][0] = (Real(2) * nearVal) / (right - left);
		Result[1][1] = (Real(2) * nearVal) / (top - bottom);
		Result[2][0] = (right + left) / (right - left);
		Result[2][1] = (top + bottom) / (top - bottom);
		Result[2][2] = -(farVal + nearVal) / (farVal - nearVal);
		Result[2][3] = Real(-1);
		Result[3][2] = -(Real(2) * farVal * nearVal) / (farVal - nearVal);
		return Result;
	}


	template <typename T, typename U>
	GLM_FUNC_QUALIFIER detail::tvec3<T> project
	(
		detail::tvec3<T> const & obj,
		detail::tmat4x4<T> const & model,
		detail::tmat4x4<T> const & proj,
		detail::tvec4<U> const & viewport
	)
	{
		detail::tvec4<T> tmp = detail::tvec4<T>(obj, T(1));
		tmp = model * tmp;
		tmp = proj * tmp;

		tmp /= tmp.w;
		tmp = tmp * T(0.5) + T(0.5);
		tmp[0] = tmp[0] * T(viewport[2]) + T(viewport[0]);
		tmp[1] = tmp[1] * T(viewport[3]) + T(viewport[1]);

		return detail::tvec3<T>(tmp);
	}

	template <typename T, typename U>
	GLM_FUNC_QUALIFIER detail::tvec3<T> unProject
	(
		detail::tvec3<T> const & win,
		detail::tmat4x4<T> const & model,
		detail::tmat4x4<T> const & proj,
		detail::tvec4<U> const & viewport
	)
	{
		detail::tmat4x4<T> inverse = glm::inverse(proj * model);

		detail::tvec4<T> tmp = detail::tvec4<T>(win, T(1));
		tmp.x = (tmp.x - T(viewport[0])) / T(viewport[2]);
		tmp.y = (tmp.y - T(viewport[1])) / T(viewport[3]);
		tmp = tmp * T(2) - T(1);

		detail::tvec4<T> obj = inverse * tmp;
		obj /= obj.w;

		return detail::tvec3<T>(obj);
	}

	template <typename T, typename U>
	detail::tmat4x4<T> pickMatrix
	(
		detail::tvec2<T> const & center,
		detail::tvec2<T> const & delta,
		detail::tvec4<U> const & viewport
	)
	{
		assert(delta.x > T(0) && delta.y > T(0));
		detail::tmat4x4<T> Result(1.0f);

		if(!(delta.x > T(0) && delta.y > T(0)))
			return Result; // Error

		detail::tvec3<T> Temp(
			(T(viewport[2]) - T(2) * (center.x - T(viewport[0]))) / delta.x,
			(T(viewport[3]) - T(2) * (center.y - T(viewport[1]))) / delta.y,
			T(0));

		// Translate and scale the picked region to the entire window
		Result = translate(Result, Temp);
		return scale(Result, detail::tvec3<T>(T(viewport[2]) / delta.x, T(viewport[3]) / delta.y, T(1)));
	}

}//namespace glm

*/
