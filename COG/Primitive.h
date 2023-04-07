#pragma once

/*******************************************************************************************************************
	Primitive.h, Primitive.cpp
	Created by Kim Kane
	Last updated: 08/04/2018
	Class finalized: 08/04/2018

	A base class for all our in-game primitives.

	[Features]
	Nothing fancy.

	[Upcoming]
	Nothing.

	[Side Notes]
	All primitive objects should be created within a game object/entity or interface object.
	All they are is a shape - nothing more, and so they do not have a position in the world (no transform).
	The tag they use is the tag of the object they are contained within.
	Rendering them as a standalone will result in them always being at the origin.

*******************************************************************************************************************/
#include <string>

class Primitive {

public:
	Primitive(const std::string& tag);
	virtual ~Primitive();

public:
	virtual void Render() = 0;

protected:
	std::string m_tag;
};
