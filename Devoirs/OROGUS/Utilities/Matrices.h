#ifndef _UTILITIES_MATRICE_H_
#define _UTILITIES_MATRICE_H_

#include "Point.h"
#include "Units.h"
#include "Vectors.h"
#include "StaticUtilities.h"

#include <limits>

// Matrices are in Column-major.
// m[j] gives the jth column
// m[j][i] gives the value at the jth column and the ith row
// Use at(row, col) to retrieve a value
template<typename U>
class Matrix3x3
{
public:
    using ValueType = ::units::GetValueType<U>;

	constexpr Matrix3x3()
	{
        // First row
		m[0][0] = U();
		m[1][0] = U();
		m[2][0] = U();

        // Second row
		m[0][1] = U();
		m[1][1] = U();
		m[2][1] = U();

        // Third row
		m[0][2] = U();
		m[1][2] = U();
		m[2][2] = U();
	}

    //template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
    constexpr Matrix3x3(U diagonale)
    {
        // First row
        m[0][0] = diagonale;
        m[1][0] = U();
        m[2][0] = U();

        // Second row
        m[0][1] = U();
        m[1][1] = diagonale;
        m[2][1] = U();

        // Third row
        m[0][2] = U();
        m[1][2] = U();
        m[2][2] = diagonale;
    }

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Matrix3x3(const Vector3<V>& row1, const Vector3<V>& row2, const Vector3<V>& row3)
	{
        // First row
		m[0][0] = row1.x();
		m[1][0] = row2.x();
		m[2][0] = row3.x();

        // Second row
		m[0][1] = row1.y();
		m[1][1] = row2.y();
		m[2][1] = row3.y();

        // Third row
		m[0][2] = row1.z();
		m[1][2] = row2.z();
		m[2][2] = row3.z();
	}

    //template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Matrix3x3(U m11, U m12, U m13, U m21, U m22, U m23, U m31, U m32, U m33)
	{
        // First row
		m[0][0] = m11;
		m[1][0] = m12;
		m[2][0] = m13;

        // Second row
		m[0][1] = m21;
		m[1][1] = m22;
		m[2][1] = m23;

        // Third row
		m[0][2] = m31;
		m[1][2] = m32;
		m[2][2] = m33;
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Matrix3x3(const Matrix3x3<V>& u)
	{
        // First row
		m[0][0] = U(u.at(0,0));
		m[1][0] = U(u.at(0,1));
		m[2][0] = U(u.at(0,2));

        // Second row
		m[0][1] = U(u.at(1,0));
		m[1][1] = U(u.at(1,1));
		m[2][1] = U(u.at(1,2));

        // Third row
		m[0][2] = U(u.at(2,0));
		m[1][2] = U(u.at(2,1));
		m[2][2] = U(u.at(2,2));
	}

	constexpr const U* constData() const { return m; }
    constexpr const ValueType* constValues() const { return static_cast<const ValueType*>((void*)&m); }

	constexpr bool isIdentity() const
	{
        U tresshold = U(std::numeric_limits<ValueType>::epsilon());
        U zero = U();
		return abs(U(1) - m[0][0]) < tresshold && abs(U(1) - m[1][1]) < tresshold && abs(U(1) - m[2][2]) < tresshold
                && (at(0, 1) == zero && at(0, 2) == zero 
                &&  at(1, 0) == zero && at(1, 2) == zero 
                &&  at(2, 0) == zero && at(2, 1) == zero);
	}

	constexpr units::Pow<U, 3> determinant() const
	{
		return (at(0, 0) * (at(1, 1) * at(2, 2) - at(1, 2) * at(2, 1))
			+ at(0, 1) * (at(1, 2) * at(2, 0) - at(1, 0) * at(2, 2))
			+ at(0, 2) * (at(1, 0) * at(2, 1) - at(1, 1) * at(2, 0)));
	}

	constexpr Matrix3x3<units::InvPow<U>> inverse() const
	{
        const Vector3<U>& a = *(reinterpret_cast<const Vector3<U>*>(m[0]));
        const Vector3<U>& b = *(reinterpret_cast<const Vector3<U>*>(m[1]));
        const Vector3<U>& c = *(reinterpret_cast<const Vector3<U>*>(m[2]));

		Vector3<units::Pow<U, 2>> r0 = b.crossProduct(c);
		Vector3<units::Pow<U, 2>> r1 = c.crossProduct(a);
		Vector3<units::Pow<U, 2>> r2 = a.crossProduct(b);
		
		auto invDet = Real(1) / r2.dotProduct(c);

        return Matrix3x3<units::InvPow<U>>(r0 * invDet, r1 * invDet, r2 * invDet);
	}

	constexpr Matrix3x3<U> transpose() const
	{
        return Matrix3x3<U>(at(0, 0), at(1, 0), at(2, 0),
            at(0, 1), at(1, 1), at(2, 1),
            at(0, 2), at(1, 2), at(2, 2));
	}

	constexpr U& at(uint32 row, uint32 col)
	{
		return m[col][row];
	}

	constexpr const U& at(uint32 row, uint32 col) const
	{
		return m[col][row];
	}

	//////////////////////////////////////
	// Assignation operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix3x3<U>& operator=(const Matrix3x3<U2>& mat)
	{
        // First row
		m[0][0] = mat.at(0,0);
		m[1][0] = mat.at(0,1);
		m[2][0] = mat.at(0,2);

        // Second row
		m[0][1] = mat.at(1,0);
		m[1][1] = mat.at(1,1);
		m[2][1] = mat.at(1,2);

        // Third row
		m[0][2] = mat.at(2,0);
		m[1][2] = mat.at(2,1);
		m[2][2] = mat.at(2,2);

		return *this;
	}

	//////////////////////////////////////
	// Coumpound assigment operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix3x3<U>& operator+=(const Matrix3x3<U2> &mat)
	{
		m[0][0] += mat.at(0,0);
		m[1][0] += mat.at(0,1);
		m[2][0] += mat.at(0,2);

		m[0][1] += mat.at(1,0);
		m[1][1] += mat.at(1,1);
		m[2][1] += mat.at(1,2);

		m[0][2] += mat.at(2,0);
		m[1][2] += mat.at(2,1);
		m[2][2] += mat.at(2,2);
		return *this;
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix3x3<U>& operator-=(const Matrix3x3<U2> &mat)
	{
		m[0][0] -= mat.at(0,0);
		m[1][0] -= mat.at(0,1);
		m[2][0] -= mat.at(0,2);

		m[0][1] -= mat.at(1,0);
		m[1][1] -= mat.at(1,1);
		m[2][1] -= mat.at(1,2);

		m[0][2] -= mat.at(2,0);
		m[1][2] -= mat.at(2,1);
		m[2][2] -= mat.at(2,2);
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Matrix3x3<U>& operator*=(const F& factor)
	{
		m[0][0] *= factor;
		m[1][0] *= factor;
		m[2][0] *= factor;

		m[0][1] *= factor;
		m[1][1] *= factor;
		m[2][1] *= factor;

		m[0][2] *= factor;
		m[1][2] *= factor;
		m[2][2] *= factor;
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Matrix3x3<U>& operator/=(const F& divisor)
	{
		m[0][0] /= divisor;
		m[1][0] /= divisor;
		m[2][0] /= divisor;

		m[0][1] /= divisor;
		m[1][1] /= divisor;
		m[2][1] /= divisor;

		m[0][2] /= divisor;
		m[1][2] /= divisor;
		m[2][2] /= divisor;
		return *this;
	}

	//////////////////////////////////////
	// Equality operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator==(const Matrix3x3<U2> &mat) const
	{
		return m[0][0] == mat.at(0,0) &&
			m[1][0] == mat.at(0,1) &&
			m[2][0] == mat.at(0,2) &&

			m[0][1] == mat.at(1,0) &&
			m[1][1] == mat.at(1,1) &&
			m[2][1] == mat.at(1,2) &&

			m[0][2] == mat.at(2,0) &&
			m[1][2] == mat.at(2,1) &&
			m[2][2] == mat.at(2,2);
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator!=(const Matrix3x3<U2> &mat) const
	{
		return !(*this == mat);
	}


	//////////////////////////////////////
	// Addition/Substraction operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix3x3<U> operator+(const Matrix3x3<U2> &mat) const
	{
		return Matrix3x3<U>(m[0][0] + mat.at(0,0),
			m[1][0] + mat.at(0,1),
			m[2][0] + mat.at(0,2),
			m[0][1] + mat.at(1,0),
			m[1][1] + mat.at(1,1),
			m[2][1] + mat.at(1,2),
			m[0][2] + mat.at(2,0),
			m[1][2] + mat.at(2,1),
			m[2][2] + mat.at(2,2));
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix3x3<U> operator-(const Matrix3x3<U2> &mat) const
	{
		return Matrix3x3<U>(m[0][0] - mat.at(0,0),
			m[1][0] - mat.at(0,1),
			m[2][0] - mat.at(0,2),
			m[0][1] - mat.at(1,0),
			m[1][1] - mat.at(1,1),
			m[2][1] - mat.at(1,2),
			m[0][2] - mat.at(2,0),
			m[1][2] - mat.at(2,1),
			m[2][2] - mat.at(2,2));
	}


	//////////////////////////////////////
	// Multiplication operators
	//////////////////////////////////////
	// Matrix<U> *  Arithmetic
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Matrix3x3<U> operator*(const F& factor) const
	{
		return Matrix3x3<U>(m[0][0] * factor,
			m[1][0] * factor,
			m[2][0] * factor,
			m[0][1] * factor,
			m[1][1] * factor,
			m[2][1] * factor,
			m[0][2] * factor,
			m[1][2] * factor,
			m[2][2] * factor);
	}

	// Matrix<U> * V
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value> > >
	constexpr Matrix3x3< units::MutiplyResultType<U, V> > operator*(const V& u) const
	{
		using ReturnType = ::units::MutiplyResultType<U, V>;
		return Matrix3x3<ReturnType>(at(0,0) * u, at(0,1) * u, at(0,2) * u,
			at(1,0) * u, at(1,1) * u, at(1,2) * u,
			at(2,0) * u, at(2,1) * u, at(2,2) * u);
	}

	// Matrix<U> * Vector<U> 
    template<typename V>
	constexpr Vector3<units::MutiplyResultType<U, V> > operator*(const Vector3<V> &v) const
	{
		using ReturnType = ::units::MutiplyResultType<U, V>;
		return Vector3<ReturnType>(at(0,0) * v.x() + at(0,1) * v.y() + at(0,2) * v.z(),
			at(1,0) * v.x() + at(1,1) * v.y() + at(1,2) * v.z(),
			at(2,0) * v.x() + at(2,1) * v.y() + at(2,2) * v.z());
	}

	//////////////////////////////////////
	// Division operators
	//////////////////////////////////////
	// Vector<U> / V
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value> > >
	constexpr Matrix3x3< units::DivideResultType<U, V> > operator/(const V& u) const
	{
		using ReturnType = ::units::DivideResultType<U, V>;
		return Matrix3x3<ReturnType>(at(0,0) / u, at(0,1) / u, at(0,2) / u,
			at(1,0) / u, at(1,1) / u, at(1,2) / u,
			at(2,0) / u, at(2,1) / u, at(2,2) / u);
	}

	// Matrix<U> / double
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Matrix3x3<U> operator/(const F& divisor)
	{
		return Matrix3x3<U>(at(0,0) / divisor,
			at(0,1) / divisor,
			at(0,2) / divisor,
			at(1,0) / divisor,
			at(1,1) / divisor,
			at(1,2) / divisor,
			at(2,0) / divisor,
			at(2,1) / divisor,
			at(2,2) / divisor);
	}

	//////////////////////////////////////
	// Other operator functions
	//////////////////////////////////////
	// Transform to UnitlessMatrix (remove units)
	constexpr Matrix3x3<Unitless<typename U::ValueType>> ToUnitless() const
	{
		using ValueType = typename U::ValueType;
		return Matrix3x3<Unitless<ValueType>>(Unitless<ValueType>((ValueType)at(0,0)), Unitless<ValueType>((ValueType)at(0,1)), Unitless<ValueType>((ValueType)at(0,2)),
			Unitless<ValueType>((ValueType)at(1,0)), Unitless<ValueType>((ValueType)at(1,1)), Unitless<ValueType>((ValueType)at(1,2)),
			Unitless<ValueType>((ValueType)at(2,0)), Unitless<ValueType>((ValueType)at(2,1)), Unitless<ValueType>((ValueType)at(2,2)));
	}

private:
	U m[3][3];
};


template<typename U>
class Matrix4x4
{
public:
    using ValueType = ::units::GetValueType<U>;

	constexpr Matrix4x4()
	{
		m[0][0] = U();
		m[1][0] = U();
		m[2][0] = U();
		m[3][0] = U();

		m[0][1] = U();
		m[1][1] = U();
		m[2][1] = U();
		m[3][1] = U();

		m[0][2] = U();
		m[1][2] = U();
		m[2][2] = U();
		m[3][2] = U();

		m[0][3] = U();
		m[1][3] = U();
		m[2][3] = U();
		m[3][3] = U();
	}

    constexpr Matrix4x4(U diagonale)
    {
        // First row
        m[0][0] = diagonale;
        m[1][0] = U();
        m[2][0] = U();
        m[3][0] = U();

        // Second row
        m[0][1] = U();
        m[1][1] = diagonale;
        m[2][1] = U();
        m[3][1] = U();

        // Third row
        m[0][2] = U();
        m[1][2] = U();
        m[2][2] = diagonale;
        m[3][2] = U();

        // Fourth row
        m[0][3] = U();
        m[1][3] = U();
        m[2][3] = U();
        m[3][3] = diagonale;
    }

	constexpr Matrix4x4(U m11, U m12, U m13, U m14, U m21, U m22, U m23, U m24, U m31, U m32, U m33, U m34, U m41, U m42, U m43, U m44)
	{
		m[0][0] = m11;
		m[1][0] = m12;
		m[2][0] = m13;
		m[3][0] = m14;

		m[0][1] = m21;
		m[1][1] = m22;
		m[2][1] = m23;
		m[3][1] = m24;

		m[0][2] = m31;
		m[1][2] = m32;
		m[2][2] = m33;
		m[3][2] = m34;

		m[0][3] = m41;
		m[1][3] = m42;
		m[2][3] = m43;
		m[3][3] = m44;
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Matrix4x4(const Vector4<V>& row1, const Vector4<V>& row2, const Vector4<V>& row3, const Vector4<V>& row4)
	{
		m[0][0] = row1.x();
		m[1][0] = row1.y();
		m[2][0] = row1.z();
		m[3][0] = row1.w();

		m[0][1] = row2.x();
		m[1][1] = row2.y();
		m[2][1] = row2.z();
		m[3][1] = row2.w();

		m[0][2] = row3.x();
		m[1][2] = row3.y();
		m[2][2] = row3.z();
		m[3][2] = row3.w();

		m[0][3] = row4.x();
		m[1][3] = row4.y();
		m[2][3] = row4.z();
		m[3][3] = row4.w();
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Matrix4x4(const Matrix4x4<V>& u)
	{
		m[0][0] = u.at(0,0);
		m[1][0] = u.at(0,1);
		m[2][0] = u.at(0,2);
		m[3][0] = u.at(0,3);

		m[0][1] = u.at(1,0);
		m[1][1] = u.at(1,1);
		m[2][1] = u.at(1,2);
		m[3][1] = u.at(1,3);

		m[0][2] = u.at(2,0);
		m[1][2] = u.at(2,1);
		m[2][2] = u.at(2,2);
		m[3][2] = u.at(2,3);

		m[0][3] = u.at(3,0);
		m[1][3] = u.at(3,1);
		m[2][3] = u.at(3,2);
		m[3][3] = u.at(3,3);
	}

	constexpr const U* constData() const { return m; }
    constexpr const ValueType* constValues() const { return static_cast<const ValueType*>((void*)&m); }

	constexpr bool isIdentity() const
	{
        U tresshold = U(std::numeric_limits<ValueType>::epsilon());
        U zero = U();
		return abs(U(1) - m[0][0]) < tresshold && abs(U(1) - m[1][1]) < tresshold && abs(U(1) - m[2][2]) < tresshold && abs(U(1) - m[3][3]) < tresshold 
                && (at(0,1) == zero && at(0,2) == zero && at(0,3) == zero 
                &&  at(1,0) == zero && at(1,2) == zero && at(1,3) == zero 
                &&  at(2,0) == zero && at(2,1) == zero && at(2,3) == zero 
                &&  at(3,0) == zero && at(3,1) == zero && at(3,2) == zero);
	}

	constexpr U& at(uint32 row, uint32 col)
	{
		return m[col][row];
	}

	constexpr const U& at(uint32 row, uint32 col) const
	{
		return m[col][row];
	}

	constexpr Matrix4x4<units::InvPow<U>> inverse() const
	{
        const Vector3<U>& a = *(reinterpret_cast<const Vector3<U>*>(m[0]));
        const Vector3<U>& b = *(reinterpret_cast<const Vector3<U>*>(m[1]));
        const Vector3<U>& c = *(reinterpret_cast<const Vector3<U>*>(m[2]));
        const Vector3<U>& d = *(reinterpret_cast<const Vector3<U>*>(m[3]));

        const U& x = at(3, 0);
        const U& y = at(3, 1);
		const U& z = at(3, 2);
		const U& w = at(3, 3);

		Vector3<units::Pow<U, 2> > s = a.crossProduct(b);
		Vector3<units::Pow<U, 2> > t = c.crossProduct(d);
		Vector3<units::Pow<U, 2> > u = a * y - b * x;
		Vector3<units::Pow<U, 2> > v = c * w - d * z;

		units::Pow<U, -4> invDet = 1.0f / (s.dotProduct(v) + t.dotProduct(u));

		Vector3<units::Pow<U, 3> > r0 = b.crossProduct(v) + t * y;
		Vector3<units::Pow<U, 3> > r1 = v.crossProduct(a) - t * x;
		Vector3<units::Pow<U, 3> > r2 = d.crossProduct(u) + s * w;
		Vector3<units::Pow<U, 3> > r3 = u.crossProduct(c) - s * z;

		return Matrix4x4<units::InvPow<U>>(r0.x() * invDet, r0.y() * invDet, r0.z() * invDet, b.dotProduct(t) * -invDet,
			                r1.x() * invDet, r1.y() * invDet, r1.z() * invDet, a.dotProduct(t) * invDet,
			                r2.x() * invDet, r2.y() * invDet, r2.z() * invDet, d.dotProduct(s) * -invDet,
			                r3.x() * invDet, r3.y() * invDet, r3.z() * invDet, c.dotProduct(s) * invDet);
	}

	constexpr Matrix4x4<U> transpose() const
	{
        return Matrix4x4<U>(at(0, 0), at(1, 0), at(2, 0), at(3, 0),
            at(0, 1), at(1, 1), at(2, 1), at(3, 1),
            at(0, 2), at(1, 2), at(2, 2), at(3, 2),
            at(0, 3), at(1, 3), at(2, 3), at(3, 3));
	}

	//////////////////////////////////////
	// Assignation operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix4x4<U>& operator=(const Matrix4x4<U2>& mat)
	{
		m[0][0] = mat.at(0,0);
		m[1][0] = mat.at(0,1);
		m[2][0] = mat.at(0,2);
		m[3][0] = mat.at(0,3);
		m[0][1] = mat.at(1,0);
		m[1][1] = mat.at(1,1);
		m[2][1] = mat.at(1,2);
		m[3][1] = mat.at(1,3);
		m[0][2] = mat.at(2,0);
		m[1][2] = mat.at(2,1);
		m[2][2] = mat.at(2,2);
		m[3][2] = mat.at(2,3);
		m[0][3] = mat.at(3,0);
		m[1][3] = mat.at(3,1);
		m[2][3] = mat.at(3,2);
		m[3][3] = mat.at(3,3);

		return *this;
	}

	//////////////////////////////////////
	// Coumpound assigment operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix4x4<U>& operator+=(const Matrix4x4<U2> &mat)
	{
		m[0][0] += mat.at(0,0);
		m[1][0] += mat.at(0,1);
		m[2][0] += mat.at(0,2);
		m[3][0] += mat.at(0,3);
		m[0][1] += mat.at(1,0);
		m[1][1] += mat.at(1,1);
		m[2][1] += mat.at(1,2);
		m[3][1] += mat.at(1,3);
		m[0][2] += mat.at(2,0);
		m[1][2] += mat.at(2,1);
		m[2][2] += mat.at(2,2);
		m[3][2] += mat.at(2,3);
		m[0][3] += mat.at(3,0);
		m[1][3] += mat.at(3,1);
		m[2][3] += mat.at(3,2);
		m[3][3] += mat.at(3,3);

		return *this;
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix4x4<U>& operator-=(const Matrix4x4<U2> &mat)
	{
		m[0][0] -= mat.at(0,0);
		m[1][0] -= mat.at(0,1);
		m[2][0] -= mat.at(0,2);
		m[3][0] -= mat.at(0,3);
		m[0][1] -= mat.at(1,0);
		m[1][1] -= mat.at(1,1);
		m[2][1] -= mat.at(1,2);
		m[3][1] -= mat.at(1,3);
		m[0][2] -= mat.at(2,0);
		m[1][2] -= mat.at(2,1);
		m[2][2] -= mat.at(2,2);
		m[3][2] -= mat.at(2,3);
		m[0][3] -= mat.at(3,0);
		m[1][3] -= mat.at(3,1);
		m[2][3] -= mat.at(3,2);
		m[3][3] -= mat.at(3,3);
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Matrix4x4<U>& operator*=(const F& factor)
	{
		m[0][0] *= factor;
		m[1][0] *= factor;
		m[2][0] *= factor;
		m[3][0] *= factor;
		m[0][1] *= factor;
		m[1][1] *= factor;
		m[2][1] *= factor;
		m[3][1] *= factor;
		m[0][2] *= factor;
		m[1][2] *= factor;
		m[2][2] *= factor;
		m[3][2] *= factor;
		m[0][3] *= factor;
		m[1][3] *= factor;
		m[2][3] *= factor;
		m[3][3] *= factor;
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Matrix4x4<U>& operator/=(const F& divisor)
	{
		m[0][0] /= divisor;
		m[1][0] /= divisor;
		m[2][0] /= divisor;
		m[3][0] /= divisor;
		m[0][1] /= divisor;
		m[1][1] /= divisor;
		m[2][1] /= divisor;
		m[3][1] /= divisor;
		m[0][2] /= divisor;
		m[1][2] /= divisor;
		m[2][2] /= divisor;
		m[3][2] /= divisor;
		m[0][3] /= divisor;
		m[1][3] /= divisor;
		m[2][3] /= divisor;
		m[3][3] /= divisor;
		return *this;
	}

	//////////////////////////////////////
	// Equality operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator==(const Matrix4x4<U2> &mat) const
	{
		return at(0,0) == mat.at(0,0) &&
			at(0,1) == mat.at(0,1) &&
			at(0,2) == mat.at(0,2) &&
			at(0,3) == mat.at(0,3) &&
			at(1,0) == mat.at(1,0) &&
			at(1,1) == mat.at(1,1) &&
			at(1,2) == mat.at(1,2) &&
			at(1,3) == mat.at(1,3) &&
			at(2,0) == mat.at(2,0) &&
			at(2,1) == mat.at(2,1) &&
			at(2,2) == mat.at(2,2) &&
			at(2,3) == mat.at(2,3) &&
			at(3,0) == mat.at(3,0) &&
			at(3,1) == mat.at(3,1) &&
			at(3,2) == mat.at(3,2) &&
			at(3,3) == mat.at(3,3);
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator!=(const Matrix4x4<U2> &mat) const
	{
		return !(*this == mat);
	}


	//////////////////////////////////////
	// Addition/Substraction operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix4x4<U> operator+(const Matrix4x4<U2> &mat) const
	{
		return Matrix4x4<U>(at(0,0) + mat.at(0, 0),
			at(0,1) + mat.at(0, 1),
			at(0,2) + mat.at(0, 2),
			at(0,3) + mat.at(0, 3),
			at(1,0) + mat.at(1, 0),
			at(1,1) + mat.at(1, 1),
			at(1,2) + mat.at(1, 2),
			at(1,3) + mat.at(1, 3),
			at(2,0) + mat.at(2, 0),
			at(2,1) + mat.at(2, 1),
			at(2,2) + mat.at(2, 2),
			at(2,3) + mat.at(2, 3),
			at(3,0) + mat.at(3, 0),
			at(3,1) + mat.at(3, 1),
			at(3,2) + mat.at(3, 2),
			at(3,3) + mat.at(3, 3));
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Matrix4x4<U> operator-(const Matrix4x4<U2> &mat) const
	{
		return Matrix4x4<U>(at(0,0) - mat.at(0, 0),
			at(0,1) - mat.at(0, 1),
			at(0,2) - mat.at(0, 2),
			at(0,3) - mat.at(0, 3),
			at(1,0) - mat.at(1, 0),
			at(1,1) - mat.at(1, 1),
			at(1,2) - mat.at(1, 2),
			at(1,3) - mat.at(1, 3),
			at(2,0) - mat.at(2, 0),
			at(2,1) - mat.at(2, 1),
			at(2,2) - mat.at(2, 2),
			at(2,3) - mat.at(2, 3),
			at(3,0) - mat.at(3, 0),
			at(3,1) - mat.at(3, 1),
			at(3,2) - mat.at(3, 2),
			at(3,3) - mat.at(3, 3));
	}


	//////////////////////////////////////
	// Multiplication operators
	//////////////////////////////////////
	// Matrix<U> *  double = Matrix<U>
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Matrix4x4<U> operator*(const F& factor) const
	{
		return Matrix4x4<U>(at(0,0) * factor,
			at(0,1) * factor,
			at(0,2) * factor,
			at(0,3) * factor,
			at(1,0) * factor,
			at(1,1) * factor,
			at(1,2) * factor,
			at(1,3) * factor,
			at(2,0) * factor,
			at(2,1) * factor,
			at(2,2) * factor,
			at(2,3) * factor,
			at(3,0) * factor,
			at(3,1) * factor,
			at(3,2) * factor,
			at(3,3) * factor);
	}

	// Matrix<U> * V = Matrix<UV>
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value> > >
	constexpr Matrix4x4< units::MutiplyResultType<U, V> > operator*(const V& u) const
	{
		using ReturnType = units::MutiplyResultType<U, V>;
		return Matrix4x4<ReturnType>(at(0,0) * u,
			at(0,1) * u,
			at(0,2) * u,
			at(0,3) * u,
			at(1,0) * u,
			at(1,1) * u,
			at(1,2) * u,
			at(1,3) * u,
			at(2,0) * u,
			at(2,1) * u,
			at(2,2) * u,
			at(2,3) * u,
			at(3,0) * u,
			at(3,1) * u,
			at(3,2) * u,
			at(3,3) * u);
	}

    template<typename V>
    constexpr Matrix4x4<units::MutiplyResultType<U, V> > operator*(const Matrix4x4<V> &m) const
    {
        const Vector4<U> firstRow = Vector4<U>(at(0, 0), at(0, 1), at(0, 2), at(0, 3));
        const Vector4<U> secondRow = Vector4<U>(at(1, 0), at(1, 1), at(1, 2), at(1, 3));
        const Vector4<U> thirdRow = Vector4<U>(at(2, 0), at(2, 1), at(2, 2), at(2, 3));
        const Vector4<U> fourthRow = Vector4<U>(at(3, 0), at(3, 1), at(3, 2), at(3, 3));

        const Vector4<V> firstCol  = Vector4<V>(m.at(0, 0), m.at(1, 0), m.at(2, 0), m.at(3, 0));
        const Vector4<V> secondCol = Vector4<V>(m.at(0, 1), m.at(1, 1), m.at(2, 1), m.at(3, 1));
        const Vector4<V> thirdCol  = Vector4<V>(m.at(0, 2), m.at(1, 2), m.at(2, 2), m.at(3, 2));
        const Vector4<V> fourthCol = Vector4<V>(m.at(0, 3), m.at(1, 3), m.at(2, 3), m.at(3, 3));
        
        using ReturnUnit = units::MutiplyResultType<U, V>;
        const Vector4<ReturnUnit> retRow1 = Vector4<ReturnUnit>(firstRow.dotProduct(firstCol), firstRow.dotProduct(secondCol), firstRow.dotProduct(thirdCol), firstRow.dotProduct(fourthCol));
        const Vector4<ReturnUnit> retRow2 = Vector4<ReturnUnit>(secondRow.dotProduct(firstCol), secondRow.dotProduct(secondCol), secondRow.dotProduct(thirdCol), secondRow.dotProduct(fourthCol));
        const Vector4<ReturnUnit> retRow3 = Vector4<ReturnUnit>(thirdRow.dotProduct(firstCol), thirdRow.dotProduct(secondCol), thirdRow.dotProduct(thirdCol), thirdRow.dotProduct(fourthCol));
        const Vector4<ReturnUnit> retRow4 = Vector4<ReturnUnit>(fourthRow.dotProduct(firstCol), fourthRow.dotProduct(secondCol), fourthRow.dotProduct(thirdCol), fourthRow.dotProduct(fourthCol));

        return Matrix4x4<ReturnUnit>(retRow1, retRow2, retRow3, retRow4);
    }

	// Matrix<U> * Vector4<U> = Vector4<U>
	template<typename V>
	constexpr Vector4<units::MutiplyResultType<U, V> > operator*(const Vector4<V> &v) const
	{
		using ReturnType = units::MutiplyResultType<U, V>;

		ReturnType xValue = at(0, 0) * v.x() + at(0, 1) * v.y() + at(0, 2) * v.z() + at(0, 3) * v.w();
		ReturnType yValue = at(1, 0) * v.x() + at(1, 1) * v.y() + at(1, 2) * v.z() + at(1, 3) * v.w();
		ReturnType zValue = at(2, 0) * v.x() + at(2, 1) * v.y() + at(2, 2) * v.z() + at(2, 3) * v.w();
		ReturnType wValue = at(3, 0) * v.x() + at(3, 1) * v.y() + at(3, 2) * v.z() + at(3, 3) * v.w();

		return Vector4<ReturnType>(xValue, yValue, zValue, wValue);
	}

	// Matrix<U> * Vector3<U> = Vector3<U>
	template<typename V>
	constexpr Vector3<units::MutiplyResultType<U, V> > operator*(const Vector3<V> &v) const
	{
		using ReturnType = units::MutiplyResultType<U, V>;

		// Par défaut, un Vector3D s'étend à un Vecteur4D avec w = 0. La dernière colonne ne s'additionne donc jamais.
		ReturnType xValue = at(0, 0) * v.x() + at(0, 1) * v.y() + at(0, 2) * v.z();
		ReturnType yValue = at(1, 0) * v.x() + at(1, 1) * v.y() + at(1, 2) * v.z();
		ReturnType zValue = at(2, 0) * v.x() + at(2, 1) * v.y() + at(2, 2) * v.z();
		//ReturnType wValue = at(3, 0) * v.x() + at(3, 1) * v.y() + at(3, 2) * v.z();

		//return Vector3<ReturnType>(xValue / wValue.ToUnitless(), yValue / wValue.ToUnitless(), zValue / wValue.ToUnitless());
		return Vector3<ReturnType>(xValue, yValue, zValue);
	}

	// Matrix<U> * Point3<U> = Point3<U>
	template<typename V>
	constexpr Point3<units::MutiplyResultType<U, V> > operator*(const Point3<V> &p) const
	{
		using ReturnType = units::MutiplyResultType<U, V>;

		// Par défaut, un Point3D s'étend à un Point4D avec W = 1. L'addition de la dernière colonne se fait donc toujours.
		ReturnType xValue = at(0, 0) * p.x() + at(0, 1) * p.y() + at(0, 2) * p.z() + at(0, 3) * V(1);
		ReturnType yValue = at(1, 0) * p.x() + at(1, 1) * p.y() + at(1, 2) * p.z() + at(1, 3) * V(1);
		ReturnType zValue = at(2, 0) * p.x() + at(2, 1) * p.y() + at(2, 2) * p.z() + at(2, 3) * V(1);
		ReturnType wValue = at(3, 0) * p.x() + at(3, 1) * p.y() + at(3, 2) * p.z() + at(3, 3) * V(1);

		return Point3<ReturnType>(xValue / wValue.ToUnitless(), yValue / wValue.ToUnitless(), zValue / wValue.ToUnitless());
	}

	//////////////////////////////////////
	// Division operators
	//////////////////////////////////////
	// Vector<U> / V
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value> > >
	constexpr Matrix4x4< units::DivideResultType<U, V> > operator/(const V& u) const
	{
		using ReturnType = ::units::DivideResultType<U, V>;
		return Matrix4x4<ReturnType>(at(0,0) / u,
			at(0,1) / u,
			at(0,2) / u,
			at(0,3) / u,
			at(1,0) / u,
			at(1,1) / u,
			at(1,2) / u,
			at(1,3) / u,
			at(2,0) / u,
			at(2,1) / u,
			at(2,2) / u,
			at(2,3) / u,
			at(3,0) / u,
			at(3,1) / u,
			at(3,2) / u,
			at(3,3) / u);
	}

	// Matrix<U> / double
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Matrix4x4<U> operator/(const F& divisor)
	{
		return Matrix4x4<U>(at(0,0) * divisor,
			at(0,1) / divisor,
			at(0,2) / divisor,
			at(0,3) / divisor,
			at(1,0) / divisor,
			at(1,1) / divisor,
			at(1,2) / divisor,
			at(1,3) / divisor,
			at(2,0) / divisor,
			at(2,1) / divisor,
			at(2,2) / divisor,
			at(2,3) / divisor,
			at(3,0) / divisor,
			at(3,1) / divisor,
			at(3,2) / divisor,
			at(3,3) / divisor);
	}

	//////////////////////////////////////
	// Other operator functions
	//////////////////////////////////////
	// Transform to Unitless matrix (remove units)
	constexpr Matrix4x4<Unitless<typename U::ValueType>> ToUnitless() const
	{
		using ValueType = typename U::ValueType;

		return Matrix4x4<Unitless<ValueType>>(Unitless<ValueType>((ValueType)at(0,0)),
			Unitless<ValueType>((ValueType)at(0,1)),
			Unitless<ValueType>((ValueType)at(0,2)),
			Unitless<ValueType>((ValueType)at(0,3)),
			Unitless<ValueType>((ValueType)at(1,0)),
			Unitless<ValueType>((ValueType)at(1,1)),
			Unitless<ValueType>((ValueType)at(1,2)),
			Unitless<ValueType>((ValueType)at(1,3)),
			Unitless<ValueType>((ValueType)at(2,0)),
			Unitless<ValueType>((ValueType)at(2,1)),
			Unitless<ValueType>((ValueType)at(2,2)),
			Unitless<ValueType>((ValueType)at(2,3)),
			Unitless<ValueType>((ValueType)at(3,0)),
			Unitless<ValueType>((ValueType)at(3,1)),
			Unitless<ValueType>((ValueType)at(3,2)),
			Unitless<ValueType>((ValueType)at(3,3)));
	}

private:
	U m[4][4];
};


template<typename S, typename V>
S& operator<<(S& os, const Matrix3x3<V>& m)
{
    os << "[(" << m.at(0,0);
    os << ", " << m.at(0,1);
    os << ", " << m.at(0,2);
    os << "),\n";

    os << " (" << m.at(1, 0);
    os << ", " << m.at(1, 1);
    os << ", " << m.at(1, 2);
    os << "),\n";

    os << " (" << m.at(2, 0);
    os << ", " << m.at(2, 1);
    os << ", " << m.at(2, 2);
    os << ")]";
    return os;
}

template<typename S, typename V>
S& operator<<(S& os, const Matrix4x4<V>& m)
{
    os << "[(" << m.at(0, 0);
    os << ", " << m.at(0, 1);
    os << ", " << m.at(0, 2);
    os << ", " << m.at(0, 3);
    os << "),\n";

    os << " (" << m.at(1, 0);
    os << ", " << m.at(1, 1);
    os << ", " << m.at(1, 2);
    os << ", " << m.at(1, 3);
    os << "),\n";

    os << " (" << m.at(2, 0);
    os << ", " << m.at(2, 1);
    os << ", " << m.at(2, 2);
    os << ", " << m.at(2, 3);
    os << "),\n";

    os << " (" << m.at(3, 0);
    os << ", " << m.at(3, 1);
    os << ", " << m.at(3, 2);
    os << ", " << m.at(3, 3);
    os << ")]";
    return os;
}

#endif
