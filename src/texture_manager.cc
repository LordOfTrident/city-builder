#include "texture_manager.hh"

#include "main/game.hh"

namespace CityBuilder {

Error TextureManager::FromFile(const std::string &p_key, const std::string &p_path,
                               const Color4i &p_a) {
	auto texture = Texture::FromFile(p_path, p_a);
	if (not texture.Ok())
		return Error::Make(texture.Desc());

	_Add(p_key, std::move(texture.Value()));

	return Error::Fine();
}

void TextureManager::Clear() {
	m_library.clear();
}

}
