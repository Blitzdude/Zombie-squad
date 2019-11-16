#include "ZombieSquad.h"
#include "Zombie.h"
#include "State.h"
#include "GlobalConstants.h"


Zombie::Zombie(float x, float y, ZombieSquad& game, ZombieHandler& handler)
	: m_game(&game), m_handler(&handler)
{
	m_currentState = new Roaming();
	SetDestroyed(false);
	SetX(x);
	SetY(y);
	m_target = GetPosition();
	SetRadius(ZOMBIE_SIZE * GAME_SCALE);
	float randomDir = ((rand() % 101) / 100.0f) * PI2;
	SetDirection(randomDir);
	SetColor(olc::DARK_GREEN);
	SetTag(ActorTag::ZOMBIE);
	
	std::cout << "Zombie created\n";
}

Zombie::~Zombie()
{
	delete m_currentState;
	m_currentState = nullptr;
	std::cout << "Zombie destroyed\n";
}


void Zombie::Draw(ZombieSquad& game)
{
#pragma warning (disable : 4244) // Disable conversion from 'float' to 'int32_t' warning

	// Draw zombie
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius(), GetColor());

	// Draw look direction
	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection()) * GetRadius()),
		(int32_t)(GetY() + sinf(GetDirection()) * GetRadius()),
		olc::RED);

	const float VISION_LINES_LENGTH = 50.0f;

	Vec2f left = GetDirectionVector().GetRotated(ZOMBIE_SIGHT_FOV_RAD);
	left = left.GetNormalized() * VISION_LINES_LENGTH + GetPosition();
	
	Vec2f right = GetDirectionVector().GetRotated(-ZOMBIE_SIGHT_FOV_RAD);
	right = right.GetNormalized() * VISION_LINES_LENGTH + GetPosition();
	
	// Draw vision cone lines
	if (game.DevMode())
	{
		game.DrawTriangle(GetX(), GetY(), left.x, left.y, right.x, right.y, olc::CYAN);

		Vec2f vec = m_target - GetPosition();

		// normalize vector
		vec.Normalize();
		float angle = std::abs(Vec2f::AngleBetween(vec, GetDirectionVector()));

		// draw the reference vector for angle
		// game.DrawLine(GetX(), GetY(), vec.x*8.0f + GetX(), vec.y*8.0f + GetY());
		game.DrawString(GetX(), GetY(), std::to_string(angle));
	
		// Draw line to current target
		game.DrawLine(m_target.x, m_target.y, GetX(), GetY(), olc::MAGENTA);
	}
	else 
	{
		game.DrawLine((int32_t)GetX(), (int32_t)GetY(), (int32_t)left.x, (int32_t)left.y, olc::CYAN);
		game.DrawLine((int32_t)GetX(), (int32_t)GetY(), (int32_t)right.x, (int32_t)right.y, olc::CYAN);
	}



	// Get closest player
	const Player* closestPlayer = m_handler->GetClosestPlayer(*this);


#pragma warning (default : 4244) // reenable the warning

}

void Zombie::Update(float dt)
{
	m_currentState->Update(*this, dt);
}

void Zombie::Chase(const Player& player)
{
	if (m_currentState->GetStateID() == StateId::ZOMBIE_ROAM ||
		m_currentState->GetStateID() == StateId::ZOMBIE_NAVIGATING)
	{
		m_currentState = new Chasing(player);
		m_currentState->Enter(*this);
	}
}

void Zombie::Die(float)
{
	if (m_currentState->GetStateID() != StateId::STATE_DEAD)
	{
		delete m_currentState;
		m_currentState = new ZombieDead();
		m_currentState->Enter(*this);
	}
}

void Zombie::Attack(float)
{
	if (m_currentState->GetStateID() == StateId::ZOMBIE_CHASE)
	{
		m_currentState = new ZombieAttacking();
		m_currentState->Enter(*this);
	}
}

void Zombie::SpawnBullet(float)
{
	// calculate position for bullet 
	Vec2f bulletSpawnPoint = GetPosition() + Vec2f(cosf(GetDirection()), sinf(GetDirection())) * (GetRadius() + 2.0f);
	// Spawn the bullet
	m_game->SpawnBullet(bulletSpawnPoint, GetDirection(), 1.0f, 0.0f, ActorTag::ZOMBIE);
}

void Zombie::NavigateTo(const Vec2f& target)
{
	if (m_currentState->GetStateID() == StateId::ZOMBIE_CHASE)
	{
		m_currentState = new Navigating(target);
		m_currentState->Enter(*this);
	}
}

bool Zombie::SeesTarget()
{
	return m_handler->ZombieSeesTarget(m_target, *this);
}


void Zombie::doMove(float dt)
{
	// get vector to target
	Vec2f vec = m_target - GetPosition();

	// normalize vector
	vec.Normalize();
	float angle = std::abs(Vec2f::AngleBetween(vec, GetDirectionVector()) );

	if (angle > 0.01f)
	{
		// Which direction to turn?
		float cross = Vec2f::CrossProduct(vec, GetDirectionVector());
		float dir = cross > 0.0f ? -1.0f : 1.0f; // Magically determined

		SetDirection(GetDirection() + ZOMBIE_TURN_SPEED * dir * dt);
	}
	
	if (Vec2f::DistanceBetween(GetPosition(), m_target) > ATTACK_RANGE 
		&& angle < 0.1f)
	{
		SetPosition(GetPosition() + vec * ZOMBIE_SPEED * dt); 
		
	}
}

void Zombie::doRoam()
{
	m_currentState = new Roaming();
	m_currentState->Enter(*this);
}

void Zombie::doNavigateTo()
{
	if (m_currentState->GetStateID() == StateId::ZOMBIE_CHASE)
	{
		delete m_currentState;
		m_currentState = new Navigating(m_target);
		m_currentState->Enter(*this);
	}
}

// TODO: This could be moved to level
Vec2f Zombie::GetRandomCellLocation()
{
	// Get the current cell
	Cell* currentCell = Level::GetCell(GetPosition());
	float cellSize = Level::GetCellSize();
	// choose a random cell from current and neighbors
	unsigned int r = rand() % currentCell->vecNeighbours.size() + 1;
	unsigned int rNeighbor = rand() % currentCell->vecNeighbours.size() + 1; // number between 0 - size
	Cell* choice = nullptr;
	if (r < currentCell->vecNeighbours.size()) 
	{
		// choose indexed neighbor
		choice = currentCell->vecNeighbours[r];
	}
	else {
		// choose the current cell
		choice = currentCell;
	}
	// pick a random location within the cell
	float rX = (rand() % 101) / 100.0f; // Returns number between 0.0 - 1.0
	float rY = (rand() % 101) / 100.0f; 
	float rPosX = choice->xPos + cellSize * rX;
	float rPosY = choice->yPos + cellSize * rY;
	// return the randomized location
	return Vec2f(rPosX, rPosY);
}
