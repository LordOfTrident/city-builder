#include "sheet.hh"

namespace CityBuilder {

Sheet::Sheet(const Vec2i &p_tileSize, Texture *p_sheet):
	m_sheet(p_sheet),
	m_tileSize(p_tileSize)
{}


void Sheet::SetSheet(Texture &p_sheet) {
	m_sheet = &p_sheet;
}

void Sheet::Render(Tile::ID p_id, const Rectf &p_dest) {
	if (m_sheet == nullptr)
		Panic(__FUNC__, "() sheet is nullptr");

	Vec2i size = m_sheet->Size();

	Rectf src(Vec2f(p_id % (size.x / m_tileSize.x) * m_tileSize.x,
	                p_id / (size.y / m_tileSize.y) * m_tileSize.y), m_tileSize);

	m_sheet->Render(src, p_dest);
}

}
