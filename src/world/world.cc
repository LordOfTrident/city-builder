#include "world.hh"

#include "../main/game.hh"

namespace CityBuilder {

World::World(const Vec2i &p_size): size(p_size) {
	tiles.resize(p_size.y);
	for (auto &row : tiles)
		row.resize(p_size.x);

	camera.pos.y = static_cast<float>(size.y) * TILE_H / 2;
}

void World::Render() {
	const float w = static_cast<float>(TILE_W) * camera.zoom;
	const float h = static_cast<float>(TILE_H) * camera.zoom;

	const float offX = (camera.pos.x + TILE_W / 2) * camera.zoom - static_cast<float>(SCREEN_W) / 2;
	const float offY = camera.pos.y * camera.zoom - static_cast<float>(SCREEN_H) / 2;

	float y = 0;
	for (auto &row : tiles) {
		float x = 0;
		for (auto &tile : row) {
			Rectf rect(x * (w / 2) + y * -(w / 2), x * (h / 2) + y *  (h / 2), w, h);

			rect.x -= offX;
			rect.y -= offY;

			rect = rect.Ceil();

			Game::Get().tileSheet.Render(tile.GetID(), rect);

			++ x;
		}

		++ y;
	}
}

}
