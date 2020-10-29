#include "Camera.h"

#include <GLM/gtc/matrix_transform.hpp>

Camera::Camera() :
	_nearPlane(0.1f),
	_farPlane(1000.0f),
	_fovRadians(glm::radians(90.0f)),
	_aspectRatio(1.0f),
	_position(glm::vec3(0.0f)),
	_normal(glm::vec3(0.0f, 0.0f, 1.0f)),
	_up(glm::vec3(0.0f, 1.0f, 0.0f)), // Using Y-up coordinate system by default
	_view(glm::mat4(1.0f)),
	_projection(glm::mat4(1.0f)),
	_viewProjection(glm::mat4(1.0f)),
	_leftPlane(-1.0f),
	_rightPlane(1.0f),
	_topPlane(1.0f),
	_bottomPlane(-1.0f),
	_isDirty(true)
{
	__CalculateProjection();
}

void Camera::SetPosition(const glm::vec3& position) {
	_position = position;
	__CalculateView();
}

void Camera::SetForward(const glm::vec3& forward) {
	_normal = forward;
	__CalculateView();
}

void Camera::LookAt(const glm::vec3& point) {
	_normal = -glm::normalize(_position - point);
	__CalculateView();
}

void Camera::SetUp(const glm::vec3& up) {
	_up = up;
	__CalculateView();
}

void Camera::ResizeWindow(int windowWidth, int windowHeight) {
	_aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	__CalculateProjection();
}

void Camera::SetFovRadians(float value) {
	_fovRadians = value;
	__CalculateProjection();
}

void Camera::SetFovDegrees(float value) {
	SetFovRadians(glm::radians(value));
}

void Camera::SetProjectionType(bool perspective)
{
	if (perspective)
		isPerspective = false;

	else if (!perspective)
		isPerspective = true;

	__CalculateProjection();
}

const glm::mat4& Camera::GetViewProjection() const {
	if (_isDirty && isPerspective) {
		_viewProjection = _projection * _view;
		_isDirty = false;
	}
	else if (_isDirty && !isPerspective)
	{
		_viewProjection = _projection1 * _view;
		_isDirty = false;
	}
	return _viewProjection;
}

void Camera::__CalculateProjection() {
	if (isPerspective)
	{
		_projection = glm::perspective(_fovRadians, _aspectRatio, _nearPlane, _farPlane);
		_isDirty = true;
	}
	else
	{
		_projection1 = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, _nearPlane, _farPlane);
		_isDirty = true;
		printf("ortho");
	}
}

void Camera::__CalculateView() {
	_view = glm::lookAt(_position, _position + _normal, _up);
	_isDirty = true;
}
