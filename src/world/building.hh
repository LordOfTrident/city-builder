#ifndef BUILDING_HH__HEADER_GUARD__
#define BUILDING_HH__HEADER_GUARD__

#include "tile.hh"

#include "../units.hh"
#include "../texture.hh"

#define BUILDING_W    32
#define BUILDING_H    32
#define BUILDING_SIZE Vec2i(BUILDING_W, BUILDING_H)

namespace CityBuilder {

struct Building {
	Building(const Vec2i &p_pos, Size p_size, Dir p_dir = Dir::Up);

	Vec2i pos;
	Size  size;
	Dir   dir;
};

}

#endif
