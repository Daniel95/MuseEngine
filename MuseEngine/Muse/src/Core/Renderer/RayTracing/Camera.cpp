#include "MusePCH.h"

#include "Camera.h"

Camera::Camera(const vec3 & position, const vec3 & lookAt, int screenSizeX, int screenSizeY)
	: transform(*new Transform(position)), lookAt(lookAt), direction((lookAt - position).normalized()), screenSizeX(screenSizeX), screenSizeY(screenSizeY) { }

Camera::~Camera() { }

void Camera::GetScreenSize(int & xSize, int & ySize) const
{
	xSize = screenSizeX;
	ySize = screenSizeY;
}

void Camera::Move(vec3 movement)
{
	transform.SetPosition(transform.GetPosition() + movement);
}

void Camera::MoveLookAt(const vec3 & movement)
{
	lookAt += movement;
	direction = (lookAt - transform.GetPosition()).normalized();
}
