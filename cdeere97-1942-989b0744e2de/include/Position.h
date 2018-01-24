////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: 1942
// Author: Chris Deere
// Date Created: 20/11/16
// Brief: The first assignment for CT4019 - Programming and Mathematics for Games in which we have to remake a classic arcade game, I chose 1942
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITION_H_
#define _POSITION_H_

class Position
{
public:
	Position();
	void Set(float a_x, float a_y);
	void Get(float& a_x, float& a_y);

private:
	float x;
	float y;
};

#endif
