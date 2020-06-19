#ifndef _UTILITIES_TRANSFORMS_H_
#define _UTILITIES_TRANSFORMS_H_

#include "Maths.h"
#include "Matrices.h"
#include "Units.h"
#include "Vectors.h"

class Transform : public Matrix4x4<Real>
{
    // Mathématiquement parlant, les unités de la matrice de transformation devraient être au format suivant :
    // --                     --
    // | Real, Real, Real,  U  |
    // | Real, Real, Real,  U  |
    // | Real, Real, Real,  U  |
    // | Real, Real, Real, Real|
    // --                     --
    //
    // Le format d'unité d'un Vector4 devrait être :
    // --   --
    // |  U  |
    // |  U  |
    // |  U  |
    // | Real|
    // --   --
    // Comme chaque composante de la matrice ou du vecteur doivent être la même, on garde la composante W du vecteur en unité U
    // et on met les unités U de la matrice en Real pour obtenir le même résultat lors de la multiplication.

public:
	static Transform MakeTranslation(const Vector3<Metre>& t)
	{
		
		return Transform(Real(1), Real(),  Real(),  t.x().ToUnitless(),
                               Real(),  Real(1), Real(),  t.y().ToUnitless(),
                               Real(),  Real(),  Real(1), t.z().ToUnitless(),
                               Real(),  Real(),  Real(),  Real(1));
	}

    static Transform MakeTranslation(const Point3<Metre>& p)
    {

        return Transform(Real(1), Real(), Real(), p.x().ToUnitless(),
            Real(), Real(1), Real(), p.y().ToUnitless(),
            Real(), Real(), Real(1), p.z().ToUnitless(),
            Real(), Real(), Real(), Real(1));
    }

    static Matrix3x3<Real> CreateRotationXMatrix(Degree angle)
    {
        Real c = Maths::Cos(angle);
        Real s = Maths::Sin(angle);

        return Matrix3x3<Real>(Real(1), Real(), Real(),
            Real(), c, -s, 
            Real(), s, c);
    }

    static Matrix3x3<Real> CreateRotationYMatrix(Degree angle)
    {
        Real c = Maths::Cos(angle);
        Real s = Maths::Sin(angle);

        return Matrix3x3<Real>(c, Real(), s,
            Real(), Real(1), Real(), 
            -s, Real(), c);
    }

    static Matrix3x3<Real> CreateRotationZMatrix(Degree angle)
    {
        Real c = Maths::Cos(angle);
        Real s = Maths::Sin(angle);

        return Matrix3x3<Real>(c, -s, Real(),
            s, c, Real(), 
            Real(), Real(), Real(1));
    }

    static Matrix3x3<Real> CreateRotationMatrix(Degree angle, const Vector3<Real>& axe)
    {
        Real c = Maths::Cos(angle);
        Real s = Maths::Sin(angle);
        Real d = 1.0f - c;

        Real x = axe.x() * d;
        Real y = axe.y() * d;
        Real z = axe.z() * d;

        Real axay = x * axe.y();
        Real axaz = x * axe.z();
        Real ayaz = y * axe.z();

        return Matrix3x3<Real>(c + x * axe.x(), axay - s * axe.z(), axaz + s * axe.y(),
            axay + s * axe.z(), c + y * axe.y(), ayaz - s * axe.x(), 
            axaz - s * axe.y(), ayaz + s * axe.x(), c + z * axe.z());
    }

	static Transform MakeRotationX(Degree angle)
	{
		Real c = Maths::Cos(angle);
		Real s = Maths::Sin(angle);

		return Transform(Real(1), Real(), Real(), Real(),
                               Real(), c, -s, Real(),
                               Real(), s, c, Real(),
                               Real(), Real(), Real(), Real(1));
	}

    static Transform MakeRotationY(Degree angle)
    {
        Real c = Maths::Cos(angle);
        Real s = Maths::Sin(angle);

        return Transform(c, Real(), s, Real(),
                               Real(), Real(1), Real(), Real(),
                               -s, Real(), c, Real(),
                               Real(), Real(), Real(), Real(1));
    }

    static Transform MakeRotationZ(Degree angle)
    {
        Real c = Maths::Cos(angle);
        Real s = Maths::Sin(angle);

        return Transform(c, -s, Real(), Real(),
                                s, c, Real(), Real(),
                                Real(), Real(), Real(1), Real(),
                                Real(), Real(), Real(), Real(1));
    }

    static Transform MakeRotation(Degree angle, const Vector3<Real>& axe)
    {
        Real c = Maths::Cos(angle);
        Real s = Maths::Sin(angle);
        Real d = 1.0f - c;

        Real x = axe.x() * d;
        Real y = axe.y() * d;
        Real z = axe.z() * d;

        Real axay = x * axe.y();
        Real axaz = x * axe.z();
        Real ayaz = y * axe.z();

        return Transform(c + x * axe.x(), axay - s * axe.z(), axaz + s * axe.y(), Real(),
                               axay + s * axe.z(), c + y * axe.y(), ayaz - s * axe.x(), Real(),
                               axaz - s * axe.y(), ayaz + s * axe.x(), c + z * axe.z(), Real(),
                               Real(), Real(), Real(), Real(1));
    }

    static Transform MakeRotation(const Vector3<Real>& axe, Real cosAngle, Real sinAngle)
    {
        Real c = cosAngle;
        Real s = sinAngle;
        Real d = 1.0f - c;

        Real x = axe.x() * d;
        Real y = axe.y() * d;
        Real z = axe.z() * d;

        Real axay = x * axe.y();
        Real axaz = x * axe.z();
        Real ayaz = y * axe.z();

        return Transform(c + x * axe.x(), axay - s * axe.z(), axaz + s * axe.y(), Real(),
            axay + s * axe.z(), c + y * axe.y(), ayaz - s * axe.x(), Real(),
            axaz - s * axe.y(), ayaz + s * axe.x(), c + z * axe.z(), Real(),
            Real(), Real(), Real(), Real(1));
    }

    static Transform MakeReflexion(const Vector3<Real>& a)
    {
        Real x = a.x() * Real(-2);
        Real y = a.y() * Real(-2);
        Real z = a.z() * Real(-2);

        Real axay = x * a.y();
        Real axaz = x * a.z();
        Real ayaz = y * a.z();

        return Transform(x * a.x() + Real(1), axay, axaz, Real(),
                               axay, y * a.y() + Real(1), ayaz, Real(),
                               axaz, ayaz, z * a.z() + Real(1), Real(),
                               Real(), Real(), Real(), Real(1));
    }

    static Transform MakeScale(const Vector3<Real>& s)
    {
        return Transform(s.x(), Real(), Real(), Real(),
            Real(), s.y(), Real(), Real(),
            Real(), Real(), s.z(), Real(),
            Real(), Real(), Real(), Real(1));
    }

    static Transform MakeScale(Real sx, Real sy, Real sz)
    {
        return Transform(sx, Real(), Real(), Real(),
            Real(), sy, Real(), Real(),
            Real(), Real(), sz, Real(),
            Real(), Real(), Real(), Real(1));
    }

    static Transform MakeScale(Real s, const Vector3<Real>& a)
    {
        s -= Real(1);
        Real x = a.x() * s;
        Real y = a.y() * s;
        Real z = a.z() * s;
        Real axay = x * a.y();
        Real axaz = x * a.z();
        Real ayaz = y * a.z();

        return Transform(x * a.x() + Real(1), axay, axaz, Real(),
            axay, y * a.y() + Real(1), ayaz, Real(),
            axaz, ayaz, z * a.z() + Real(1), Real(),
            Real(), Real(), Real(), Real(1));
    }

    static Transform MakeSkew(Degree d, const Vector3<Real>& a, const Vector3<Real>& b)
    {
        Real t = Maths::Tan(d);
        Real x = a.x() * t;
        Real y = a.y() * t;
        Real z = a.z() * t;
        
        return Transform(x * b.x() + Real(1), x * b.y(), x * b.z(), Real(),
            y * b.x(), y * b.y() + Real(1), y * b.z(), Real(),
            z * b.x(), z * b.y(), z * b.z() + Real(1), Real(),
            Real(), Real(), Real(), Real(1));
    }

	static Transform MakeShearX(Real shear)
	{
		// TP2 : À compléter
		return Transform();
	}

	static Transform MakeShearY(Real shear)
	{
		// TP2 : À compléter
		return Transform();
	}

	static Transform MakeShearZ(Real shear)
	{
		// TP2 : À compléter
		return Transform();
	}

    Transform()
        : Matrix4x4<Real>(1)
    {
    }

	Transform(Real diagonale)
		: Matrix4x4<Real>(diagonale)
	{
	}

    Transform(Real m00, Real m01, Real m02, Real m03, Real m10, Real m11, Real m12, Real m13, Real m20, Real m21, Real m22, Real m23, Real m30, Real m31, Real m32, Real m33)
        : Matrix4x4<Real>(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
    {
    }

    Transform(const Vector3<Real>& scale, const Matrix3x3<Real>& rot, const Vector3<Metre>& translation)
        : Matrix4x4<Real>(scale.x() * rot.at(0, 0), rot.at(0, 1), rot.at(0, 2), translation.x().ToUnitless(), rot.at(1, 0), scale.y() * rot.at(1, 1), rot.at(1, 2), translation.y().ToUnitless(), rot.at(2, 0), rot.at(2, 1), scale.z() * rot.at(2, 2), translation.z().ToUnitless(), Real(), Real(), Real(), Real(1))
    {
    }

    Transform(const Matrix4x4<Real>& m)
        : Matrix4x4<Real>(m)
    {
    }

    Vector3<Metre> getTranslation() const
    {
        return Vector3<Metre>(at(0, 3).ToUnit<Metre>(), at(1, 3).ToUnit<Metre>(), at(2, 3).ToUnit<Metre>());
    }
};

#endif 
