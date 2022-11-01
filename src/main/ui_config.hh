#ifndef UI_CONFIG_HH__HEADER_GUARD__
#define UI_CONFIG_HH__HEADER_GUARD__

#include "config.hh"

#include "../utils.hh"
#include "../text/text_renderer.hh"

namespace CityBuilder {

// General UI

#define UI_FONT_W 6
#define UI_FONT_H 9

#define UI_BUTTON_W    50
#define UI_BUTTON_H    15
#define UI_BUTTON_SIZE Vec2f(UI_BUTTON_W, UI_BUTTON_H)

#define UI_TEXT_CENTER_X(P_T, P_W, P_S) (P_W / 2 - static_cast<int>(STR_LEN(P_T)) * \
                                        UI_FONT_W * P_S / 2)
#define UI_TEXT_CENTER_Y(P_H, P_S)      (P_H / 2 - UI_FONT_H * P_S / 2)

#define UI_TEXT_CENTER(P_T, P_W, P_H, P_S) Vec2f(UI_TEXT_CENTER_X(P_T, P_W, P_S), \
                                                 UI_TEXT_CENTER_Y(P_H, P_S))

#define UI_FADEIN_TIME  10
#define UI_FADEOUT_TIME 10

// UI paused

#define UI_PAUSED_TEXT "Paused"

#define UI_PAUSED_BUTTON_X   2
#define UI_PAUSED_BUTTON_Y   2
#define UI_PAUSED_BUTTON_POS Vec2f(UI_PAUSED_BUTTON_X, UI_PAUSED_BUTTON_Y)

#define UI_PAUSED_BUTTON_W    19
#define UI_PAUSED_BUTTON_H    14
#define UI_PAUSED_BUTTON_SIZE Vec2f(UI_PAUSED_BUTTON_W, UI_PAUSED_BUTTON_H)

// UI menu buttons

#define UI_MENU_BUTTONS_COUNT 5

#define UI_MENU_BUTTON_TEXT_SCALE 1

#define UI_MENU_BUTTON_HOME_TEXT     "Home"
#define UI_MENU_BUTTON_START_TEXT    "Play"
#define UI_MENU_BUTTON_SETTINGS_TEXT "Settings"
#define UI_MENU_BUTTON_CREDITS_TEXT  "Credits"
#define UI_MENU_BUTTON_QUIT_TEXT     "Quit"

#define UI_MENU_BUTTON_W    70
#define UI_MENU_BUTTON_H    15
#define UI_MENU_BUTTON_SIZE Vec2f(UI_MENU_BUTTON_W, UI_MENU_BUTTON_H)

// UI menu

#define UI_MENU_SPACE 10

#define UI_MENU_W    75
#define UI_MENU_H    95
#define UI_MENU_SIZE Vec2f(UI_MENU_W, UI_MENU_H)

#define UI_MENU_X   0
#define UI_MENU_Y   (SCREEN_H / 2 - UI_MENU_H / 2) - 25
#define UI_MENU_POS Vec2f(UI_MENU_X, UI_MENU_Y)

// UI list

#define UI_LIST_PADDING 10

#define UI_LIST_W    232
#define UI_LIST_H    142
#define UI_LIST_SIZE Vec2f(UI_LIST_W, UI_LIST_H)

#define UI_LIST_X   (UI_MENU_X + UI_MENU_W + \
                    (SCREEN_W - UI_MENU_X - UI_MENU_W - UI_LIST_W) / 2)
#define UI_LIST_Y   (SCREEN_H / 2 - UI_LIST_H / 2)
#define UI_LIST_POS Vec2f(UI_LIST_X, UI_LIST_Y)

#define UI_LIST_DESC_SCALE  0.8
#define UI_LIST_TITLE_SCALE 1

#define UI_LIST_TITLE_OFF_X(P_TEXT)  UI_TEXT_CENTER_X(P_TEXT, UI_LIST_W, UI_LIST_TITLE_SCALE)
#define UI_LIST_TITLE_OFF_Y          UI_LIST_PADDING
#define UI_LIST_TITLE_OFFSET(P_TEXT) Vec2f(UI_LIST_TITLE_OFF_X(P_TEXT), UI_LIST_TITLE_OFF_Y)

// UI updates menu

#define UI_UPDATES_TITLE_TEXT "Update v" TO_STR(VERSION_MAJOR) "." \
                                         TO_STR(VERSION_MINOR) "." \
                                         TO_STR(VERSION_PATCH)

// UI credits menu

#define UI_CREDITS_TITLE_TEXT "Credits"

// UI dialog

#define UI_DIALOG_PADDING         6
#define UI_DIALOG_BUTTONS_PADDING 9

#define UI_DIALOG_BUTTONS_OFF_X  0
#define UI_DIALOG_BUTTONS_OFF_Y  8
#define UI_DIALOG_BUTTONS_OFFSET Vec2f(UI_DIALOG_BUTTONS_OFF_X, UI_DIALOG_BUTTONS_OFF_Y)

#define UI_DIALOG_BUTTON_TEXT_SCALE 0.8

#define UI_DIALOG_BUTTON_YES_TEXT "Yes"
#define UI_DIALOG_BUTTON_NO_TEXT  "No"

#define UI_DIALOG_TEXT_SCALE     0.7
#define UI_DIALOG_LINE_LEN_LIMIT ((UI_DIALOG_W - UI_DIALOG_PADDING) / \
                                 (UI_FONT_W * UI_DIALOG_TEXT_SCALE) - 2)

#define UI_DIALOG_W    141
#define UI_DIALOG_H    56
#define UI_DIALOG_SIZE Vec2f(UI_DIALOG_W, UI_DIALOG_H)

#define UI_DIALOG_X   (SCREEN_W / 2 - UI_DIALOG_W / 2)
#define UI_DIALOG_Y   (SCREEN_H / 2 - UI_DIALOG_H / 2)
#define UI_DIALOG_POS Vec2f(UI_DIALOG_X, UI_DIALOG_Y)

}

#endif
