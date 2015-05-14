#include "Math/Vector3.h"

template<> const Vector3<float> Vector3<float>::ZERO(0.0f, 0.0f, 0.0f);
template<> const Vector3<float> Vector3<float>::UNIT_X(1.0f, 0.0f, 0.0f);
template<> const Vector3<float> Vector3<float>::UNIT_Y(0.0f, 1.0f, 0.0f);
template<> const Vector3<float> Vector3<float>::UNIT_Z(0.0f, 0.0f, 1.0f);
template<> const Vector3<float> Vector3<float>::NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f);
template<> const Vector3<float> Vector3<float>::NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f);
template<> const Vector3<float> Vector3<float>::NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f);
template<> const Vector3<float> Vector3<float>::ONE(1.0f, 1.0f, 1.0f);

template<> const Vector3<double> Vector3<double>::ZERO(0.0, 0.0, 0.0);
template<> const Vector3<double> Vector3<double>::UNIT_X(1.0, 0.0, 0.0);
template<> const Vector3<double> Vector3<double>::UNIT_Y(0.0, 1.0, 0.0);
template<> const Vector3<double> Vector3<double>::UNIT_Z(0.0, 0.0, 1.0);
template<> const Vector3<double> Vector3<double>::NEGATIVE_UNIT_X(-1.0, 0.0, 0.0);
template<> const Vector3<double> Vector3<double>::NEGATIVE_UNIT_Y(0.0, -1.0, 0.0);
template<> const Vector3<double> Vector3<double>::NEGATIVE_UNIT_Z(0.0, 0.0, -1.0);
template<> const Vector3<double> Vector3<double>::ONE(1.0, 1.0, 1.0);
