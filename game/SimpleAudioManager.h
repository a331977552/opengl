#pragma once
#include <sound/irrKlang.h>
using namespace irrklang;
class SimpleAudioManager
{
public :

	static SimpleAudioManager& getInstance();
	ISoundEngine* getSoundEngine();

	
private:
	ISoundEngine *engine;
	SimpleAudioManager();
public :

	~SimpleAudioManager();
};

