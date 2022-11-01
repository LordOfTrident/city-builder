#include "font_manager.hh"

namespace CityBuilder {

namespace Text {
	Error FontManager::FromFile(const std::string &p_key,
	                            const std::string &p_sheetPath, const std::string &p_infoPath) {
		auto font = Font::FromFile(p_sheetPath, p_infoPath);
		if (not font.Ok())
			return Error::Make(font.Desc());

		_Add(p_key, std::move(font.Value()));

		return Error::Fine();
	}

	void FontManager::Clear() {
		m_library.clear();
	}
}

}
