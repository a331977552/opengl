#include "SimpleAudioManager.h"



SimpleAudioManager& SimpleAudioManager::getInstance()
{
	static SimpleAudioManager *manager = new SimpleAudioManager();

	return *manager;
}

irrklang::ISoundEngine* SimpleAudioManager::getSoundEngine()
{

	return engine;
}

SimpleAudioManager::SimpleAudioManager()
{
	engine=	irrklang::createIrrKlangDevice();
}


SimpleAudioManager::~SimpleAudioManager()
{
	engine->removeAllSoundSources();
	engine->drop();
	
}
