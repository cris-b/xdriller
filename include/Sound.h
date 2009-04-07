#ifndef Sound_H
#define Sound_H

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include <Ogre.h>

class Sound
{
    public:

    Sound() {m_Sound = NULL;};
    Sound(std::string filename);
    ~Sound();

    void play();
    void pause();

    private:
    Mix_Chunk* 	m_Sound;

};


#endif

