/// \file MyPainting.h
/// - It contains the MyPainting class to draw on appication 
#pragma once

/// \mainpage GAS. A3. Unhappy Flying Reptile.
///
/// \section intro Program Introduction
/// - Implement flying for reptile.
/// - Implement physics engine to crush reptile with boxes.
/// - Display the score for game.
///
/// \section version Current version of this program
/// <ul>
/// <li>\author         GuenYoung Gil</li>
/// <li>\version        1.00.00</li>
/// <li>\date           2016.03.29</li>
/// <li>\copyright      GuenYoung Gil</li>
/// <ul>
#include "Box.h"

/// <summary>
/// - This class implements "Unhappy Flying Reptile"
/// - Create backround images with chroma key on Memory Device Context.
/// - Draw  slingshot and reptile on Memory Device Context.
/// </summary>

class MyPainting
{
private:
	//===========================================[ CONSTANTS ]===========================================//
	const wchar_t* BACKGROUND_BMP = L"res\\Background.bmp";	///< resource location
	const wchar_t* MIDGROUND_BMP = L"res\\Midground.bmp";
	const wchar_t* FOREGROUND_BMP = L"res\\Foreground.bmp";
	const wchar_t* REPTILE_PNG = L"res\\reptile.png";
	const wchar_t* FLY_REPTILE_PNG = L"res\\flyReptile.png";
	const wchar_t* DEAD_REPTILE_PNG = L"res\\deadReptile.png";
	const wchar_t* SLINGSHOT1_PNG = L"res\\slingshot1.png";
	const wchar_t* SLINGSHOT2_PNG = L"res\\slingshot2.png";
	const wchar_t* WOODBOX_PNG = L"res\\woodBox.png";
	const wchar_t* STONE_PNG = L"res\\stone.png";
	const wchar_t* GLASS_PNG = L"res\\glass.png";
	const float PI = 3.1416f;	// pi
	const int ROTATEING_ANGLE_AT_ONCE = 5;	// the angle to rotate every falling frame (5 degrees)
	const int MAX_VEL_X_PIXEL = 8;
	const int MIN_VEL_X_PIXEL = -8;
	const int MAX_VEL_Y_PIXEL = 5;
	const int MIN_VEL_Y_PIXEL = -5;
	const int MAX_FULL_SCREEN_EFFECT = 5;
	const int MAX_BOXES = 6;

public:
	const static int SCORE_SHOT_REPTILE = 100;
	const static int SCORE_DISTRUPTED_BOX = 50;

	//===========================================[ MEMBERS ]===========================================//
private:
	Gdiplus::Bitmap* pBackgroundBmp = nullptr;	///< bitmap pointer for image resources
	Gdiplus::Bitmap* pMidgroundBmp = nullptr;
	Gdiplus::Bitmap* pForegroundBmp = nullptr;
	Gdiplus::Bitmap* pSh1Bmp = nullptr;
	Gdiplus::Bitmap* pSh2Bmp = nullptr;
	Gdiplus::Bitmap* pReptileBmp = nullptr;
	Gdiplus::Bitmap* pFlyReptileBmp = nullptr;
	Gdiplus::Bitmap* pDeadReptileBmp = nullptr;
	Gdiplus::Bitmap* pWoodboxBmp = nullptr;
	Gdiplus::Bitmap* pStoneBmp = nullptr;
	Gdiplus::Bitmap* pGlassBmp = nullptr;
	ImageAttributes imgAttr;

	// the first values (it is not changed after inputted once)
	// it is for calculating the ratio compared to changed size 
	int firstScreenWidth;	///< screen width at first time
	int firstScreenHeight;	///< screen height at first time

	int firstSlingshotX;	///< slingshot X-axis at first time
	int firstSlingshotY;	///< slingshot Y-axis at first time
	int firstSh1Width;		///< slingshot 1 width at first time
	int firstSh1Height;		///< slingshot 1 height at first time
	int firstSh2Width;		///< slingshot 2 width at first time
	int firstSh2Height;		///< slingshot 2 height at first time

	int firstReptileWidth;	///< reptile width at first time
	int firstReptileHeight;	///< reptile height at first time
	int firstReptileX;		///< the first reptile x-axis 
	int firstReptileY;		///< the first reptile y-axis
	int firstReptile_X_Velocity;	///< the first reptile velocity
	int firstReptile_Y_Velocity;	///< the first reptile velocity
	int firstSkyUpperBoundary;	///< the upper boundary in sky to fly
	int firstSkyLowerBoundary;	///< the lower boundary in sky to fly

	int firstFontSize;

	int prevScreenWidth;	///< screen width in previous drawing
	int prevScreenHeight;	///< screen width in previous drawing

	// the current values (These values keep same value during having the same size, but change when size is changed)
	int curSlingshotX;	///< slingshot X-axis at first time
	int curSlingshotY;	///< slingshot Y-axis at first time
	int curSh1Width;	///< slingshot 1 width at first time
	int curSh1Height;	///< slingshot 1 height at first time
	int curSh2Width;	///< slingshot 2 width at first time
	int curSh2Height;	///< slingshot 2 height at first time

	int curReptileWidth;	///< reptile width at first time
	int curReptileHeight;	///< reptile height at first time
	int curReptileX;		///< the first reptile x-axis 
	int curReptileY;		///< the first reptile y-axis
	int curReptile_X_Velocity;	///< the first reptile velocity
	int curReptile_Y_Velocity;	///< the first reptile velocity
	int curReptileStartPointX;	///< current reptile's start point x for drawing
	int curReptileStartPointY;	///< current reptile's start point y for drawing
	int curSkyUpperBoundary;	///< the upper boundary in sky to fly
	int curSkyLowerBoundary;	///< the lower boundary in sky to fly

	int curFontSize;

	int rotateAngleForReptile;	///< The angle of reptile
	bool typeFlyReptile;		///< Indicate the reptile's png whether is unfolded wing or not when he/she is flying
	int cntFullScreenEffect;	///< Indicate how much frames left when full screen is effecting

	// Box
	int curBoxWidth;	///< box-width now
	int curBoxHeight;	///< box-height now
	int firstBoxWidth;
	int firstBoxHeight;
	Box box[6];			///< Box stacked numbers
						//      5
						//    3   4
						//  0   1   2
	bool hasBoxCollision;	///< indicate that box has collision

	int score;			///< score for game

//===========================================[ METHODS ]===========================================//
public:
	//================================[ CREATOR/DESTROYER ]================================// 
	MyPainting(int width, int height);	///< creator
	~MyPainting();	///< destroyer

	//==================================[ SETTER/GETTER ]==================================//
	void SetHasBoxCollision(bool isIt) { this->hasBoxCollision = isIt; }
	void SetScore(int score) { this->score = score; }	/// setter for score
	int GetScore() { return this->score; }		/// getter for score

	//======================================[ NORMAL ]======================================//
	void DrawBackGround(HDC* dcMem, int x, int y, int width, int height, bool isStartFalling);	///< draw backgrounds
	void DrawSlingShot(HDC* dcMem, int width, int height, int numSshot);	///< draw slingshot
	void DrawReptile(HDC* dcMem, int width, int height, bool isReset);		///< draw slingshot
	void EndDrawing(int width, int height);	/// free resource for drawing
	bool IsClickedReptile(int x, int y);	/// check reptile is clicked by user's mouse cursor
	bool DyingReptile(HDC* dcMem, int width, int height, bool isStartFalling);	/// draw dying reptile
	void DrawRotateBitmap(HDC* dcMem, float degrees, Gdiplus::Bitmap* bitmap, int x, int y);	/// draw bitmap is rotating on the degrees
	void DrawBoxes(HDC* dcMem);	/// draw boxes
	void DrawScore(HDC* dcMem);	/// draw score
	void CollisionHandle(int width, int height);	/// handle collision with reptile and boxes
	void RespowneBoxes(int width, int height);		/// respown boxes
	void UpdateMembersWhenChageWinSize(int width, int height);	/// update all members which should be changed when the window size is changed 
};