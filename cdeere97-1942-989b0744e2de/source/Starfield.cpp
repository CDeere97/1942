//Initialise a bunch of random X and Y values for stars,
//within the bounds of the screen area.
#include <cstdlib>
#include "Position.h"
#include "Starfield.h"
#include <UGFW.h>

Starfield::Starfield()
{
	UG::GetScreenSize(iScreenWidth, iScreenHeight);
}


void Starfield::Initialise()
{
	Position *stop = pos + (sizeof(pos) / sizeof(Position));

	for (Position *posArray = pos; posArray < stop; posArray++)
	{
		int newX = rand() % iScreenWidth;
		int newY = rand() % iScreenHeight;

		(*posArray).Set(newX, newY);
	}
}

void Starfield::DrawStars()
{
	Position *stop = pos + (sizeof(pos) / sizeof(Position));

	for (Position *posArray = pos; posArray < stop; posArray++)
	{
		float posX = 0.f, posY = 0.f;
		stop->Get(posX, posY);

		UG::DrawLine(posX - 1, posY - 1, posX + 1, posY + 1);
		UG::DrawLine(posX - 1, posY + 1, posX + 1, posY - 1);
	}
}

void Starfield::UpdateStars()
{
	Position *stop = pos + (sizeof(pos) / sizeof(Position));

	for (Position *posArray = pos; posArray < stop; posArray++)
	{
		float posX = 0.f, posY = 0.f;
		posArray->Get(posX, posY);

		float newPosX = --posX;
		posArray->Set(newPosX, posY);

		if (newPosX < 0)
		{
			posArray->Set(iScreenWidth, rand() % iScreenHeight);
		}
	}
}