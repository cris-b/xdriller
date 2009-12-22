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

    _audioOpen = false;

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
    else
    {
        _audioOpen = true;

        numChannels = Mix_AllocateChannels(MAX_CHANNELS);

        channelMap = new int[numChannels];

        for (int i=0; i<numChannels; i++)
        {
            channelMap[i] = -1;
        }


        Ogre::LogManager::getSingleton().logMessage(
                "SoundManager: " + Ogre::StringConverter::toString(numChannels) + " channels allocated");

        mMusic = NULL;

        setMusicVolume(ConfigManager::getSingleton().getInt("music_volume"));
        setSoundVolume(ConfigManager::getSingleton().getInt("sound_volume"));

    }



}
SoundManager::~SoundManager()
{
    if(!_audioOpen) return;

    delete [] channelMap;

    for(int i = 0; i < NUM_SOUNDS; i++)
    {
        delete mSound[i];
    }
}

void SoundManager::loadMusic(std::string filename)
{
    if(!_audioOpen) return;

    filename = ConfigManager::getSingleton().getString("resource_path") + "/sounds/" + filename;
    if(mMusic != NULL) delete mMusic;
    mMusic = new Music(filename);
}

void SoundManager::playMusic(bool loop)
{
    if(!_audioOpen) return;

    if(loop) mMusic->play(true);
    else mMusic->play(false);
}
void SoundManager::stopMusic()
{
    if(!_audioOpen) return;

    mMusic->stop();
}

void SoundManager::loadSounds()
{
    if(!_audioOpen) return;

    mSound[SOUND_BREAK]     = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "break.ogg");
    mSound[SOUND_JOIN]      = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "join.ogg");
    mSound[SOUND_KICK]      = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "kick.ogg");
    mSound[SOUND_AIR]       = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "air.ogg");
    mSound[SOUND_SQUASH]    = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "squash.ogg");
    mSound[SOUND_RESURRECT] = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "resurrect.ogg");
    mSound[SOUND_FALLING]   = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "falling.ogg");
    mSound[SOUND_MENU1]     = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "menu_1.ogg");
    mSound[SOUND_MENU2]     = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "menu_2.ogg");
    mSound[SOUND_MENU3]     = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "menu_3.ogg");
    mSound[SOUND_MENU4]     = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "menu_4.ogg");
    mSound[SOUND_JUMP]      = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "jump.ogg");
    mSound[SOUND_LIFEUP]      = new Sound(ConfigManager::getSingleton().getString("resource_path") + "/sounds/" +  "lifeup.ogg");
}

void SoundManager::playSound(int type)
{
    if(!_audioOpen) return;

    int chan;
    int numEqualSounds;

    numEqualSounds = getNumEqualSounds(type);

    if(type == SOUND_FALLING && numEqualSounds >= 1) return;
    else if(numEqualSounds > MAX_EQUAL_SOUNDS) return;

    chan = mSound[type]->play();

    mapChannel(type,chan);

    if(chan >= 0) Mix_ChannelFinished(SoundManager::getSingleton().channelFinishCallback);
}

void SoundManager::stopSound(int type)
{
    if(!_audioOpen) return;

    for (int i=0; i<numChannels; i++)
    {
        if(channelMap[i] == type)
        {
            //Mix_HaltChannel(i);
            Mix_FadeOutChannel(i,100);
            mapChannel(-1,i);
        }
    }
}

void SoundManager::stopAllSounds()
{
    if(!_audioOpen) return;

    for (int i=0; i<numChannels; i++)
    {
        if(channelMap[i] != -1)
        {
            //Mix_HaltChannel(i);
            Mix_FadeOutChannel(i,100);
            mapChannel(-1,i);
        }
    }
}

void SoundManager::setMusicVolume(int vol)
{
    if(!_audioOpen) return;

		if( vol > 100 )
		{
			vol = 100;
		}
		if( vol < 0 )
		{
			vol = 0;
		}

		Mix_VolumeMusic(int((128.0*vol)/100.0));

		musicVolume = vol;


}

void SoundManager::setSoundVolume(int vol)
{
	if(!_audioOpen) return;

    if( vol > 100 )
    {
        vol = 100;
    }
    if( vol < 0 )
    {
        vol = 0;
    }

    Mix_Volume( -1, int((128.0*vol)/100.0));
    soundVolume = vol;
}

void SoundManager::mapChannel(int type,int chan)
{
    if(!_audioOpen) return;

    if(chan >= 0 && chan < numChannels)
    {
        channelMap[chan] = type;
    }
    else
    {
        Ogre::LogManager::getSingleton().logMessage("SoundManager: Warning channel out of scope");
    }
}

int SoundManager::getNumEqualSounds(int type)
{
    if(!_audioOpen) return 0;

    int num = 0;

    for (int i=0; i<numChannels; i++)
    {
        if(channelMap[i] == type)
        {
            num++;
        }
    }

    return num;

}



