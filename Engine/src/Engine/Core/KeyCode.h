#pragma once

namespace Engine
{
	enum class KeyCode : uint16_t
	{
		E_KEY_SPACE = 32,
		E_KEY_APOSTROPHE = 39, /* ' */
		E_KEY_COMMA = 44,    /* , */
		E_KEY_MINUS = 45,    /* - */
		E_KEY_PERIOD = 46,     /* . */
		E_KEY_SLASH = 47,      /* / */
		E_KEY_0 = 48,
		E_KEY_1 = 49,
		E_KEY_2 = 50,
		E_KEY_3 = 51,
		E_KEY_4 = 52,
		E_KEY_5 = 53,
		E_KEY_6 = 54,
		E_KEY_7 = 55,
		E_KEY_8 = 56,
		E_KEY_9 = 57,
		E_KEY_SEMICOLON = 59, /* ; */
		E_KEY_EQUAL = 61,     /* = */
		E_KEY_A = 65,
		E_KEY_B = 66,
		E_KEY_C = 67,
		E_KEY_D = 68,
		E_KEY_E = 69,
		E_KEY_F = 70,
		E_KEY_G = 71,
		E_KEY_H = 72,
		E_KEY_I = 73,
		E_KEY_J = 74,
		E_KEY_K = 75,
		E_KEY_L = 76,
		E_KEY_M = 77,
		E_KEY_N = 78,
		E_KEY_O = 79,
		E_KEY_P = 80,
		E_KEY_Q = 81,
		E_KEY_R = 82,
		E_KEY_S = 83,
		E_KEY_T = 84,
		E_KEY_U = 85,
		E_KEY_V = 86,
		E_KEY_W = 87,
		E_KEY_X = 88,
		E_KEY_Y = 89,
		E_KEY_Z = 90,
		E_KEY_LEFT_BRACKET = 91,  /* [ */
		E_KEY_BACKSLASH = 92,     /* \ */
		E_KEY_RIGHT_BRACKET = 93, /* ] */
		E_KEY_GRAVE_ACCENT = 96,  /* ` */
		E_KEY_WORLD_1 = 161,      /* non-US #1 */
		E_KEY_WORLD_2 = 162, /* non-US #2 */ /* Funct keys */
		E_KEY_ESCAPE = 256,
		E_KEY_ENTER = 257,
		E_KEY_TAB = 258,
		E_KEY_BACKSPACE = 259,
		E_KEY_INSERT = 260,
		E_KEY_DELETE = 261,
		E_KEY_RIGHT = 262,
		E_KEY_LEFT = 263,
		E_KEY_DOWN = 264,
		E_KEY_UP = 265,
		E_KEY_PAGE_UP = 266,
		E_KEY_PAGE_DOWN = 267,
		E_KEY_HOME = 268,
		E_KEY_END = 269,
		E_KEY_CAPS_LOCK = 280,
		E_KEY_SCROLL_LOCK = 281,
		E_KEY_NUM_LOCK = 282,
		E_KEY_PRINT_SCREEN = 283,
		E_KEY_PAUSE = 284,
		E_KEY_F1 = 290,
		E_KEY_F2 = 291,
		E_KEY_F3 = 292,
		E_KEY_F4 = 293,
		E_KEY_F5 = 294,
		E_KEY_F6 = 295,
		E_KEY_F7 = 296,
		E_KEY_F8 = 297,
		E_KEY_F9 = 298,
		E_KEY_F10 = 299,
		E_KEY_F11 = 300,
		E_KEY_F12 = 301,
		E_KEY_F13 = 302,
		E_KEY_F14 = 303,
		E_KEY_F15 = 304,
		E_KEY_F16 = 305,
		E_KEY_F17 = 306,
		E_KEY_F18 = 307,
		E_KEY_F19 = 308,
		E_KEY_F20 = 309,
		E_KEY_F21 = 310,
		E_KEY_F22 = 311,
		E_KEY_F23 = 312,
		E_KEY_F24 = 313,
		E_KEY_F25 = 314,
		E_KEY_KP_0 = 320,
		E_KEY_KP_1 = 321,
		E_KEY_KP_2 = 322,
		E_KEY_KP_3 = 323,
		E_KEY_KP_4 = 324,
		E_KEY_KP_5 = 325,
		E_KEY_KP_6 = 326,
		E_KEY_KP_7 = 327,
		E_KEY_KP_8 = 328,
		E_KEY_KP_9 = 329,
		E_KEY_KP_DECIMAL = 330,
		E_KEY_KP_DIVIDE = 331,
		E_KEY_KP_MULTIPLY = 332,
		E_KEY_KP_SUBTRACT = 333,
		E_KEY_KP_ADD = 334,
		E_KEY_KP_ENTER = 335,
		E_KEY_KP_EQUAL = 336,
		E_KEY_LEFT_SHIFT = 340,
		E_KEY_LEFT_CONTROL = 341,
		E_KEY_LEFT_ALT = 342,
		E_KEY_LEFT_SUPER = 343,
		E_KEY_RIGHT_SHIFT = 344,
		E_KEY_RIGHT_CONTROL = 345,
		E_KEY_RIGHT_ALT = 346,
		E_KEY_RIGHT_SUPER = 347,

	};
}

#define KEY_SPACE 32
#define KEY_APOSTROPHE 39 /* ' */
#define KEY_COMMA 44      /* , */
#define KEY_MINUS 45      /* - */
#define KEY_PERIOD 46     /* . */
#define KEY_SLASH 47      /* / */
#define KEY_0 48
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57
#define KEY_SEMICOLON 59 /* ; */
#define KEY_EQUAL 61     /* = */
#define KEY_A 65
#define KEY_B 66
#define KEY_C 67
#define KEY_D 68
#define KEY_E 69
#define KEY_F 70
#define KEY_G 71
#define KEY_H 72
#define KEY_I 73
#define KEY_J 74
#define KEY_K 75
#define KEY_L 76
#define KEY_M 77
#define KEY_N 78
#define KEY_O 79
#define KEY_P 80
#define KEY_Q 81
#define KEY_R 82
#define KEY_S 83
#define KEY_T 84
#define KEY_U 85
#define KEY_V 86
#define KEY_W 87
#define KEY_X 88
#define KEY_Y 89
#define KEY_Z 90
#define KEY_LEFT_BRACKET 91  /* [ */
#define KEY_BACKSLASH 92     /* \ */
#define KEY_RIGHT_BRACKET 93 /* ] */
#define KEY_GRAVE_ACCENT 96  /* ` */
#define KEY_WORLD_1 161      /* non-US #1 */
#define KEY_WORLD_2 162 /* non-US #2 */ * Funct keys * /
#define KEY_ESCAPE 256
#define KEY_ENTER 257
#define KEY_TAB 258
#define KEY_BACKSPACE 259
#define KEY_INSERT 260
#define KEY_DELETE 261
#define KEY_RIGHT 262
#define KEY_LEFT 263
#define KEY_DOWN 264
#define KEY_UP 265
#define KEY_PAGE_UP 266
#define KEY_PAGE_DOWN 267
#define KEY_HOME 268
#define KEY_END 269
#define KEY_CAPS_LOCK 280
#define KEY_SCROLL_LOCK 281
#define KEY_NUM_LOCK 282
#define KEY_PRINT_SCREEN 283
#define KEY_PAUSE 284
#define KEY_F1 290
#define KEY_F2 291
#define KEY_F3 292
#define KEY_F4 293
#define KEY_F5 294
#define KEY_F6 295
#define KEY_F7 296
#define KEY_F8 297
#define KEY_F9 298
#define KEY_F10 299
#define KEY_F11 300
#define KEY_F12 301
#define KEY_F13 302
#define KEY_F14 303
#define KEY_F15 304
#define KEY_F16 305
#define KEY_F17 306
#define KEY_F18 307
#define KEY_F19 308
#define KEY_F20 309
#define KEY_F21 310
#define KEY_F22 311
#define KEY_F23 312
#define KEY_F24 313
#define KEY_F25 314
#define KEY_KP_0 320
#define KEY_KP_1 321
#define KEY_KP_2 322
#define KEY_KP_3 323
#define KEY_KP_4 324
#define KEY_KP_5 325
#define KEY_KP_6 326
#define KEY_KP_7 327
#define KEY_KP_8 328
#define KEY_KP_9 329
#define KEY_KP_DECIMAL 330
#define KEY_KP_DIVIDE 331
#define KEY_KP_MULTIPLY 332
#define KEY_KP_SUBTRACT 333
#define KEY_KP_ADD 334
#define KEY_KP_ENTER 335
#define KEY_KP_EQUAL 336
#define KEY_LEFT_SHIFT 340
#define KEY_LEFT_CONTROL 341
#define KEY_LEFT_ALT 342
#define KEY_LEFT_SUPER 343
#define KEY_RIGHT_SHIFT 344
#define KEY_RIGHT_CONTROL 345
#define KEY_RIGHT_ALT 346
#define KEY_RIGHT_SUPER 347
#define KEY_MENU 348