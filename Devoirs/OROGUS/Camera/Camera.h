#ifndef _CAMERA_CAMERA_H_
#define _CAMERA_CAMERA_H_

#include "../Utilities/Matrices.h"
#include "../Utilities/Point.h"
#include "../Utilities/Units.h"
#include "../Utilities/Vectors.h"

class Camera
{
    static Vector3<Real> s_worldUp;

	Degree m_fovy;
	Metre m_near;
	Metre m_far;
	Real m_ratio;
	Point3<Metre> m_position;
	Point3<Metre> m_lookAt;
	Vector3<Real> m_upVector;
	Degree m_pitchAngle;
	Metre m_zoom;
	bool m_modeFirstPerson;
public:
	static void SetWorldUp(const Vector3<Real>& up);

    Camera();

    Degree fieldOfView() const;
    Degree& fieldOfView();

    Metre near() const;
    Metre& near();

    Metre far() const;
    Metre& far();

    Real ratio() const;
    Real& ratio();

	bool isFirstPerson() const;
	void setCameraMode(bool firstPerson);

    const Point3<Metre>& position() const;
    Point3<Metre>& position();

    const Point3<Metre>& lookAt() const;
	Point3<Metre>& lookAt();

    const Vector3<Real>& upVector() const;
	Vector3<Real>& upVector();

    void reset();

    void strafe(Metre delta);
    void upDown(Metre delta);
    void forward(Metre delta);

    void yaw(Degree delta);
    void pitch(Degree delta);

    void setCoordinate(Point3<Metre> lookAt, Point3<Metre> position);

    Matrix4x4<Real> getPerspective() const;
    Matrix4x4<Real> getView() const;

    void log() const;
};

#endif