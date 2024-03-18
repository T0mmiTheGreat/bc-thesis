/**
 * @file constants.hpp
 * @author Tomáš Ludrovan
 * @brief Defined constants
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>


// File paths
// Using `#define` for constants might be a little taboo in C++ but I don't
// care. Show me a simpler way to do this

#define FONTS_PATH "font/"
#define IMAGES_PATH "sprite/"

#define FONT_PATH_JULEE   FONTS_PATH "Julee-Regular.ttf"
#define FONT_PATH_MCLAREN FONTS_PATH "McLaren-Regular.ttf"
#define FONT_PATH_ROBOTO  FONTS_PATH "Roboto-Regular.ttf"

#define IMAGE_PATH_PTERO              IMAGES_PATH "ptero.png"
#define IMAGE_PATH_ICON_NEW           IMAGES_PATH "new.png"
#define IMAGE_PATH_ICON_OPEN          IMAGES_PATH "open.png"
#define IMAGE_PATH_ICON_SAVE          IMAGES_PATH "save.png"
#define IMAGE_PATH_ICON_SAVE_AS       IMAGES_PATH "save-as.png"
#define IMAGE_PATH_ICON_UNDO          IMAGES_PATH "undo.png"
#define IMAGE_PATH_ICON_REDO          IMAGES_PATH "redo.png"
#define IMAGE_PATH_ICON_COGWHEEL      IMAGES_PATH "cogwheel.png"
#define IMAGE_PATH_ICON_BACK          IMAGES_PATH "back.png"
#define IMAGE_PATH_ICON_SELECT_TOOL   IMAGES_PATH "select-tool.png"
#define IMAGE_PATH_ICON_PLAYER_TOOL   IMAGES_PATH "player-tool.png"
#define IMAGE_PATH_ICON_OBSTACLE_TOOL IMAGES_PATH "obstacle-tool.png"
#define IMAGE_PATH_ICON_TRASH_CAN     IMAGES_PATH "trash-can.png"
#define IMAGE_PATH_ICON_RESIZE        IMAGES_PATH "resize.png"
#define IMAGE_PATH_ICON_INPUT_WSAD    IMAGES_PATH "wsad.png"
#define IMAGE_PATH_ICON_INPUT_TGFH    IMAGES_PATH "tgfh.png"
#define IMAGE_PATH_ICON_INPUT_IKJL    IMAGES_PATH "ikjl.png"
#define IMAGE_PATH_ICON_INPUT_ARROWS  IMAGES_PATH "arrow-keys.png"
#define IMAGE_PATH_ICON_BOT           IMAGES_PATH "bot.png"
#define IMAGE_PATH_BONUS_HP           IMAGES_PATH "bonus-hp.png"

#endif // CONSTANTS_HPP
