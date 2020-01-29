#include "MusePCH.h"

#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const vec3 & position, const vec3 & lookAt, int screenSizeX, int screenSizeY, float vfov, const vec3 & up)
	: Camera(position, lookAt, screenSizeX, screenSizeY), up(up)
{
	direction = (lookAt - position).normalized();

	const float aspect = static_cast<float>(screenSizeX) / static_cast<float>(screenSizeY);
	const float theta = static_cast<float>(vfov * M_PI / 180);
	size.m_Y = tan(theta);
	size.m_X = aspect * size.m_Y;

	UpdateLookingPlane();
}

PerspectiveCamera::~PerspectiveCamera() { }

const std::shared_ptr<Ray> PerspectiveCamera::GetLookingRay(float pixelX, float pixelY) const
{
	std::shared_ptr<Ray> ray = std::make_shared<Ray>();

	ray->Origin = transform.GetPosition();

	const float invertedPixelY = abs(pixelY - static_cast<float>(screenSizeY));

	const float x = pixelX / static_cast<float>(screenSizeX);
	const float y = invertedPixelY / static_cast<float>(screenSizeY);

	const vec3 lookingPlanePoint = lowerLeftCorner + x * horizontal + y * vertical;
	ray->Direction = (lookingPlanePoint - transform.GetPosition()).normalized();

	return ray;
}

void PerspectiveCamera::Move(vec3 movement)
{
	movement.m_X *= -1;

	Camera::Move(movement);
	UpdateLookingPlane();
}

void PerspectiveCamera::MoveLookAt(const vec3 & movement)
{
	lookAt += movement;
	direction = (lookAt - transform.GetPosition()).normalized();

	UpdateLookingPlane();
}

void PerspectiveCamera::UpdateLookingPlane()
{
	const vec3 rightDirection = (up.cross(direction)).normalized();
	const vec3 upDirection = direction.cross(rightDirection);
	horizontal = size.m_X * rightDirection;
	vertical = size.m_Y * upDirection;
	lowerLeftCorner = transform.GetPosition() - horizontal * 0.5f - vertical * 0.5f + direction;
}

