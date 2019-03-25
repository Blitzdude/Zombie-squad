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

	// Setters
	void SetStart(const Vec2f& v) 
	{
		// creates a new line and replaces the old one
		*this = sLine(v, this->length, this->angle);
	};
	void SetEnd(const Vec2f& v) 
	{
		// Creates a new Line and replaces the old one
		*this = sLine(this->s, v);
	};


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
		DrawLine(l.GetStart().x, l.GetStart().y, l.GetEnd().x, l.GetEnd().y, olc::MAGENTA);
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		const int NUM_LINES = 8;
		const float RADIUS = 10.0f;

		Vec2f center(ScreenWidth() / 2.0f, ScreenHeight() / 2.0f);

		for (int i = 0; i < NUM_LINES; i++) // add lines to vector
		{
			vecLines.push_back({center, RADIUS * i + RADIUS, 0.0f});
		}

		// update the positions of the vectors 
		// update rotating vectors
		for (int i = 0; i < vecLines.size(); i++)
		{
			// Rotate the current point
			// set the start of the next point as the end of the last one.
			if (i != vecLines.size() - 1) // not the last element
			{
				vecLines[i + 1].SetStart(vecLines[i].GetEnd());
			}
		}

		// Create stencil texture target
		stencil = new olc::Sprite(ScreenWidth(), ScreenHeight());

		prevPos = vecLines.back().GetEnd();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		// clamp elapsed time to 60 fps
		
		totalTime += fElapsedTime;

		// Clear the screen
		SetPixelMode(olc::Pixel::NORMAL);
		Clear(olc::BLACK);

		// update rotating vectors
		for (int i = 0; i < vecLines.size(); i++)
		{
			float count = i % 2 == 0 ? -1.0f : 1.0f;
			// Rotate the current point
			vecLines[i].Rotate(fElapsedTime * 3.0f / (count*i+1) ); // DO NOT DIVIDE BY ZERO!
			// set the start of the next point as the end of the last one.
			if (i != vecLines.size() - 1) // not the last element
			{
				vecLines[i + 1].SetStart(vecLines[i].GetEnd());
			}
		}

		// set the draw target to another texture
		SetDrawTarget(stencil);

		// at the start the elapsed time is large, this check stops it from drawing
		// a nasty straight line
		if ((prevPos - vecLines.back().GetEnd()).Length() < 10.0f) // magic number, adjust if stops drawing
		{
		// draw a pixel at the last point in the texture
			DrawLine(vecLines.back().GetEnd().x, vecLines.back().GetEnd().y, 
					 prevPos.x, prevPos.y);
		}
		prevPos = vecLines.back().GetEnd();

		// change draw target to visible screen
		SetDrawTarget(nullptr);
		// draw the stencil
		DrawSprite(0, 0, stencil);
		
		// Draw the lines
		for (auto itr : vecLines)
		{
			DrawClockLine(itr);
		}

		return true;	
	}

	olc::Sprite* stencil;
	std::vector<sLine> vecLines;
	float totalTime = 0.0f;
	Vec2f prevPos;
};


int main()
{
	Example demo;
	if (demo.Construct(600, 400, 2, 2))
		demo.Start();

	return 0;
}