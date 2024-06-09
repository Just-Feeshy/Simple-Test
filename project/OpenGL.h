#pragma once

#if defined(ANDROID)

#define GLES

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#elif defined(EMSCRIPTEN)

#define GLES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#elif defined(IPHONE)

#define GLES
#define GLES3_API

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#elif defined(LINUX)

#define GLES3_API
#define NEED_EXTENSIONS
#define DYNAMIC_OGL
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#define FORCE_NON_PO2

#elif defined(MACOSX)

// #define GLES3_API (Might be needed for ANGLE)
#define NEED_EXTENSIONS
#define DYNAMIC_OGL
#define GL_GLEXT_PROTOTYPES

#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#define FORCE_NON_PO2

#define glBindFramebuffer glBindFramebufferEXT
#define glBindRenderbuffer glBindRenderbufferEXT
#define glGenFramebuffers glGenFramebuffersEXT
#define glDeleteFramebuffers glDeleteFramebuffersEXT
#define glGenRenderbuffers glGenRenderbuffersEXT
#define glDeleteRenderbuffers glDeleteRenderbuffersEXT
#define glFramebufferRenderbuffer glFramebufferRenderbufferEXT
#define glFramebufferTexture2D glFramebufferTexture2DEXT
#define glRenderbufferStorage glRenderbufferStorageEXT
#define glCheckFramebufferStatus glCheckFramebufferStatusEXT
#define glCheckFramebufferStatus glCheckFramebufferStatusEXT
#define glGenerateMipmap glGenerateMipmapEXT
#define glGetFramebufferAttachmentParameteriv glGetFramebufferAttachmentParameterivEXT
#define glGetRenderbufferParameteriv glGetRenderbufferParameterivEXT
#define glIsFramebuffer glIsFramebufferEXT
#define glIsRenderbuffer glIsRenderbufferEXT

#elif defined(WINDOWS)

#include <windows.h>
#ifndef NATIVE_TOOLKIT_SDL_ANGLE
#include <gl/GL.h>
#endif

typedef ptrdiff_t GLsizeiptrARB;
#define NEED_EXTENSIONS
#define DYNAMIC_OGL

#ifdef NATIVE_TOOLKIT_SDL_ANGLE
#define GLES
#endif

#ifdef NATIVE_TOOLKIT_SDL_ANGLE
#include <SDL_opengles2.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif

#define glBindBuffer glBindBufferEXT
#define glGenBuffers glGenBuffersEXT
#define glBufferData glBufferDataEXT
#define glAttachShader glAttachShaderEXT
#define glCompileShader glCompileShaderEXT
#define glCreateProgram glCreateProgramEXT
#define glCreateShader glCreateShaderEXT
#define glEnableVertexAttribArray glEnableVertexAttribArrayEXT
#define glGetAttribLocation glGetAttribLocationEXT
#define glGetShaderiv glGetShaderivEXT
#define glGetShaderInfoLog glGetShaderInfoLogEXT
#define glLinkProgram glLinkProgramEXT
#define glShaderSource glShaderSourceEXT
#define glUseProgram glUseProgramEXT
#define glVertexAttribPointer glVertexAttribPointerEXT
#define glBindVertexArray glBindVertexArrayEXT
#define glGenVertexArrays glGenVertexArraysEXT
#define glIsFramebuffer glIsFramebufferEXT
#define glIsRenderbuffer glIsRenderbufferEXT

#endif


#ifdef WINDOWS
typedef HDC WinDC;
typedef HGLRC WinGLContext;
#else
typedef void* WinDC;
typedef void* WinGLContext;
#endif

#ifndef GL_BUFFER_SIZE

#define GL_BUFFER_SIZE                0x8764
#define GL_BUFFER_USAGE               0x8765
#define GL_ARRAY_BUFFER               0x8892
#define GL_ELEMENT_ARRAY_BUFFER       0x8893
#define GL_ARRAY_BUFFER_BINDING       0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY                  0x88B8
#define GL_WRITE_ONLY                 0x88B9
#define GL_READ_WRITE                 0x88BA
#define GL_BUFFER_ACCESS              0x88BB
#define GL_BUFFER_MAPPED              0x88BC
#define GL_BUFFER_MAP_POINTER         0x88BD
#define GL_STREAM_DRAW                0x88E0
#define GL_STREAM_READ                0x88E1
#define GL_STREAM_COPY                0x88E2
#define GL_STATIC_DRAW                0x88E4
#define GL_STATIC_READ                0x88E5
#define GL_STATIC_COPY                0x88E6
#define GL_DYNAMIC_DRAW               0x88E8
#define GL_DYNAMIC_READ               0x88E9
#define GL_DYNAMIC_COPY               0x88EA
#define GL_COMPILE_STATUS             0x8B81
#define GL_LINK_STATUS                0x8B82
#define GL_VALIDATE_STATUS            0x8B83
#define GL_INFO_LOG_LENGTH            0x8B84
#define GL_ATTACHED_SHADERS           0x8B85
#define GL_ACTIVE_UNIFORMS            0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH  0x8B87
#define GL_SHADER_SOURCE_LENGTH       0x8B88
#define GL_VERTEX_SHADER              0x8B31
#define GL_FRAGMENT_SHADER            0x8B30
#define GL_TEXTURE0                   0x84C0
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE              0x812F
#endif

#ifndef GL_POINT_SMOOTH
#define GL_POINT_SMOOTH               0x0B10
#endif

#ifndef GL_LINE_SMOOTH
#define GL_LINE_SMOOTH                0x0B20
#endif

#ifdef NEED_EXTENSIONS

#include "OpenGLExtensions.h"
#define CHECK_EXT (x) x

#else

#define CHECK_EXT (x) 1

#endif
