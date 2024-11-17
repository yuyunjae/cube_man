#pragma once

//////////////////////////////////////////////////////////////////////////////

#ifndef _CUBE_H_
#define _CUBE_H_

#define _CRT_SECURE_NO_WARNINGS

//----------------------------------------------------------------------------
// 
// --- Include system headers ---
//

#include <cmath>
#include <iostream>

//----------------------------------------------------------------------------
//
// --- Include OpenGL header files and helpers ---
//
//   The location of these files vary by operating system.  We've included
//     copies of open-soruce project headers in the "GL" directory local
//     this this "include" directory.
//

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include "GL/glew.h"
#  include "GL/freeglut.h"
#  include "GL/freeglut_ext.h"
#endif  // __APPLE__

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

//----------------------------------------------------------------------------
//
//  --- Include our class libraries and constants ---
//

//  Helper function to load vertex and fragment shader files
GLuint InitShader(const char* vertexShaderFile, const char* fragmentShaderFile);

//  Defined constant for when numbers are too small to be used in the
//    denominator of a division operation.  This is only used if the
//    DEBUG macro is defined.
const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);


#pragma comment(lib, "glew32.lib")

//#include "CheckError.h"

// #define Print(x)  do { std::cerr << #x " = " << (x) << std::endl; } while(0)


#endif // _CUBE_H_
