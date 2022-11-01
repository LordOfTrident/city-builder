#include "sdl_ext.hh"

namespace CityBuilder {

SDL_Color SDL_GetSurfacePixel(SDL_Surface *p_surface, int p_x, int p_y) {
	Uint32 *pixels = (Uint32*)p_surface->pixels;

	SDL_LockSurface(p_surface);

	SDL_Color color;

	SDL_GetRGBA(pixels[p_surface->w * p_y + p_x], p_surface->format,
	            &color.r, &color.g, &color.b, &color.a);

	SDL_UnlockSurface(p_surface);

	return color;
}

SDL_Surface *SDL_LoadBMPWithTransparency(const char *p_path, int p_r, int p_g, int p_b) {
	SDL_Surface *surface = SDL_LoadBMP(p_path);
	if (surface == nullptr)
		return nullptr;

	std::uint32_t colorKey = SDL_MapRGB(surface->format, p_r, p_g, p_b);
	SDL_SetColorKey(surface, true, colorKey);

	return surface;
}

void SDL_RenderColoredRect(SDL_Renderer *p_renderer,
                           const SDL_Rect &p_rect, const SDL_Color &p_color) {
	SDL_SetRenderDrawColor(p_renderer, p_color.r, p_color.g, p_color.b, p_color.a);
	SDL_RenderFillRect(p_renderer, &p_rect);
}

}
