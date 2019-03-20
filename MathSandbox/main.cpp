#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Vec2.h"

#include <vector>
#include <string>

struct sLine
{
	sLine(const Vec2f &v1, float len, float ang)
		: s(v1), length(len), angle(ang)
	{
		// calculate the endpoint from length and angle
		Vec2f v(cosf(angle), sinf(angle));
		e = v.GetNormalized() * length + s;
	};


	sLine(const Vec2f &v1, const Vec2f &v2)
		: s(v1), e(v2) 
	{
		length = (s - e).Length();
		angle = Vec2f::AngleBetween((s - e), { 1.0f, 0.0f });
	};

	void Rotate(float val)
	{
		angle += val;
		// calculate new end point
		Vec2f v(cosf(angle), sinf(angle));
		e = v.GetNormalized() * length + s;
	}

	Vec2f s;
	Vec2f e;
	float length;
	float angle;
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

	void DrawClockLine(const sLine& l)
	{
		DrawCircle(l.e.x, l.e.y, 3.0f, olc::BLUE);
		DrawLine(l.s.x, l.s.y, l.e.x, l.e.y);
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		Vec2f center(ScreenWidth() / 2.0f, ScreenHeight() / 2.0f);

		Vec2f vec1S(center);
		Vec2f vec1E(center.x + RADIUS, center.y);
		vecLines.emplace_back(vec1S, vec1E);

		Vec2f vec2S(center);
		vecLines.emplace_back(vec2S, RADIUS, 1.4f);

		
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
		Vec2f vecRef(1.0f, 0.0f);

		// update rotating vectors
		for (int i = 0; i < vecLines.size(); i++)
		{
			vecLines[i].Rotate(fElapsedTime);
		}
		
		for (auto line : vecLines)
		{
			float angle = Vec2f::PolarAngle(line.e - line.s);

			// DRAWING
			for (float i = 2*PI - angle; i > 0.0f ; i -= 0.01f)
			{

				float dx = cosf(i)*vecRef.x - sinf(i)*vecRef.y;
				float dy = sinf(i)*vecRef.x + cosf(i)*vecRef.y;

				Draw(dx*(RADIUS / 2.0f) + line.s.x, dy*(RADIUS / 2.0f) + line.s.y, olc::GREEN);
			}
		}

		for (auto itr : vecLines)
		{
			DrawClockLine(itr);
		}

		
		return true;	
	}

	std::vector<sLine> vecLines;
	float totalTime = 0.0f;
};


int main()
{

	Example demo;
	if (demo.Construct(600, 400, 2, 2))
		demo.Start();

	return 0;
}