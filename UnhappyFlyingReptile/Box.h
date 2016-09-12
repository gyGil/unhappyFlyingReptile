#pragma once

/// <summary>
///	- It contains information for each box
/// </summary>
class Box {
	//===========================================[ CONSTANTS ]===========================================//
	const int BOX_ORIGINAL_FORCE = 1;
private:
	//===========================================[ MEMBERS ]===========================================//
	Gdiplus::Bitmap* bitmap;
	int angle;
	int firstX;
	int firstY;
	int x;
	int y;
	int velX;
	int velY;

	//===========================================[ Methods ]===========================================//
public:
	void SetX(int x) { this->x = x; }
	void SetY(int y) { this->y = y; }
	void SetFirstX(int x) { this->firstX = x; }
	void SetFirstY(int y) { this->firstY = y; }
	void SetVelX(int velX) { this->velX = velX; }
	void SetVelY(int velY) { this->velY = velY; }
	void SetAngle(int angle) { this->angle = angle; }
	void SetBitemap(Gdiplus::Bitmap* bitmap) { this->bitmap = bitmap; }
	int GetX() { return this->x; }
	int GetY() { return this->y; }
	int GetFirstX() { return this->firstX; }
	int GetFirstY() { return this->firstY; }
	int GetVelX() { return this->velX; }
	int GetVelY() { return this->velY; }
	int GetAngle() { return this->angle; }
	Gdiplus::Bitmap* GetBitmap() { return this->bitmap; }

	Box() {
		bitmap = nullptr;	///< bitmap pointer for image resources
		this->firstX = this->firstY = this->x = this->y = this->velX = this->velY = this->angle = 0;
	}
};
