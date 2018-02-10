#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER
/*/
#include "../entities/player/fpsplayer.h"
#include "../terrain/terrain.h"
#include "../data/time.h"

#include <unordered_map>
#include <functional>

struct GLFWwindow;

class KeyboardEvents
{
public:
	explicit KeyboardEvents(void)
	{
		auto r = [&](FPSPlayer* p, Terrain* t, bool& m, Time* ti)->void 
		{
			*p->Running() = true;
			p->SpeedUp();
		};
		auto w = [&](FPSPlayer* p, Terrain* t, bool& m, Time* ti)->void
		{
			p->Move(FPSPlayer::FORWARD, t->GetYPosOfPlayer(p->Position().x, p->Position().z));
			m = true;
		};
		auto s = [&](FPSPlayer* p, Terrain* t, bool& m, Time* ti)->void
		{
			p->Move(FPSPlayer::BACKWARD, t->GetYPosOfPlayer(p->Position().x, p->Position().z));
			m = true;
		};
		auto a = [&](FPSPlayer* p, Terrain* t, bool& m, Time* ti)->void
		{
			p->Strafe(FPSPlayer::LEFT, t->GetYPosOfPlayer(p->Position().x, p->Position().z));
			m = true;
		};
		auto d = [&](FPSPlayer* p, Terrain* t, bool& m, Time* ti)->void
		{
			p->Strafe(FPSPlayer::RIGHT, t->GetYPosOfPlayer(p->Position().x, p->Position().z));
			m = true;
		};
		auto space = [&](FPSPlayer* p, Terrain* t, bool& m, Time* ti)->void
		{
			p->InitializeJump(ti->deltaT);
		};
		auto t = [&](FPSPlayer* p, Terrain* t, bool& m, Time* ti)->void
		{
			p->InitializeFlourish(ti->deltaT);
			p->DisableFlourish();
		};
		auto b = [&](FPSPlayer* p, Terrain* t, bool& m, Time* ti)->void
		{
			p->InitializeBoost(ti->deltaT);
			p->DisableBoost();
		};
	}

	void GetEvent(FPSPlayer* player)
	{

	}
private:
	std::unordered_map<unsigned int, std::function<void(FPSPlayer* p, Terrain* t, bool& m, Time* ti)>> m_events;
};
*/
#endif