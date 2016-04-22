// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_LOADER_HPP
#define _MAP_LOADER_HPP

#include <string>
#include <map>
using namespace std;

typedef std::map<std::string,std::string> MessageMap;

// a basic window abstraction - demo purposes only
class WindowSettings
{
public:
	int x,y,w,h;
	string name;

	WindowSettings()
		: x(0), y(0), w(100), h(100), name("Untitled")
	{
	}

	WindowSettings(int x, int y, int w, int h, const string& name)
	{
		this->x=x;
		this->y=y;
		this->w=w;
		this->h=h;
		this->name=name;
	}
};

#endif
