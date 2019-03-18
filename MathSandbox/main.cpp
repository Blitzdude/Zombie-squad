#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Vec2.h"

#include <vector>
#include <string>

struct sLines
{
	sLines(const Vec2f &v1, const Vec2f &v2)
		: s(v1), e(v2) {};

	Vec2f s;
	Vec2f e;
};

// GLOBAL Constants
const float RADIUS = 100.0f;
const int NUM_LINES = 1;


class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:

	void DrawVecLine(const Vec2f& start, const Vec2f& end)
	{
		DrawCircle(start.x, start.y, 3.0f, olc::RED);
		DrawCircle(end.x, end.y, 3.0f, olc::BLUE);
		DrawLine(start.x, start.y, end.x, end.y);
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		Vec2f center(ScreenWidth() / 2.0f, ScreenHeight() / 2.0f);

		for (int i = 0; i < NUM_LINES; i++)
		{
			vecLines.push_back({center, { center.x + RADIUS + 10.0f * i, center.y }});
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		const float PI = 3.14159;
		const float RADIUS = 100.0f;
		totalTime += fElapsedTime;

		SetPixelMode(olc::Pixel::NORMAL);
		Clear(olc::BLACK);

		Vec2f center(ScreenWidth() / 2.0f, ScreenHeight() / 2.0f);

		// update rotating vectors

		
		Vec2f vec1S(center);
		Vec2f vecRef(1.0f, 0.0f);
		Vec2f vec1E(cosf(totalTime / 2.0f)*RADIUS + vec1S.x, sinf(totalTime / 2.0f)*RADIUS + vec1S.y);
		Vec2f vec1Dir(vec1E - vec1S);
		
		Vec2f vec2S(center);
		Vec2f vec2E(cosf(totalTime / 2.0f)*(RADIUS / 2.0f) + vec2S.x, sinf(totalTime / 2.0f)*(RADIUS / 2.0f) + vec2S.y);
		Vec2f vec2Dir(vec2E - vec2S);

		float angle1 = Vec2f::PolarAngle(vec1Dir);
		float angle2 = Vec2f::PolarAngle(vec2Dir);

		float degrees = Rad2Degrees(angle1);


		// DRAWING
	

		for (float i = 2*PI - angle1; i > 0.0f ; i -= 0.01f)
		{

			float dx = cosf(i)*vecRef.x - sinf(i)*vecRef.y;
			float dy = sinf(i)*vecRef.x + cosf(i)*vecRef.y;

			Draw(dx*(RADIUS / 2.0f) + vec1S.x, dy*(RADIUS / 2.0f) + vec1S.y, olc::GREEN);
		}
		
		for (float i = 2 * PI - angle2; i > 0.0f; i -= 0.01f)
		{
			float dx = cosf(i)*vecRef.x - sinf(i)*vecRef.y;
			float dy = sinf(i)*vecRef.x + cosf(i)*vecRef.y;

			Draw(dx*(RADIUS / 3.0f) + vec2S.x, dy*(RADIUS / 3.0f) + vec2S.y, olc::MAGENTA);
		}

		DrawString(10, 10, "angle: " + std::to_string(degrees));
		

		return true;	
	}

	std::vector<sLines> vecLines;
	float totalTime = 0.0f;
};


int main()
{

	Example demo;
	if (demo.Construct(600, 400, 2, 2))
		demo.Start();

	return 0;
}