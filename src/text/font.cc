#include "font.hh"

#include "../main/config.hh"

namespace CityBuilder {

namespace Text {
	ErrorOr<Font> Font::FromFile(const std::string &p_sheetPath, const std::string &p_infoPath) {
		INI ini;
		auto err = ini.ParseFile(p_infoPath);
		if (not err.Ok())
			return ErrorOr<Font>::Make("'", p_infoPath, "': ", err.Desc());

		if (not ini.HasInSection("char", "w", "h"))
			return ErrorOr<Font>::Make("'", p_infoPath, "': Incomplete 'char' (requires keys w, h)");

		Maybe<size_t> w = ini.Size("w", "char");
		Maybe<size_t> h = ini.Size("h", "char");
		if (w.none)
			return ErrorOr<Font>::Make("'", p_infoPath, "': 'char::w' expected to be an integer");
		else if (h.none)
			return ErrorOr<Font>::Make("'", p_infoPath, "': 'char::h' expected to be an integer");

		if (not ini.HasInSection("alpha", "r", "g", "b"))
			return ErrorOr<Font>::Make("'", p_infoPath,
			                           "': Incomplete 'alpha' (requires keys r, g, b)");

		Maybe<size_t> r = ini.Size("r", "alpha");
		Maybe<size_t> g = ini.Size("g", "alpha");
		Maybe<size_t> b = ini.Size("b", "alpha");
		if (r.none)
			return ErrorOr<Font>::Make("'", p_infoPath, "': 'alpha::r' expected to be an integer");
		else if (g.none)
			return ErrorOr<Font>::Make("'", p_infoPath, "': 'alpha::g' expected to be an integer");
		else if (b.none)
			return ErrorOr<Font>::Make("'", p_infoPath, "': 'alpha::b' expected to be an integer");

		if (w.unwrap == 0)
			return ErrorOr<Font>::Make("'", p_infoPath, "': 'char::w' is 0");
		else if (h.unwrap == 0)
			return ErrorOr<Font>::Make("'", p_infoPath, "': 'char::h' is 0");

#ifdef CITY_BUILDER_LOG
		Log("Loaded font info file '", p_infoPath, "'");
#endif

		SDL_Surface *surface = SDL_LoadBMPWithTransparency(p_sheetPath.c_str(),
		                                                   r.unwrap, g.unwrap, b.unwrap);
		if (surface == nullptr)
			return ErrorOr<Font>::Make("Failed to load texture '", p_sheetPath, "': ",
			                           SDL_GetError());

#ifdef CITY_BUILDER_LOG
		Log("Loaded font sheet file '", p_sheetPath, "'");
#endif

		return ErrorOr<Font>::Fine(std::move(Font(surface, w.unwrap, h.unwrap, surface->w / w.unwrap)));
	}

	Font::Font(SDL_Surface *p_sheet, size_t p_chW, size_t p_chH, size_t p_chsInARow):
		sheet(p_sheet),

		m_chW(p_chW),
		m_chH(p_chH),
		m_chsInARow(p_chsInARow)
	{
		if (sheet == nullptr)
			Panic("Attempted to construct Font from nullptr sheet");
	}

	Font::Font(Font &&p_font):
		Font(p_font.sheet, p_font.m_chW, p_font.m_chH, p_font.m_chsInARow)
	{
		p_font.sheet = nullptr;
	}

	Font::~Font() {
		if (sheet != nullptr) {
			SDL_FreeSurface(sheet);

			sheet = nullptr;
		}
	}

	void Font::Free() {
		if (sheet == nullptr)
			Panic("Attempt to free a nullptr font sheet");

		SDL_FreeSurface(sheet);
		sheet = nullptr;
	}

	size_t Font::CharW() const {
		return m_chW;
	}

	size_t Font::CharH() const {
		return m_chH;
	}

	Vec2i Font::GetCharSheetPos(char p_ch) const {
		return Vec2i(p_ch % m_chsInARow * m_chW, p_ch / m_chsInARow * m_chH);
	}
}

}
