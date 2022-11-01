#ifndef UI_HH__HEADER_GUARD__
#define UI_HH__HEADER_GUARD__

#include <vector> // std::vector
#include <string> // std::string
#include <cmath>  // std::max

#include "utils.hh"
#include "units.hh"
#include "texture.hh"
#include "manager.hh"

#include "text/font.hh"
#include "text/text_renderer.hh"

#define UI_ID_SCREEN static_cast<CityBuilder::UI::ID>(-1)

namespace CityBuilder {

enum MouseButton {
	None = 0,
	Left,
	Right
};

class UI {
public:
	using ID = size_t;

	struct Layout {
		enum Type {
			Horiz = 0,
			Vert
		};

		Layout(Type p_type, const Vec2f &p_pos = Vec2f(), float p_padding = 0);
		Layout(Type p_type, const Rectf &p_rect, float p_padding = 0);

		void  AddWidget(const Vec2f &p_size);
		Vec2f NextPos() const;

		bool fixedSize, ignore;

		Type  type;
		Rectf rect, viewport;
		Vec2f insidesSize;
		float padding;
	};

	struct Style {
		Style();

		Text::Font *font;

		struct TextButton {
			enum State {
				Idle = 0,
				Hover,
				Click,

				StateCount
			};

			TextButton();

			Vec2f    textOffset[StateCount];
			Color4f  color[StateCount];
			Texture *texture;
		} textButton;

		struct TextLabel {
			enum {
				Text = 0,
				Shadow,

				PartCount
			};

			TextLabel();

			Vec2f   shadowOffset;
			Color4f color[PartCount];
		} textLabel;

		struct Frame {
			Frame();

			Texture *texture;
		} frame;
	};

	class StyleManager : public Manager<Style, std::string> {
	public:
		void Add(const std::string &p_key, const Style &p_style);
	};

	UI(): style(nullptr) {}

	// TODO: Make BeginScrollingFrame and EndScrollingFrame, give layouts relative starting point
	// so scrolling frames work

	void ResetFocus();
	bool NoFocus();

	void Begin(const Vec2f &p_pos = Vec2f(),
	           Layout::Type p_type = Layout::Horiz, float p_padding = 0);
	void End();

	void BeginFrame(const Vec2f &p_size, Layout::Type p_type = Layout::Horiz,
	                float p_padding = 0, const Vec2f &p_offset = Vec2f(0));
	void EndFrame();

	void BeginLayout(Layout::Type p_type, float p_padding = 0,
	                 const Vec2f &p_offset = Vec2f(0));
	void EndLayout();

	void BeginFloating(Layout::Type p_type, const Vec2f &p_pos,
	                   float p_padding = 0, const Vec2f &p_offset = Vec2f(0));
	void EndFloating();

	bool TextButton(ID p_id, const Vec2f &p_size, const std::string &p_text,
	                float p_textScale = 1, const Vec2f &p_offset = Vec2f(0), bool p_active = true);
	bool ImageButton(ID p_id, const Vec2f &p_size, Texture &p_texture,
	                 const Vec2f &p_offset = Vec2f(0), bool p_active = true);

	void TextLabel(const std::string &p_text, float p_scale = 1, const Vec2f &p_offset = Vec2f(0),
	               size_t p_lineChLimit = TEXT_LINE_CH_LIMIT_NONE);
	void ImageLabel(Texture &p_texture, size_t p_frames = 1, size_t p_frame = 0,
	                float p_scale = 1, const Vec2f &p_offset = Vec2f(0));

	bool Screen(ID p_id = UI_ID_SCREEN);

	Style *style;

private:
	// A helper function that fixes the boundary relative to the viewport
	bool MouseInBoundary(const Rectf &p_rect);
	bool MousePressed(MouseButton p_button);

	bool NestViewportInto(Rectf &p_a, const Rectf &p_b);

	Layout &TopLayout();

	bool m_clickWasted;

	Maybe<ID> m_active, m_hot;

	std::vector<Layout> m_layouts;
};

}

#endif
