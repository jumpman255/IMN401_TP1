#ifndef _UTILITIES_VECTEUR_H_
#define _UTILITIES_VECTEUR_H_

#include "Types.h"
#include "Units.h"
#include "StaticUtilities.h"

#include <limits>
#include <stdexcept>

// Create a vector with component of type U
template<typename U>
class Vector2;

template<typename U>
class Vector3;

template<typename U>
class Vector4;


////////////////////////////////////////////
// Vector2 with component Unit
////////////////////////////////////////////
template<typename U>
class Vector2
{
public:
	using ValueType = units::GetValueType<U>;

	constexpr static uint32 DIMENSION = 2;

	constexpr Vector2()
		: v { U(), U() }
	{
	}
	
	constexpr Vector2(U xpos, U ypos)
		: v { xpos, ypos }
	{
	}


	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Vector2(const Vector2<V>& u)
		: v { u[0], u[1] }
	{
	}

    constexpr const U* constData() const { return v; }
	constexpr const typename ValueType* constValues() const { return static_cast<const ValueType*>((void*)&v); }

	constexpr bool isNull() const
	{
        U tresshold = U(std::numeric_limits<ValueType>::epsilon());
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

	constexpr U length() const
	{
		return sqrt(lengthSquared());
	}

	constexpr units::Pow<U, 2> lengthSquared() const
	{
		return dotProduct(*this, *this);
	}

	constexpr Vector2<Unitless<ValueType> > normalized() const
	{
		double len = dLenSquare();

		if (len - 1.0 == 0.0)
			return Vector2<Unitless<ValueType>>(Unitless<ValueType>((ValueType)v[0]), Unitless<ValueType>((ValueType)v[1]));
		else if (len > 0.0)
			return *this / length();
		else
			return Vector2<Unitless<ValueType>>();
	}

	constexpr void correct()
	{
        U tresshold = U(std::numeric_limits<ValueType>::epsilon());
        if (abs(v[0]) < tresshold)
            v[0] = U(0);

        if (abs(v[1]) < tresshold)
            v[1] = U(0);
	}

	template<typename U2>
	constexpr static units::MutiplyResultType<U, U2> dotProduct(const Vector2<U>& v1, const Vector2<U2>& v2)
	{
		return v1.dotProduct(v2);
	}

	template<typename U2>
	constexpr units::MutiplyResultType<U, U2> dotProduct(const Vector2<U2>& v2) const
	{
		using ReturnType = units::MutiplyResultType<U, U2>;
		return ReturnType(v[0] * v2.x() + v[1] * v2.y());
	}

	//////////////////////////////////////
	// Assignation operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector2<U>& operator=(const Vector2<U2>& vec)
	{
		v[0] = vec.x();
		v[1] = vec.y();
		return *this;
	}

	//////////////////////////////////////
	// Coumpound assigment operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector2<U>& operator+=(const Vector2<U2> &vector)
	{
		v[0] += vector.x();
		v[1] += vector.y();
		return *this;
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector2<U>& operator-=(const Vector2<U2> &vector)
	{
		v[0] -= vector.x();
		v[1] -= vector.y();
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector2<U>& operator*=(const F& factor)
	{
		v[0] *= factor;
		v[1] *= factor;
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector2<U>& operator/=(const F& divisor)
	{
		v[0] /= divisor;
		v[1] /= divisor;
		return *this;
	}

	//////////////////////////////////////
	// Equality operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator==(const Vector2<U2> &v2) const
	{
		return v[0] == v2.x() && v[1] == v2.y();
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator!=(const Vector2<U2> &v2) const
	{
		return !(*this == v2);
	}


	//////////////////////////////////////
	// Addition/Substraction operators
	//////////////////////////////////////

	// Vector<U> + Vector<U2> -> Vector<U>
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector2<U> operator+(const Vector2<U2> &v2) const
	{
		return Vector2<U>(v[0] + v2.x(), v[1] + v2.y());
	}

	// Vector<U> - Vector<U2> -> Vector<U>
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector2<U> operator-(const Vector2<U2> &v2) const
	{
		return Vector2<U>(v[0] - v2.x(), v[1] - v2.y());
	}


	//////////////////////////////////////
	// Multiplication operators
	//////////////////////////////////////
	// Vector<U> *  Arithmetic -> Vector<U>
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector2<U> operator*(const F& factor) const
	{
		return Vector2<U>(v[0] * factor, v[1] * factor);
	}

	// Vector<U> * V -> Vector<U.V>
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value> > >
	constexpr Vector2< units::MutiplyResultType<U, V> > operator*(const V& u) const
	{
		using ReturnType = units::MutiplyResultType<U, V>;
		return Vector2<ReturnType>(v[0] * u, v[1] * u);
	}

	//////////////////////////////////////
	// Division operators
	//////////////////////////////////////
	// Vector<U> / V -> Vector<U/V>
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value> > >
	constexpr Vector2< units::DivideResultType<U, V> > operator/(const V& u) const
	{
		using ReturnType = ::units::DivideResultType<U, V>;
		return Vector2<ReturnType>(v[0] / u, v[1] / u);
	}

	// Vector<U> / Arithmetic -> Vector<U>
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector2<U> operator/(const F& divisor) const
	{
		return Vector2<U>(v[0] / divisor, v[1] / divisor);
	}

	//////////////////////////////////////
	// Other operator functions
	//////////////////////////////////////
	constexpr Vector2<U> operator-() const
	{
		return Vector2<U>(-v[0], -v[1]);
	}

	//////////////////////////////////////
	// Components functions
	//////////////////////////////////////
	template <typename U2>
	constexpr Vector2<U> ParallelComponent(const Vector2<U2>& v1) const
	{
		return (dotProduct(v1) / v1.lengthSquared()) * v1;
	}

	template <typename U2>
	constexpr Vector2<U> PerpendicularComponent(const Vector2<U2>& v1) const
	{
		return *this - ParallelComponent(v1);
	}

	// Transform to Unitless vector (remove units)
	constexpr Vector2<Unitless<ValueType> > ToUnitless() const
	{
		return Vector2<Unitless<ValueType>>(Unitless<ValueType>((ValueType)v[0]), Unitless<ValueType>((ValueType)v[1]));
	}

private:
	constexpr Vector2(const U other[DIMENSION])
	{
		v[0] = other[0];
		v[1] = other[1];
	}

	constexpr double dLenSquare() const
	{
		return (double)v[0].Value() * (double)v[0].Value() + (double)v[1].Value() * (double)v[1].Value();
	}

	U v[DIMENSION];
};


////////////////////////////////////////////
// Vector3 with component Unit
////////////////////////////////////////////
template<typename U>
class Vector3
{
public:
	using ValueType = units::GetValueType<U>;

	constexpr static uint32 DIMENSION = 3;

	constexpr Vector3()
		: v { U(), U(), U() }
	{
	}
	
	constexpr Vector3(U xpos, U ypos, U zpos)
		: v { xpos, ypos, zpos }
	{
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Vector3(const Vector2<V>& u, V zpos)
		: v { u.x(), u.y(), zpos }
	{
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Vector3(const Vector3<V>& u)
		: v { u.x(), u.y(), u.z() }
	{
	}

	constexpr const U* constData() const { return v; }
    constexpr const ValueType* constValues() const { return static_cast<const ValueType*>((void*)&v); }

	constexpr bool isNull() const
	{
        U tresshold = U(std::numeric_limits<ValueType>::epsilon());
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

	constexpr U length() const
	{
		return sqrt(lengthSquared());
	}

	constexpr units::MutiplyResultType<U, U> lengthSquared() const
	{
		return dotProduct(*this, *this);
	}

	constexpr Vector3<Unitless<ValueType>> normalized() const
	{
		double len = dLenSquare();

		if (len - 1.0 == 0.0)
			return Vector3<Unitless<ValueType>>(Unitless<ValueType>((ValueType)v[0]), Unitless<ValueType>((ValueType)v[1]), Unitless<ValueType>((ValueType)v[2]));
		else if (len > 0.0)
			return *this / length();
		else
			return Vector3<Unitless<ValueType>>();
	}

	constexpr void correct()
	{
        U tresshold = U(std::numeric_limits<ValueType>::epsilon());
        if (abs(v[0]) < tresshold)
            v[0] = U(0);

        if (abs(v[1]) < tresshold)
            v[1] = U(0);

        if (abs(v[2]) < tresshold)
            v[2] = U(0);
	}

	template<typename U2>
	constexpr static units::MutiplyResultType<U, U2> dotProduct(const Vector3<U>& v1, const Vector3<U2>& v2)
	{
		return v1.dotProduct(v2);
	}

	template<typename U2>
	constexpr units::MutiplyResultType<U, U2> dotProduct(const Vector3<U2>& v2) const
	{
		using ReturnType = ::units::MutiplyResultType<U, U2>;
		return ReturnType(v[0] * v2.x() + v[1] * v2.y() + v[2] * v2.z());
	}

	template<typename U2>
	constexpr static Vector3< units::MutiplyResultType<U, U2> > crossProduct(const Vector3<U>& v1, const Vector3<U2>& v2)
	{
		return v1.crossProduct(v2);
	}

	template<typename U2>
	constexpr Vector3< units::MutiplyResultType<U, U2> > crossProduct(const Vector3<U2>& v2) const
	{
		return Vector3< units::MutiplyResultType<U, U2> >(v[1] * v2.z() - v[2] * v2.y(),
			v[2] * v2.x() - v[0] * v2.z(),
			v[0] * v2.y() - v[1] * v2.x());
	}

	template<typename U2>
	constexpr static Vector3<Unitless<ValueType> > normal(const Vector3<U>& v1, const Vector3<U2>& v2)
	{
		return crossProduct(v1, v2).normalized();
	}

	//////////////////////////////////////
	// Assignation operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector3<U>& operator=(const Vector3<U2>& vec)
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
	constexpr Vector3<U>& operator+=(const Vector3<U2> &vector)
	{
		v[0] += vector.x();
		v[1] += vector.y();
		v[2] += vector.z();
		return *this;
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector3<U>& operator-=(const Vector3<U2> &vector)
	{
		v[0] -= vector.x();
		v[1] -= vector.y();
		v[2] -= vector.z();
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector3<U>& operator*=(const F& factor)
	{
		v[0] *= factor;
		v[1] *= factor;
		v[2] *= factor;
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector3<U>& operator/=(const F& divisor)
	{
		v[0] /= divisor;
		v[1] /= divisor;
		v[2] /= divisor;
		return *this;
	}


	//////////////////////////////////////
	// Equality operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator==(const Vector3<U2> &v2) const
	{
		return v[0] == v2.x() && v[1] == v2.y() && v[2] == v2.z();
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator!=(const Vector3<U2> &v2) const
	{
		return !(*this == v2);
	}


	//////////////////////////////////////
	// Addition/Substraction operators
	//////////////////////////////////////

	// Vector<U> + Vector<U2> -> Vector<U>, si U et U2 sont compatibles
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector3<U> operator+(const Vector3<U2> &v2) const
	{
		return Vector3<U>(v[0] + v2.x(), v[1] + v2.y(), v[2] + v2.z());
	}

	// Vector<U> - Vector<U2> -> Vector<U>, si U et U2 sont compatibles
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector3<U> operator-(const Vector3<U2> &v2) const
	{
		return Vector3<U>(v[0] - v2.x(), v[1] - v2.y(), v[2] - v2.z());
	}


	//////////////////////////////////////
	// Multiplication operators
	//////////////////////////////////////
	// Vector<U> *  Arithmetic -> Vector<U>
	/*template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector3<U> operator*(const F& factor) const
	{
		return Vector3<U>(v[0] * factor, v[1] * factor, v[2] * factor);
	}*/

	// Vector<U> * V -> Vector<U.V>
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value > > >
	constexpr Vector3< units::MutiplyResultType<U, V> > operator*(const V& u) const
	{
		using ReturnType = units::MutiplyResultType<U, V>;
		return Vector3<ReturnType>(v[0] * u, v[1] * u, v[2] * u);
	}

	
	//////////////////////////////////////
	// Division operators
	//////////////////////////////////////
	// Vector<U> / V -> Vector<U/V>
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value > > >
	constexpr Vector3< units::DivideResultType<U, V> > operator/(const V& u) const
	{
		using ReturnType = units::DivideResultType<U, V>;
		return Vector3<ReturnType>(v[0] / u, v[1] / u, v[2] / u);
	}

	// Vector<U> / Arithmetic -> Vector<U>
	/*template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector3<U> operator/(const F& divisor) const
	{
		return Vector3<U>(v[0] / divisor, v[1] / divisor, v[2] / divisor);
	}*/

	//////////////////////////////////////
	// Other operator functions
	//////////////////////////////////////
	constexpr Vector3<U> operator-() const
	{
		return Vector3<U>(-v[0], -v[1], -v[2]);
	}

	//////////////////////////////////////
	// Components functions
	//////////////////////////////////////
	template <typename U2>
	constexpr Vector3<U> ParallelComponent(const Vector3<U2>& v1) const
	{
		return (dotProduct(v1) / v1.lengthSquared()) * v1;
	}

	template <typename U2>
	constexpr Vector3<U> PerpendicularComponent(const Vector3<U2>& v1) const
	{
		return *this - ParallelComponent(v1);
	}

	// Transform to Unitless vector (remove units)
	constexpr Vector3<Unitless<ValueType> > ToUnitless() const
	{
		return Vector3<Unitless<ValueType>>(Unitless<ValueType>((ValueType)v[0]), Unitless<ValueType>((ValueType)v[1]), Unitless<ValueType>((ValueType)v[2]));
	}

private:
	constexpr Vector3(const U other[DIMENSION])
	{
		v[0] = other[0];
		v[1] = other[1];
		v[2] = other[2];
	}

	constexpr double dLenSquare() const
	{
		return (double)v[0].Value() * (double)v[0].Value() + (double)v[1].Value() * (double)v[1].Value() + (double)v[2].Value() * (double)v[2].Value();
	}

	U v[DIMENSION];
};


////////////////////////////////////////////
// Vector4 with component Unit
////////////////////////////////////////////
template<typename U>
class Vector4
{
public:
	using ValueType = ::units::GetValueType<U>;

	constexpr static uint32 DIMENSION = 4;

	constexpr Vector4()
		: v { U(), U(), U(), U() }
	{
	}

	constexpr Vector4(U xpos, U ypos, U zpos)
		: v { xpos, ypos, zpos, U() }
	{
		// Un Vector4 construit à partir d'un Vector3 a la composante W à 0
		// Si la composante W est à 1, ça devrait être un Point.
	}

    constexpr Vector4(U xpos, U ypos, U zpos, U wpos)
        : v{ xpos, ypos, zpos, wpos }
    {
    }

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Vector4(const Vector3<V>& u)
		: v{ u.x(), u.y(), u.z(), U() }
	{
		// Un Vector4 construit à partir d'un Vector3 a la composante W à 0
		// Si la composante W est à 1, ça devrait être un Point.
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Vector4(const Vector3<V>& u, V wpos)
		: v { u.x(), u.y(), u.z(), wpos }
	{
	}

	template<typename V, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, V> > >
	constexpr Vector4(const Vector4<V>& u)
		: v { u.x(), u.y(), u.z(), u.w() }
	{
	}

	constexpr const U* constData() const { return v; }
    constexpr const ValueType* constValues() const { return static_cast<const ValueType*>((void*)&v); }

	constexpr bool isNull() const
	{
        U tresshold = U(std::numeric_limits<ValueType>::epsilon());
		return abs(v[0]) < tresshold && abs(v[1]) < tresshold && abs(v[2]) < tresshold && abs(v[3]) < tresshold;
	}

	constexpr U x() const { return v[0]; }
	constexpr U y() const { return v[1]; }
	constexpr U z() const { return v[2]; }
	constexpr U w() const { return v[3]; }

	constexpr U& x() { return v[0]; }
	constexpr U& y() { return v[1]; }
	constexpr U& z() { return v[2]; }
	constexpr U& w() { return v[3]; }

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

	constexpr U length() const
	{
		return sqrt(lengthSquared());
	}

	constexpr units::MutiplyResultType<U, U> lengthSquared() const
	{
		return dotProduct(*this, *this);
	}

	constexpr Vector4<Unitless<ValueType> > normalized() const
	{
		double len = dLenSquare();

		if (len - 1.0 == 0.0)
			return Vector4<Unitless<ValueType>>(Unitless<ValueType>((ValueType)v[0]), Unitless<ValueType>((ValueType)v[1]), Unitless<ValueType>((ValueType)v[2]), Unitless<ValueType>((ValueType)v[3]));
		else if (len > 0.0)
			return *this / length();
		else
			return Vector4<Unitless<ValueType>>();
	}

	constexpr void correct()
	{
        U tresshold = U(std::numeric_limits<ValueType>::epsilon());
        if (abs(v[0]) < tresshold)
            v[0] = U(0);

        if (abs(v[1]) < tresshold)
            v[1] = U(0);

        if (abs(v[2]) < tresshold)
            v[2] = U(0);

        if (abs(v[3]) < tresshold)
            v[3] = U(0);
	}

	template<typename U2>
	constexpr static units::MutiplyResultType<U, U2> dotProduct(const Vector4<U>& v1, const Vector4<U2>& v2)
	{
		return v1.dotProduct(v2);
	}

	template<typename U2>
	constexpr units::MutiplyResultType<U, U2> dotProduct(const Vector4<U2>& v2) const
	{
		using ReturnType = units::MutiplyResultType<U, U2>;
		return ReturnType(v[0] * v2.x() + v[1] * v2.y() + v[2] * v2.z() + v[3] * v2.w());
	}

	//////////////////////////////////////
	// Assignation operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector4<U>& operator=(const Vector4<U2>& vec)
	{
		v[0] = vec.x();
		v[1] = vec.y();
		v[2] = vec.z();
		v[3] = vec.w();
		return *this;
	}

	//////////////////////////////////////
	// Coumpound assigment operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector4<U>& operator+=(const Vector4<U2> &vector)
	{
		v[0] += vector.x();
		v[1] += vector.y();
		v[2] += vector.z();
		v[3] += vector.w();
		return *this;
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector4<U>& operator-=(const Vector4<U2> &vector)
	{
		v[0] -= vector.x();
		v[1] -= vector.y();
		v[2] -= vector.z();
		v[3] -= vector.w();
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector4<U>& operator*=(const F& factor)
	{
		v[0] *= factor;
		v[1] *= factor;
		v[2] *= factor;
		v[3] *= factor;
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector4<U>& operator/=(const F& divisor)
	{
		v[0] /= divisor;
		v[1] /= divisor;
		v[2] /= divisor;
		v[3] /= divisor;
		return *this;
	}

	//////////////////////////////////////
	// Equality operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator==(const Vector4<U2> &v2) const
	{
		return v[0] == v2.x() && v[1] == v2.y() && v[2] == v2.z() && v[3] == v2.w();
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr bool operator!=(const Vector4<U2> &v2) const
	{
		return !(*this == v2);
	}


	//////////////////////////////////////
	// Addition/Substraction operators
	//////////////////////////////////////
	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector4<U> operator+(const Vector4<U2> &v2) const
	{
		return Vector4<U>(v[0] + v2.x(), v[1] + v2.y(), v[2] + v2.z(), v[3] + v2.w());
	}

	template<typename U2, typename = StaticUtilities::EnableIf< units::IsEquivalent<U, U2> > >
	constexpr Vector4<U> operator-(const Vector4<U2> &v2) const
	{
		return Vector4<U>(v[0] - v2.x(), v[1] - v2.y(), v[2] - v2.z(), v[3] - v2.w());
	}


	//////////////////////////////////////
	// Multiplication operators
	//////////////////////////////////////
	// Vector<U> *  double
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector4<U> operator*(const F& factor) const
	{
		return Vector4<U>(v[0] * factor, v[1] * factor, v[2] * factor, v[3] * factor);
	}

	// Vector<U> * V
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value > > >
	constexpr Vector4< units::MutiplyResultType<U, V> > operator*(const V& u) const
	{
		using ReturnType = units::MutiplyResultType<U, V>;
		return Vector4<ReturnType>(v[0] * u, v[1] * u, v[2] * u, v[3] * u);
	}

	
    //////////////////////////////////////
	// Division operators
	//////////////////////////////////////
	// Vector<U> / V
	template<typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<V>::Value, units::IsUnitless<V>::Value > > >
	constexpr Vector4< units::DivideResultType<U, V> > operator/(const V& u) const
	{
		using ReturnType = units::MutiplyResultType<U, V>;
		return Vector4<ReturnType>(v[0] / u, v[1] / u, v[2] / u, v[3] / u);
	}

	// Vector<U> / Arithmetic
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr Vector4<U> operator/(const F& divisor) const
	{
		return Vector4<U>(v[0] / divisor, v[1] / divisor, v[2] / divisor, v[3] / divisor);
	}

	//////////////////////////////////////
	// Other operator functions
	//////////////////////////////////////
	constexpr Vector4<U> operator-() const
	{
		return Vector4<U>(-v[0], -v[1], -v[2], -v[3]);
	}

	//////////////////////////////////////
	// Components functions
	//////////////////////////////////////
	template <typename U2>
	constexpr Vector4<U> ParallelComponent(const Vector4<U2>& v1) const
	{
		return (dotProduct(v1) / v1.lengthSquared()) * v1;
	}

	template <typename U2>
	constexpr Vector4<U> PerpendicularComponent(const Vector4<U2>& v1) const
	{
		return *this - ParallelComponent(v1);
	}

	// Transform to Unitless vector (remove units)
	constexpr Vector4<Unitless<ValueType>> ToUnitless() const
	{
		return Vector4<Unitless<ValueType>>(Unitless<ValueType>((ValueType)v[0]), Unitless<ValueType>((ValueType)v[1]), Unitless<ValueType>((ValueType)v[2]), Unitless<ValueType>((ValueType)v[3]));
	}

private:
	constexpr Vector4(const U other[DIMENSION])
	{
		v[0] = other[0];
		v[1] = other[1];
		v[2] = other[2];
		v[3] = other[3];
	}

	constexpr double dLenSquare() const
	{
		return (double)v[0].Value() * (double)v[0].Value() + (double)v[1].Value() * (double)v[1].Value() + (double)v[2].Value() * (double)v[2].Value() + (double)v[3].Value() * (double)v[3].Value();
	}

	U v[DIMENSION];
};


////////////////////////////////////////////
// Vector global operators
////////////////////////////////////////////
// Arithmetic * Vector2<U>
/*template<typename F, typename U, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
constexpr Vector2<U> operator*(const F& factor, const Vector2<U> &vector)
{
	return vector * factor;
}

// Arithmetic * Vector3<U>
template<typename F, typename U, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
constexpr Vector3<U> operator*(const F& factor, const Vector3<U> &vector)
{
	return vector * factor;
}

// Arithmetic * Vector4<U>
template<typename F, typename U, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
constexpr Vector4<U> operator*(const F& factor, const Vector4<U> &vector)
{
	return vector * factor;
}*/

// Unit * Vector2<U>
template<typename U, typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<U>::Value, ::units::IsUnitless<U>::Value> > >
constexpr Vector2< units::MutiplyResultType<V, U> > operator*(const U& u, const Vector2<V>& v)
{
	return v * u;
}

// Unit * Vector3<U>
template<typename U, typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<U>::Value, ::units::IsUnitless<U>::Value> > >
constexpr Vector3< units::MutiplyResultType<V, U> > operator*(const U& u, const Vector3<V>& v)
{
	return v * u;
}

// Unit * Vector4<U>
template<typename U, typename V, typename = StaticUtilities::EnableIf< StaticUtilities::Or<units::IsUnit<U>::Value, ::units::IsUnitless<U>::Value> > >
constexpr Vector4< units::MutiplyResultType<V, U> > operator*(const U& u, const Vector4<V>& v)
{
	return v * u;
}


template<typename U>
constexpr Vector2<U> abs(const Vector2<U>& v)
{
	return Vector2<U>(abs(v.x()), abs(v.y()));
}

template<typename U>
constexpr Vector3<U> abs(const Vector3<U>& v)
{
	return Vector3<U>(abs(v.x()), abs(v.y()), abs(v.z()));
}

template<typename U>
constexpr Vector4<U> abs(const Vector4<U>& v)
{
	return Vector4<U>(abs(v.x()), abs(v.y()), abs(v.z()), abs(v.w()));
}

template<typename S, typename V>
S& operator<<(S& os, const Vector2<V>& v)
{
	os << '(' << v.x();
	os << ", " << v.y();
	os << ')';
	return os;
}

template<typename S, typename V>
S& operator<<(S& os, const Vector3<V>& v)
{
	os << '(' << v.x();
	os << ", " << v.y();
	os << ", " << v.z();
	os << ')';
	return os;
}

template<typename S, typename V>
S& operator<<(S& os, const Vector4<V>& v)
{
	os << '(' << v.x();
	os << ", " << v.y();
	os << ", " << v.z();
	os << ", " << v.w();
	os << ')';
	return os;
}

#endif