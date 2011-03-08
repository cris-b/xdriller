#ifndef Sound_H
#define Sound_H

#include "SDL/SDL.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "SDL_mixer/SDL_mixer.h"
#else
#include "SDL/SDL_mixer.h"
#endif

#include <Ogre.h>

class Sound
{
    public:

    Sound() {m_Sound = NULL;};
    Sound(std::string filename);
    ~Sound();

    int  play();

    void finishCallback(int chan);

    private:
    Mix_Chunk* 	m_Sound;

};


#endif

