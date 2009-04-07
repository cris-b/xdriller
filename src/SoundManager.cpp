#include "SoundManager.h"
#include "ConfigManager.h"



template<> SoundManager* Ogre::Singleton<SoundManager>::ms_Singleton = 0;
SoundManager* SoundManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
SoundManager& SoundManager::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}

SoundManager::SoundManager()
{

    int audio_rate = ConfigManager::getSingleton().getInt("audio_rate");
    Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
    int audio_channels = ConfigManager::getSingleton().getInt("audio_channels");
    int audio_buffers = ConfigManager::getSingleton().getInt("audio_buffers");


    Ogre::LogManager::getSingleton().logMessage("SoundManager: Initializing audio");

    SDL_Init(SDL_INIT_AUDIO);

    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
    {
        Ogre::LogManager::getSingleton().logMessage( "SoundManager: ERROR, unable to open audio" );
        Mix_CloseAudio(); //necesario, bueno?
        SDL_Quit();
    }

    mMusic = NULL;

    setMusicVolume(ConfigManager::getSingleton().getInt("music_volume"));
    setSoundVolume(ConfigManager::getSingleton().getInt("sound_volume"));

}

void SoundManager::loadMusic(std::string filename)
{
    filename = ConfigManager::getSingleton().getString("resource_path") + "sounds/" + filename;
    if(mMusic != NULL) delete mMusic;
    mMusic = new Music(filename);
}

void SoundManager::playMusic()
{
    mMusic->play();
}

void SoundManager::loadSounds()
{
    mSoundBreak = new Sound(ConfigManager::getSingleton().getString("resource_path") + "sounds/" +  "break.wav");
    mSoundJoin  = new Sound(ConfigManager::getSingleton().getString("resource_path") + "sounds/" +  "join.wav");
    mSoundKick  = new Sound(ConfigManager::getSingleton().getString("resource_path") + "sounds/" +  "kick.wav");
    mSoundAir   = new Sound(ConfigManager::getSingleton().getString("resource_path") + "sounds/" +  "air.wav");
}

void SoundManager::playSound(int type)
{

    switch(type)
    {
        case SOUND_BREAK:
            mSoundBreak->play();
            break;
        case SOUND_JOIN:
            mSoundJoin->play();
            break;
        case SOUND_KICK:
            mSoundKick->play();
            break;
        case SOUND_AIR:
            mSoundAir->play();
            break;
        default:
            break;
    }

}

void SoundManager::setMusicVolume(int vol)
{
		if( vol > 128 )
		{
			vol = 128;
		}
		if( vol < 0 )
		{
			vol = 0;
		}

		Mix_VolumeMusic(vol);

		musicVolume = vol;


}

void SoundManager::setSoundVolume(int vol)
	{
		if( vol > 128 )
		{
			vol = 128;
		}
		if( vol < 0 )
		{
			vol = 0;
		}

		Mix_Volume( -1, vol);
		soundVolume = vol;
}
