#ifndef SHEET_HH__HEADER_GUARD__
#define SHEET_HH__HEADER_GUARD__

#include "utils.hh"
#include "units.hh"
#include "texture.hh"

#include "world/tile.hh"

namespace CityBuilder {

class Sheet {
public:
	Sheet(const Vec2i &p_tileSize, Texture *p_sheet = nullptr);

	void SetSheet(Texture &p_sheet);
	void Render(Tile::ID p_id, const Rectf &p_dest);

private:
	Texture *m_sheet;
	Vec2i    m_tileSize;
};

}

#endif
