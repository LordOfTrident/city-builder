#ifndef CAMERA_HH__HEADER_GUARD__
#define CAMERA_HH__HEADER_GUARD__

#include "../units.hh"
#include "../utils.hh"

#include "../main/config.hh"

namespace CityBuilder {

class Camera {
public:
	Camera(const Vec2f &p_pos = Vec2f());

	void Up();
	void Right();
	void Left();
	void Down();

	void Move(const Vec2f &p_off);

	void ZoomIn();
	void ZoomOut();

	Vec2f pos;
	float zoom;
};

}

#endif
