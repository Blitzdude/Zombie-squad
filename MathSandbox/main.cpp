#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Vec2.h"

#include <vector>
#include <string>

class sLine
{
public:
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
		// calculate length and angle
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

	// Getters
	const Vec2f& GetStart() const { return s; }
	const Vec2f& GetEnd() const { return e; }
	float GetLength() const { return length; }
	float GetAngle() const { return angle; }

private:
	Vec2f s;
	Vec2f e;
	float length;
	float angle;
};

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:
	// Draws the line with a blue circle at the end
	void DrawClockLine(const sLine& l)
	{
		DrawCircle(l.GetEnd().x, l.GetEnd().y, 3.0f, olc::BLUE);
		DrawLine(l.GetStart().x, l.GetStart().y, l.GetEnd().x, l.GetEnd().y);
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		const int NUM_LINES = 10;
		const float RADIUS = 20.0f;

		Vec2f center(ScreenWidth() / 2.0f, ScreenHeight() / 2.0f);

		for (int i = 0; i < NUM_LINES; i++) // add lines to vector
		{
			vecLines.push_back({center, RADIUS * i + 50.0f, 0.0f});
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		totalTime += fElapsedTime;

		// Clear the screen
		SetPixelMode(olc::Pixel::NORMAL);
		Clear(olc::BLACK);


		// update rotating vectors
		for (int i = 0; i < vecLines.size(); i++)
		{
			vecLines[i].Rotate(fElapsedTime * (3.0f / (i+1)) ); // DO NOT DIVIDE BY ZERO!
		}

		
		// for each line, draw an arc according to its polar angle (angle relative to x-axis)
		for (auto line : vecLines)
		{
			float ang = Vec2f::PolarAngle(line.GetEnd() - line.GetStart());

			Vec2f vecRef(1.0f, 0.0f);	// x-axis unit vector

			for (float i = 2*PI - ang; i > 0.0f; i -= 0.01f)
			{
				float dx = cosf(i)*vecRef.x - sinf(i)*vecRef.y;
				float dy = sinf(i)*vecRef.x + cosf(i)*vecRef.y;

				Draw(dx*(line.GetLength() - 20.0f) + line.GetStart().x, dy*(line.GetLength() - 20.0f) + line.GetStart().y, olc::GREEN);
			}
		}
		
		// Draw the lines
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