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
*/#include "../../SDL_internal.h"

#if SDL_VIDEO_DRIVER_DUMMY

#include "SDL_nullvideo.h"
#include "SDL_nullopengl.h"
#include "../SDL_sysvideo.h"
#include "SDL_render.h"
#include "SDL_opengl.h"

typedef struct
{
    PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
    PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
    PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
    PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
    PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
    void APIENTRY (*glGetIntegerv)(GLenum pname, GLint * params);
    GLint APIENTRY (*glGetError)();
    void APIENTRY (*glGenTextures)(GLsizei n, GLuint * textures);
    void APIENTRY (*glBindTexture)(GLenum, GLuint);
} GL_Data;

static void Desktop_BindFramebufferEXT(GLenum target, GLuint framebuffer)
{
    SDL_LogError(0, "test");
    SDL_Window* window = SDL_GL_GetCurrentWindow();
    GL_Data* gl_data = NULL;
    
    SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
    if (display != NULL && display->fullscreen_window != NULL) {
        gl_data = SDL_GetWindowData(
                                    display->fullscreen_window, "_GL_Data"
                                    );
        if (framebuffer == 0)
        {
            framebuffer = SDL_GetWindowData(
                                            window, "_GL_WindowFramebuffer"
                                            );
        }
        gl_data->glBindFramebufferEXT(target, framebuffer);
    } else {
        gl_data = SDL_GetWindowData(
                                    window, "_GL_Data"
                                    );
        gl_data->glBindFramebufferEXT(target, framebuffer);
    }
}

int Desktop_GL_LoadLibrary(_THIS, const char *path)
{
	RETURN_INTERNAL(GL_LoadLibrary, path)
}

void *Desktop_Internal_GL_GetProcAddress(_THIS, const char *proc)
{
    RETURN_INTERNAL(GL_GetProcAddress, proc);
}

void *Desktop_GL_GetProcAddress(_THIS, const char *proc)
{
    if (strcmp(proc, "glBindFramebuffer") != 0 &&
        strcmp(proc, "glBindFramebufferEXT") != 0)
    {
        return Desktop_Internal_GL_GetProcAddress(_this, proc);
    }
    else
    {
        return &Desktop_BindFramebufferEXT;
    }
}

void Desktop_GL_UnloadLibrary(_THIS)
{
	CALL_INTERNAL(GL_UnloadLibrary)
}

SDL_GLContext Desktop_Internal_GL_CreateContext(_THIS, SDL_Window * window)
{
    RETURN_INTERNAL(GL_CreateContext, window)
}

SDL_GLContext Desktop_GL_CreateContext(_THIS, SDL_Window * window)
{
    if (window != NULL && window->driverdata == NULL) {
        SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
        if (display != NULL && display->fullscreen_window != NULL) {
            GL_Data* gl_data = SDL_GetWindowData(display->fullscreen_window, "_SDL_GL_Data");
            
            SDL_Renderer* fullscreen_renderer = SDL_GetWindowData(
                    display->fullscreen_window, "_SDL_WindowRenderer"
            );
            
            SDL_Texture* texture =
            SDL_CreateTexture(fullscreen_renderer,
                              SDL_GetWindowPixelFormat(window),
                              SDL_TEXTUREACCESS_TARGET, window->w, window->h);
            GLint texture_id = SDL_GL_BindTexture(texture, NULL, NULL);
            int error = gl_data->glGetError();
            SDL_LogError(0, "GLError %d", error);
            SDL_SetWindowData(window, "_SDL_WindowTexture", texture);
            SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
            ((SDL_VideoDevice*)_this->driverdata)->gl_config.share_with_current_context = 1;
            SDL_GLContext context = Desktop_Internal_GL_CreateContext(_this, display->fullscreen_window);
            GLuint framebuffer = 0;
            gl_data->glGenFramebuffersEXT(1, &framebuffer);
            SDL_SetWindowData(window, "_SDL_WindowFramebuffer", framebuffer);
            gl_data->glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
            gl_data->glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, texture_id, 0);
             error = gl_data->glGetError();
            SDL_LogError(0, "GLError %d", error);
            return context;
        }
    } else {
        GL_Data* gl_data = SDL_GetWindowData(window, "_SDL_GL_Data");
        if (gl_data == NULL)
        {
            gl_data = SDL_malloc(sizeof(GL_Data));
            gl_data->glGenFramebuffersEXT = Desktop_Internal_GL_GetProcAddress(_this, "glGenFramebuffersEXT");
            gl_data->glDeleteFramebuffersEXT = Desktop_Internal_GL_GetProcAddress(_this, "glDeleteFramebuffersEXT");
            gl_data->glFramebufferTexture2DEXT = Desktop_Internal_GL_GetProcAddress(_this, "glFramebufferTexture2DEXT");
            gl_data->glBindFramebufferEXT = Desktop_Internal_GL_GetProcAddress(_this, "glBindFramebufferEXT");
            gl_data->glCheckFramebufferStatusEXT = Desktop_Internal_GL_GetProcAddress(_this, "glCheckFramebufferStatusEXT");
            gl_data->glGetIntegerv = Desktop_Internal_GL_GetProcAddress(_this, "glGetIntegerv");
            gl_data->glGetError = Desktop_Internal_GL_GetProcAddress(_this, "glGetError");
            gl_data->glBindTexture = Desktop_Internal_GL_GetProcAddress(_this, "glBindTexture");
            gl_data->glGenTextures = Desktop_Internal_GL_GetProcAddress(_this, "glGenTextures");
            SDL_SetWindowData(window, "_SDL_GL_Data", gl_data);
        }
        return Desktop_Internal_GL_CreateContext(_this, window);
    }
}

int Desktop_GL_MakeCurrent(_THIS, SDL_Window * window,
                              SDL_GLContext context)
{
    if (window != NULL && window->driverdata == NULL) {
        SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
        if (display != NULL && display->fullscreen_window != NULL) {
            RETURN_INTERNAL(GL_MakeCurrent, display->fullscreen_window, context)
        }
    } else{
        RETURN_INTERNAL(GL_MakeCurrent, window, context)
    }
}

int Desktop_GL_SetSwapInterval(_THIS, int interval)
{
	//CALL_INTERNAL(GL_SetSwapInterval, interval)
}

int Desktop_GL_GetSwapInterval(_THIS)
{
	//CALL_INTERNAL(GL_GetSwapInterval)
}

void Desktop_GL_SwapWindow(_THIS, SDL_Window * window)
{
    if (window->driverdata == NULL) {
        SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
        if (display != NULL && display->fullscreen_window != NULL) {
            SDL_Rect window_src;
            window_src.x = 0;
            window_src.y = 0;
            window_src.w = window->w;
            window_src.h = window->h;
            
            SDL_Rect window_dst;
            window_dst.x = window->x;
            window_dst.y = window->y;
            window_dst.w = window->w;
            window_dst.h = window->h;
            
            SDL_Renderer* fullscreen_renderer = SDL_GetWindowData(
                    display->fullscreen_window, "_SDL_WindowRenderer"
            );
            
            SDL_Texture* window_texture = SDL_GetWindowData(
                    window, "_SDL_WindowTexture"
            );
            
            SDL_GLContext previous_context = SDL_GL_GetCurrentContext();
            SDL_Window* previous_window = SDL_GL_GetCurrentWindow();

            SDL_SetRenderDrawColor(fullscreen_renderer, 0, 0, 0, 255);
            SDL_RenderClear(fullscreen_renderer);
            // Base off shaped window later
            SDL_RenderCopyEx(fullscreen_renderer,
                             window_texture,
                             &window_src,
                             &window_dst,
                             0,
                             NULL,
                             SDL_FLIP_NONE);
            SDL_RenderPresent(fullscreen_renderer);
            SDL_GL_MakeCurrent(previous_context, previous_window);
        }
    } else {
        CALL_INTERNAL(GL_SwapWindow, window)
    }
}

void Desktop_GL_DeleteContext(_THIS, SDL_GLContext context)
{
	CALL_INTERNAL(GL_DeleteContext, context)
}


#endif /* SDL_VIDEO_DRIVER_DUMMY */

/* vi: set ts=4 sw=4 expandtab: */
