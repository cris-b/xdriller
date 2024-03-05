#ifndef Sound_H
#define Sound_H

#include "SDL.h"
#include "SDL_mixer.h"

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

