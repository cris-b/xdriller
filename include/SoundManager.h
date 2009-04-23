#ifndef SoundManager_H
#define SoundManager_H

#include <Ogre.h>
#include <OgreSingleton.h>

#include "Music.h"
#include "Sound.h"

#define SOUND_BREAK         0
#define SOUND_JOIN          1
#define SOUND_KICK          2
#define SOUND_AIR           3
#define SOUND_SQUASH        4
#define SOUND_RESURRECT     5
#define SOUND_FALLING       6
#define SOUND_MENU1         7
#define SOUND_MENU2         8
#define SOUND_MENU3         9
#define SOUND_MENU4         10


#define NUM_SOUNDS          11

#define MAX_CHANNELS        10
#define MAX_EQUAL_SOUNDS    3


class SoundManager : public Ogre::Singleton<SoundManager>
{
    public:

    SoundManager();
    ~SoundManager();

    void loadMusic(std::string filename);
    void loadSounds();

    void playMusic(bool loop = true);
    void stopMusic();


    void playSound(int type);
    void stopSound(int type);
    void stopAllSounds();
    int  getNumEqualSounds(int type);

    void setMusicVolume(int vol);
    void setSoundVolume(int vol);

    int getMusicVolume() {return musicVolume;}
    int getSoundVolume() {return soundVolume;}

    void mapChannel(int soundNum,int chan);
    static void channelFinishCallback(int chan) { SoundManager::getSingletonPtr()->mapChannel(-1,chan); }

    static SoundManager& getSingleton(void);
    static SoundManager* getSingletonPtr(void);

int *channelMap;
    private:

    int numChannels;



    int musicVolume;
    int soundVolume;

    Music *mMusic;
    Sound *mSound[NUM_SOUNDS];

};


#endif
