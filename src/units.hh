#ifndef UNITS_HH__HEADER_GUARD__
#define UNITS_HH__HEADER_GUARD__

#include <cstdint> // std::int32_t, std::uint8_t
#include <cstdlib> // std::size_t
#include <ostream> // std::ostream
#include <cmath>   // std::round

#include <SDL2/SDL.h>

using std::uint8_t;
using std::uint32_t;
using std::size_t;

template<typename T>
struct Vec2 {
	Vec2(T p_n = 0):    x(p_n), y(p_n) {}
	Vec2(T p_x, T p_y): x(p_x), y(p_y) {}

	Vec2(const Vec2 &p_vec2): x(p_vec2.x), y(p_vec2.y) {}
	Vec2(const SDL_Point &p_point): x(static_cast<T>(p_point.x)), y(static_cast<T>(p_point.y)) {}

	Vec2 &operator =(const Vec2 &p_vec2) {
		x = p_vec2.x;
		y = p_vec2.y;

		return *this;
	}

	Vec2 operator +(const Vec2 &p_r) const {
		return Vec2(x + p_r.x, y + p_r.y);
	}

	Vec2 operator -(const Vec2 &p_r) const {
		return Vec2(x - p_r.x, y - p_r.y);
	}

	Vec2 operator *(const Vec2 &p_r) const {
		return Vec2(x * p_r.x, y * p_r.y);
	}

	Vec2 operator /(const Vec2 &p_r) const{
		return Vec2(x / p_r.x, y / p_r.y);
	}

	Vec2 &operator +=(const Vec2 &p_r) {
		x += p_r.x;
		y += p_r.y;

		return *this;
	}

	Vec2 &operator -=(const Vec2 &p_r) {
		x -= p_r.x;
		y -= p_r.y;

		return *this;
	}

	Vec2 &operator *=(const Vec2 &p_r) {
		x *= p_r.x;
		y *= p_r.y;

		return *this;
	}

	Vec2 &operator /=(const Vec2 &p_r) {
		x /= p_r.x;
		y /= p_r.y;

		return *this;
	}

	bool operator ==(const Vec2 &p_r) {
		return x == p_r.x and y == p_r.y;
	}

	bool operator !=(const Vec2 &p_r) {
		return x != p_r.x or y != p_r.y;
	}

	operator Vec2<int32_t>() const {
		return Vec2<int32_t>(static_cast<int32_t>(x), static_cast<int32_t>(y));
	}

	operator Vec2<float>() const {
		return Vec2<float>(static_cast<float>(x), static_cast<float>(y));
	}

	operator SDL_Point() const {
		SDL_Point point;
		point.x = static_cast<int>(x);
		point.y = static_cast<int>(y);

		return point;
	}

	Vec2<T> Round() const {
		return Vec2<T>(std::round(x), std::round(y));
	}

	Vec2<T> Ceil() const {
		return Vec2<T>(std::ceil(x), std::ceil(y));
	}

	Vec2<T> Floor() const {
		return Vec2<T>(std::floor(x), std::floor(y));
	}

	friend std::ostream &operator <<(std::ostream &p_stream, const Vec2 &p_vec) {
		p_stream << '{' << p_vec.x << ", " << p_vec.y << '}';

	    return p_stream;
	}

	T x, y;
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int32_t>;

template<typename T>
struct Rect {
	Rect(): x(0), y(0), w(0), h(0) {}
	Rect(T p_x, T p_y, T p_w, T p_h): x(p_x), y(p_y), w(p_w), h(p_h) {}
	Rect(const Vec2<T> &p_pos, const Vec2<T> &p_size = Vec2<T>()):
		x(p_pos.x),
		y(p_pos.y),
		w(p_size.x),
		h(p_size.y)
	{}

	Rect(const Rect &p_rect): x(p_rect.x), y(p_rect.y), w(p_rect.w), h(p_rect.h) {}
	Rect(const SDL_Rect &p_rect):
		x(static_cast<T>(p_rect.x)),
		y(static_cast<T>(p_rect.y)),
		w(static_cast<T>(p_rect.w)),
		h(static_cast<T>(p_rect.h))
	{}

	Rect &operator =(const Rect &p_rect) {
		x = p_rect.x;
		y = p_rect.y;
		w = p_rect.w;
		h = p_rect.h;

		return *this;
	}

	bool operator ==(const Rect &p_r) {
		return x == p_r.x and y == p_r.y and w == p_r.w and h == p_r.h;
	}

	bool operator !=(const Rect &p_r) {
		return x != p_r.x or y != p_r.y or w != p_r.w or h != p_r.h;
	}

	operator Rect<int32_t>() const {
		return Rect<int32_t>(static_cast<int32_t>(x), static_cast<int32_t>(y),
		                     static_cast<int32_t>(w), static_cast<int32_t>(h));
	}

	operator Rect<float>() const {
		return Rect<float>(static_cast<float>(x), static_cast<float>(y),
		                   static_cast<float>(w), static_cast<float>(h));
	}

	operator SDL_Rect() const {
		SDL_Rect rect;
		rect.x = static_cast<int>(x);
		rect.y = static_cast<int>(y);
		rect.w = static_cast<int>(w);
		rect.h = static_cast<int>(h);

		return rect;
	}

	Rect<T> Round() const {
		return Rect<T>(std::round(x), std::round(y), std::round(w), std::round(h));
	}

	Rect<T> Ceil() const {
		return Rect<T>(std::ceil(x), std::ceil(y), std::ceil(w), std::ceil(h));
	}

	Rect<T> Floor() const {
		return Rect<T>(std::floor(x), std::floor(y), std::floor(w), std::floor(h));
	}

	Vec2<T> Pos() const {
		return Vec2<T>(x, y);
	}

	Vec2<T> Size() const {
		return Vec2<T>(w, h);
	}

	bool Touches(const Rect &p_rect) const {
		SDL_Rect a = *this, b = p_rect;

		return SDL_HasIntersection(&a, &b);
	}

	bool Touches(const Vec2<T> &p_pos) const {
		SDL_Rect  a = *this;
		SDL_Point b = p_pos;

		return SDL_PointInRect(&b, &a);
	}

	friend std::ostream &operator <<(std::ostream &p_stream, const Rect &p_rect) {
		p_stream << '{' << p_rect.x << ", " << p_rect.y << ", " <<
		                   p_rect.w << ", " << p_rect.h << '}';

	    return p_stream;
	}

	T x, y, w, h;
};

using Rectf = Rect<float>;
using Recti = Rect<int32_t>;

template<typename T>
struct Color4 {
	Color4(T p_n = 255): r(p_n), g(p_n), b(p_n), a(p_n) {}

	Color4(T p_r, T p_g, T p_b, T p_a = 255): r(p_r), g(p_g), b(p_b), a(p_a) {}

	Color4(const Color4    &p_color): r(p_color.r), g(p_color.g), b(p_color.b), a(p_color.a) {}
	Color4(const SDL_Color &p_color):
		r(p_color.r),
		g(p_color.g),
		b(p_color.b),
		a(p_color.a)
	{}

	Color4 &operator =(const Color4 &p_color) {
		r = p_color.r;
		g = p_color.g;
		b = p_color.b;
		a = p_color.a;

		return *this;
	}

	Color4 operator +(const Color4 &p_r) const {
		return Color4(r + p_r.r, g + p_r.g, b + p_r.b, a + p_r.a);
	}

	Color4 operator -(const Color4 &p_r) const {
		return Color4(r - p_r.r, g - p_r.g, b - p_r.b, a - p_r.a);
	}

	Color4 operator *(const Color4 &p_r) const {
		return Color4(r * p_r.r, g * p_r.g, b * p_r.b, a * p_r.a);
	}

	Color4 operator /(const Color4 &p_r) const {
		return Color4(r / p_r.r, g / p_r.g, b / p_r.b, a / p_r.a);
	}

	Color4 &operator +=(const Color4 &p_r) {
		r += p_r.r;
		g += p_r.g;
		b += p_r.b;
		a += p_r.a;

		return *this;
	}

	Color4 &operator -=(const Color4 &p_r) {
		r -= p_r.r;
		g -= p_r.g;
		b -= p_r.b;
		a -= p_r.a;

		return *this;
	}

	Color4 &operator *=(const Color4 &p_r) {
		r *= p_r.r;
		g *= p_r.g;
		b *= p_r.b;
		a *= p_r.a;

		return *this;
	}

	Color4 &operator /=(const Color4 &p_r) {
		r /= p_r.r;
		g /= p_r.g;
		b /= p_r.b;
		a /= p_r.a;

		return *this;
	}

	bool operator ==(const Color4 &p_r) {
		return r == p_r.r and g == p_r.g and b == p_r.b and a == p_r.a;
	}

	bool operator !=(const Color4 &p_r) {
		return r != p_r.r or g != p_r.g or b != p_r.b or a != p_r.a;
	}

	operator Color4<uint8_t>() const {
		return Color4<uint8_t>(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
		                       static_cast<uint8_t>(b), static_cast<uint8_t>(a));
	}

	operator Color4<float>() const {
		return Color4<float>(static_cast<float>(r), static_cast<float>(g),
		                     static_cast<float>(b), static_cast<float>(a));
	}

	operator SDL_Color() const {
		SDL_Color color;
		color.r = std::round(r);
		color.g = std::round(g);
		color.b = std::round(b);
		color.a = std::round(a);

		return color;
	}

	friend std::ostream &operator <<(std::ostream &p_stream, const Color4 &p_color) {
		p_stream << '{' << +p_color.r << ", " << +p_color.g << ", " <<
		                   +p_color.b << ", " << +p_color.a << '}';

	    return p_stream;
	}

	T r, g, b, a;
};

using Color4f = Color4<float>;
using Color4i = Color4<uint8_t>;

#endif
