#pragma once

#ifdef _DEBUG

#define WXUSINGDLL

#endif

#define FREEGLUT_STATIC
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

/*
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/glcanvas.h>
*/

#ifdef WINVER

#include <Windows.h>

#endif

#include <SDL.h>

//standard libraries
#include <iostream>

#include <string>
#include <sstream>
#include <fstream>
#include <map>

#include <stdexcept>
#include <cassert>

//my headers
#define DO_LOG
#include "util/basic/console_log.h"

#define FILE_SIG ("(at " + std::string(__FILE__) + ":" + std::to_string(__LINE__) + ")")
#define FILE_FUNC_SIG ("(at " + std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " + std::string(__func__) + "())")
