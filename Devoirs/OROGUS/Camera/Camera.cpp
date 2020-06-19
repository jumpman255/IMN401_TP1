#include "Camera.h"

#include "../Utilities/Maths.h"
#include "../Utilities/Transforms.h"

#include <iostream>

Vector3<Real> Camera::s_worldUp = Vector3<Real>(0, 1, 0);

void Camera::SetWorldUp(const Vector3<Real>& up)
{
	s_worldUp = up;
}

Camera::Camera()
	: m_modeFirstPerson(true)
{
    reset();
}

Degree Camera::fieldOfView() const
{
    return m_fovy;
}

Degree& Camera::fieldOfView() 
{ 
    return m_fovy; 
}

Metre Camera::near() const
{
    return m_near;
}

Metre& Camera::near()
{
    return m_near;
}

Metre Camera::far() const
{
    return m_far;
}

Metre& Camera::far()
{
    return m_far;
}

Real Camera::ratio() const
{
    return m_ratio;
}

Real& Camera::ratio()
{
    return m_ratio;
}

bool Camera::isFirstPerson() const
{
	return m_modeFirstPerson;
}

void Camera::setCameraMode(bool firstPerson)
{
	m_modeFirstPerson = firstPerson;
}

const Point3<Metre>& Camera::position() const
{
    return m_position;
}

Point3<Metre>& Camera::position()
{
    return m_position;
}

const Point3<Metre>& Camera::lookAt() const
{
	return m_lookAt;
}

Point3<Metre>& Camera::lookAt()
{
	return m_lookAt;
}

const Vector3<Real>& Camera::upVector() const
{
    return m_upVector;
}

Vector3<Real>& Camera::upVector()
{
	return m_upVector;
}

void Camera::reset()
{
	m_upVector = s_worldUp;
    setCoordinate(Point3<Metre>(Metre(0), Metre(0), Metre(0)), Point3<Metre>(Metre(0), Metre(0), Metre(-5)));
}

void Camera::strafe(Metre delta)
{
	Vector3<Real> toCam = (m_position - m_lookAt).normalized();
	Vector3<Real> side = upVector().crossProduct(toCam).normalized();
    m_position += side * delta;
	m_lookAt += side * delta;
}

void Camera::upDown(Metre delta)
{
	Vector3<Real> toCam = (m_position - m_lookAt).normalized();
	Vector3<Real> side = upVector().crossProduct(toCam).normalized();
	Vector3<Real> up = toCam.crossProduct(side).normalized();
    m_position += up * delta;
	m_lookAt += up * delta;
}

void Camera::forward(Metre delta)
{
	Vector3<Real> forward = (m_lookAt - m_position).normalized();
	if (m_modeFirstPerson)
	{
		m_position += forward * delta;
		m_lookAt += forward * delta;
	}
	else
	{
		// Wheel positive go to the reference point, so the zoom must diminish
		m_zoom -= delta;
		if (m_zoom < Metre(0.1f))
		{
			m_zoom = Metre(0.1f);
		}

		m_position = m_lookAt - forward * m_zoom;
	}
}

void Camera::yaw(Degree delta)
{
	if (m_modeFirstPerson)
	{
		Vector3<Real> forward = (m_lookAt - m_position).normalized();
		Vector3<Real> side = forward.crossProduct(upVector()).normalized();
		Vector3<Real> up = side.crossProduct(forward).normalized();
		Transform t = Transform::MakeRotation(delta, up);
		Vector3<Real> newDir = (t * forward).normalized();
		m_lookAt = m_position + newDir * m_zoom;
	}
	else
	{
		Vector3<Real> forward = (m_position - m_lookAt).normalized();
		Vector3<Real> side = forward.crossProduct(upVector()).normalized();
		Vector3<Real> up = side.crossProduct(forward).normalized();
		Transform t = Transform::MakeRotation(-delta, s_worldUp);
		Vector3<Real> newDir = (t * forward).normalized();
		m_position = m_lookAt + (newDir * m_zoom);
	}
}

void Camera::pitch(Degree delta)
{
	Degree oldPitch = m_pitchAngle;
	m_pitchAngle += delta;
	if (m_pitchAngle > Degree(89.9f) || m_pitchAngle < Degree(-89.9f))
	{
		m_pitchAngle = oldPitch;
		return;
	}

	if (m_modeFirstPerson)
	{
		Vector3<Real> forward = (m_lookAt - m_position).normalized();
		Vector3<Real> side = forward.crossProduct(upVector()).normalized();
		Transform t = Transform::MakeRotation(delta, side);
		Vector3<Real> newDir = (t * forward).normalized();
		m_lookAt = m_position + newDir * m_zoom;
	}
	else
	{
		Vector3<Real> forward = (m_position - m_lookAt).normalized();
		Vector3<Real> side = forward.crossProduct(upVector()).normalized();
		Transform t = Transform::MakeRotation(delta, side);
		Vector3<Real> newDir = (t * forward).normalized();
		m_position = m_lookAt + (newDir * m_zoom);
	}
}

void Camera::setCoordinate(Point3<Metre> lookAt, Point3<Metre> position)
{
	m_zoom = (position - lookAt).length();
    m_position = position;
	m_lookAt = lookAt;
	m_pitchAngle = Maths::Arcsin((position.y() - lookAt.y()) / m_zoom);
}

Matrix4x4<Real> Camera::getPerspective() const
{
	// RH
	Real g = 1.0f / Maths::Tan(m_fovy * 0.5f);
	Real k = -(m_far + m_near) / (m_far - m_near);
	Metre k2 = (-2 * m_far * m_near) / (m_far - m_near);
	return Matrix4x4<Real>(g / m_ratio, Real(), Real(), Real(),
		Real(), g, Real(), Real(),
		Real(), Real(), k, k2.ToUnitless(),
		Real(), Real(), Real(-1), Real());

	// LH
	/*Real g = 1.0f / Maths::Tan(m_fovy * 0.5f);
	Real k = (m_far + m_near) / (m_far - m_near);
	Metre k2 = (-2 * m_far * m_near) / (m_far - m_near);
	return Matrix4x4<Real>(g / m_ratio, Real(), Real(), Real(),
		Real(), g, Real(), Real(),
		Real(), Real(), k, k2.ToUnitless(),
		Real(), Real(), Real(1), Real());*/
}

Matrix4x4<Real> Camera::getView() const
{
	// RH
	Vector3<Real> f((lookAt() - position()).normalized()); // Forward vector
	Vector3<Real> s(f.crossProduct(upVector()).normalized()); // Right vector
	Vector3<Real> u(s.crossProduct(f).normalized()); // Up vector

	// Mathématiquement parlant, les valeurs X, Y et Z de la translation devrait être en unité, et tout le reste en Unitless
	// Parce que l'implémentation de la matrice implique le même type pour chaque valeur, il faut "fudger" les maths :
	// - Les coodonnées homogènes du Vector4 sont en unité, sauf la valeur de W qui devrait être sans unité 
	// - L'implémentation aura la coordonnée homogène en unité, donc on doit enlever l'unité de la translation dans la matrice
	// - La multiplication donnera donc la bonne unité, comme si c'était placé correctement.
	const Vector3<Metre> eye = Vector3<Metre>(position().x(), position().y(), position().z());
	Matrix4x4<Real> result = Matrix4x4<Real>(s.x(),  s.y(),  s.z(), -(s.dotProduct(eye).ToUnitless()),
											 u.x(),  u.y(),  u.z(), -(u.dotProduct(eye).ToUnitless()),
											-f.x(), -f.y(), -f.z(), (f.dotProduct(eye).ToUnitless()),
											 Real(), Real(), Real(), Real(1));

	// LH
    /*Vector3<Real> f((lookAt() - position()).normalized()); // Forward vector
    Vector3<Real> s(upVector().crossProduct(f).normalized()); // Right vector
    Vector3<Real> u(f.crossProduct(s).normalized()); // Up vector

    // Mathématiquement parlant, les valeurs X, Y et Z de la translation devrait être en unité, et tout le reste en Unitless
    // Parce que l'implémentation de la matrice implique le même type pour chaque valeur, il faut "fudger" les maths :
    // - Les coodonnées homogènes du Vector4 sont en unité, sauf la valeur de W qui devrait être sans unité 
    // - L'implémentation aura la coordonnée homogène en unité, donc on doit enlever l'unité de la translation dans la matrice
    // - La multiplication donnera donc la bonne unité, comme si c'était placé correctement.
    const Vector3<Metre> eye = Vector3<Metre>(position().x(), position().y(), position().z());
    Matrix4x4<Real> result = Matrix4x4<Real>(s.x(),  s.y(),  s.z(),  -s.dotProduct(eye).ToUnitless(),
                                             u.x(),  u.y(),  u.z(),  -u.dotProduct(eye).ToUnitless(),
                                             f.x(),  f.y(),  f.z(),  -f.dotProduct(eye).ToUnitless(),
                                             Real(), Real(), Real(), Real(1));*/
    return result;
}

void Camera::log() const
{
    std::cout << "==============================" << std::endl;
	std::cout << "Camera (";
	if (isFirstPerson())
	{
		std::cout << "First person";
	}
	else
	{
		std::cout << "Third person";
	}
	std::cout << ")" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "Position : " << position() << std::endl;
    std::cout << "LookAt : " << lookAt() << std::endl;
	std::cout << "Up : " << upVector() << std::endl;
	std::cout << "Zoom : " << m_zoom << std::endl;
    std::cout << "View : " << std::endl << getView() << std::endl;
    std::cout << "==============================" << std::endl;
}