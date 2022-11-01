#include "ui.hh"

#include "main/game.hh"

namespace CityBuilder {

UI::Layout::Layout(Type p_type, const Vec2f &p_pos, float p_padding):
	fixedSize(false),
	ignore(false),

	type(p_type),
	rect(p_pos, Vec2f()),
	padding(p_padding)
{}

UI::Layout::Layout(Type p_type, const Rectf &p_rect, float p_padding):
	fixedSize(true),
	ignore(false),

	type(p_type),
	rect(p_rect),
	padding(p_padding)
{}

void UI::Layout::AddWidget(const Vec2f &p_size) {
	switch (type) {
	case Horiz:
		insidesSize.x += p_size.x + padding;
		insidesSize.y  = std::max(insidesSize.y, p_size.y) + padding;

		break;

	case Vert:
		insidesSize.x  = std::max(insidesSize.x, p_size.x) + padding;
		insidesSize.y += p_size.y + padding;

		break;

	default: UNREACHABLE();
	}
}

Vec2f UI::Layout::NextPos() const {
	switch (type) {
	case Horiz: return Vec2f(insidesSize.x + padding, padding);
	case Vert:  return Vec2f(padding, insidesSize.y + padding);

	default: UNREACHABLE();
	}

	SILENCE_RETURN_WARNING();
}

UI::Style::Style(): font(nullptr) {}

UI::Style::TextButton::TextButton():
	color({Color4f(255), Color4f(255), Color4f(255)}),
	texture(nullptr)
{}

UI::Style::TextLabel::TextLabel():
	shadowOffset(Vec2f(1)),
	color({Color4f(255), Color4f(0, 0, 0, 180)})
{}

UI::Style::Frame::Frame(): texture(nullptr) {}

void UI::StyleManager::Add(const std::string &p_key, const Style &p_style) {
	_Add(p_key, p_style);
}

void UI::ResetFocus() {
	m_active.none = true;
	m_hot.none    = true;
}

bool UI::NoFocus() {
	return m_active.none and m_hot.none;
}

void UI::Begin(const Vec2f &p_pos, Layout::Type p_type, float p_padding) {
	if (m_layouts.size() > 0)
		Panic("UI::", __FUNC__, "() called more than once");

	Layout layout(p_type, p_pos, p_padding);
	layout.viewport = Rectf(p_pos, Vec2f(SCREEN_W - p_pos.x, SCREEN_H - p_pos.y));

	Game::Get().SetViewport(layout.viewport);

	m_layouts.push_back(layout);
}

void UI::End() {
	if (m_layouts.size() != 1)
		Panic("UI::", __FUNC__, "() called without a matching UI::Begin()");

	m_layouts.pop_back();

	Game::Get().ResetViewport();
}

void UI::BeginFrame(const Vec2f &p_size, Layout::Type p_type,
                    float p_padding, const Vec2f &p_offset) {
	if (style == nullptr)
		Panic("UI::", __FUNC__, "() called without setting style (it is nullptr)");
	else if (style->frame.texture == nullptr)
		Panic("UI::", __FUNC__, "() called without setting frame.texture (it is nullptr)");

	if (m_layouts.size() < 1)
		Panic("UI::Begin() was not called before UI::", __FUNC__, "()");

	Rectf  rect(TopLayout().NextPos() + p_offset, p_size);
	Layout layout(p_type, rect, p_padding);
	layout.viewport = rect;
	layout.ignore   = not NestViewportInto(layout.viewport, TopLayout().viewport);

	m_layouts.push_back(layout);
	Game::Get().SetViewport(layout.viewport);

	style->frame.texture->Render(Rectf(Vec2f(), p_size));
}

void UI::EndFrame() {
	if (m_layouts.size() <= 1)
		Panic("UI::", __FUNC__, "() called without a matching UI::BeginFrame()");

	if (MouseInBoundary(Rectf(Vec2f(), TopLayout().rect.Size())))
		m_clickWasted = true;

	Layout &layout = TopLayout();

	m_layouts.at(m_layouts.size() - 2).AddWidget(layout.rect.Size());
	m_layouts.pop_back();

	Game::Get().SetViewport(TopLayout().viewport);
}

void UI::BeginLayout(Layout::Type p_type, float p_padding, const Vec2f &p_offset) {
	if (m_layouts.size() < 1)
		Panic("UI::Begin() was not called before UI::", __FUNC__, "()");

	Layout layout(p_type, TopLayout().NextPos() + p_offset, p_padding);
	layout.viewport = Rectf(layout.rect.Pos(), TopLayout().viewport.Size());
	layout.ignore   = not NestViewportInto(layout.viewport, TopLayout().viewport);

	m_layouts.push_back(layout);
	Game::Get().SetViewport(layout.viewport);
}

void UI::EndLayout() {
	if (m_layouts.size() <= 1)
		Panic("UI::", __FUNC__, "() called without a matching UI::BeginLayout()");

	Layout &layout = TopLayout();

	m_layouts.at(m_layouts.size() - 2).AddWidget(layout.insidesSize);
	m_layouts.pop_back();

	Game::Get().SetViewport(TopLayout().viewport);
}

void UI::BeginFloating(Layout::Type p_type, const Vec2f &p_pos,
	                   float p_padding, const Vec2f &p_offset) {
	if (m_layouts.size() < 1)
		Panic("UI::Begin() was not called before UI::", __FUNC__, "()");

	Layout layout(p_type, p_pos + p_offset, p_padding);
	layout.viewport = Rectf(layout.rect.Pos(), TopLayout().viewport.Size());
	layout.ignore   = not NestViewportInto(layout.viewport, TopLayout().viewport);

	m_layouts.push_back(layout);
	Game::Get().SetViewport(layout.viewport);
}

void UI::EndFloating() {
	if (m_layouts.size() <= 1)
		Panic("UI::", __FUNC__, "() called without a matching UI::BeginFloating()");

	m_layouts.pop_back();
	Game::Get().SetViewport(TopLayout().viewport);
}

bool UI::TextButton(ID p_id, const Vec2f &p_size, const std::string &p_text,
                    float p_textScale, const Vec2f &p_offset, bool p_active) {
	if (style == nullptr)
		Panic("UI::", __FUNC__, "() called without setting style (it is nullptr)");
	else if (style->font == nullptr)
		Panic("UI::", __FUNC__, "() called without setting font (it is nullptr)");
	else if (style->textButton.texture == nullptr)
		Panic("UI::", __FUNC__, "() called without setting textButton.texture (it is nullptr)");

	if (m_layouts.size() < 1)
		Panic("UI::Begin() was not called before UI::", __FUNC__, "()");

	Layout &layout = TopLayout();
	if (layout.ignore)
		return false;

	Rectf rect(layout.NextPos() + p_offset, p_size);

	layout.AddWidget(rect.Size() + p_offset);

	bool clicked = false;

	if (p_active) {
		if (m_active == p_id and MousePressed(MouseButton::None)) {
			m_active.none = true;
			clicked       = MouseInBoundary(rect);
		} else if (m_hot == p_id) {
			if (MouseInBoundary(rect)) {
				if (m_active.none and not MousePressed(MouseButton::None))
					m_active = p_id;
			} else
				m_hot.none = true;
		} else if (m_hot.none and MouseInBoundary(rect))
			m_hot = p_id;
	} else {
		if (m_active == p_id)
			m_active.none = true;
		if (m_hot == p_id)
			m_hot.none = true;
	}

	Style::TextButton::State state;
	if (m_active == p_id)
		state = Style::TextButton::Click;
	else if (m_hot == p_id)
		state = Style::TextButton::Hover;
	else
		state = Style::TextButton::Idle;

	Texture *texture    = style->textButton.texture;
	Color4f &color      = style->textButton.color[state];
	Vec2f   &textOffset = style->textButton.textOffset[state];

	Rectf src(Vec2f(), texture->Size());
	src.h /= 3;
	src.y  = src.h * (m_active == p_id? 2 : (m_hot == p_id? 1 : 0));

	texture->Render(src, rect);

	auto rendered = Game::Get().textRenderer.Render(p_text, *style->font);
	if (not rendered.Ok())
		Panic(rendered);

	Vec2f size = static_cast<Vec2f>(rendered.Value()->Size()) * p_textScale;
	Rectf dest = Rectf(Vec2f(rect.w / 2 - size.x / 2 + rect.x,
	                         rect.h / 2 - size.y / 2 + rect.y) + textOffset, size);

	rendered.Value()->SetColor(color);
	rendered.Value()->Render(dest);

	return clicked;
}

bool UI::ImageButton(ID p_id, const Vec2f &p_size, Texture &p_texture,
                     const Vec2f &p_offset, bool p_active) {
	if (m_layouts.size() < 1)
		Panic("UI::Begin() was not called before UI::", __FUNC__, "()");

	Layout &layout = TopLayout();
	if (layout.ignore)
		return false;

	Rectf rect(layout.NextPos() + p_offset, p_size);

	layout.AddWidget(rect.Size() + p_offset);

	bool clicked = false;

	if (p_active) {
		if (m_active == p_id and MousePressed(MouseButton::None)) {
			m_active.none = true;
			clicked       = MouseInBoundary(rect);
		} else if (m_hot == p_id) {
			if (MouseInBoundary(rect)) {
				if (m_active.none and not MousePressed(MouseButton::None))
					m_active = p_id;
			} else
				m_hot.none = true;
		} else if (m_hot.none and MouseInBoundary(rect))
			m_hot = p_id;
	} else {
		if (m_active == p_id)
			m_active.none = true;
		if (m_hot == p_id)
			m_hot.none = true;
	}

	Rectf src(Vec2f(), p_texture.Size());
	src.h /= 3;
	src.y  = src.h * (m_active == p_id? 2 : (m_hot == p_id? 1 : 0));

	p_texture.Render(src, rect);

	return clicked;
}

void UI::TextLabel(const std::string &p_text, float p_scale, const Vec2f &p_offset,
                   size_t p_lineChLimit) {
	if (style == nullptr)
		Panic("UI::", __FUNC__, "() called without setting style (it is nullptr)");
	else if (style->font == nullptr)
		Panic("UI::", __FUNC__, "() called without setting font (it is nullptr)");

	if (m_layouts.size() < 1)
		Panic("UI::Begin() was not called before UI::", __FUNC__, "()");

	Layout &layout = TopLayout();
	if (layout.ignore)
		return;

	auto rendered = Game::Get().textRenderer.Render(p_text, *style->font,
	                                                Color4f(255), p_lineChLimit);
	if (not rendered.Ok())
		Panic(rendered);

	Rectf rect(layout.NextPos() + p_offset, static_cast<Vec2f>(rendered.Value()->Size()) * p_scale);

	layout.AddWidget(rect.Size() + p_offset);

	Color4f &shadowColor = style->textLabel.color[Style::TextLabel::Shadow];
	if (shadowColor.a > 0) {
		rendered.Value()->SetColor(shadowColor);
		rendered.Value()->Render(Rectf(rect.Pos() + style->textLabel.shadowOffset, rect.Size()));
	}

	rendered.Value()->SetColor(style->textLabel.color[Style::TextLabel::Text]);
	rendered.Value()->Render(rect);
}

void UI::ImageLabel(Texture &p_texture, size_t p_frames, size_t p_frame,
                    float p_scale, const Vec2f &p_offset) {
	if (m_layouts.size() < 1)
		Panic("UI::Begin() was not called before UI::", __FUNC__, "()");

	Layout &layout = TopLayout();
	if (layout.ignore)
		return;

	Rectf rect(layout.NextPos() + p_offset, static_cast<Vec2f>(p_texture.Size()) * p_scale);
	layout.AddWidget(rect.Size() + p_offset);

	Rectf src(Vec2f(), p_texture.Size());
	src.h /= p_frames;
	src.y  = src.h * p_frame;

	p_texture.Render(src, rect);
}

bool UI::Screen(ID p_id) {
	if (m_clickWasted)
		return m_clickWasted = false;

	if (m_active == p_id and MousePressed(MouseButton::None)) {
		m_active.none = true;

		return true;
	} else if (m_active.none and not MousePressed(MouseButton::None))
		m_active = p_id;

	return false;
}

bool UI::MouseInBoundary(const Rectf &p_rect) {
	Vec2i mouse = Game::Get().MousePos();

	if (mouse.x < TopLayout().viewport.w and mouse.y < TopLayout().viewport.h)
		return p_rect.Touches(mouse);
	else
		return false;
}

bool UI::MousePressed(MouseButton p_button) {
	return Game::Get().MousePressed() == p_button;
}

bool UI::NestViewportInto(Rectf &p_a, const Rectf &p_b) {
	if (p_a.x >= p_b.w or p_a.y >= p_b.h or
	    p_a.x + p_a.w < 0 or p_a.y + p_a.h < 0)
		return false;

	if (p_a.x + p_a.w > p_b.w)
		p_a.w = p_b.w - p_a.x;
	if (p_a.y + p_a.h > p_b.h)
		p_a.h = p_b.h - p_a.y;

	p_a.x += p_b.x;
	p_a.y += p_b.y;

	return true;
}

UI::Layout &UI::TopLayout() {
	return m_layouts.at(m_layouts.size() - 1);
}

}
