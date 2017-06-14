#include "WAVE.h"

class Sound;


extern Sound *SoundEngine;


class Sound
{
public:
	Sound();
	~Sound();

	bool Load(char *fileName, bool LOOP);

	int Initialize();

	void PlayMusic(int soundNdx);
	void ChangeMusicVolume();
	void StopMusic();
	void PlayFX(int soundNdx);

	IXAudio2 *engine;
	IXAudio2SourceVoice *musicSource, *fxSource;
	IXAudio2MasteringVoice *master;

	std::vector<Wave*> soundList;

	enum { MENUBACKGROUND, GAMEBACKGROUND, BATTLESOUNDS, FIRE, HIT, DIE, RELOAD, ENEMYSHOT, HEARTBEAT, ARMOR};

	float fxVolume, musicVolume;
};

