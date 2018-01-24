////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: 1942
// Author: Chris Deere
// Date Created: 20/11/16
// Brief: The first assignment for CT4019 - Programming and Mathematics for Games in which we have to remake a classic arcade game, I chose 1942
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UGFW.h"
#include "Enumerations.h"

#include <time.h>
#include <sstream>
#include <Windows.h>

#include "GameStates.h"
#include "Position.h"

#include "Plane.h"

#define BOUNDARY_OFFSET 40


//Screen Dimensions
int g_iScreenWidth = 0;
int g_iScreenHeight = 0;


//Function to draw the court lines for the scene
void DrawCourt();

//#pragma region Background - All of our Back ground function defnitions
//
//struct Background
//{
//	float x;
//	float y;
//	int iSpriteID = -1;
//	int xDir = 0;
//	int yDir = 0;
//	int iSpeed = 4;
//	int iBgSize = 32;
//	
//	Position pos;
//	//A boolean variable to notify us if we need to be reset to the centre
//	bool bBackgroundReset = true;
//
//};
//void InitialiseBackground(Background& a_Background, const char* a_BackgroundImageFilename, float a_xPos, float a_yPos);
//void TileBackground(Background& a_Background);
//void SetBackgroundSpeed(Background& a_Backgorund, int a_iSpeed);
//void MoveBackground(Background& a_Background, float a_delta);
//
//
//#pragma endregion

#pragma region Plane - All of our Plane functions and structures
//\==============================================================================
//\ A Structure in C/C++ should be viewed as a simple collection of similar data into an object
//\ This is a grouping of data the in some way belongs together or has a relationship
//\ with other data.
//\ This will become more apparent with time for now view this as we intend on having
//\ a structure for a Plane object. This structure will contain all the required data for a Plane
//\ We can then use this structure as a simple blueprint for creating/instantiating a Plane
//\==============================================================================

//This is a function to initialise our Plane with some appropriate data
void InitialisePlane(Plane& a_Plane, const char* a_PlaneImageFilename, float a_xPos, float a_yPos);
//Plane movement function
void MovePlane(Plane& Plane);
//A function to set our input keys for the Plane
void SetPlaneMovementKeys(Plane& a_Plane, short a_upKey, short a_downKey, short a_leftKey, short a_rightKey);
void SetPlaneSpeed(Plane& a_Plane, int a_iSpeed);
#pragma endregion

#pragma region Enemy - All of our Enemy structure and functions
//\============================================================================================
//\ We should wrap our Enemy data up into a structure similar to our Plane data
//\============================================================================================

struct Enemy
{
	int iSpriteID = -1;
	int xDir = 0;
	int yDir = 0;
	int iSpeed = 4;
	int iRadius = 16;
	int iEWidth = 32;
	int iEHeight = 32;
	Position pos;
	//A boolean variable to notify us if we need to be reset to the centre
	bool bEnemyReset = true;

};
//we will for the most part require slightly similar functions to those provided for our Plane structure
void InitialiseEnemy(Enemy& a_Enemy, const char* a_EnemyImageFilename, float a_xPos, float a_yPos);
void SetEnemySpeed(Enemy& a_Enemy, int a_iSpeed);
void MoveEnemy(Enemy& a_Enemy);


#pragma endregion

#pragma region Bullet - All Bullet structures and definitions

struct Bullet
{
	int iSpriteID = -1;
	int xDir = 0;
	int yDir = 0;
	int iMovementSpeed = 1;
	int iShootSpeed = 6;
	int iBWidth = 32;
	int iBHeight = 32;

	Position pos;

	short upKey = -1;
	short downKey = -1;
	short leftKey = -1;
	short rightKey = 1;

	bool bBulletReset = true;
};

//Bullet Functions

void InitialiseBullet(Bullet& a_Bullet, const char* a_BulletImageFilename, float a_xPos, float a_yPos);
void AttachBullet(Bullet& a_Bullet);
//A function to set our input keys for the Plane
void SetBulletAttachKeys(Bullet& a_Bullet, short a_upKey, short a_downKey, short a_leftKey, short a_rightKey);
void SetBulletSpeed(Bullet& a_Bullet, int a_iMoveSpeed);
void ShootKeyPressed(Bullet& a_Bullet);
void ShootBullet(Bullet& a_Bullet, Plane& a_Plane);

#pragma endregion

//We will need a test collision function
void TestCollision(Enemy& a_Enemy, Plane& a_Plane, Bullet& a_Bullet);
void InitialiseGameAssets(/*Background& a_Background,*/ Enemy& a_Enemy, Plane& a_Plane, Bullet& a_Bullet);

//Player Score
int g_iPlayerScore;
//Player Lives
int g_iLives;
//Variable to see if player is shooting
bool g_bIsShooting;
//Timer variable for the splash screen
float g_fSplashTime = 5.f;
//Highscores array





void DrawScores();
void DrawHighScores();

int main(int argv, char* argc[])
{

	//In order for rand to work we need to seed it with a random value
	srand((unsigned int)time(nullptr));

	if (UG::Create(1024, 768, false, "1942", 100, 100))
	{

		UG::GetScreenSize(g_iScreenWidth, g_iScreenHeight);

		UG::SetBackgroundColor(UG::SColour(0x2A, 0x57, 0x66, 0xFF));
		UG::AddFont("./fonts/Lintsec.fnt");

		//Set up Background
		//Background background;
		//Set up Enemy.
		Enemy l_Enemy;
		//Set up  Plane
		Plane Plane;
		//Set Up Bullet
		Bullet bullet;

		GameState currentState = SPLASH;

		do
		{

			//Clear our screen of drawing
			UG::ClearScreen();

			//Start the timer for the splash screen
			float delta = UG::GetDeltaTime();
			g_fSplashTime -= delta;

			switch (currentState)
			{
			case SPLASH:
			{
				UG::SetFont("./fonts/Lintsec.fnt");
				UG::DrawString("1942", g_iScreenWidth * 0.45f, g_iScreenHeight * 0.7f, 1.5f);
				UG::DrawString("By Chris Deere", g_iScreenWidth * 0.4f, g_iScreenHeight * 0.2, 0.75f);
				UG::DrawString("Assets from GameMaker", g_iScreenWidth * 0.4f, g_iScreenHeight * 0.1f, 0.5f);

				if (UG::IsKeyDown(UG::KEY_ENTER) || g_fSplashTime < 0.f)
				{
					currentState = MENU;
				}
				break;
			}
			case MENU:
			{
				//Player Score is 0
				g_iPlayerScore = 0;
				//Player has 3 lives
				g_iLives = 3;
				//Player is not shooting
				g_bIsShooting = false;
				UG::SetFont("./fonts/Lintsec.fnt");
				UG::DrawString("1942", g_iScreenWidth * 0.45f, g_iScreenHeight * 0.7f, 1.5f);
				UG::DrawString("...PRESS SPACE TO PLAY...", g_iScreenWidth*0.2f, g_iScreenHeight * 0.55f);
				UG::DrawString("...PRESS ESCAPE TO QUIT...", g_iScreenWidth*0.2f, g_iScreenHeight * 0.5f);
				UG::DrawString("Move with the arrow keys", g_iScreenWidth*0.3f, g_iScreenHeight * 0.3f, 0.75f);
				UG::DrawString("Shoot with X", g_iScreenWidth*0.4f, g_iScreenHeight * 0.2f, 0.75f);
				if (UG::IsKeyDown(UG::KEY_SPACE))
				{

					currentState = GAMEPLAY;
					//TileBackground(background);
					InitialiseGameAssets(/*background,*/ l_Enemy, Plane, bullet);

				}
				// quit our application when escape is pressed
				if (UG::IsKeyDown(UG::KEY_ESCAPE))
				{
					UG::Close();
				}

				break;

			}
			case GAMEPLAY:
			{
				
				//TileBackground(background);
				MoveEnemy(l_Enemy);
				MovePlane(Plane);
				AttachBullet(bullet);
				ShootKeyPressed(bullet);
				ShootBullet(bullet, Plane);


				TestCollision(l_Enemy, Plane, bullet);

				
				DrawScores();


				if (UG::IsKeyDown(UG::KEY_ESCAPE) || g_iLives <= 0)
				{

					currentState = GAMEOVER;
					UG::StopDrawingSprite(l_Enemy.iSpriteID);
					UG::DestroySprite(l_Enemy.iSpriteID);

					UG::StopDrawingSprite(Plane.iSpriteID);
					UG::StopDrawingSprite(bullet.iSpriteID);
					UG::DestroySprite(Plane.iSpriteID);
					UG::DestroySprite(bullet.iSpriteID);

				}

				break;
			}
			case GAMEOVER:
			{
				
				std::ostringstream ssConverter;
				ssConverter << "You Scored: " << g_iPlayerScore;
				UG::SetFont("./fonts/Lintsec.fnt");
				UG::DrawString(ssConverter.str().c_str(), (int)(g_iScreenWidth * 0.3f), g_iScreenHeight * 0.5f);

				UG::DrawString("...PRESS ENTER TO CONTINUE...", g_iScreenWidth*0.2f, g_iScreenHeight * 0.3f);
				
				if (UG::IsKeyDown(UG::KEY_ENTER))
				{
					currentState = HIGHSCORES;
				}
				break;
			}
			case HIGHSCORES:
			{
				UG::SetFont("./fonts/Lintsec.fnt");

				UG::DrawString("High Scores: ", g_iScreenWidth * 0.40f, g_iScreenHeight * 0.9f);
				UG::DrawString("...PLAY AGAIN (Y)ES OR (N)O...", g_iScreenWidth*0.2f, g_iScreenHeight * 0.3f);

				if(UG::IsKeyDown(UG::KEY_Y))
				{
					currentState = MENU;
				}
				else if (UG::IsKeyDown(UG::KEY_N))
				{
					UG::Close();
				}

				DrawHighScores();

				break;
			}
			default:

				break;
			}

			DrawCourt();
			UG::SetFont(nullptr);

		} while (UG::Process());




		UG::Dispose();


	}
	return 0;
}

void DrawScores()
{
	std::ostringstream ssConverter;

	ssConverter << "Score: " << g_iPlayerScore << "                        1942                       " <<"Lives: "<< g_iLives;

	
	UG::SetFont("./fonts/Lintsec.fnt");
	UG::DrawString(ssConverter.str().c_str(), (int)(g_iScreenWidth * 0.01f), g_iScreenHeight - 2, 1.f);

	
}

void DrawHighScores()
{
	const int c_iMaxScoreSize = 10;
    int iArrayScores[c_iMaxScoreSize] = { 1000, 900, 800, 700, 600, 500, 400, 300, 200, 100 };
	int iTemp;

	if (g_iPlayerScore > iArrayScores[c_iMaxScoreSize - 1])
	{
		iArrayScores[c_iMaxScoreSize - 1] = g_iPlayerScore;

		for (int c = 0; c < (c_iMaxScoreSize - 1); c++)
		{
			for (int d = 0; d < c_iMaxScoreSize - c - 1; d++)
			{
				if (iArrayScores[d] < iArrayScores[d + 1])
				{
					iTemp = iArrayScores[d];
					iArrayScores[d] = iArrayScores[d + 1];
					iArrayScores[d + 1] = iTemp;
				}
			}
		}
	}

	

	for (int i = 0; i < c_iMaxScoreSize; i++)
	{

		std::ostringstream ssConverter;
		ssConverter << iArrayScores[i];


		UG::DrawString(ssConverter.str().c_str(), (int)(g_iScreenWidth * 0.45f), g_iScreenHeight * (0.85 - (i * 0.05f)));

	}

	
	
}

void DrawCourt()
{
	int iScreenWidth = 0, iScreenHeight = 0;
	UG::GetScreenSize(iScreenWidth, iScreenHeight);
	//\==============================================================================================================================
	//Some simple Line drawing to draw the lines on the court.
	//Top and Bottom lines
	UG::DrawLine(0, iScreenHeight - BOUNDARY_OFFSET, iScreenWidth, iScreenHeight - BOUNDARY_OFFSET, UG::SColour(0xFF, 0xFF, 0xFF, 0xFF));
	//UG::DrawLine(0, BOUNDARY_OFFSET, iScreenWidth, BOUNDARY_OFFSET, UG::SColour(0xFF, 0xFF, 0xFF, 0xFF));
	//Vertical court separator
	/*int courtWidth = iScreenHeight - 80;
	int lineSegmentLength = courtWidth / 25;
	for (int iStart = BOUNDARY_OFFSET; iStart < iScreenHeight - BOUNDARY_OFFSET; iStart += (2 * lineSegmentLength))
	{
	int iEnd = iStart + lineSegmentLength;
	UG::DrawLine((int)(iScreenWidth * 0.5f), iStart, (int)(iScreenWidth * 0.5f), iEnd, UG::SColour(0xFF, 0xFF, 0xFF, 0xFF));
	}*/
	//\==============================================================================================================================
}

//\==============================================================================================================================

//#pragma region Background Structure Function Definitions 
//
////void InitialiseBackground(Background& a_Background, const char* a_BackgroundImageFilename, float a_xPos, float a_yPos)
////{
////	a_Background.pos.Set(a_xPos, a_yPos);
////	a_Background.iSpriteID = UG::CreateSprite(a_BackgroundImageFilename, (float)(a_Background.iBgSize), (float)(a_Background.iBgSize), true);
////	UG::MoveSprite(a_Background.iSpriteID, a_xPos, a_yPos);
////}
//
//void TileBackground(Background& a_Background)
//{
//	//Tile array
//	Background tiles[832];
//
//	tiles[0].iSpriteID = UG::CreateSprite("./images/water.png", 32, 32);
//	UG::DrawSprite(tiles[0].iSpriteID);
//
//	//stick our first tile in the lower left corner of the screen
//	float halfSize = tiles[0].iBgSize * 0.5f;
//	float xPos = halfSize;
//	float yPos = halfSize;
//	tiles[0].x = xPos;
//	tiles[0].y = yPos;
//
//	//now we can use the duplicate sprite function in the framework for the rest of the sprites
//	for (int i = 1; i < 832; ++i)
//	{
//		//duplicate the sprite from our initial sprite foe this example
//		tiles[i].iSpriteID = UG::DuplicateSprite(tiles[0].iSpriteID);
//		UG::DrawSprite(tiles[i].iSpriteID);
//
//		//lets increment our X value by our sprites size
//		xPos += tiles[i].iBgSize;
//		//if we go out the bounds of the screen then we should reset our x pos to the halfsize 
//		//and increment our y value
//		if (xPos > g_iScreenWidth)
//		{
//			yPos += tiles[i].iBgSize;
//			xPos = halfSize;
//		}
//		//set the sprites x & y Pos
//		tiles[i].x = xPos;
//		tiles[i].y = yPos;
//	}
//}
//
//void SetBackgroundSpeed(Background& a_Backgorund, int a_iSpeed)
//{
//
//}
//
//void MoveBackground(Background& a_Background, float a_delta)
//{
//	a_Background.y -= a_delta * a_Background.iSpeed;
//	float halfSize = 0.5f * a_Background.iBgSize;
//	if (a_Background.y < -halfSize)
//	{
//		//we have gone over the bottom limits of our scroll field
//		//work out by how much we have traversed over our limit -- this bit is important to stop gaps from appearing in our tiles
//		float diff = -halfSize - a_Background.y; //y is negative so this will add on the positive as two negatives make a positive
//									//now move our tile to our off screen buffer row 
//		a_Background.y = (g_iScreenHeight + 1.5f * a_Background.iBgSize) - diff;
//	}
//	UG::MoveSprite(a_Background.iSpriteID, a_Background.x, a_Background.y);
//}
//
//#pragma endregion

#pragma region Plane Structure Function Definitions

void InitialisePlane(Plane& a_Plane, const char* a_PlaneImageFilename, float a_xPos, float a_yPos)
{
	a_Plane.pos.Set(a_xPos, a_yPos);
	a_Plane.iSpriteID = UG::CreateSprite(a_PlaneImageFilename, (float)a_Plane.iWidth, (float)a_Plane.iHeight);
	float fX = 0.f, fY = 0.f;
	a_Plane.pos.Get(fX, fY);
	UG::MoveSprite(a_Plane.iSpriteID, fX, fY);
}

void SetPlaneMovementKeys(Plane& a_Plane, short a_upKey, short a_downKey, short a_leftKey, short a_rightKey)
{
	a_Plane.upKey = a_upKey;
	a_Plane.downKey = a_downKey;
	a_Plane.leftKey = a_leftKey;
	a_Plane.rightKey = a_rightKey;
}

void SetPlaneSpeed(Plane& a_Plane, int a_iSpeed)
{
	a_Plane.iSpeed = a_iSpeed;
}


void MovePlane(Plane& a_Plane)
{
	int iYDirection = 0;
	int iXDirection = 0;

	if (UG::IsKeyDown(a_Plane.upKey))
	{
		iYDirection = 1;
	}
	if (UG::IsKeyDown(a_Plane.downKey))
	{
		iYDirection = -1;
	}
	if (UG::IsKeyDown(a_Plane.leftKey))
	{
		iXDirection = -1;
	}
	if (UG::IsKeyDown(a_Plane.rightKey))
	{
		iXDirection = 1;
	}
	float pX = 0.f, pY = 0.f;
	a_Plane.pos.Get(pX, pY);

	float newPositionY = pY + iYDirection * a_Plane.iSpeed;
	float newPositionX = pX + iXDirection * a_Plane.iSpeed;
	float fHalfHeight = a_Plane.iHeight * 0.5f;
	float fHalfWidth = a_Plane.iWidth * 0.5f;

	if (newPositionY - fHalfHeight < 0.f)
	{
		newPositionY = 0 + fHalfHeight;
	}
	else if (newPositionY + fHalfHeight > g_iScreenHeight)
	{
		newPositionY = g_iScreenHeight - fHalfHeight;
	}
	else if (newPositionX + fHalfWidth > g_iScreenWidth)
	{
		newPositionX = g_iScreenWidth - fHalfWidth;
	}
	else if (newPositionX - fHalfWidth < 0.f)
	{
		newPositionX = 0 + fHalfWidth;
	}
	pY = newPositionY;
	pX = newPositionX;

	UG::MoveSprite(a_Plane.iSpriteID, pX, pY);
	a_Plane.pos.Set(pX, pY);

}

#pragma endregion
//\==============================================================================================================================

#pragma region Enemy Structure Function Definitions

void InitialiseEnemy(Enemy& a_Enemy, const char* a_EnemyImageFilename, float a_xPos, float a_yPos)
{
	a_Enemy.pos.Set(a_xPos, a_yPos);
	a_Enemy.iSpriteID = UG::CreateSprite(a_EnemyImageFilename, (float)(a_Enemy.iEWidth), (float)(a_Enemy.iEHeight), true);
	UG::MoveSprite(a_Enemy.iSpriteID, a_xPos, a_yPos);
}

void SetEnemySpeed(Enemy& a_Enemy, int a_iSpeed)
{
	a_Enemy.iSpeed = a_iSpeed;
}

void MoveEnemy(Enemy& a_Enemy)
{
	if (a_Enemy.bEnemyReset)
	{
		/*int startingXDirection = rand() % 2;
		a_Enemy.xDir = startingXDirection == 1 ? 1 : -1;*/
		int Direction = -1;
		a_Enemy.yDir = Direction == 1 ? 1 : -1;
		a_Enemy.bEnemyReset = false;
	}


	float eX = 0.f, eY = 0.f;
	a_Enemy.pos.Get(eX, eY);
	float fNewEnemyX = eX + a_Enemy.xDir * a_Enemy.iSpeed;
	float fNewEnemyY = eY + a_Enemy.yDir * a_Enemy.iSpeed;

	//The Enemies will respawn at the top of the screen if they go off the edge
	if (fNewEnemyY < 0.f)
	{
		//the following line is what is called a ternary operator it is the same as an if/else statement left of '?' is condition left of ':' is true, right of ':' is false condition;
		fNewEnemyY = (a_Enemy.yDir == 1) ? (float)g_iScreenHeight - a_Enemy.iRadius : a_Enemy.iRadius;
		//a_Enemy.yDir *= -1;
		a_Enemy.bEnemyReset = true;
	}

	//If the Enemy exits the side of the screen bring it back to the center of the screen
	if (fNewEnemyX > g_iScreenWidth)
	{
		
		a_Enemy.bEnemyReset = true;
	}

	if (fNewEnemyX < 0.f)
	{
		
		a_Enemy.bEnemyReset = true;
	}

	if (a_Enemy.bEnemyReset)
	{
		fNewEnemyX = rand() % g_iScreenWidth;
		fNewEnemyY = g_iScreenHeight + 20;
	}
	eX = fNewEnemyX;
	eY = fNewEnemyY;

	UG::MoveSprite(a_Enemy.iSpriteID, eX, eY);
	a_Enemy.pos.Set(eX, eY);
}

#pragma endregion

#pragma region Bullet Structure Function Definitions

void InitialiseBullet(Bullet& a_Bullet, const char* a_BulletImageFilename, float a_xPos, float a_yPos)
{
	a_Bullet.pos.Set(a_xPos, a_yPos);
	a_Bullet.iSpriteID = UG::CreateSprite(a_BulletImageFilename, (float)(a_Bullet.iBWidth), (float)(a_Bullet.iBHeight), true);
	UG::MoveSprite(a_Bullet.iSpriteID, a_xPos, a_yPos);

}

void SetBulletAttachKeys(Bullet& a_Bullet, short a_upKey, short a_downKey, short a_leftKey, short a_rightKey)
{
	a_Bullet.upKey = a_upKey;
	a_Bullet.downKey = a_downKey;
	a_Bullet.leftKey = a_leftKey;
	a_Bullet.rightKey = a_rightKey;
}

void SetBulletSpeed(Bullet& a_Bullet, int a_iMovementSpeed)
{
	a_Bullet.iMovementSpeed = a_iMovementSpeed;
}


void AttachBullet(Bullet& a_Bullet)
{
	if (g_bIsShooting == false)
	{
		int iYDirection = 0;
		int iXDirection = 0;

		if (UG::IsKeyDown(a_Bullet.upKey))
		{
			iYDirection = 1;
		}
		if (UG::IsKeyDown(a_Bullet.downKey))
		{
			iYDirection = -1;
		}
		if (UG::IsKeyDown(a_Bullet.leftKey))
		{
			iXDirection = -1;
		}
		if (UG::IsKeyDown(a_Bullet.rightKey))
		{
			iXDirection = 1;
		}
		float pX = 0.f, pY = 0.f;
		a_Bullet.pos.Get(pX, pY);

		float newPositionY = pY + iYDirection * a_Bullet.iMovementSpeed;
		float newPositionX = pX + iXDirection * a_Bullet.iMovementSpeed;
		float StopY = 32.5;
		float StopX = 32.5;

		if (newPositionY - StopY < 0.f)
		{
			newPositionY = 0 + StopY;
		}
		else if (newPositionY + StopY > g_iScreenHeight)
		{
			newPositionY = g_iScreenHeight - StopY;
		}
		else if (newPositionX + StopX > g_iScreenWidth)
		{
			newPositionX = g_iScreenWidth - StopX;
		}
		else if (newPositionX - StopX < 0.f)
		{
			newPositionX = 0 + StopX;
		}
		pY = newPositionY;
		pX = newPositionX;

		UG::MoveSprite(a_Bullet.iSpriteID, pX, pY);
		a_Bullet.pos.Set(pX, pY);
	}
}

void ShootKeyPressed(Bullet& a_Bullet)
{
	if (UG::IsKeyDown(UG::KEY_X))
	{
		g_bIsShooting = true;
	}
}

void ShootBullet(Bullet& a_Bullet, Plane& a_Plane)
{
	if (g_bIsShooting == true)

	{

		float pX = 0.f, pY = 0.f;
		a_Plane.pos.Get(pX, pY);

		if (a_Bullet.bBulletReset)
		{
			/*int startingXDirection = rand() % 2;
			a_Enemy.xDir = startingXDirection == 1 ? 1 : -1;*/
			int Direction = 1;
			a_Bullet.yDir = Direction == 1 ? 1 : 1;
			a_Bullet.bBulletReset = false;
		}


		float bX = 0.f, bY = 0.f;
		a_Bullet.pos.Get(bX, bY);

		float newXPosition = bX + a_Bullet.xDir * a_Bullet.iShootSpeed;
		float newYPosition = bY + a_Bullet.yDir * a_Bullet.iShootSpeed;

		if (newYPosition > g_iScreenHeight)
		{
			//the following line is what is called a ternary operator it is the same as an if/else statement left of '?' is condition left of ':' is true, right of ':' is false condition;
			newYPosition = (a_Bullet.yDir == 1) ? (float)g_iScreenHeight - a_Bullet.iBHeight : a_Bullet.iBHeight;
			//a_Enemy.yDir *= -1;
			a_Bullet.bBulletReset = true;
		}

		if (a_Bullet.bBulletReset)
		{
			float pX = 0.f, pY = 0.f;
			a_Plane.pos.Get(pX, pY);
			newXPosition = pX;
			newYPosition = pY;
			g_bIsShooting = false;
		}

		bX = newXPosition;
		bY = newYPosition;
		UG::MoveSprite(a_Bullet.iSpriteID, bX, bY);
		a_Bullet.pos.Set(bX, bY);

	}
}


#pragma endregion

void TestCollision(Enemy& a_Enemy, Plane& a_Plane, Bullet& a_Bullet)
{
	//Reset Variables
	if (a_Enemy.bEnemyReset)
	{
		/*int startingXDirection = rand() % 2;
		a_Enemy.xDir = startingXDirection == 1 ? 1 : -1;*/
		int Direction = -1;
		a_Enemy.yDir = Direction == 1 ? 1 : -1;
		a_Enemy.bEnemyReset = false;
	}

	if (a_Bullet.bBulletReset)
	{
		/*int startingXDirection = rand() % 2;
		a_Enemy.xDir = startingXDirection == 1 ? 1 : -1;*/
		int Direction = 1;
		a_Bullet.yDir = Direction == 1 ? 1 : 1;
		a_Bullet.bBulletReset = false;
	}

	//Collision testing between player and enemy
	//Cast here gets rid of warning.
	int halfPlaneHeight = (int)(a_Plane.iHeight * 0.5f);
	int halfBulletHeight = (int)(a_Bullet.iBHeight * 0.5f);
	int halfEnemyHeight = (int)(a_Enemy.iEHeight * 0.5f);

	float eX = 0.f, eY = 0.f;
	a_Enemy.pos.Get(eX, eY);

	float pX = 0.f, pY = 0.f;
	a_Plane.pos.Get(pX, pY);

	float bX = 0.f, bY = 0.f;
	a_Bullet.pos.Get(bX, bY);

	if (eY - halfEnemyHeight < pY + halfPlaneHeight && eY + halfEnemyHeight > pY - halfPlaneHeight)
	{
		int halfEnemyWidth = (int)(a_Enemy.iEHeight * 0.5f);
		int halfPlaneWidth = (int)(a_Plane.iWidth * 0.5f);

		if (eX - halfEnemyWidth < pX + halfPlaneWidth && eX + halfEnemyWidth > pX - halfPlaneWidth)
		{
			a_Enemy.bEnemyReset = true;
			g_iLives -= 1;
		}

	}

	//Collision testing between the enemy and bullet
	if (g_bIsShooting == true) // Only test for collisions if bullet is being shot
	{
	

		// Resets bullet if it collides with an enemy
		if (bY + halfBulletHeight > eY - halfEnemyHeight && bY - halfBulletHeight < eY + halfEnemyHeight)
		{
			int halfEnemyWidth = (int)(a_Enemy.iEWidth * 0.5f);
			int halfBulletWidth = (int)(a_Bullet.iBWidth * 0.5f);

			if (eX - halfEnemyWidth < bX + halfBulletWidth && eX + halfEnemyWidth > bX - halfBulletWidth)
			{
				a_Bullet.bBulletReset = true;
				a_Enemy.bEnemyReset = true;
				g_iPlayerScore += 100;

			}

		}
	}

	if (a_Enemy.bEnemyReset)
	{
		eX = rand() % g_iScreenWidth;
		eY = g_iScreenHeight + 20;
		UG::MoveSprite(a_Enemy.iSpriteID, eX, eY);
		a_Enemy.pos.Set(eX, eY);
	}

	if (a_Bullet.bBulletReset)
	{
		a_Bullet.pos.Get(bX, bY);
		a_Plane.pos.Get(pX, pY);
		bX = pX;
		bY = pY;
		UG::MoveSprite(a_Bullet.iSpriteID, pX, pY);
		a_Bullet.pos.Set(pX, pY);
		g_bIsShooting = false;
	}


}

#pragma endregion


void InitialiseGameAssets(/*Background& a_Background,*/ Enemy& a_Enemy, Plane& a_Plane1, Bullet& a_Bullet)
{
	/*InitialiseBackground(a_Background, "./images/water.png", 0.f, 0.f);
	SetBackgroundSpeed(a_Background, 4);
	UG::DrawSprite(a_Background.iSpriteID);*/

	InitialiseEnemy(a_Enemy, "./images/enemy1_strip2.png", rand() % g_iScreenWidth, g_iScreenHeight + 20.f);
	SetEnemySpeed(a_Enemy, 4);
	UG::DrawSprite(a_Enemy.iSpriteID);

	InitialisePlane(a_Plane1, "./images/myplane_strip2.png", g_iScreenWidth * 0.5f, g_iScreenHeight * 0.1f);
	SetPlaneMovementKeys(a_Plane1, UG::KEY_UP, UG::KEY_DOWN, UG::KEY_LEFT, UG::KEY_RIGHT);
	SetPlaneSpeed(a_Plane1, 4);
	UG::DrawSprite(a_Plane1.iSpriteID);

	InitialiseBullet(a_Bullet, "./images/bullet.png", g_iScreenWidth * 0.5f, g_iScreenHeight * 0.1f);
	SetBulletAttachKeys(a_Bullet, UG::KEY_UP, UG::KEY_DOWN, UG::KEY_LEFT, UG::KEY_RIGHT);
	SetBulletSpeed(a_Bullet, 4);
	UG::DrawSprite(a_Bullet.iSpriteID);
}