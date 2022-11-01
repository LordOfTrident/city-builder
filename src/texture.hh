#ifndef TEXTURE_HH__HEADER_GUARD__
#define TEXTURE_HH__HEADER_GUARD__

#include <SDL2/SDL.h>

#include "utils.hh"
#include "units.hh"
#include "sdl_ext.hh"

namespace CityBuilder {

struct Texture {
	[[nodiscard]]
	static ErrorOr<Texture> FromSurface(SDL_Surface *p_surface);

	[[nodiscard]]
	static ErrorOr<Texture> FromFile(const std::string &p_path,
	                                 const Color4i &p_a = Color4i(255, 0, 255));

	Texture(SDL_Texture *p_raw);
	Texture(Texture &&p_texture);
	Texture(const Texture &p_texture) = delete;
	Texture()                         = delete;

	~Texture();

	void Free();

	void Render(const Recti &p_dest);
	void Render(const Vec2i &p_pos);
	void Render(const Recti &p_src, const Recti &p_dest);

	Vec2i Size() const;

	void SetColor(const Color4i &p_color);

	SDL_Texture *raw;
};

}

#endif
