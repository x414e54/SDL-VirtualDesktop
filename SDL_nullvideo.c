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

/* Dummy SDL video driver implementation; this is just enough to make an
 *  SDL-based application THINK it's got a working video driver, for
 *  applications that call SDL_Init(SDL_INIT_VIDEO) when they don't need it,
 *  and also for use as a collection of stubs when porting SDL to a new
 *  platform for which you haven't yet written a valid video driver.
 *
 * This is also a great way to determine bottlenecks: if you think that SDL
 *  is a performance problem for a given platform, enable this driver, and
 *  then see if your application runs faster without video overhead.
 *
 * Initial work by Ryan C. Gordon (icculus@icculus.org). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "DUMMY" by Sam Lantinga.
 */

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_nullvideo.h"
#include "SDL_nullevents_c.h"
#include "SDL_nullframebuffer_c.h"
#include "SDL_nullwindow.h"
#include "SDL_nullclipboard.h"
#include "SDL_nullopengl.h"

#define DUMMYVID_DRIVER_NAME "dummy"

/* Initialization/Query functions */
static int Desktop_VideoInit(_THIS);
static int Desktop_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);
static void Desktop_VideoQuit(_THIS);

/* DUMMY driver bootstrap functions */

static int
DUMMY_Available(void)
{
    return 1;
    /*
    const char *envr = SDL_getenv("SDL_VIDEODRIVER");
    if ((envr) && (SDL_strcmp(envr, DUMMYVID_DRIVER_NAME) == 0)) {
        return (1);
    }

    return (0);*/
}

static void
DUMMY_DeleteDevice(SDL_VideoDevice * device)
{
    SDL_free(device);
}

static SDL_VideoDevice *
DUMMY_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        SDL_free(device);
        return (0);
    }

    /* Set the function pointers */
    device->VideoInit = Desktop_VideoInit;
    device->VideoQuit = Desktop_VideoQuit;
   // device->GetDisplayModes = Desktop_SetDisplayMode;
    //device->GetDisplayBounds = Desktop_GetDisplayBounds;
    device->SetDisplayMode = Desktop_SetDisplayMode;
    //device->SuspendScreenSaver = Desktop_SuspendScreenSaver;
    device->PumpEvents = Desktop_PumpEvents;

    device->CreateWindow = Desktop_CreateWindow;
    device->CreateWindowFrom = Desktop_CreateWindowFrom;
    device->SetWindowTitle = Desktop_SetWindowTitle;
    device->SetWindowIcon = Desktop_SetWindowIcon;
    device->SetWindowPosition = Desktop_SetWindowPosition;
    device->SetWindowSize = Desktop_SetWindowSize;
    device->SetWindowMinimumSize = Desktop_SetWindowMinimumSize;
    device->SetWindowMaximumSize = Desktop_SetWindowMaximumSize;
    device->ShowWindow = Desktop_ShowWindow;
    device->HideWindow = Desktop_HideWindow;
    device->RaiseWindow = Desktop_RaiseWindow;
    device->MaximizeWindow = Desktop_MaximizeWindow;
    device->MinimizeWindow = Desktop_MinimizeWindow;
    device->RestoreWindow = Desktop_RestoreWindow;
    device->SetWindowBordered = Desktop_SetWindowBordered;
    device->SetWindowFullscreen = Desktop_SetWindowFullscreen;
    device->SetWindowGammaRamp = Desktop_SetWindowGammaRamp;
    device->SetWindowGrab = Desktop_SetWindowGrab;
    device->DestroyWindow = Desktop_DestroyWindow;
    device->GetWindowWMInfo = Desktop_GetWindowWMInfo;

    //device->shape_driver.CreateShaper = Desktop_CreateShaper;
    //device->shape_driver.SetWindowShape = Desktop_SetWindowShape;
    //device->shape_driver.ResizeWindowShape = Desktop_ResizeWindowShape;

    device->GL_LoadLibrary = Desktop_GL_LoadLibrary;
    device->GL_GetProcAddress = Desktop_GL_GetProcAddress;
    device->GL_UnloadLibrary = Desktop_GL_UnloadLibrary;
    device->GL_CreateContext = Desktop_GL_CreateContext;
    device->GL_MakeCurrent = Desktop_GL_MakeCurrent;
    device->GL_SetSwapInterval = Desktop_GL_SetSwapInterval;
    device->GL_GetSwapInterval = Desktop_GL_GetSwapInterval;
    device->GL_SwapWindow = Desktop_GL_SwapWindow;
    device->GL_DeleteContext = Desktop_GL_DeleteContext;

    device->SetClipboardText = Desktop_SetClipboardText;
    device->GetClipboardText = Desktop_GetClipboardText;
    device->HasClipboardText = Desktop_HasClipboardText;
    
    device->free = DUMMY_DeleteDevice;

    return device;
}

VideoBootStrap DUMMY_bootstrap = {
    DUMMYVID_DRIVER_NAME, "SDL dummy video driver",
    DUMMY_Available, DUMMY_CreateDevice
};

/* Available video drivers */
static VideoBootStrap *bootstrap[] = {
#if SDL_VIDEO_DRIVER_COCOA
    &COCOA_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_X11
    &X11_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_MIR
    &MIR_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_DIRECTFB
    &DirectFB_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_WINDOWS
    &WINDOWS_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_HAIKU
    &HAIKU_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_PANDORA
    &PND_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_UIKIT
    &UIKIT_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_ANDROID
    &Android_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_PSP
    &PSP_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_RPI
    &RPI_bootstrap,
#endif 
#if SDL_VIDEO_DRIVER_WAYLAND
    &Wayland_bootstrap,
#endif
#if SDL_VIDEO_DRIVER_DUMMY
    &DUMMY_bootstrap,
#endif
    NULL
};

static int Desktop_VideoInitInternal(_THIS)
{
    SDL_VideoDevice** init_device = (SDL_VideoDevice**)&_this->driverdata;
    SDL_VideoDevice *video;
    const char *driver_name = NULL;
    const char *hint;
    int i;

    /* Select the proper video driver */
    video = NULL;
    if (driver_name == NULL) {
        driver_name = SDL_getenv("SDL_VIDEODRIVER");
    }
    if (driver_name != NULL) {
        for (i = 0; bootstrap[i]; ++i) {
            if (SDL_strncasecmp(bootstrap[i]->name, driver_name, SDL_strlen(driver_name)) == 0) {
                if (bootstrap[i]->available()) {
                    video = bootstrap[i]->create(index);
                    break;
                }
            }
        }
    } else {
        for (i = 0; bootstrap[i]; ++i) {
            if (bootstrap[i]->available()) {
                video = bootstrap[i]->create(index);
                if (video != NULL) {
                    break;
                }
            }
        }
    }
    if (video == NULL) {
        if (driver_name) {
            return SDL_SetError("%s not available", driver_name);
        }
        return SDL_SetError("No available video device");
    }
    *init_device = video;
    (*init_device)->name = bootstrap[i]->name;
    (*init_device)->next_object_id = 1;

    /* Set some very sane GL defaults */
    (*init_device)->gl_config.driver_loaded = 0;
    (*init_device)->gl_config.dll_handle = NULL;
    (*init_device)->gl_config.red_size = 3;
    (*init_device)->gl_config.green_size = 3;
    (*init_device)->gl_config.blue_size = 2;
    (*init_device)->gl_config.alpha_size = 0;
    (*init_device)->gl_config.buffer_size = 0;
    (*init_device)->gl_config.depth_size = 16;
    (*init_device)->gl_config.stencil_size = 0;
    (*init_device)->gl_config.double_buffer = 1;
    (*init_device)->gl_config.accum_red_size = 0;
    (*init_device)->gl_config.accum_green_size = 0;
    (*init_device)->gl_config.accum_blue_size = 0;
    (*init_device)->gl_config.accum_alpha_size = 0;
    (*init_device)->gl_config.stereo = 0;
    (*init_device)->gl_config.multisamplebuffers = 0;
    (*init_device)->gl_config.multisamplesamples = 0;
    (*init_device)->gl_config.retained_backing = 1;
    (*init_device)->gl_config.accelerated = -1;  /* accelerated or not, both are fine */
    (*init_device)->gl_config.profile_mask = 0;
#if SDL_VIDEO_OPENGL
    (*init_device)->gl_config.major_version = 2;
    (*init_device)->gl_config.minor_version = 1;
#elif SDL_VIDEO_OPENGL_ES2
    (*init_device)->gl_config.major_version = 2;
    (*init_device)->gl_config.minor_version = 0;
    (*init_device)->gl_config.profile_mask = SDL_GL_CONTEXT_PROFILE_ES;
#elif SDL_VIDEO_OPENGL_ES
    (*init_device)->gl_config.major_version = 1;
    (*init_device)->gl_config.minor_version = 1;
    (*init_device)->gl_config.profile_mask = SDL_GL_CONTEXT_PROFILE_ES;
#endif
    (*init_device)->gl_config.flags = 0;
    (*init_device)->gl_config.framebuffer_srgb_capable = 0;

    (*init_device)->gl_config.share_with_current_context = 0;

    (*init_device)->current_glwin_tls = SDL_TLSCreate();
    (*init_device)->current_glctx_tls = SDL_TLSCreate();

    /* Initialize the video subsystem */
    if ((*init_device)->VideoInit(*init_device) < 0) {
        return -1;
    }

    /* Duplicate display list to original code */
    if (_this->displays != NULL) {
        (*init_device)->displays = _this->displays;
        (*init_device)->num_displays = _this->num_displays;
        for (int i = 0; i < _this->num_displays; ++i) {
            _this->displays[i].fullscreen_window =
                SDL_CreateWindow("SDL Virtual Desktop", 0, 0,
                    _this->displays[i].current_mode.w,
                    _this->displays[i].current_mode.h,
                    SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN |
                    SDL_WINDOW_BORDERLESS);
        }
    }
    return 0;
}

int
Desktop_VideoInit(_THIS)
{
    SDL_DisplayMode mode;

    /* Use a fake 32-bpp desktop mode */
    mode.format = SDL_PIXELFORMAT_RGB888;
    mode.w = 1024;
    mode.h = 768;
    mode.refresh_rate = 0;
    mode.driverdata = NULL;
    _this->driverdata = NULL;
    
    Desktop_VideoInitInternal(_this);

    /* We're done! */
    return 0;
}

static int
Desktop_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode)
{
    return 0;
}

void
Desktop_VideoQuit(_THIS)
{
    CALL_INTERNAL(VideoQuit);
}

#endif /* SDL_VIDEO_DRIVER_DUMMY */

/* vi: set ts=4 sw=4 expandtab: */
