/// \file MyPainting.cpp
/// - It contains the MyPainting class Methods definition to draw on application.

#include "stdafx.h"
#include "MyPainting.h"
#include <string>
#pragma once

/// <summary>
/// \details
/// - load image to bitmap buffer
/// </summary>
MyPainting::MyPainting(int width, int height)
{
	// load background image (L = wchar_t)
	this->pBackgroundBmp = Bitmap::FromFile(BACKGROUND_BMP);	///< bitmap pointer for image resources
	this->pMidgroundBmp = Bitmap::FromFile(MIDGROUND_BMP);
	this->pForegroundBmp = Bitmap::FromFile(FOREGROUND_BMP);
	this->pSh1Bmp = Bitmap::FromFile(SLINGSHOT1_PNG);
	this->pSh2Bmp = Bitmap::FromFile(SLINGSHOT2_PNG);
	this->pReptileBmp = Bitmap::FromFile(REPTILE_PNG);
	this->pFlyReptileBmp = Bitmap::FromFile(FLY_REPTILE_PNG);
	this->pDeadReptileBmp = Bitmap::FromFile(DEAD_REPTILE_PNG);
	this->pWoodboxBmp = Bitmap::FromFile(WOODBOX_PNG);
	this->pStoneBmp = Bitmap::FromFile(STONE_PNG);
	this->pGlassBmp = Bitmap::FromFile(GLASS_PNG);

	/// images' width and height
	this->firstSh1Width = this->curSh1Width = pSh1Bmp->GetWidth() / 2;
	this->firstSh1Height = this->curSh1Height = pSh1Bmp->GetHeight() / 2;
	this->firstSh2Width = this->curSh2Width = pSh2Bmp->GetWidth() / 2;
	this->firstSh2Height = this->curSh2Height = pSh2Bmp->GetHeight() / 2;
	this->firstReptileWidth = this->curReptileWidth = pReptileBmp->GetWidth() / 2;
	this->firstReptileHeight = this->curReptileHeight = pReptileBmp->GetHeight() / 2;

	/// initialize the members
	this->firstScreenWidth = prevScreenWidth = width;
	this->firstScreenHeight = prevScreenHeight = height;
	this->firstSlingshotX = this->curSlingshotX = width / 10;
	this->firstSlingshotY = this->curSlingshotY = height * 4 / 5;
	this->firstReptileX = this->curReptileX = this->curReptileStartPointX = 0;
	this->firstReptileY = this->curReptileY = this->curReptileStartPointY = height / 2;
	this->firstReptile_X_Velocity = this->curReptile_X_Velocity =width / 250;
	this->firstReptile_Y_Velocity = this->curReptile_Y_Velocity = 0;

	this->firstSkyUpperBoundary = this->curSkyUpperBoundary = 0;
	this->firstSkyLowerBoundary = this->curSkyLowerBoundary = 400;

	this->rotateAngleForReptile = 0;
	this->typeFlyReptile = true;
	this->cntFullScreenEffect = -1;

	// create colour range for green chroma key 
	imgAttr.SetColorKey(Gdiplus::Color(0, 150, 0), Gdiplus::Color(150, 255, 70));

	// initialize the boxes
	srand(time(NULL));
	int type = rand() % 3;
	Gdiplus::Bitmap* bitmap = nullptr;

	if (type == 0)
		bitmap = this->pGlassBmp;
	else if (type == 1)
		bitmap = this->pStoneBmp;
	else
		bitmap = this->pWoodboxBmp;
	
	for (int i = 0; i < MyPainting::MAX_BOXES; ++i)
		this->box[i].SetBitemap(bitmap);	
	this->firstBoxWidth = this->curBoxWidth = this->firstReptileWidth;
	this->firstBoxHeight = this->curBoxHeight = this->firstReptileHeight;
	int tmpWidth = this->firstReptileWidth * 2 / 3;
	int tmpHeight = this->firstReptileHeight * 2 / 3;

	box[0].SetX(width / 2);
	box[0].SetY(height * 10 / 11);	// ground height
	box[1].SetX(box[0].GetX() + tmpWidth);
	box[1].SetY(box[0].GetY());
	box[2].SetX(box[1].GetX() + tmpWidth);
	box[2].SetY(box[0].GetY());
	box[3].SetX(box[0].GetX() + tmpWidth / 2);
	box[3].SetY(box[0].GetY() - tmpHeight);
	box[4].SetX(box[3].GetX() + tmpWidth);
	box[4].SetY(box[3].GetY());
	box[5].SetX(box[3].GetX() + tmpWidth / 2);
	box[5].SetY(box[3].GetY() - tmpHeight);

	box[0].SetFirstX(box[0].GetX());
	box[0].SetFirstY(box[0].GetY());	// ground height
	box[1].SetFirstX(box[1].GetX());
	box[1].SetFirstY(box[1].GetY());
	box[2].SetFirstX(box[2].GetX());
	box[2].SetFirstY(box[2].GetY());
	box[3].SetFirstX(box[3].GetX());
	box[3].SetFirstY(box[3].GetY());
	box[4].SetFirstX(box[4].GetX());
	box[4].SetFirstY(box[4].GetY());
	box[5].SetFirstX(box[5].GetX());
	box[5].SetFirstY(box[5].GetY());

	this->hasBoxCollision = false;

	this->score = 0;	// start score
	this->firstFontSize = this->curFontSize = 15;
}

/// <summary>
/// \details
/// - delete bitmap buffer
/// </summary>
MyPainting::~MyPainting()
{
	// delete resouces
	delete pBackgroundBmp;
	delete pMidgroundBmp;
	delete pForegroundBmp;
	delete pSh1Bmp;
	delete pSh2Bmp;
	delete pReptileBmp;
}

/// <summary>
/// \details
/// - It checks whether reptile is clicked or not
///
/// \param <b>int</b> x point for mouse
/// \param <b>int</b> y point for mouse
///
/// \return <b>bool</b>
/// </summary>
bool MyPainting::IsClickedReptile(int x, int y)
{
	bool ret = false;

	if (((x <= this->curReptileX + this->curReptileWidth) && (x >= this->curReptileX)) &&
		((y <= this->curReptileY + this->curReptileHeight) && (y >= this->curReptileY)))
		ret = true;

	return ret;
}


/// <summary>
/// \details
/// - It makes that reptile is dying 
///
/// \param <b>HDC*</b> dcMem - memory device context 
/// \param <b>int</b> width
/// \param <b>int</b> height
/// \param <b>bool</b> isStartFalling - true: it's the first time to fall down(shooted)
///
/// \return <b>bool</b> - it returns is it still dying or not
/// </summary>
bool MyPainting::DyingReptile(HDC* dcMem, int width, int height, bool isStartFalling)
{
	Gdiplus::Graphics g(*dcMem);

	if (isStartFalling)
	{
		this->curReptile_X_Velocity = this->curReptile_X_Velocity / 2;
		this->curReptile_Y_Velocity = 1;	
		this->rotateAngleForReptile = 0;
	}
	else
	{
		PlaySound(L"res\\falling.wav", NULL, SND_ASYNC | SND_NOSTOP);
	}

	// Reptile finish to fall down (rolling on the ground)
	if (this->curReptileY > height * 9 / 10)
	{
		int speedToStumpleOnGround = 5;
		
		// rolling speed on ground (direction different)
		if (this->curReptile_X_Velocity < 0)	
		{
			this->rotateAngleForReptile -= 2 * this->ROTATEING_ANGLE_AT_ONCE;
			this->curReptileX -= speedToStumpleOnGround * width / this->firstScreenWidth;
		}
		else   
		{
			this->rotateAngleForReptile += 2 * this->ROTATEING_ANGLE_AT_ONCE;
			this->curReptileX += speedToStumpleOnGround * width / this->firstScreenWidth;
		}
			
		DrawRotateBitmap(dcMem, this->rotateAngleForReptile,this->pDeadReptileBmp,this->curReptileX,this->curReptileY);
		return false;
	}

	// implement accelator for gravity
	int accelator = 2; 
	this->curReptile_Y_Velocity += accelator * height / this->firstScreenHeight;;
	this->curReptileY += this->curReptile_Y_Velocity;

	this->curReptileX += this->curReptile_X_Velocity;

	// when reptile hit at the end of screen.
	if ((this->curReptileX > width) || (this->curReptileX < this->curReptileStartPointX))
	{
		this->curReptileX = this->curReptileStartPointX;
	}

	// draw reptile rotated
	if (this->curReptile_X_Velocity < 0)
		this->rotateAngleForReptile -= this->ROTATEING_ANGLE_AT_ONCE;
	else
		this->rotateAngleForReptile += this->ROTATEING_ANGLE_AT_ONCE;

	DrawRotateBitmap(dcMem, this->rotateAngleForReptile, this->pDeadReptileBmp, this->curReptileX, this->curReptileY);

	return true;
}


/// <summary>
/// \details
/// - It takes care about things after finishing drawing
///
/// \param <b>int</b> width
/// \param <b>int</b> height
///
/// \return <b>void</b>
/// </summary>
void MyPainting::EndDrawing(int width, int height)
{
	this->prevScreenWidth = width;
	this->prevScreenHeight = height;
}

/// <summary>
/// \details
/// - draw backgounds with chroma key
///
/// \param <b>HDC*</b> dcMem - memory device context 
/// \param <b>int</b> x
/// \param <b>int</b> y
/// \param <b>int</b> width
/// \param <b>int</b> height
///
/// \return <b>void</b>
/// </summary>
void MyPainting::DrawBackGround(HDC* dcMem, int x, int y, int width, int height, bool isStartFalling)
{
	// paint with white on screen buffer
	Gdiplus::Graphics g(*dcMem);
	SolidBrush whiteBrush(Color(255, 255, 255));
	g.FillRectangle(&whiteBrush, x, y, width, height);

	// draw over back/mid/fore-ground images with chroma key(Green)
	g.DrawImage(pBackgroundBmp, x, y, width, height);
	g.DrawImage(pMidgroundBmp, RectF((float)x, (float)y, (float)width, (float)height), 0.0, 0.0, (float)pMidgroundBmp->GetWidth(), (float)pMidgroundBmp->GetHeight(), UnitPixel, &imgAttr);
	g.DrawImage(pForegroundBmp, RectF((float)x, (float)y, (float)width, (float)height), 0, 0, (float)pForegroundBmp->GetWidth(), (float)pForegroundBmp->GetHeight(), UnitPixel, &imgAttr);

	if (isStartFalling)
		this->cntFullScreenEffect = MAX_FULL_SCREEN_EFFECT;

	// Side effect when reptile shotted
	if (this->cntFullScreenEffect > 0)
	{		
		SolidBrush brush(Color(127 /*A*/, 255 - 50 * this->cntFullScreenEffect /*R*/, 0 /*G*/, 0 /*B*/));
		g.FillRectangle(&brush, 0, 0, width, height);
		--this->cntFullScreenEffect;
	}
}

/// <summary>
/// \details
/// - draw slingshot
///
/// \param <b>HDC*</b> dcMem - memory device context 
/// \param <b>int</b> width
/// \param <b>int</b> height
///
/// \return <b>void</b>
/// </summary>
void  MyPainting::DrawSlingShot(HDC* dcMem, int width, int height,int numSshot)
{
	Gdiplus::Graphics g(*dcMem);

	int slingshot1_Width = this->firstSh1Width * width / this->firstScreenWidth;
	int slingshot1_Height = this->firstSh1Height * height / this->firstScreenHeight;
	int slingshot2_Width = this->firstSh2Width * width / this->firstScreenWidth;
	int slingshot2_Height = this->firstSh2Height * height / this->firstScreenHeight;

	int slingshotX = this->firstSlingshotX * width / this->firstScreenWidth;
	int slingshotY = this->firstSlingshotY * height / this->firstScreenHeight;

	// draw slingshot
	if (numSshot == 1)
		g.DrawImage(pSh1Bmp, slingshotX, slingshotY, slingshot1_Width, slingshot1_Height);
	else
		g.DrawImage(pSh2Bmp, slingshotX, slingshotY, slingshot2_Width, slingshot2_Height);
}

/// <summary>
/// \details
/// - draw flying reptile with random speed.
/// - the speed is accelated and deaccelated, also it is spawned when go over right side on screen.
/// - it changes all proper values which are affected when screen is resized.
///
/// \param <b>HDC*</b> dcMem - memory device context 
/// \param <b>int</b> reptileX - current reptile x position
/// \param <b>int</b> reptileY - current reptile y position
/// \param <b>int</b> width
/// \param <b>int</b> height
///
/// \return <b>void</b>
/// </summary>
void MyPainting::DrawReptile(HDC* dcMem, int width, int height, bool isReset)
{
	Gdiplus::Graphics g(*dcMem);
	
	// Implement accelator, and apply to speed (including check the flying area boudaries)
	srand(time(NULL));
	int accelator = rand() % 3 - 1;	// random accelator (-1, 0, 1)
	int tmpNextVelocityX = this->curReptile_X_Velocity + accelator;
	accelator = rand() % 3 - 1;	// random accelator (-1, 0, 1)
	int tmpNextVelocityY = this->curReptile_Y_Velocity + accelator;

	// Determine X-velocity by checking max-min velocity
	if (tmpNextVelocityX > this->MAX_VEL_X_PIXEL)
		this->curReptile_X_Velocity = this->MAX_VEL_X_PIXEL;
	else if (tmpNextVelocityX < this->MIN_VEL_X_PIXEL)
		this->curReptile_X_Velocity = this->MIN_VEL_X_PIXEL;
	else
		this->curReptile_X_Velocity = tmpNextVelocityX;

	// Determine Y-velocity by checking max-min velocity
	if (tmpNextVelocityY > this->MAX_VEL_Y_PIXEL)
		this->curReptile_Y_Velocity = this->MAX_VEL_Y_PIXEL;
	else if (tmpNextVelocityY < this->MIN_VEL_Y_PIXEL)
		this->curReptile_Y_Velocity = this->MIN_VEL_Y_PIXEL;
	else
		this->curReptile_Y_Velocity = tmpNextVelocityY;

	// Set next Y position and check the boundaries
	int tmpReptileY = this->curReptileY + this->curReptile_Y_Velocity;
	if ((tmpReptileY >= this->curSkyUpperBoundary) && (tmpReptileY <= this->curSkyLowerBoundary))
		this->curReptileY = tmpReptileY;

	this->curReptileX += this->curReptile_X_Velocity;

	// Set the x, y points change when they reset or outouf x-axis boundry.
	if (isReset)
	{
		this->curReptileX = this->curReptileStartPointX;
		this->curReptileY = this->curReptileStartPointY;
	}
	else if(this->curReptileX > width)
		this->curReptileX = this->curReptileStartPointX;
	else if (this->curReptileX < 0)
		this->curReptileX = width;

	// draw reptile
	if(this->typeFlyReptile)
		g.DrawImage(pReptileBmp, this->curReptileX, this->curReptileY, this->curReptileWidth, this->curReptileHeight);
	else
		g.DrawImage(pFlyReptileBmp, this->curReptileX, this->curReptileY, this->curReptileWidth, this->curReptileHeight);
	this->typeFlyReptile = !this->typeFlyReptile;
}

/// <summary>
/// \details
/// - Draw boxes on canvas
///
/// \param <b>dcMem</b> 
///
/// \return <b>void</b>
/// </summary>
void MyPainting::DrawBoxes(HDC* dcMem)
{
	Gdiplus::Graphics g(*dcMem);
	for (int i = 0; i < MyPainting::MAX_BOXES; ++i)
	{
		if(this->box[i].GetAngle() != 0)
			DrawRotateBitmap(dcMem, this->box[i].GetAngle(), this->box[i].GetBitmap(), this->box[i].GetX(), this->box[i].GetY());
		else
			g.DrawImage(this->box[i].GetBitmap(), this->box[i].GetX(), this->box[i].GetY(), this->curBoxWidth, this->curBoxHeight);
	}
		
}

/// <summary>
/// \details
/// - place the all boxes on the start place.
///
/// \param <b>width</b> 
/// \param <b>height</b> 
///
/// \return <b>void</b>
/// </summary>
void MyPainting::RespowneBoxes(int width, int height)
{
	// initialize the boxes
	srand(time(NULL));
	int type = rand() % 3;
	Gdiplus::Bitmap* bitmap = nullptr;

	if (type == 0)
		bitmap = this->pGlassBmp;
	else if (type == 1)
		bitmap = this->pStoneBmp;
	else
		bitmap = this->pWoodboxBmp;

	for (int i = 0; i < MyPainting::MAX_BOXES; ++i)
	{
		this->box[i].SetBitemap(bitmap);
		this->box[i].SetVelX(0);
		this->box[i].SetVelY(0);
		this->box[i].SetAngle(0);
	}

	for (int i = 0; i < MyPainting::MAX_BOXES; ++i)
	{
		this->box[i].SetX(this->box[i].GetFirstX() * width / this->firstScreenWidth);
		this->box[i].SetY(this->box[i].GetFirstY() * height / this->firstScreenHeight);
	}
}

/// <summary>
/// \details
/// - Change all members' values related to window size when window size is changed
///
/// \param <b>width</b> 
/// \param <b>height</b> 
///
/// \return <b>void</b>
/// </summary>
void MyPainting::UpdateMembersWhenChageWinSize(int width, int height)
{
	if ((width != this->prevScreenWidth) || (height != this->prevScreenHeight))
	{
		
		this->curReptileWidth = this->firstReptileWidth * width / this->firstScreenWidth;
		this->curReptileHeight = this->firstReptileHeight * height / this->firstScreenHeight;
		this->curReptileStartPointX = this->firstReptileX * width / this->firstScreenWidth;
		this->curReptileStartPointY = this->firstReptileY * height / this->firstScreenHeight;
		this->curReptile_X_Velocity = this->curReptile_X_Velocity * width / this->prevScreenWidth;
		this->curReptileX = this->curReptileX * width / this->prevScreenWidth;
		this->curReptile_Y_Velocity = this->curReptile_Y_Velocity * height / this->prevScreenHeight;
		this->curReptileY = this->curReptileY * height / this->prevScreenHeight;
		this->curSkyUpperBoundary = this->firstSkyUpperBoundary * height / this->firstScreenHeight;
		this->curSkyLowerBoundary = this->firstSkyLowerBoundary * height / this->firstScreenHeight;
		this->curBoxWidth = this->firstBoxWidth *width / this->firstScreenWidth;
		this->curBoxHeight = this->firstBoxHeight * height / this->firstScreenHeight;

		for (int i = 0; i < MyPainting::MAX_BOXES; ++i)
		{
			if (this->box[i].GetAngle() == 0 && this->box[i].GetVelX() == 0 && this->box[i].GetVelY() == 0)
			{
				this->box[i].SetX(this->box[i].GetFirstX() * width / this->firstScreenWidth);
				this->box[i].SetY(this->box[i].GetFirstY() * height / this->firstScreenHeight);
			}
			else
			{
				this->box[i].SetX(this->box[i].GetX() * width / this->prevScreenWidth);
				this->box[i].SetY(this->box[i].GetY() * height / this->prevScreenHeight);
			}
		}

		this->prevScreenWidth = width;
		this->prevScreenHeight = height;

		this->curFontSize = this->firstFontSize * height / this->firstScreenHeight;
	}
}

/// <summary>
/// \details
/// - It implement the physics engine when the reptile crushe with boxes
///
/// \param <b>width</b> 
/// \param <b>height</b> 
///
/// \return <b>void</b>
/// </summary>
void MyPainting::CollisionHandle(int width, int height)
{
	
	int obj_width = this->curReptileWidth;		// Reptile and box width and height are the same.
	int obj_height = this->curReptileHeight;
	int rightX_Reptile = this->curReptileX + obj_width;
	int bottomY_Reptile = this->curReptileY + obj_height;
	int amplifiedReptileVelX = this->curReptile_X_Velocity * 3;
	int amplifiedReptileVelY = this->curReptile_Y_Velocity * 3 / 4;

	// Find the all boxes having collision with reptile
	for (int i = 0; i < MyPainting::MAX_BOXES; ++i)
	{
		if ((this->box[i].GetX() < rightX_Reptile) && (this->box[i].GetX() + obj_width > this->curReptileX) &&
			(this->box[i].GetY() < bottomY_Reptile) && (this->box[i].GetY() + obj_height > this->curReptileY))
		{
			this->box[i].SetVelX(amplifiedReptileVelX);
			this->box[i].SetVelY(amplifiedReptileVelY);

			if (!this->hasBoxCollision)	// first collision
			{
				this->score += MyPainting::SCORE_DISTRUPTED_BOX;
				this->hasBoxCollision = true;
			}			
		}			
	}

	if (!this->hasBoxCollision) return;

	// Handle collision among the boxes
	for (int i = 0; i < MyPainting::MAX_BOXES; ++i)
	{
		for (int j = i; j < MyPainting::MAX_BOXES; ++j)
		{
			if (i != j)	// left boxes
			{
				if ((this->box[i].GetX() < this->box[j].GetX() + obj_width) && (this->box[i].GetX() + obj_width > this->box[j].GetX()) && 
					(this->box[i].GetY() < this->box[j].GetY() + obj_height) && (this->box[i].GetY() + obj_height > this->box[j].GetY()))
				{
						this->box[j].SetVelX(this->box[j].GetVelX() + this->box[i].GetVelX() / 7);
						this->box[j].SetVelY(this->box[j].GetVelY() + this->box[i].GetVelY() / 7);
				}
			}
		}	
	}

	int groundY = height * 10 / 11;	// ground height

	// Determin the final position
	for (int i = 0; i < MyPainting::MAX_BOXES; ++i)
	{
		// x-axis
		int boxVelx = this->box[i].GetVelX();
		if (boxVelx != 0)
		{
			this->box[i].SetX(this->box[i].GetX() + boxVelx);
			if (boxVelx > 0)
			{
				this->box[i].SetVelX(boxVelx - 1);
				this->box[i].SetAngle(this->box[i].GetAngle() + MyPainting::ROTATEING_ANGLE_AT_ONCE * boxVelx / 2);
			}
			else
			{
				this->box[i].SetVelX(boxVelx + 1);
				this->box[i].SetAngle(this->box[i].GetAngle() - MyPainting::ROTATEING_ANGLE_AT_ONCE * boxVelx / 2);
			}
		}

		// Check whether box over ground
		if (this->box[i].GetY() + this->box[i].GetVelY() > groundY)
		{
			this->box[i].SetY(groundY);
			if (this->box[i].GetVelY() > 0)
				this->box[i].SetVelY(this->box[i].GetVelY() * -1 / 2);
		}		

		this->box[i].SetY(this->box[i].GetY() + this->box[i].GetVelY());

		if (this->box[i].GetY() < groundY)
			this->box[i].SetVelY(this->box[i].GetVelY() + 1);
	}
}

/// <summary>
/// \details
/// - draw the score
///
/// \param <b>HDC*</b> dcMem - memory device context 
///
/// \return <b>void</b>
/// </summary>
void MyPainting::DrawScore(HDC* dcMem)
{
	Gdiplus::Graphics g(*dcMem);
	SolidBrush redBrush(Color(255, 0, 0));
	Font* myfont = new Font(L"Times New Roman", this->curFontSize);
	WCHAR string[20] = L"";
	swprintf(string, 20, L"Score: %d", this->score);

	g.DrawString(string, 20, myfont, PointF(0, 0), &redBrush);
}

/// <summary>
/// \details
/// - draw the object is rotated on the degrees
///
/// \param <b>HDC*</b> dcMem - memory device context 
/// \param <b>float</b> degrees - the angle to rotate
/// \param <b>Gdiplus::Bitmap*</b> bitmap - bitmap image
/// \param <b>int</b> x - x-point
/// \param <b>int</b> y - y-point 
///
/// \return <b>void</b>
/// </summary>
void MyPainting::DrawRotateBitmap(HDC* dcMem, float degrees, Gdiplus::Bitmap* bitmap,int x, int y)
{
	Gdiplus::Graphics g(*dcMem);

	float radians = degrees * this->PI / 180;
	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);

	// set each end points for new bitmap
	int p1_x = (int)(bitmap->GetHeight() * sine) * -1;
	int p1_y = (int)(bitmap->GetHeight() * cosine);
	int p2_x = (int)(bitmap->GetWidth() * cosine - this->pDeadReptileBmp->GetHeight() * sine);
	int p2_y = (int)(bitmap->GetHeight() * cosine + this->pDeadReptileBmp->GetWidth() * sine);
	int p3_x = (int)(bitmap->GetWidth() *cosine);
	int p3_y = (int)(bitmap->GetWidth() * sine);

	//get width and height for new bitmap
	int min_x = min(0, min(p1_x, min(p2_x, p3_x)));
	int min_y = min(0, min(p1_y, min(p2_y, p3_y)));
	int max_x = max(0, max(p1_x, max(p2_x, p3_x)));
	int max_y = max(0, max(p1_y, max(p2_y, p3_y)));

	int destBmWidth = max_x - min_x;
	int destBmHeight = max_y - min_y;

	// create new bitmap
	Gdiplus::Bitmap* destBm = new Gdiplus::Bitmap(destBmWidth,destBmHeight);

	// traverse the each pixel and find corresponed position and copy the color.
	for (int y = min_y; y < max_y; y++)
	{
		for (int x = min_x; x < max_x; x++)
		{
			/*
			newx = x.cos(angle) + y.sin(angle)
			newy = y.cos(angle) - x.sin(angle)
			*/
			int sourcex = (int)(x*cosine + y*sine);
			int sourcey = (int)(y*cosine - x*sine);
			
			// copy pixel in range of source bitmap to rotated bitmap
			if (sourcex >= 0 && sourcex < this->pReptileBmp->GetWidth() && sourcey >= 0
				&& sourcey < this->pReptileBmp->GetHeight())
			{
				Color c;
				bitmap->GetPixel(sourcex, sourcey, &c);
				destBm->SetPixel(x - min_x, y - min_y, c);
			}		
		}
	}

	// scale to proper size with current reptile size
	int w = this->curReptileWidth * destBmWidth / bitmap->GetWidth();
	int h = this->curReptileHeight * destBmHeight / bitmap->GetHeight();

	g.DrawImage(destBm, x, y, w, h);
}