#include "texture.hh"

#include "main/config.hh"
#include "main/game.hh"

namespace CityBuilder {

ErrorOr<Texture> Texture::FromSurface(SDL_Surface *p_surface) {
	SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::Get().renderer, p_surface);
	if (texture == nullptr)
		return ErrorOr<Texture>::Make("Failed to create texture from surface: ", SDL_GetError());

	return ErrorOr<Texture>::Fine(std::move(Texture(texture)));
}

ErrorOr<Texture> Texture::FromFile(const std::string &p_path, const Color4i &p_a) {
	SDL_Surface *surface = SDL_LoadBMPWithTransparency(p_path.c_str(), p_a.r, p_a.g, p_a.b);
	if (surface == nullptr)
		return ErrorOr<Texture>::Make("Failed to load texture '", p_path, "': ", SDL_GetError());

	auto ret = FromSurface(surface);
	if (not ret.Ok())
		return ErrorOr<Texture>::Make(ret.Desc());

	SDL_FreeSurface(surface);

#ifdef CITY_BUILDER_LOG
	Log("Loaded texture '", p_path, "'");
#endif

	return ErrorOr<Texture>::Fine(std::move(ret.Value()));
}

Texture::Texture(SDL_Texture *p_raw):
	raw(p_raw)
{
	if (raw == nullptr)
		Panic("Attempted to construct Texture from nullptr");
}

Texture::Texture(Texture &&p_texture):
	Texture(p_texture.raw)
{
	p_texture.raw = nullptr;
}

Texture::~Texture() {
	if (raw != nullptr) {
		SDL_DestroyTexture(raw);

		raw = nullptr;
	}
}

void Texture::Free() {
	if (raw == nullptr)
		Panic("Attempt to free a nullptr texture");

	SDL_DestroyTexture(raw);
	raw = nullptr;
}

void Texture::Render(const Recti &p_dest) {
	SDL_Rect dest = p_dest;

	SDL_RenderCopy(Game::Get().renderer, raw, nullptr, &dest);
}

void Texture::Render(const Vec2i &p_pos) {
	SDL_Rect dest(Recti(p_pos, Size()));

	SDL_RenderCopy(Game::Get().renderer, raw, nullptr, &dest);
}

void Texture::Render(const Recti &p_src, const Recti &p_dest) {
	SDL_Rect src = p_src, dest = p_dest;

	SDL_RenderCopy(Game::Get().renderer, raw, &src, &dest);
}

Vec2i Texture::Size() const {
	int w, h;
	SDL_QueryTexture(raw, nullptr, nullptr, &w, &h);

	return Vec2i(w, h);
}

void Texture::SetColor(const Color4i &p_color) {
	SDL_SetTextureColorMod(raw, p_color.r, p_color.g, p_color.b);
	SDL_SetTextureAlphaMod(raw, p_color.a);
}

}
