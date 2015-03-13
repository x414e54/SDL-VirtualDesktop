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

#ifndef _SDL_desktopwindow_h
#define _SDL_desktopwindow_h

extern int Desktop_CreateWindow(_THIS, SDL_Window * window);
extern int Desktop_CreateWindowFrom(_THIS, SDL_Window * window, const void *data);
extern void Desktop_SetWindowTitle(_THIS, SDL_Window * window);
extern void Desktop_SetWindowIcon(_THIS, SDL_Window * window, SDL_Surface * icon);
extern void Desktop_SetWindowPosition(_THIS, SDL_Window * window);
extern void Desktop_SetWindowMinimumSize(_THIS, SDL_Window * window);
extern void Desktop_SetWindowMaximumSize(_THIS, SDL_Window * window);
extern void Desktop_SetWindowSize(_THIS, SDL_Window * window);
extern void Desktop_ShowWindow(_THIS, SDL_Window * window);
extern void Desktop_HideWindow(_THIS, SDL_Window * window);
extern void Desktop_RaiseWindow(_THIS, SDL_Window * window);
extern void Desktop_MaximizeWindow(_THIS, SDL_Window * window);
extern void Desktop_MinimizeWindow(_THIS, SDL_Window * window);
extern void Desktop_RestoreWindow(_THIS, SDL_Window * window);
extern void Desktop_SetWindowBordered(_THIS, SDL_Window * window, SDL_bool bordered);
extern void Desktop_SetWindowFullscreen(_THIS, SDL_Window * window, SDL_VideoDisplay * display, SDL_bool fullscreen);
extern int Desktop_SetWindowGammaRamp(_THIS, SDL_Window * window, const Uint16 * ramp);
extern void Desktop_SetWindowGrab(_THIS, SDL_Window * window, SDL_bool grabbed);
extern void Desktop_DestroyWindow(_THIS, SDL_Window * window);
extern SDL_bool Desktop_GetWindowWMInfo(_THIS, SDL_Window * window,
                                    struct SDL_SysWMinfo *info);

#endif /* _SDL_desktopwindow_h */

/* vi: set ts=4 sw=4 expandtab: */
