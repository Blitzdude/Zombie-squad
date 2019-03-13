#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <vector>
#include <string>

struct sVec2
{
	float x, y;
};

struct sRadarPoint
{
	float px, py;
	float radius;
	float lifetime = 2.0f;
};

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		srand(time(NULL));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		SetPixelMode(olc::Pixel::NORMAL);
		Clear(olc::BLACK);

		const float PI = 3.14159265359f;
		const float RADIUS = 200.0f;
		const float CX = ScreenWidth() / 2.0f;
		const float CY = ScreenHeight() / 2.0f;
		const float MAX_TIME = 5.0f;
		const float ANG_STEP = 0.05f;

		static float delta = 0.0f;
		static float ang = 0.0f;
		delta += fElapsedTime;
		nextPing -= fElapsedTime;
		
		static sVec2 prevP;

		float i = 0;
		for (ang = delta; ang <= 2*PI + delta; ang += ANG_STEP)
		{
			FillTriangle(CX, CY, cosf(ang) * RADIUS + CX, sinf(ang) * RADIUS + CY, prevP.x, prevP.y, olc::Pixel(0, (fmod(ang - delta, 2*PI) / (2*PI))*255, 0));
			//DrawLine(CX, CY, cosf(ang) * RADIUS + CX, sinf(ang) * RADIUS + CY, olc::Pixel(0, (int)i % 255, 0));
			prevP = { cosf(ang) * RADIUS + CX , sinf(ang) * RADIUS + CY };
		
		}

		if (nextPing < 0.0f)
		{
			// new dot
			// random range along line
			float t = (rand() % 100)*0.01f;
			float sizeT = (rand() % 100)*0.01f;
			vecRadarPoints.push_back({cosf(ang - ANG_STEP) * (RADIUS*t) + CX, sinf(ang - ANG_STEP) * (RADIUS*t) + CY, 10.0f*sizeT+3.0f, 2.0f });
			// reset time
			nextPing = ((rand() % 100)*0.01f) * MAX_TIME;
		}

		// Update dots
		for (auto& point : vecRadarPoints)
		{
			point.lifetime = point.lifetime - fElapsedTime;
		}

		// remove all dots with ended lifetime
		vecRadarPoints.erase(std::remove_if(vecRadarPoints.begin(), vecRadarPoints.end(),
			[](sRadarPoint p) { return p.lifetime < 0.0f;  }), vecRadarPoints.end());

		// Draw dots
		SetPixelMode(olc::Pixel::ALPHA);

		for (auto point : vecRadarPoints)
			FillCircle(point.px, point.py, point.radius, olc::Pixel(255, 0, 0, (int)(127*point.lifetime)));

		return true;
	}

	// Member variables
	std::vector<sRadarPoint> vecRadarPoints;
	float nextPing = 0.0f;
};


int main()
{
	Example demo;
	if (demo.Construct(600, 400, 2, 2))
		demo.Start();

	return 0;
}