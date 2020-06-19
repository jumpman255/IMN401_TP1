#ifndef _UTILITIES_COLOR_H_
#define _UTILITIES_COLOR_H_

#include "Types.h"
#include "StaticUtilities.h"

#include <iostream>

class Color;
class ColorRGB;

class ColorRGB
{
public:
	static ColorRGB Black()
	{
		return ColorRGB();
	}

	static ColorRGB White()
	{
		return ColorRGB(1.0f, 1.0f, 1.0f);
	}

	static ColorRGB Red()
	{
		return ColorRGB(1.0f, 0.0f, 0.0f);
	}

	static ColorRGB Green()
	{
		return ColorRGB(0.0f, 1.0f, 0.0f);
	}

	static ColorRGB Blue()
	{
		return ColorRGB(0.0f, 0.0f, 1.0f);
	}

	constexpr ColorRGB()
		: ColorRGB(0.0f, 0.0f, 0.0f)
	{
	}

	constexpr ColorRGB(uint8 r, uint8 g, uint8 b)
		: ColorRGB(r / 255.0f, g / 255.0f, b / 255.0f)
	{
	}

	constexpr ColorRGB(float r, float g, float b)
		: m_data{ r, g, b }
	{
	}

	constexpr ColorRGB(const ColorRGB& u)
		: ColorRGB(u.r(), u.g(), u.b())
	{
	}

	constexpr const float* constData() const { return m_data; }

	constexpr bool isBlack() const
	{
		return m_data[0] == 0.0f && m_data[1] == 0.0f && m_data[2] == 0.0f;
	}

	constexpr float r() const { return m_data[0]; }
	constexpr float g() const { return m_data[1]; }
    constexpr float b() const { return m_data[2]; }

	constexpr void setR(float aR) { m_data[0] = aR; }
	constexpr void setG(float aG) { m_data[1] = aG; }
	constexpr void setB(float aB) { m_data[2] = aB; }

	
	//////////////////////////////////////
	// Assignation operators
	//////////////////////////////////////
	constexpr ColorRGB& operator=(const ColorRGB& c)
	{
		m_data[0] = c.r();
		m_data[1] = c.g();
		m_data[2] = c.b();
		return *this;
	}

	//////////////////////////////////////
	// Coumpound assigment operators
	//////////////////////////////////////
	
	constexpr ColorRGB &operator+=(const ColorRGB &c)
	{
		m_data[0] += c.r();
		m_data[1] += c.g();
		m_data[2] += c.b();
		return *this;
	}

	constexpr ColorRGB &operator-=(const ColorRGB &c)
	{
		m_data[0] -= c.r();
		m_data[1] -= c.g();
		m_data[2] -= c.b();
		return *this;
	}

	constexpr ColorRGB &operator*=(const ColorRGB &c)
	{
		m_data[0] *= c.r();
		m_data[1] *= c.g();
		m_data[2] *= c.b();
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr ColorRGB& operator*=(const F& factor)
	{
		m_data[0] *= factor;
		m_data[1] *= factor;
		m_data[2] *= factor;
		return *this;
	}

	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr ColorRGB& operator/=(const F& divisor)
	{
		m_data[0] /= divisor;
		m_data[1] /= divisor;
		m_data[2] /= divisor;
		return *this;
	}

	//////////////////////////////////////
	// Equality operators
	//////////////////////////////////////
	constexpr bool operator==(const ColorRGB &c) const
	{
		return r() == c.r() && g() == c.g() && b() == c.b();
	}

	constexpr bool operator!=(const ColorRGB &c) const
	{
		return !(*this == c);
	}


	//////////////////////////////////////
	// Addition/Substraction operators
	//////////////////////////////////////
	constexpr ColorRGB operator+(const ColorRGB &c) const
	{
		return ColorRGB(r() + c.r(), g() + c.g(), b() + c.b());
	}

	constexpr ColorRGB operator-(const ColorRGB &c) const
	{
		return ColorRGB(r() - c.r(), g() - c.g(), b() - c.b());
	}


	//////////////////////////////////////
	// Multiplication operators
	//////////////////////////////////////
	// Vector<U> *  double
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr ColorRGB operator*(const F& factor) const
	{
		return ColorRGB(r() * factor, g() * factor, b() * factor);
	}

	constexpr ColorRGB operator*(const ColorRGB &c) const
	{
		return ColorRGB(r() * c.r(), g() * c.g(), b() * c.b());
	}

	//////////////////////////////////////
	// Division operators
	//////////////////////////////////////

	// Vector<U> / double
	template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
	constexpr ColorRGB operator/(const F& divisor)
	{
		return ColorRGB(r() / divisor, g() / divisor, b() / divisor);
	}

    friend std::ostream& operator<<(std::ostream& os, const ColorRGB& c)
    {
        os << "(r:" << c.r() << ", g:" << c.g() << " b:" << c.b() << ")";
        return os;
    }

private:
	float m_data[3]; // RGB format
};


class Color
{
public:
    static Color Black()
    {
        return Color();
    }

    static Color White()
    {
        return Color(1.0f, 1.0f, 1.0f);
    }

    static Color Red()
    {
        return Color(1.0f, 0.0f, 0.0f);
    }

    static Color Green()
    {
        return Color(0.0f, 1.0f, 0.0f);
    }

    static Color Blue()
    {
        return Color(0.0f, 0.0f, 1.0f);
    }

    constexpr Color()
        : Color(0.0f, 0.0f, 0.0f, 1.0f)
    {
    }

    constexpr Color(uint8 r, uint8 g, uint8 b)
        : Color(r, g, b, 1.0f)
    {
    }

    constexpr Color(float r, float g, float b)
        : Color(r, g, b, 1.0f)
    {
    }

    constexpr Color(uint8 r, uint8 g, uint8 b, uint8 a)
        : Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f)
    {
    }

    constexpr Color(uint8 r, uint8 g, uint8 b, float a)
        : Color(r / 255.0f, g / 255.0f, b / 255.0f, a)
    {
    }

    constexpr Color(float r, float g, float b, float a)
        : m_data{ r, g, b, a }
    {
    }

    constexpr Color(const ColorRGB& colorRGB)
        : Color(colorRGB.r(), colorRGB.g(), colorRGB.b(), 1.0f)
    {
    }

    constexpr Color(const Color& u)
        : Color(u.r(), u.g(), u.b(), u.a())
    {
    }

    constexpr const float* constData() const { return m_data; }

    constexpr bool isBlack() const
    {
        return m_data[0] == 0.0f && m_data[1] == 0.0f && m_data[2] == 0.0f && m_data[3] == 1.0f;
    }

    constexpr float r() const { return m_data[0]; }
    constexpr float g() const { return m_data[1]; }
    constexpr float b() const { return m_data[2]; }
    constexpr float a() const { return m_data[3]; }

    constexpr void setR(float aR) { m_data[0] = aR; }
    constexpr void setG(float aG) { m_data[1] = aG; }
    constexpr void setB(float aB) { m_data[2] = aB; }
    constexpr void setA(float aA) { m_data[3] = aA; }


    //////////////////////////////////////
    // Assignation operators
    //////////////////////////////////////
    constexpr Color& operator=(const Color& c)
    {
        m_data[0] = c.r();
        m_data[1] = c.g();
        m_data[2] = c.b();
        m_data[3] = c.a();
        return *this;
    }

    //////////////////////////////////////
    // Coumpound assigment operators
    //////////////////////////////////////

    constexpr Color &operator+=(const Color &c)
    {
        m_data[0] += c.r();
        m_data[1] += c.g();
        m_data[2] += c.b();
        m_data[3] += c.a();
        return *this;
    }

    constexpr Color &operator-=(const Color &c)
    {
        m_data[0] -= c.r();
        m_data[1] -= c.g();
        m_data[2] -= c.b();
        m_data[3] -= c.a();
        return *this;
    }

    constexpr Color &operator*=(const Color &c)
    {
        m_data[0] *= c.r();
        m_data[1] *= c.g();
        m_data[2] *= c.b();
        m_data[3] *= c.a();
        return *this;
    }

    template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
    constexpr Color& operator*=(const F& factor)
    {
        m_data[0] *= factor;
        m_data[1] *= factor;
        m_data[2] *= factor;
        m_data[3] *= factor;
        return *this;
    }

    template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
    constexpr Color& operator/=(const F& divisor)
    {
        m_data[0] /= divisor;
        m_data[1] /= divisor;
        m_data[2] /= divisor;
        m_data[3] /= divisor;
        return *this;
    }

    //////////////////////////////////////
    // Equality operators
    //////////////////////////////////////
    constexpr bool operator==(const Color &c) const
    {
        return r() == c.r() && g() == c.g() && b() == c.b() && a() == c.a();
    }

    constexpr bool operator!=(const Color &c) const
    {
        return !(*this == c);
    }


    //////////////////////////////////////
    // Addition/Substraction operators
    //////////////////////////////////////
    constexpr Color operator+(const Color &c) const
    {
        return Color(r() + c.r(), g() + c.g(), b() + c.b(), a() + c.a());
    }

    constexpr Color operator-(const Color &c) const
    {
        return Color(r() - c.r(), g() - c.g(), b() - c.b(), a() - c.a());
    }


    //////////////////////////////////////
    // Multiplication operators
    //////////////////////////////////////
    // Vector<U> *  double
    template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
    constexpr Color operator*(const F& factor) const
    {
        return Color(r() * factor, g() * factor, b() * factor, a() * factor);
    }

    constexpr Color operator*(const Color &c) const
    {
        return Color(r() * c.r(), g() * c.g(), b() * c.b(), a() * c.a());
    }

    //////////////////////////////////////
    // Division operators
    //////////////////////////////////////

    // Vector<U> / double
    template<typename F, typename = StaticUtilities::EnableIf< StaticUtilities::IsArithmetic<F> > >
    constexpr Color operator/(const F& divisor)
    {
        return Color(r() / divisor, g() / divisor, b() / divisor, a() / divisor);
    }

    friend std::ostream& operator<<(std::ostream& os, const Color& c)
    {
        os << "(r:" << c.r() << ", g:" << c.g() << " b:" << c.b() << ", a:" << c.a() << ")";
        return os;
    }

private:
    float m_data[4]; // RGBA format
};

#endif

