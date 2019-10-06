#include "GlobalConstants.h"
#include "State.h"
#include "Player.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Level.h"

void Chasing::Enter(Zombie& zombie)
{
	zombie.SetColor(olc::DARK_YELLOW);
}

void Chasing::Update(Zombie& zombie, float dt)
{	
	if (zombie.SeesTarget())
	{
		zombie.SetTarget(m_chaseTarget->GetPosition());
		zombie.doMove(dt);
	}
	else
	{
		// go to navigate to
		zombie.SetTarget(m_chaseTarget->GetPosition());
		zombie.doNavigateTo();
	}
}

void Controlled::Enter(Player& player)
{
	player.SetColor(olc::CYAN);
}

void Controlled::Update(Player& player, float dt)
{
	// Take input commands
}

void Watching::Enter(Player& player)
{
	player.SetColor(olc::WHITE);
}

void Watching::Update(Player& player, float dt)
{
	// Get closest zombie
	const Zombie* targetZombie = player.GetVisibleZombie();
	if (targetZombie != nullptr)
	{
		// If zombie is in front of player, turn to face the zombie
		float angle = std::abs(Vec2f::AngleBetween(player.GetDirectionVector(), targetZombie->GetPosition() - player.GetPosition() ));

		if (angle > 0.05f) // MAGIC
		{
			// Which direction to turn?
			float cross = Vec2f::CrossProduct(targetZombie->GetPosition() - player.GetPosition(), player.GetDirectionVector());
			// turn accordingly
			cross < 0.0f ? player.TurnRight(dt) : player.TurnLeft(dt);
		}
		else 
		{
			// Zombie in front. Fire the gun!
			player.Attack(dt);
		}
	}
	else {
		// otherwise turn to face the direction you are watching
		// TODO: have the watching player actor turn back to the direction they are looking at
	}
}


void Flying::Update(Bullet& bullet, float dt)
{
	float dir = bullet.GetDirection();
	// New position
	Vec2f newPos(bullet.GetPosition() + Vec2f(cosf(dir), sinf(dir)) * BULLET_SPEED * dt);
	bullet.SetPosition(newPos);

	bullet.m_lifeTime -= dt;
	if (bullet.GetIsHit() || bullet.m_lifeTime < 0.0f)
	{
		bullet.SetDestroyed(true);
	}
}

void Roaming::Enter(Zombie& zombie)
{
	zombie.SetColor(olc::DARK_GREEN);
}

void Roaming::Update(Zombie& zombie, float dt)
{
	// add to timer
	m_timer += dt;
	if (m_timer >= m_timeToChangeDir)
	{
		m_timer = 0.0f;
		// randomize direction
		Vec2f r = zombie.GetRandomCellLocation();
		zombie.SetTarget(r);
		// randomize timer
		m_timeToChangeDir = (((std::rand() % 101) / 100.0f) * TIME_UNTIL_CHANGE_DIR) + MIN_TIME_UNTIL_CHANGE_DIR;
	}

	zombie.doMove(dt);
}

void ZombieDead::Enter(Zombie& zombie)
{
	zombie.SetColor(olc::DARK_RED);
}

void ZombieDead::Update(Zombie& zombie, float dt)
{
	m_deathTime += dt;
	if (m_deathTime > DYING_TIME)
	{
		zombie.SetDestroyed(true);
	}
}

void PlayerDead::Enter(Player& player)
{
	player.SetColor(olc::DARK_RED);

}

void PlayerDead::Update(Player&, float dt)
{
	m_deathTime += dt;
	// Do not destroy players! just lie there... dead
}

void Navigating::Enter(Zombie& zombie)
{
	// sets a path to follow
	m_path = Level::GetPathToTarget(zombie.GetPosition(), m_target);
	zombie.SetColor(olc::DARK_BLUE);
}

void Navigating::Update(Zombie& zombie, float dt)
{
	if (!m_path.empty())
	{
		Vec2f nextLocation = Level::GetCellCenterPos(m_path.back().first, m_path.back().second);
		if (Vec2f::DistanceBetween(zombie.GetPosition(), nextLocation) <= Level::GetCellSize() / 2.0f)
		{
			// zombie is close enough, we can change the position
			m_path.pop_back();
			// if the zombie is on the current cell in the list -> pop_back
			// repeat until list is gone. 
		}
		else 
		{
			zombie.SetTarget(nextLocation);
			zombie.doMove(dt);
		}
	}
	else 
	{
		// the path has been finished, go to roam state
		zombie.doRoam();
	}
}

void ZombieAttacking::Enter(Zombie& zombie)
{
	zombie.SetColor(olc::DARK_RED);
}

void ZombieAttacking::Update(Zombie& zombie, float dt)
{
	m_timer += dt;

	if (m_timer >= ZOMBIE_ATTACK_SPEED * 2.0f)
	{
		// time to go back to roaming
		zombie.doRoam();
	}
	else if (m_timer >= ZOMBIE_ATTACK_SPEED && !m_hasAttacked)
	{
		zombie.SpawnBullet(dt);
		zombie.SetColor(olc::DARK_GREY);
		m_hasAttacked = true;
	}
}
