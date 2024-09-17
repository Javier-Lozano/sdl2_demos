#include "input.h"

/***** Globals  *****/

static Uint8 g_KeyboardPressState[SDL_NUM_SCANCODES];
static Uint8 g_MousePressState[MOUSE_BUTTON_COUNT];
static int g_MouseX;
static int g_MouseY;

/***** Functions ******/

void Input_Update()
{
	// Keyboard 

	int length;
	const Uint8 *keyboard_state = SDL_GetKeyboardState(&length);

	for(int i = 0; i < length; i++)
	{
		Uint8 state = 0;

		if (keyboard_state[i])
		{
			state = BUTTON_STATE_DOWN;

			if (!CHECK_SC_DOWN(i))
			{
				state |= BUTTON_STATE_PRESSED;
			}
		}
		else
		{
			if (CHECK_SC_DOWN(i))
			{
				state |= BUTTON_STATE_RELEASED;
			}
		}

		g_KeyboardPressState[i] = state;
	}

	// Mouse

	int button_state = SDL_GetMouseState(&g_MouseX, &g_MouseY);

	// Iterate over only 3 main mouse buttons
	for(int i = 0; i < MOUSE_BUTTON_COUNT; i++)
	{
		Uint8 button_pressed = (button_state >> i) & 1;
		Uint8 state  = 0;

		if (button_pressed)
		{
			state = BUTTON_STATE_DOWN;

			if (!CHECK_MOUSE_DOWN(i))
			{
				state |= BUTTON_STATE_PRESSED;
			}
		}
		else
		{
			if (CHECK_MOUSE_DOWN(i))
			{
				state |= BUTTON_STATE_RELEASED;
			}
		}

		g_MousePressState[i] = state;
	}
}

bool Input_CheckScancode(SDL_Scancode scancode, Input_ButtonState state)
{
	if (scancode < 0 || scancode > SDL_NUM_SCANCODES) { return false; }
	return (g_KeyboardPressState[scancode] & state) == state;
}

bool Input_CheckMouseButton(Input_MouseButton button, Input_ButtonState state)
{
	if (button < 0 || button > MOUSE_BUTTON_COUNT) { return false; }
	return (g_MousePressState[button] & state) == state;
}

void Input_GetMousePosition(int *x, int *y)
{
	if (x != NULL)
	{
		*x = g_MouseX;
	}
	if (y != NULL)
	{
		*y = g_MouseY;
	}
}
