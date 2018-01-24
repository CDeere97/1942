////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: 1942
// Author: Chris Deere
// Date Created: 20/11/16
// Brief: The first assignment for CT4019 - Programming and Mathematics for Games in which we have to remake a classic arcade game, I chose 1942
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _Plane_H_
#define _Plane_H_
#include "Position.h"

// This is our structure definition

class Plane
{
public: //All public??

	//Plane Data with default values assigned
	int iSpriteID = -1;
	//Width & Height variables
	int iWidth = 65;
	int iHeight = 65;
	//Plane Speed variable
	int iSpeed = 1;
	//Plane screen position variables
	Position pos;
	//Plane Input Key variables
	short upKey = -1;
	short downKey = -1;
	short leftKey = -1;
	short rightKey = 1;

};

#endif
