#pragma once
#include <memory>
#include <sstream>
#include <iostream>
#include <stdexcept>
/*
	This file literally just holds definitions for various warning messages and error messages across the program. This serves the purpose of standardizing
	the engine's messages in a way that is as clear and easy to read as possible! 
*/

#define OGL3D_ERROR(message)\
{\
std::stringstream m; \
m << "OGL3D Error: " << message << std::endl;\
throw std::runtime_error(m.str());\
}

#define OGL3D_WARNING(message)\
std::wclog << "OGL3D Warning: " << message << std::endl;

#define OGL3D_INFO(message)\
std::wclog << "OGL3D Info: " << message << std::endl;