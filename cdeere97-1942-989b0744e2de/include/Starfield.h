#ifndef _STARFIELD_H_
#define _STARFIELD_H_

#include "Position.h"

class Starfield
{
public:
	void Initialise();
	void DrawStars();
	void UpdateStars();
	Starfield();

private:
	int iScreenWidth, iScreenHeight;
	Position pos[100];
};

#endif
