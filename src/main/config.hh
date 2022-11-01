#ifndef CONFIG_HH__HEADER_GUARD__
#define CONFIG_HH__HEADER_GUARD__

namespace CityBuilder {

#define TITLE    "City builder"
#define APP_NAME "city-builder"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define UPDATE_NOTES \
	"First release", \
	"A nice menu and cool logo", \
	"Nice fading animations", \
	"Some basic 10x10 flat grass world"

#define CREDITS \
	"Art, coding by LordOfTrident", \
	"https://github.com/LordOfTrident/"

#define CITY_BUILDER_DEBUG

#define FPS_CAP 60

#define SCREEN_W    448
#define SCREEN_H    256
#define SCREEN_SIZE Vec2i(SCREEN_W, SCREEN_H)

#define SCALE 3

#define MAP_SIZE 10

#define CAM_SENSITIVITY  2
#define DRAG_SENSITIVITY 0.6

#define ZOOM_SENSITIVITY 0.08
#define ZOOM_MAX         2
#define ZOOM_MIN         0.35

#if defined(CITY_BUILDER_DEBUG) and not defined(CITY_BUILDER_LOG)
#	define CITY_BUILDER_LOG
#endif

}

#endif
