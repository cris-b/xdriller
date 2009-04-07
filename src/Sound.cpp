#include "Sound.h"


Sound::Sound(std::string filename)
{
    Ogre::LogManager::getSingleton().logMessage("Loading sound " + Ogre::String(filename));

    m_Sound = Mix_LoadWAV(filename.c_str());

    if(m_Sound == NULL)
    {
        Ogre::String s;
        s = "Failed to load sound " + Ogre::String(filename);
        Ogre::LogManager::getSingleton().logMessage(s);
    }
}


Sound::~Sound()
{
    if(m_Sound != NULL)
    {
        Mix_FreeChunk( m_Sound );
    }
}

void Sound::play()
{
    if(m_Sound != NULL)
    {
		if( Mix_PlayChannel(-1, m_Sound, 0) == -1)
		{
		    Ogre::LogManager::getSingleton().logMessage("Failed to play sound");
			//std::cout << "Failed to play sound " << m_Filename << " " << Mix_GetError() << std::endl;
		}
    }
}

