#include "camera.h"
#include <glm/ext/matrix_transform.hpp>

cam::Camera::Camera(glm::vec3&& position, glm::vec3&& up)
	:
	view(glm::mat4{ 1.f }),
	this_pos(position),
	eye_pos({ 0,0,0 }),
	up_pos(up)
{
	view = glm::lookAt(this_pos, eye_pos, up);
}

glm::vec3 cam::Camera::position() const
{
	auto inverted = glm::inverse(view);
	return inverted[3];
}

void cam::Camera::translate(glm::vec3 const& v)
{
	view = glm::translate(view, v);
}

glm::vec3 cam::Camera::forward() const
{
	const glm::mat4 inverted = glm::inverse(view);
	return normalize(glm::vec3(inverted[2]));
}

glm::vec3 cam::Camera::right() const
{
	const glm::mat4 inverted = glm::inverse(view);
	return normalize(glm::vec3(inverted[0]));
}

void cam::Camera::update()
{
	view = glm::lookAt(this->position(), glm::vec3{ 0,0,0 } + (eye_pos), up_pos);
}

float cam::Camera::yaw() const
{
	glm::vec3 rotationDir = position() - eye_pos;
	rotationDir = glm::normalize(rotationDir);
	float yaw = atan2f(rotationDir.z, rotationDir.x);
	if (yaw < 0)
		yaw += 2 * 3.1415926535f;
	return yaw * 180 / 3.1415926535f;
}

float cam::Camera::pitch() const
{
	glm::vec3 rotationDir = position() - eye_pos;
	rotationDir = glm::normalize(rotationDir);
	float pitch = atan2f(rotationDir.z, rotationDir.y);
	if (pitch < 0)
		pitch += 2 * 3.1415926535f;
	return pitch * 180 / 3.1415926535f;
}

void cam::Camera::rotate(float_t yaw, float_t pitch)
{
	if (yaw != 0)
		eye_pos += glm::vec3{ glm::inverse(view)[0] } *(yaw * 0.15f);
	if (pitch != 0)
		eye_pos += glm::vec3{ glm::inverse(view)[1] } *(pitch * 0.15f);

	update();
}
