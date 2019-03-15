#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Vec2.h"

#include <vector>
#include <string>


class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:

	void DrawVec2(const Vec2<float>& start, const Vec2<float>& end)
	{
		DrawCircle(start.x, start.y, 3.0f, olc::RED);
		DrawCircle(end.x, end.y, 3.0f, olc::BLUE);
		DrawLine(start.x, start.y, end.x, end.y);
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here


		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		SetPixelMode(olc::Pixel::NORMAL);
		
		Clear(olc::BLACK);


		Vec2<float> vec1S(10.0f, 20.0f);
		Vec2<float> vec1E(200.0f, 300.0f);

		Vec2<float> vec2S(100.0f, 20.0f);
		Vec2<float> vec2E(100.0f, 120.0f);

		DrawVec2(vec1S, vec1E);
		DrawVec2(vec2S, vec2E);


		
		return true;	
	}

	
};


int main()
{
	Example demo;
	if (demo.Construct(600, 400, 2, 2))
		demo.Start();

	return 0;
}