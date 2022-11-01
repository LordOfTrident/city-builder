#include "building.hh"

namespace CityBuilder {

Building::Building(const Vec2i &p_pos, Size p_size, Dir p_dir):
	pos(p_pos),
	size(p_size),
	dir(p_dir)
{}

}
