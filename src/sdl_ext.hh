#ifndef SDL_EXT_HH__HEADER_GUARD__
#define SDL_EXT_HH__HEADER_GUARD__

#include <cassert> // assert
#include <cstdint> // std::uint32_t
#include <cmath>   // std::round

#include <SDL2/SDL.h>

#include "utils.hh"

namespace CityBuilder {

SDL_Color    SDL_GetSurfacePixel(SDL_Surface *p_surface, int p_x, int p_y);
SDL_Surface *SDL_LoadBMPWithTransparency(const char *p_path, int p_r, int p_g, int p_b);

void SDL_RenderColoredRect(SDL_Renderer *p_renderer,
                           const SDL_Rect &p_rect, const SDL_Color &p_color);

}

#endif
