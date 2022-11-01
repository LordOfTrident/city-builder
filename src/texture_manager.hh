#ifndef TEXTURE_MANAGER_HH__HEADER_GUARD__
#define TEXTURE_MANAGER_HH__HEADER_GUARD__

#include <string> // std::string

#include <SDL2/SDL.h>

#include "utils.hh"
#include "manager.hh"
#include "units.hh"
#include "texture.hh"

namespace CityBuilder {

class TextureManager : public Manager<Texture, std::string> {
public:
	Error FromFile(const std::string &p_key, const std::string &p_path,
	               const Color4i &p_a = Color4i(255, 0, 255));

	void Clear();
};

}

#endif
