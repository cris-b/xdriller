#ifndef SoundManager_H
#define SoundManager_H

#include <Ogre.h>
#include <OgreSingleton.h>

#include "Music.h"
#include "Sound.h"

#define SOUND_BREAK 0
#define SOUND_JOIN  1
#define SOUND_KICK  2
#define SOUND_AIR   3


class SoundManager : public Ogre::Singleton<SoundManager>
{
    public:

    SoundManager();
    ~SoundManager();


    void loadMusic(std::string filename);
    void loadSounds();

    void playMusic();
    void playSound(int type);

    void setMusicVolume(int vol);
    void setSoundVolume(int vol);

    int getMusicVolume() {return musicVolume;}
    int getSoundVolume() {return soundVolume;}

    static SoundManager& getSingleton(void);
    static SoundManager* getSingletonPtr(void);

    private:

    int musicVolume;
    int soundVolume;

    Music *mMusic;
    Sound *mSoundBreak;
    Sound *mSoundJoin;
    Sound *mSoundKick;
    Sound *mSoundAir;

};


#endif
