#include "MusePCH.h"

#include "Transform.h"

Transform::Transform(const vec3 & position, const vec3 & scale)
	: scale(scale)
{
	matrix.setIdentity();
	matrix.scale(scale);
	SetPosition(position);
}

Transform::~Transform() { }

vec3 Transform::GetPosition() const
{
	vec3 position = vec3(matrix.m4x4[3][0], matrix.m4x4[3][1], matrix.m4x4[3][2]);
	return position;
}

vec3 Transform::GetScale() const
{
	return scale;
}

void Transform::SetPosition(const vec3 & worldPosition)
{
	matrix.setRow(3, vec4(worldPosition.m_X, worldPosition.m_Y, worldPosition.m_Z, 1));
}

void Transform::Translate(const vec3 & translation)
{
	matrix.addTranslation(translation);
}

void Transform::Scale(const vec3 & scaleAddition)
{
	scale += scaleAddition;
	matrix.scale(scaleAddition);
}

void Transform::Rotate(const vec3 & eulerAngles)
{
	matrix.addEulerRotation(eulerAngles);
}

vec3 Transform::InverseTransformPoint(const vec3 & worldPoint) const
{
	vec4 transformed = matrix.inversed() * vec4(worldPoint.m_X, worldPoint.m_Y, worldPoint.m_Z, 1);
	return vec3(transformed.m_X, transformed.m_Y, transformed.m_Z);
}

vec3 Transform::InverseTransformVector(const vec3 & worldVector) const
{
	vec4 transformed = matrix.inversed() * vec4(worldVector.m_X, worldVector.m_Y, worldVector.m_Z, 0);
	return vec3(transformed.m_X, transformed.m_Y, transformed.m_Z);
}

vec3 Transform::TransformPoint(const vec3 & localPoint) const
{
	vec4 transformed = matrix * vec4(localPoint.m_X, localPoint.m_Y, localPoint.m_Z, 1);
	return vec3(transformed.m_X, transformed.m_Y, transformed.m_Z);
}

vec3 Transform::TransformVector(const vec3 & localVector) const
{
	vec4 transformed = matrix * vec4(localVector.m_X, localVector.m_Y, localVector.m_Z, 0);
	return vec3(transformed.m_X, transformed.m_Y, transformed.m_Z);
}