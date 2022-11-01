#ifndef WORLD_HH__HEADER_GUARD__
#define WORLD_HH__HEADER_GUARD__

#include <vector> // std::vector
#include <cmath>  // std::ceil

#include "../units.hh"

#include "camera.hh"
#include "tile.hh"
#include "building.hh"

namespace CityBuilder {

class World {
public:
	World(const Vec2i &p_size);

	void Render();

	Camera camera;
	Vec2i  size;

	std::vector<std::vector<Tile>> tiles;
	std::vector<Building>          buildings;
};

}

#endif
