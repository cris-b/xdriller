#ifndef Music_H
#define Music_H

#include "SDL.h"
#include "SDL_mixer.h"

#include <Ogre.h>

class Music
{
    public:

    Music(std::string filename = "");
    ~Music();

    void play(bool loop = false);
    void stop();
    void load(std::string filename);

    private:
    Mix_Music* 	m_Music;

};


#endif
