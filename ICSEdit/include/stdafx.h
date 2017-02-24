/*
(c) 2016,2017 Grain

This file is part of ICSEdit.

ICSEdit is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ICSEdit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ICSEdit.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include <unordered_map>
#include <vector>

#include <stdexcept>
#include <cassert>

//my headers
#define DO_LOG
#include "util/basic/console_log.h"

#define FILE_SIG ("(at " + std::string(__FILE__) + ":" + std::to_string(__LINE__) + ")")
#define FILE_FUNC_SIG ("(at " + std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " + std::string(__func__) + "())")

