#pragma once

/*******************************************************************************************************************
	Log.h, Log.cpp
	Created by Kim Kane
	Last updated: 04/01/2018
	
	A simple logging system that displays a message to the console window in a number of different colours.
	The colour intesity can be changed by passing true or false as an argument to the Colour function.
	
	More log types can be added to the LogType enum if necessary.
	This must also be updated within the switch statement in the Debug function.
	
	Supports variables of other data types (int, float, etc.) - when not passing a variable use FL_LOG_EMPTY.
	A macro can also be used for simplicity: #define LOG(message) std::cout << message << std::endl;

	Also supports OpenGL error messages (see defined macros below).

*******************************************************************************************************************/
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>

#define FL_LOG_EMPTY -1

/*******************************************************************************************************************
	Macros have been defined for differentiating between Debug and Release modes.
	The Debug and OpenGLDebug functions will only be called during Debug mode.

	You could also use ASSERT(function) if(!(function)) __debugbreak(); to generate a breakpoint.
*******************************************************************************************************************/
#if DEBUG_MODE == 1
	#define FL_LOG(message, variable, type) Debug(message, variable, type)
	#define FL_GLCALL(function) function; OpenGLDebug(#function, __FILE__, __LINE__)
#elif defined(RELEASE_MODE)
	#define FL_LOG(message, variable, type)
	#define FL_GLCALL(function) function
#endif

/*******************************************************************************************************************
	Enums that define the log type (error message, etc.) and the log colour (colour of text in console)
*******************************************************************************************************************/
enum LogType	{ LOG_MESSAGE, LOG_WARN, LOG_ERROR, LOG_SUCCESS, LOG_MEMORY, LOG_RESOURCE, LOG_BREAK };
enum LogColour	{ COLOR_GREY, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN, COLOR_RED, COLOR_PINK, COLOR_YELLOW, COLOR_WHITE };

void Colour(LogColour colour, bool intensity = true);
void OpenGLDebug(const char* function, const char* fileName, int line);

/*******************************************************************************************************************
	Debug function that outputs text to the console window. Replaces std::cout << for faster debugging
*******************************************************************************************************************/
template <typename T> inline void Debug(const std::string& message, T variable, LogType type)
{
	//--- Create a local string to hold final debug message
	std::string outputString; 

	//--- Create a local sstream that holds the T variable data (int, float, char, etc.)
	std::stringstream tempVariable;

	//--- Store this data in our sstream so we can return a string copy of it using .str()
	tempVariable << variable; 
	
	//--- If variable is FL_LOG_EMPTY, just return the message. Otherwise, concatenate the strings
	if (variable != (T)FL_LOG_EMPTY)	{ outputString = message + tempVariable.str(); } 
	else								{ outputString = message; }

	//--- Switch to check the passed in LogType and output the passed in message and/or variable (complete with pretty colours), to make debugging easy and fast
	switch (type)  {

		case LOG_MESSAGE:	{ Colour(COLOR_GREY); std::cout << "[MESSAGE] " << outputString << std::endl; break; }
		case LOG_WARN:		{ Colour(COLOR_YELLOW); std::cout << "[WARNING] " << outputString << std::endl; break; }
		case LOG_ERROR:		{ Colour(COLOR_RED); std::cout << "[ERROR] " << outputString << std::endl; break; }
		case LOG_SUCCESS:	{ Colour(COLOR_GREEN); std::cout << "[SUCCESS] " << outputString << std::endl; break; }
		case LOG_MEMORY:	{ Colour(COLOR_PINK); std::cout << "[MEMORY] " << outputString << std::endl; break; }
		case LOG_RESOURCE:	{ Colour(COLOR_CYAN); std::cout << "[RESOURCE] " << outputString << std::endl; break; }
		case LOG_BREAK:		{ Colour(COLOR_WHITE); std::cout << "------------------------" << outputString << std::endl; break; }
	}

	Colour(COLOR_GREY);
}