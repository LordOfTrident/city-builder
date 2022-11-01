#include "tile.hh"

namespace CityBuilder {

Tile::Tile(Type p_type, bool p_canPlaceOn):
	canPlaceOn(p_canPlaceOn),
	building(nullptr),
	type(p_type)
{}

Tile::ID Tile::GetID() {
	return static_cast<Tile::ID>(type);
}

}
