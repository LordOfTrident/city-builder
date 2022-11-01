#ifndef GAME_HH__HEADER_GUARD__
#define GAME_HH__HEADER_GUARD__

#include <cstdlib>  // std::exit, EXIT_FAILURE, EXIT_SUCCESS
#include <iostream> // std::cout, std::cerr
#include <utility>  // std::pair, std::get
#include <cassert>  // assert
#include <cstring>  // std::memset

#include <SDL2/SDL.h>

#include "config.hh"
#include "ui_config.hh"

#include "../utils.hh"
#include "../units.hh"
#include "../math.hh"
#include "../timer.hh"

#include "../texture_manager.hh"
#include "../sheet.hh"
#include "../ui.hh"

#include "../text/text_renderer.hh"
#include "../text/font_manager.hh"

#include "../world/world.hh"

#define SCREEN_RECT Recti(0, 0, SCREEN_W, SCREEN_H)

namespace CityBuilder {

extern const char *updateNotes[], *credits[];

namespace ID {
	enum ID : UI::ID {
		Button_Menu_Home = 0,
		Button_Menu_Start,
		Button_Menu_Settings,
		Button_Menu_Credits,
		Button_Menu_Quit,

		Button_Dialog_Yes,
		Button_Dialog_No,

		Button_Paused
	};
}

class Game {
public:
	enum class State {
		InMenu = 0,
		InGame
	};

	static Game &Create(int p_argc, char **p_argv);
	static void  Delete();

	static Game &Get();

	Game(Game &&p_game)      = delete;
	Game(const Game &p_game) = delete;

	void Render();
	void Input();
	void Update();

	// SDL viewports get messed up with logical size, so we define functions which fix it
	void ResetViewport();
	void SetViewport(const Rectf &p_viewport);

	Vec2i       MousePos();
	MouseButton MousePressed();

	bool Paused();
	bool Quit();

	SDL_Window   *window;
	SDL_Renderer *renderer;

	UI::StyleManager  styles;
	Text::FontManager fonts;
	TextureManager    textures;
	Text::Renderer    textRenderer;

	Sheet tileSheet, buildingSheet;
	World world;

	UI ui;

	size_t tick;

private:
	enum class Menu {
		Home = 0,
		Start,
		Settings,
		Credits
	};

	enum class DialogResponse {
		None = 0,
		Yes,
		No
	};

	struct Timer {
		enum Type {
			FadeOut = 0,
			FadeIn,

			Count
		};
	};

	Game(int p_argc, char **p_argv);
	~Game();

	void LoadTexture(const std::string &p_key, const std::string &p_path);
	void LoadFont(const std::string &p_key,
	              const std::string &p_sheetPath, const std::string &p_infoPath);

	void LoadAssets();
	void InitUIStyles();
	void InitTimers();

	void RenderGame();
	void RenderPaused();

	void RenderMenu();
	void RenderMenuHome();
	void RenderMenuStart();
	void RenderMenuSettings();
	void RenderMenuCredits();

	void RenderDarkenScreen(float p_a);

	void InputGame();
	void EventsGame();

	DialogResponse UIDialog(const std::string &p_text);

	void SetState(State p_state);

	SDL_Event      m_event;
	const uint8_t *m_keyboard;
	Vec2i          m_mouse, m_prevMouse;
	MouseButton    m_mouseButton;

	Recti m_baseViewport, m_viewport;

	State m_state;
	Menu  m_menu;

	TimerHandler<Timer::Count> m_timers;

#define NEW_FLAG(P_NAME) unsigned P_NAME: 1

	struct {
		NEW_FLAG(quit);
		NEW_FLAG(paused);

		NEW_FLAG(quitDialog);
		NEW_FLAG(disableUI);
		NEW_FLAG(draggingScreen);
	} m_flag;

	static Game *m_instance;
};

}

#endif
