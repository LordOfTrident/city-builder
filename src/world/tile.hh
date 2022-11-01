#ifndef TILE_HH__HEADER_GUARD__
#define TILE_HH__HEADER_GUARD__

#include "../utils.hh"

#define TILE_W    64
#define TILE_H    32
#define TILE_SIZE Vec2i(TILE_W, TILE_H)

namespace CityBuilder {

enum Size {
	S1x1 = 0,
	S2x1,
	S2x2
};

enum class Dir {
	Up = 0,
	Right,
	Down,
	Left
};

struct Building;

struct Tile {
	using ID = size_t;

	enum Type {
		Grass = 0,
		Dirt,
		Sand,
		Stone,
		Water,

		Count
	};

	Tile(Type p_type = Grass, bool p_canPlaceOn = true);

	ID GetID();

	bool      canPlaceOn;
	Building *building;
	Type      type;
};

}

#endif
