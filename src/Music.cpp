#include "Music.h"

Music::Music(std::string filename)
{
    if(filename != "")
    {
        load(filename);
    }


}


Music::~Music()
{
    if(m_Music != NULL)
    {
        Mix_FreeMusic( m_Music );
    }
}

void Music::load(std::string filename)
{

    Mix_HaltMusic();

    Ogre::LogManager::getSingleton().logMessage("Loading music " + Ogre::String(filename));
    m_Music = Mix_LoadMUS(filename.c_str());
    if(m_Music == NULL)
    {
        Ogre::String s;
        s = "Failed to load music " + filename;
        Ogre::LogManager::getSingleton().logMessage(s);
    }
}

void Music::play(bool loop)
{


    if(loop) Mix_FadeInMusic(m_Music, -1, 100);
    else Mix_FadeInMusic(m_Music, 0, 100);

}
void Music::stop()
{
    if(m_Music != NULL)
    {
        Mix_HaltMusic();
    }
}
