 #include "game.hh"

namespace CityBuilder {

const char *updateNotes[] = {UPDATE_NOTES}, *credits[] = {CREDITS};

Game *Game::m_instance = nullptr;

Game &Game::Create(int p_argc, char **p_argv) {
	return *new Game(p_argc, p_argv);
}

void Game::Delete() {
	delete m_instance;
}

Game &Game::Get() {
	if (m_instance == nullptr)
		Panic("Game instance requested, but it is nullptr");

	return *m_instance;
}

Game::Game(int p_argc, char **p_argv):
	tileSheet(TILE_SIZE),
	buildingSheet(BUILDING_SIZE),

	world(MAP_SIZE),

	tick(0),

	m_baseViewport(SCREEN_RECT),
	m_viewport(SCREEN_RECT),

	m_state(Game::State::InMenu),
	m_menu(Game::Menu::Home)
{
	UNUSED(p_argc);
	UNUSED(p_argv);

	m_instance = this;

	std::memset(&m_flag, 0, sizeof(m_flag));

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		Panic("SDL2 Error: ", SDL_GetError());
#ifdef CITY_BUILDER_LOG
	else
		Log("Initialized SDL2");
#endif

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                          SCREEN_W * SCALE, SCREEN_H * SCALE, SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
		Panic("SDL2 Error: ", SDL_GetError());
#ifdef CITY_BUILDER_LOG
	else
		Log("Created the window");
#endif

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
		Panic("SDL2 Error: ", SDL_GetError());
#ifdef CITY_BUILDER_LOG
	else
		Log("Created the renderer");
#endif

	if (not SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"))
		Panic("SDL2 Error: ", SDL_GetError());
#ifdef CITY_BUILDER_LOG
	else
		Log("Set hint");
#endif

	if (SDL_RenderSetLogicalSize(renderer, SCREEN_W, SCREEN_H) != 0)
		Panic("SDL2 Error: ", SDL_GetError());
#ifdef CITY_BUILDER_LOG
	else
		Log("Set logical size");
#endif

	if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
		Panic("SDL2 Error: ", SDL_GetError());
#ifdef CITY_BUILDER_LOG
	else
		Log("Set blend mode");
#endif

	SDL_Surface *icon = SDL_LoadBMPWithTransparency("./res/icon.bmp", 255, 0, 255);
	if (icon == nullptr)
		Panic("Failed to load the icon: ", SDL_GetError());

	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);

	m_keyboard = SDL_GetKeyboardState(nullptr);

	Log("--------------------------------");

	LoadAssets();

#ifdef CITY_BUILDER_LOG
	Log("Loaded all assets");
#endif

	InitUIStyles();

#ifdef CITY_BUILDER_LOG
	Log("Initialized UI styles");
#endif

	InitTimers();

#ifdef CITY_BUILDER_LOG
	Log("Initialized timers");
#endif
}

void Game::LoadTexture(const std::string &p_key, const std::string &p_path) {
	auto err = textures.FromFile(p_key, p_path);
	if (not err.Ok())
		Panic(err);
}

void Game::LoadFont(const std::string &p_key,
                    const std::string &p_sheetPath, const std::string &p_infoPath) {
	auto err = fonts.FromFile(p_key, p_sheetPath, p_infoPath);
	if (not err.Ok())
		Panic(err);
}

void Game::LoadAssets() {
	LoadTexture("tile_sheet", "./res/tile_sheet.bmp");
	LoadTexture("logo",       "./res/logo.bmp");

	LoadTexture("frames/dialog", "./res/frames/dialog.bmp");
	LoadTexture("frames/menu",   "./res/frames/menu.bmp");
	LoadTexture("frames/list",   "./res/frames/list.bmp");

	LoadTexture("buttons/default", "./res/buttons/default.bmp");
	LoadTexture("buttons/menu",    "./res/buttons/menu.bmp");
	LoadTexture("buttons/back",    "./res/buttons/back.bmp");

	LoadFont("default", "./res/fonts/default.bmp", "./res/fonts/default.ini");

	assert(UI_FONT_W == fonts.Get("default").CharW());
	assert(UI_FONT_H == fonts.Get("default").CharH());

	tileSheet.SetSheet(textures.Get("tile_sheet"));
	//buildingSheet.SetSheet(textures.Get("building_sheet")); // TODO: Building sheet
}

void Game::InitUIStyles() {
	UI::Style style;

	/* General style */
	style.font = &fonts.Get("default");

	style.textLabel.shadowOffset = Vec2f(1);

	style.textLabel.color[UI::Style::TextLabel::Text]   = Color4f(66, 76, 110);
	style.textLabel.color[UI::Style::TextLabel::Shadow] = Color4f(93, 44, 40);

	/* Black screen style */
	style.textLabel.color[UI::Style::TextLabel::Text]   = Color4f(199, 207, 221);
	style.textLabel.color[UI::Style::TextLabel::Shadow] = Color4f(0);

	styles.Add("black_screen", style);

	/* Dialog style */
	style.frame.texture = &textures.Get("frames/dialog");

	style.textLabel.color[UI::Style::TextLabel::Text]   = Color4f(199, 207, 221);
	style.textLabel.color[UI::Style::TextLabel::Shadow] = Color4f(66,  76,  110);

	style.textButton.textOffset[UI::Style::TextButton::Idle]  = Vec2f();
	style.textButton.textOffset[UI::Style::TextButton::Hover] = Vec2f();
	style.textButton.textOffset[UI::Style::TextButton::Click] = Vec2f(1);

	style.textButton.color[UI::Style::TextButton::Idle]  = Color4f(101, 115, 146);
	style.textButton.color[UI::Style::TextButton::Hover] = Color4f(101, 115, 146);
	style.textButton.color[UI::Style::TextButton::Click] = Color4f(101, 115, 146);

	style.textButton.texture = &textures.Get("buttons/default");

	styles.Add("dialog", style);

	/* List style */
	style.frame.texture = &textures.Get("frames/list");

	style.textLabel.color[UI::Style::TextLabel::Text]   = Color4f(66, 76, 110, 200);
	style.textLabel.color[UI::Style::TextLabel::Shadow] = Color4f(0);

	style.textButton.textOffset[UI::Style::TextButton::Idle]  = Vec2f();
	style.textButton.textOffset[UI::Style::TextButton::Hover] = Vec2f();
	style.textButton.textOffset[UI::Style::TextButton::Click] = Vec2f(1);

	style.textButton.color[UI::Style::TextButton::Idle]  = Color4f(101, 115, 146);
	style.textButton.color[UI::Style::TextButton::Hover] = Color4f(101, 115, 146);
	style.textButton.color[UI::Style::TextButton::Click] = Color4f(101, 115, 146);

	style.textButton.texture = &textures.Get("buttons/default");

	styles.Add("list", style);

	/* Menu style */
	style.frame.texture = &textures.Get("frames/menu");

	style.textLabel.color[UI::Style::TextLabel::Text]   = Color4f(199, 207, 221);
	style.textLabel.color[UI::Style::TextLabel::Shadow] = Color4f(66,  76,  110);

	style.textButton.textOffset[UI::Style::TextButton::Idle]  = Vec2f(-2, 0);
	style.textButton.textOffset[UI::Style::TextButton::Hover] = Vec2f( 2, 0);
	style.textButton.textOffset[UI::Style::TextButton::Click] = Vec2f( 3, 1);

	style.textButton.color[UI::Style::TextButton::Idle]  = Color4f(101, 115, 146);
	style.textButton.color[UI::Style::TextButton::Hover] = Color4f(101, 115, 146);
	style.textButton.color[UI::Style::TextButton::Click] = Color4f(101, 115, 146);

	style.textButton.texture = &textures.Get("buttons/menu");

	styles.Add("menu", style);
}

void Game::InitTimers() {
	m_timers.Init(Timer::FadeOut, UI_FADEOUT_TIME, [this]() {
		switch (m_state) {
		case State::InMenu:
			SetState(State::InGame);
			m_timers.Start(Timer::FadeIn);

			break;

		case State::InGame:
			SetState(State::InMenu);
			m_timers.Start(Timer::FadeIn);

			m_flag.disableUI = true;

			break;

		default: UNREACHABLE();
		}
	});

	m_timers.Init(Timer::FadeIn, UI_FADEIN_TIME, [this]() {
		switch (m_state) {
		case State::InMenu: m_flag.disableUI = false; break;

		default: break;
		}
	});
}

Game::~Game() {
#ifdef CITY_BUILDER_LOG
	Log("--------------------------------");
#endif

	textRenderer.ClearCache();
	fonts.Clear();
#ifdef CITY_BUILDER_LOG
	Log("Destroyed all assets");
#endif

	SDL_DestroyRenderer(renderer);
#ifdef CITY_BUILDER_LOG
	Log("Destroyed the renderer");
#endif

	SDL_DestroyWindow(window);
#ifdef CITY_BUILDER_LOG
	Log("Destroyed the window");

	Log("Quitting SDL2");
#endif
	SDL_Quit();
}

void Game::RenderDarkenScreen(float p_a) {
	SDL_RenderColoredRect(renderer, SCREEN_RECT, Color4f(0, 0, 0, p_a));
}

Game::DialogResponse Game::UIDialog(const std::string &p_text) {
	auto response = DialogResponse::None;

	RenderDarkenScreen(100);

	ui.style = &styles.Get("dialog");
	ui.Begin(UI_DIALOG_POS);
	{
		ui.BeginFrame(UI_DIALOG_SIZE, UI::Layout::Vert, UI_DIALOG_PADDING);
		{
			ui.TextLabel(p_text, UI_DIALOG_TEXT_SCALE, Vec2f(), UI_DIALOG_LINE_LEN_LIMIT);

			ui.BeginFloating(UI::Layout::Horiz, Vec2f(UI_DIALOG_PADDING, 17),
			                 UI_DIALOG_BUTTONS_PADDING, UI_DIALOG_BUTTONS_OFFSET);
			{
				if (ui.TextButton(ID::Button_Dialog_Yes, UI_BUTTON_SIZE, UI_DIALOG_BUTTON_YES_TEXT,
				                  UI_DIALOG_BUTTON_TEXT_SCALE)) {
					response = DialogResponse::Yes;

					ui.ResetFocus();
				}
				if (ui.TextButton(ID::Button_Dialog_No, UI_BUTTON_SIZE, UI_DIALOG_BUTTON_NO_TEXT,
				                  UI_DIALOG_BUTTON_TEXT_SCALE)) {
					response = DialogResponse::No;

					ui.ResetFocus();
				}
			}
			ui.EndFloating();
		}
		ui.EndFrame();
	}
	ui.End();

	return response;
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	switch (m_state) {
	case State::InMenu:  RenderMenu();    break;
	case State::InGame:  RenderGame();    break;

	default: UNREACHABLE();
	}

	if (m_timers.Active(Timer::FadeIn))
		RenderDarkenScreen(m_timers.GetUnit(Timer::FadeIn) * 255);
	else if (m_timers.Active(Timer::FadeOut))
		RenderDarkenScreen(m_timers.GetUnit(Timer::FadeOut, true) * 255);

	SDL_RenderPresent(renderer);
}

void Game::RenderGame() {
	SDL_RenderColoredRect(renderer, SCREEN_RECT, Color4f(19, 19, 19));

	world.Render();

	if (m_flag.paused)
		RenderPaused();
}

void Game::RenderPaused() {
	RenderDarkenScreen(100);

	ui.style = &styles.Get("black_screen");
	ui.Begin(UI_TEXT_CENTER(UI_PAUSED_TEXT, SCREEN_W, SCREEN_H, 1));
	{
		ui.TextLabel(UI_PAUSED_TEXT);
	}
	ui.End();

	ui.Begin(UI_PAUSED_BUTTON_POS);
	{
		if (ui.ImageButton(ID::Button_Paused, UI_PAUSED_BUTTON_SIZE,
		                   textures.Get("buttons/back"))) {
			m_flag.paused = false;

			m_timers.Start(Timer::FadeOut);
		}
	}
	ui.End();
}

void Game::RenderMenu() {
	ui.style = &styles.Get("menu");
	ui.Begin(UI_MENU_POS, UI::Layout::Vert);
	{
		ui.ImageLabel(textures.Get("logo"));

		ui.BeginFrame(UI_MENU_SIZE, UI::Layout::Vert);
		{
			assert(UI_MENU_BUTTONS_COUNT == 5);

			bool active = not m_flag.quitDialog and not m_flag.disableUI;

			if (ui.TextButton(ID::Button_Menu_Home, UI_MENU_BUTTON_SIZE,
			                  UI_MENU_BUTTON_HOME_TEXT, 1, Vec2f(0, UI_MENU_SPACE), active))
				m_menu = Menu::Home;

			if (ui.TextButton(ID::Button_Menu_Start, UI_MENU_BUTTON_SIZE,
			                  UI_MENU_BUTTON_START_TEXT, 1, Vec2f(), active))
				m_timers.Start(Timer::FadeOut);

			if (ui.TextButton(ID::Button_Menu_Settings, UI_MENU_BUTTON_SIZE,
			                  UI_MENU_BUTTON_SETTINGS_TEXT, 1, Vec2f(), active))
				m_menu = Menu::Settings;

			if (ui.TextButton(ID::Button_Menu_Credits, UI_MENU_BUTTON_SIZE,
			                  UI_MENU_BUTTON_CREDITS_TEXT, 1, Vec2f(), active))
				m_menu = Menu::Credits;

			if (ui.TextButton(ID::Button_Menu_Quit, UI_MENU_BUTTON_SIZE,
			                  UI_MENU_BUTTON_QUIT_TEXT, 1, Vec2f(), active))
				m_flag.quitDialog = true;
		}
		ui.EndFrame();
	}
	ui.End();

	switch (m_menu) {
	case Menu::Home:     RenderMenuHome();     break;
	case Menu::Start:    RenderMenuStart();    break;
	case Menu::Settings: RenderMenuSettings(); break;
	case Menu::Credits:  RenderMenuCredits();  break;

	default: break;
	}

	if (m_flag.quitDialog) {
		auto response = UIDialog("Are you sure you want to quit?");
		if (response == DialogResponse::Yes)
			m_flag.quit = true;
		else if (response == DialogResponse::No)
			m_flag.quitDialog = false;
	}

	if (ui.Screen())
		Log("Screen clicked");
}

void Game::RenderMenuHome() {
	ui.style = &styles.Get("list");
	ui.Begin(UI_LIST_POS);
	{
		ui.BeginFrame(UI_LIST_SIZE, UI::Layout::Vert);
		{
			ui.TextLabel(UI_UPDATES_TITLE_TEXT, UI_LIST_TITLE_SCALE,
			             UI_LIST_TITLE_OFFSET(UI_UPDATES_TITLE_TEXT));

			for (size_t i = 0; i < ARR_SIZE(updateNotes); ++ i)
				ui.TextLabel(std::string("- ") + updateNotes[i], UI_LIST_DESC_SCALE,
				             Vec2f(UI_LIST_PADDING, i == 0? UI_LIST_PADDING : 3));
		}
		ui.EndFrame();
	}
	ui.End();
}

void Game::RenderMenuStart() {

}

void Game::RenderMenuSettings() {
	ui.style = &styles.Get("dialog");
	ui.Begin(UI_LIST_POS);
	{
	}
	ui.End();
}

void Game::RenderMenuCredits() {
	ui.style = &styles.Get("list");
	ui.Begin(UI_LIST_POS);
	{
		ui.BeginFrame(UI_LIST_SIZE, UI::Layout::Vert);
		{
			ui.TextLabel(UI_CREDITS_TITLE_TEXT, UI_LIST_TITLE_SCALE,
			             UI_LIST_TITLE_OFFSET(UI_CREDITS_TITLE_TEXT));

			for (size_t i = 0; i < ARR_SIZE(credits); ++ i)
				ui.TextLabel(credits[i], UI_LIST_DESC_SCALE,
				             Vec2f(UI_LIST_PADDING, i == 0? UI_LIST_PADDING : 3));
		}
		ui.EndFrame();
	}
	ui.End();
}

void Game::Input() {
	while (SDL_PollEvent(&m_event)) {
		switch (m_event.type) {
		case SDL_QUIT: m_flag.quit = true; break;

		case SDL_WINDOWEVENT:
			switch (m_event.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					SDL_Rect viewport;
					SDL_RenderGetViewport(renderer, &viewport);

					m_baseViewport = viewport;
				}

				break;

			default: break;
			}

			break;

		case SDL_MOUSEMOTION:
			m_prevMouse = m_mouse;

			m_mouse.x = m_event.motion.x;
			m_mouse.y = m_event.motion.y;

			break;

		case SDL_MOUSEBUTTONUP:
			m_mouseButton = MouseButton::None;

			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (m_event.button.button) {
			case SDL_BUTTON_LEFT:  m_mouseButton = MouseButton::Left;  break;
			case SDL_BUTTON_RIGHT: m_mouseButton = MouseButton::Right; break;
			}

			break;

		default: break;
		}

		switch (m_state) {
		case State::InGame: EventsGame(); break;

		default: break;
		}
	}

	switch (m_state) {
	case State::InGame: InputGame(); break;

	default: break;
	}
}

void Game::InputGame() {
	if (m_flag.paused)
		return;

	if (m_keyboard[SDL_SCANCODE_W])
		world.camera.Up();
	if (m_keyboard[SDL_SCANCODE_A])
		world.camera.Left();
	if (m_keyboard[SDL_SCANCODE_S])
		world.camera.Down();
	if (m_keyboard[SDL_SCANCODE_D])
		world.camera.Right();
}

void Game::EventsGame() {
	if (m_flag.paused and m_event.type != SDL_KEYDOWN)
		return;

	switch (m_event.type) {
	case SDL_KEYDOWN:
		switch (m_event.key.keysym.sym) {
		case SDLK_SPACE: m_flag.paused = not m_flag.paused; break;

		default: break;
		}

		break;

	case SDL_MOUSEBUTTONDOWN:
		if (ui.NoFocus() and m_event.button.button == SDL_BUTTON_RIGHT)
			m_flag.draggingScreen = true;

		break;

	case SDL_MOUSEBUTTONUP:
		if (m_flag.draggingScreen)
			m_flag.draggingScreen = false;

		break;

	case SDL_MOUSEMOTION:
		if (m_flag.draggingScreen)
			world.camera.Move(m_prevMouse - m_mouse);

		break;

	case SDL_MOUSEWHEEL:
		if (m_event.wheel.y > 0)
			world.camera.ZoomIn();
		else if (m_event.wheel.y < 0)
			world.camera.ZoomOut();

		break;

	default: break;
	}
}

void Game::Update() {
	++ tick;

	m_timers.Update();
}

void Game::ResetViewport() {
	SDL_Rect viewport = m_baseViewport;
	SDL_RenderSetViewport(renderer, &viewport);

	m_viewport = SCREEN_RECT;
}

void Game::SetViewport(const Rectf &p_viewport) {
	SDL_Rect viewport = Rectf(p_viewport.Pos() + m_baseViewport.Pos(), p_viewport.Size()).Round();
	SDL_RenderSetViewport(renderer, &viewport);

	m_viewport = p_viewport;
}

Vec2i Game::MousePos() {
	return m_mouse - m_viewport.Pos();
}

MouseButton Game::MousePressed() {
	return m_mouseButton;
}

void Game::SetState(State p_state) {
	m_state = p_state;
	ui.ResetFocus();
}

bool Game::Paused() {
	return m_flag.paused;
}

bool Game::Quit() {
	return m_flag.quit;
}

}
