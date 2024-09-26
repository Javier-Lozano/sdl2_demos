#include "input.h"

/***** Globals  *****/

static Uint8 g_KeyboardPressState[SDL_NUM_SCANCODES];
static Uint8 g_MousePressState[MOUSE_BUTTON_COUNT];
static int g_MouseX[2];
static int g_MouseY[2];

/***** Functions ******/

void InputUpdate()
{
	// Keyboard 

	int length;
	const Uint8 *keyboard_state = SDL_GetKeyboardState(&length);

	for(int i = 0; i < length; i++)
	{
		Uint8 state = 0;

		if (keyboard_state[i])
		{
			state = STATE_DOWN;

			if (!CHECK_SC_DOWN(i))
			{
				state |= STATE_PRESSED;
			}
		}
		else
		{
			if (CHECK_SC_DOWN(i))
			{
				state |= STATE_RELEASED;
			}
		}

		g_KeyboardPressState[i] = state;
	}

	// Mouse

	g_MouseX[1] = g_MouseX[0];
	g_MouseY[1] = g_MouseY[0];
	int button_state = SDL_GetMouseState(g_MouseX, g_MouseY);

	// Iterate over only 3 main mouse buttons
	for(int i = 0; i < MOUSE_BUTTON_COUNT; i++)
	{
		Uint8 button_pressed = (button_state >> i) & 1;
		Uint8 state  = 0;

		if (button_pressed)
		{
			state = STATE_DOWN;

			if (!CHECK_MOUSE_DOWN(i))
			{
				state |= STATE_PRESSED;
			}
		}
		else
		{
			if (CHECK_MOUSE_DOWN(i))
			{
				state |= STATE_RELEASED;
			}
		}

		g_MousePressState[i] = state;
	}
}

bool InputCheckScancode(SDL_Scancode scancode, InputButtonState state)
{
	if (scancode < 0 || scancode > SDL_NUM_SCANCODES) { return false; }
	return (g_KeyboardPressState[scancode] & state) == state;
}

bool InputCheckMouseButton(InputMouseButton button, InputButtonState state)
{
	if (button < 0 || button > MOUSE_BUTTON_COUNT) { return false; }
	return (g_MousePressState[button] & state) == state;
}

void InputGetMousePosition(int *x, int *y, int *dx, int *dy)
{
	if (x != NULL)
	{
		*x = g_MouseX[0];
	}
	if (y != NULL)
	{
		*y = g_MouseY[0];
	}
	if (dx != NULL)
	{
		*dx = g_MouseX[0] - g_MouseX[1];
	}
	if (dy != NULL)
	{
		*dy = g_MouseY[0] - g_MouseY[1];
	}
}
