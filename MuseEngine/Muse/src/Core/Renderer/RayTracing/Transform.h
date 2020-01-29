#pragma once

#include "vec3.h"
#include "mat4.h"

class Scene;

class Transform
{
public:
	Transform(const vec3 & position = vec3(0, 0, 0), const vec3 & scale = vec3(1, 1, 1));
	virtual ~Transform();

	vec3 GetPosition() const;
	vec3 GetScale() const;
	void SetPosition(const vec3 & worldPosition);
	void Translate(const vec3 & translation);
	void Scale(const vec3 & scale);
	void Rotate(const vec3 & eulerAngles);
	//World to local point 
	vec3 InverseTransformPoint(const vec3 & worldPoint) const;
	//World to local vector
	vec3 InverseTransformVector(const vec3 & worldVector) const;
	//Local to world point
	vec3 TransformPoint(const vec3 & localPoint) const;
	//Local to world vector
	vec3 TransformVector(const vec3 & localVector) const;

private:
	mat4 matrix;
	vec3 scale;

};