/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "../../SDL_internal.h"

#if SDL_VIDEO_DRIVER_DUMMY

#include "SDL_nullvideo.h"
#include "SDL_nullwindow.h"
#include "../SDL_sysvideo.h"
#include "SDL_render.h"

int Desktop_CreateWindow(_THIS, SDL_Window * window)
{
    /* Not a robust method to do this just preliminary */
    SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
    if (display->fullscreen_window == NULL)
    {
        CALL_INTERNAL(CreateWindow, window)
        SDL_Renderer *renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED
        );
        SDL_SetWindowData(window, "_SDL_WindowRenderer", renderer);
    }
    return 0;
}

int Desktop_CreateWindowFrom(_THIS, SDL_Window * window, const void *data)
{
	//CALL_INTERNAL(CreateWindowFrom, window, data)
}

void Desktop_SetWindowTitle(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(SetWindowTitle, window)
}

void Desktop_SetWindowIcon(_THIS, SDL_Window * window, SDL_Surface * icon)
{
	//CALL_INTERNAL(SetWindowIcon, window, icon)
}

void Desktop_SetWindowPosition(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(SetWindowPosition, window)
}

void Desktop_SetWindowMinimumSize(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(SetWindowMinimumSize, window)
}

void Desktop_SetWindowMaximumSize(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(SetWindowMaximumSize, window)
}

void Desktop_SetWindowSize(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(SetWindowSize, window)
}

void Desktop_ShowWindow(_THIS, SDL_Window * window)
{
    if (window->driverdata != NULL)
    {
        CALL_INTERNAL(ShowWindow, window)
    }
}

void Desktop_HideWindow(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(HideWindow, window)
}

void Desktop_RaiseWindow(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(RaiseWindow, window)
}

void Desktop_MaximizeWindow(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(MaximizeWindow, window)
}

void Desktop_MinimizeWindow(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(MinimizeWindow, window)
}

void Desktop_RestoreWindow(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(RestoreWindow, window)
}

void Desktop_SetWindowBordered(_THIS, SDL_Window * window, SDL_bool bordered)
{
	//CALL_INTERNAL(SetWindowBordered, window, bordered)
}

void Desktop_SetWindowFullscreen(_THIS, SDL_Window * window, SDL_VideoDisplay * display, SDL_bool fullscreen)
{
	//CALL_INTERNAL(SetWindowFullscreen, window, display, fullscreen)
}

int Desktop_SetWindowGammaRamp(_THIS, SDL_Window * window, const Uint16 * ramp)
{
	//CALL_INTERNAL(SetWindowGammaRamp, window, ramp)
}

void Desktop_SetWindowGrab(_THIS, SDL_Window * window, SDL_bool grabbed)
{
	//CALL_INTERNAL(SetWindowGrab, window, grabbed)
}

void Desktop_DestroyWindow(_THIS, SDL_Window * window)
{
	//CALL_INTERNAL(DestroyWindow, window)
}

SDL_bool Desktop_GetWindowWMInfo(_THIS, SDL_Window * window,
                                    struct SDL_SysWMinfo *info)
{
	//CALL_INTERNAL(GetWindowWMInfo, window, info)
}


#endif /* SDL_VIDEO_DRIVER_DUMMY */

/* vi: set ts=4 sw=4 expandtab: */
