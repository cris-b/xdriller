#include "Music.h"

Music::Music(std::string filename)
{
    m_Music = Mix_LoadMUS(filename.c_str());

    Ogre::LogManager::getSingleton().logMessage("Loading music " + Ogre::String(filename));

    if(m_Music == NULL)
    {
        Ogre::String s;
        s = "Failed to load music " + filename;
        Ogre::LogManager::getSingleton().logMessage(s);
    }
}


Music::~Music()
{
    if(m_Music != NULL)
    {
        Mix_FreeMusic( m_Music );
    }
}

void Music::play()
{
    if(m_Music != NULL)
    {
        //segundo parametro num loops: -1 = infinito
        Mix_FadeInMusic(m_Music, -1, 100);
    }
}
void Music::stop()
{
    if(m_Music != NULL)
    {
        Mix_FadeOutMusic(100);
    }
}
