#include "camera.hh"

namespace CityBuilder {

Camera::Camera(const Vec2f &p_pos): pos(p_pos), zoom(1) {}

void Camera::Up() {
	pos.y -= CAM_SENSITIVITY / zoom;
}

void Camera::Right() {
	pos.x += CAM_SENSITIVITY / zoom;
}

void Camera::Left() {
	pos.x -= CAM_SENSITIVITY / zoom;
}

void Camera::Down() {
	pos.y += CAM_SENSITIVITY / zoom;
}

void Camera::Move(const Vec2f &p_off) {
	pos += p_off * DRAG_SENSITIVITY / zoom;
}

void Camera::ZoomIn() {
	zoom += ZOOM_SENSITIVITY * zoom;

	if (zoom > ZOOM_MAX)
		zoom = ZOOM_MAX;
}

void Camera::ZoomOut() {
	zoom -= ZOOM_SENSITIVITY * zoom;

	if (zoom < ZOOM_MIN)
		zoom = ZOOM_MIN;
}

}
