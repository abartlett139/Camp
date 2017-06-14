#include "Sound.h"


Sound::Sound()
{
}


Sound::~Sound()
{
	engine->Release();
}

bool Sound::Load(char * fileName, bool LOOP)
{
	Wave *tempSound = new Wave();
	if (LOOP) {
		tempSound->setToLoop();
	}
	if (!tempSound->load(fileName)) {
		engine->Release();
		CoUninitialize();
		return false;
	}
	else {
		soundList.push_back(tempSound);
		return true;
	}
}

int Sound::Initialize()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (FAILED(XAudio2Create(&engine))) {
		CoUninitialize();
		return -1;
	}

	if (FAILED(engine->CreateMasteringVoice(&master))) {
		engine->Release();
		CoUninitialize();
		return -1;
	}

	musicVolume = .5f;
	fxVolume = .5f;

//	Audio Contributions
//	CC BY 3.0

//	"Dragon Warrior" by Marcus Dellicompagni (www.PoundSound.co.uk)
//	http://freesound.org/people/dingo1/sounds/243979/
	if (!Load("audio/menuBackgroundSound.wav", true))
		return -3;

//	"Air Raid Soundcape" by CGEffecx (http://freesound.org/people/CGEffex/sounds/93843/)
	if (!Load("audio/gameBackgroundSound.wav", true))
		return -3;

//	http://freesound.org/people/qubodup/sounds/161346/	(PUBLIC DOMAIN--NO CREDIT REQUIRED)
	if (!Load("audio/battleSounds.wav", true))
		return -3;

// "Gunshot" by mdl_03 (http://freesound.org/people/mdl_03/sounds/209554/)
	if (!Load("audio/gunshot.wav", false))
		return -3;

//	CC
	if (!Load("audio/playerHit.wav", false))
		return -3;

//	CC
	if (!Load("audio/playerDie.wav", false))
		return -3;

//	CC
	if (!Load("audio/reload.wav", false))
		return -3;
//	http://freesound.org/people/LeMudCrab/sounds/163456/	(CC)
	if (!Load("audio/enemyShot.wav", false))
		return -3;
	if (!Load("audio/heartbeat.wav", false))
		return -3;
//	"Drop armor and bag" by Tristan Lohengrin (http://freesound.org/people/Tristan_Lohengrin/sounds/241635/)
	if (!Load("audio/armor.wav", false))
		return -3;
}

void Sound::PlayMusic(int soundNdx)
{
	engine->CreateSourceVoice(&musicSource, soundList[soundNdx]->wf());
	musicSource->SetVolume(musicVolume, XAUDIO2_COMMIT_ALL);


	musicSource->Start();
	musicSource->SubmitSourceBuffer(soundList[soundNdx]->xaBuffer());
}

void Sound::ChangeMusicVolume()
{
	musicSource->SetVolume(musicVolume, XAUDIO2_COMMIT_ALL);
}


void Sound::StopMusic() {
	musicSource->Stop();
}


void Sound::PlayFX(int soundNdx) {
	engine->CreateSourceVoice(&fxSource, soundList[soundNdx]->wf());

	fxSource->SetVolume(fxVolume, XAUDIO2_COMMIT_ALL);
	fxSource->Start();
	fxSource->SubmitSourceBuffer(soundList[soundNdx]->xaBuffer());
}