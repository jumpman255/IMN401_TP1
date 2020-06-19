#ifndef _UTILITIES_POINT_H_
#define _UTILITIES_POINT_H_

#include "StaticUtilities.h"
#include "Types.h"
#include "Units.h"
#include "Vectors.h"

#include <stdexcept>

////////////////////////////////////////////
// Point3 with component Unit
////////////////////////////////////////////
template<typename U>
class Point2
{
public:
	using ValueType = units::GetValueType<U>;

	constexpr static uint32 DIMENSION = 2;

	constexpr Point2()
		: v{ U(), U() }
	{
	}

	constexpr Point2(U xpos, U ypos)
		: v{ xpos, ypos }
	{
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Point2(const Point2<V>& u)
		: v{ u.x(), u.y() }
	{
	}

	constexpr const U* constData() const { return v; }
	constexpr const ValueType* constValues() const { return static_cast<const ValueType*>((void*)&v); }

	constexpr bool isNull() const
	{
		U tresshold = U(0.0000000001);
		return abs(v[0]) < tresshold && abs(v[1]) < tresshold;
	}

	constexpr U x() const { return v[0]; }
	constexpr U y() const { return v[1]; }

	constexpr U& x() { return v[0]; }
	constexpr U& y() { return v[1]; }

	constexpr U& at(uint32 idx)
	{
		if (idx >= DIMENSION)
			throw std::invalid_argument("L'index est trop grand.");
		return v[idx];
	}

	constexpr U at(uint32 idx) const
	{
		if (idx >= DIMENSION)
			throw std::invalid_argument("L'index est trop grand.");
		return v[idx];
	}

	constexpr U& operator[](uint32 idx)
	{
		return v[idx];
	}

	constexpr U operator[](uint32 idx) const
	{
		return v[idx];
	}

	constexpr void correct()
	{
		if (isNull())
		{
			v[0] = U();
			v[1] = v[0];
			v[2] = v[0];
		}
	}

	constexpr void offset(U x, U y)
	{
		v[0] += x;
		v[1] += y;
	}

	constexpr Vector2<Unitless<ValueType> > normalized() const
	{
		return Vector2<U>(v[0], v[1]).normalized();
	}

	constexpr Vector2<U> distanceFromOrigin() const
	{
		return Vector2<U>(v[0], v[1]);
	}

	//////////////////////////////////////
	// Assignation operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point2<U>& operator=(const Point2<U2>& vec)
	{
		v[0] = vec.x();
		v[1] = vec.y();
		return *this;
	}

	//////////////////////////////////////
	// Coumpound assigment operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point2<U>& operator+=(const Vector2<U2> &vector)
	{
		v[0] += vector.x();
		v[1] += vector.y();
		return *this;
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point2<U>& operator-=(const Vector2<U2> &vector)
	{
		v[0] -= vector.x();
		v[1] -= vector.y();
		return *this;
	}

	//////////////////////////////////////
	// Equality operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator==(const Point2<U2> &v2) const
	{
		return v[0] == v2.x() && v[1] == v2.y();
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator!=(const Point2<U2> &v2) const
	{
		return !(*this == v2);
	}


	//////////////////////////////////////
	// Addition/Substraction operators
	//////////////////////////////////////

	// Point<U> + Vector<U> -> Point<U>
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point2<U> operator+(const Vector2<U2> &v2) const
	{
		return Point2<U>(v[0] + v2.x(), v[1] + v2.y());
	}

	// Point<U> - Vector<U> -> Point<U>
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point2<U> operator-(const Vector2<U2> &v2) const
	{
		return Point2<U>(v[0] - v2.x(), v[1] - v2.y());
	}

	// Point<U> - Point<U> -> Vector<U>
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector2<U> operator-(const Point2<U2> &v2) const
	{
		return Vector2<U>(v[0] - v2.x(), v[1] - v2.y());
	}

	//////////////////////////////////////
	// Other operator functions
	//////////////////////////////////////
	constexpr Point2<U> operator-() const
	{
		return Point2<U>(-v[0], -v[1]);
	}

	// Transform to Unitless point (remove units)
	constexpr Point2<Unitless<ValueType> > ToUnitless() const
	{
		return Point2<Unitless<ValueType>>(Unitless<ValueType>((ValueType)v[0]), Unitless<ValueType>((ValueType)v[1]));
	}

private:
	constexpr Point2(const U other[DIMENSION])
	{
		v[0] = other[0];
		v[1] = other[1];
	}

	U v[DIMENSION];
};

////////////////////////////////////////////
// Point3 with component Unit
////////////////////////////////////////////
template<typename U>
class Point3
{
public:
	using ValueType = units::GetValueType<U>;

	constexpr static uint32 DIMENSION = 3;

	constexpr Point3()
		: v { U(), U(), U() }
	{
	}

	constexpr Point3(U xpos, U ypos, U zpos)
		: v { xpos, ypos, zpos }
	{
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Point3(const Point3<V>& u)
		: v { u.x(), u.y(), u.z() }
	{
	}

	constexpr const U* constData() const { return v; }
    constexpr const ValueType* constValues() const { return static_cast<const ValueType*>((void*)&v); }

	constexpr bool isNull() const
	{
		U tresshold = U(0.0000000001);
		return abs(v[0]) < tresshold && abs(v[1]) < tresshold && abs(v[2]) < tresshold;
	}

	constexpr U x() const { return v[0]; }
	constexpr U y() const { return v[1]; }
	constexpr U z() const { return v[2]; }

	constexpr U& x() { return v[0]; }
	constexpr U& y() { return v[1]; }
	constexpr U& z() { return v[2]; }

	constexpr U& at(uint32 idx)
	{
		if (idx >= DIMENSION)
			throw std::invalid_argument("L'index est trop grand.");
		return v[idx];
	}

	constexpr U at(uint32 idx) const
	{
		if (idx >= DIMENSION)
			throw std::invalid_argument("L'index est trop grand.");
		return v[idx];
	}

	constexpr U& operator[](uint32 idx)
	{
		return v[idx];
	}

	constexpr U operator[](uint32 idx) const
	{
		return v[idx];
	}

	constexpr void correct()
	{
		if (isNull())
		{
			v[0] = U();
			v[1] = v[0];
			v[2] = v[0];
		}
	}

	constexpr void offset(U x, U y, U z)
	{
		v[0] += x;
		v[1] += y;
		v[2] += z;
	}

    constexpr Vector3<Unitless<ValueType> > normalized() const
    {
        return Vector3<U>(v[0], v[1], v[2]).normalized();
    }

    constexpr Vector3<U> distanceFromOrigin() const
    {
        return Vector3<U>(v[0], v[1], v[2]);
    }

	//////////////////////////////////////
	// Assignation operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point3<U>& operator=(const Point3<U2>& vec)
	{
		v[0] = vec.x();
		v[1] = vec.y();
		v[2] = vec.z();
		return *this;
	}

	//////////////////////////////////////
	// Coumpound assigment operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point3<U>& operator+=(const Vector3<U2> &vector)
	{
		v[0] += vector.x();
		v[1] += vector.y();
		v[2] += vector.z();
		return *this;
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point3<U>& operator-=(const Vector3<U2> &vector)
	{
		v[0] -= vector.x();
		v[1] -= vector.y();
		v[2] -= vector.z();
		return *this;
	}

	//////////////////////////////////////
	// Equality operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator==(const Point3<U2> &v2) const
	{
		return v[0] == v2.x() && v[1] == v2.y() && v[2] == v2.z();
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator!=(const Point3<U2> &v2) const
	{
		return !(*this == v2);
	}


	//////////////////////////////////////
	// Addition/Substraction operators
	//////////////////////////////////////

	// Point<U> + Vector<U> -> Point<U>
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point3<U> operator+(const Vector3<U2> &v2) const
	{
		return Point3<U>(v[0] + v2.x(), v[1] + v2.y(), v[2] + v2.z());
	}

	// Point<U> - Vector<U> -> Point<U>
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Point3<U> operator-(const Vector3<U2> &v2) const
	{
		return Point3<U>(v[0] - v2.x(), v[1] - v2.y(), v[2] - v2.z());
	}

	// Point<U> - Point<U> -> Vector<U>
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector3<U> operator-(const Point3<U2> &v2) const
	{
		return Vector3<U>(v[0] - v2.x(), v[1] - v2.y(), v[2] - v2.z());
	}

	//////////////////////////////////////
	// Other operator functions
	//////////////////////////////////////
	constexpr Point3<U> operator-() const
	{
		return Point3<U>(-v[0], -v[1], -v[2]);
	}

	// Transform to Unitless point (remove units)
	constexpr Point3<Unitless<ValueType> > ToUnitless() const
	{
		return Point3<Unitless<ValueType>>(Unitless<ValueType>((ValueType)v[0]), Unitless<ValueType>((ValueType)v[1]), Unitless<ValueType>((ValueType)v[2]));
	}

private:
	constexpr Point3(const U other[DIMENSION])
	{
		v[0] = other[0];
		v[1] = other[1];
		v[2] = other[2];
	}

	U v[DIMENSION];
};


template<typename U>
constexpr Point2<U> abs(const Point2<U>& v)
{
	return Point2<U>(abs(v.x()), abs(v.y()));
}

template<typename U>
constexpr Point3<U> abs(const Point3<U>& v)
{
	return Point3<U>(abs(v.x()), abs(v.y()), abs(v.z()));
}

template<typename S, typename V>
S& operator<<(S& os, const Point2<V>& v)
{
	os << '(' << v.x();
	os << ", " << v.y();
	os << ')';
	return os;
}

template<typename S, typename V>
S& operator<<(S& os, const Point3<V>& v)
{
	os << '(' << v.x();
	os << ", " << v.y();
	os << ", " << v.z();
	os << ')';
	return os;
}

#endif