#include "ConfigManager.h"

#include <iostream>
#include <fstream>

using namespace Ogre;

template<> ConfigManager* Ogre::Singleton<ConfigManager>::msSingleton = 0;
ConfigManager* ConfigManager::getSingletonPtr(void)
{
    return msSingleton;
}
ConfigManager& ConfigManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}

ConfigManager::ConfigManager(Ogre::String filename)
{
    this->filename = filename;

}

void ConfigManager::setValue(Ogre::String key, Ogre::String value)
{
	config[ key ] = value;
}

bool ConfigManager::hasKey(Ogre::String key)
{
	if(config.find( key ) == config.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

Ogre::String ConfigManager::getString(Ogre::String key)
{
	if(config.find( key ) != config.end())
	{

		Ogre::String s = config[ key ];
		return s;
	}
	else return "";
}

int ConfigManager::getInt(Ogre::String key)
{
	if(config.find( key ) != config.end())
	{

		Ogre::String s = config[ key ];
		return StringConverter::parseInt(s);
	}
	else return 0;
}

int ConfigManager::save()
{

    std::ofstream file;
    file.open (filename.c_str());
    file << "#xdriller config file" << std::endl << std::endl;


    ConfMap::iterator iter;
    for( iter = config.begin(); iter != config.end(); iter++ )
    {
        file << iter->first << " = " << iter->second << std::endl;
    }

    file.close();

	return 0;
}

int ConfigManager::load()
{
    LogManager::getSingleton().logMessage("Loading " + filename);

    cf.load(filename);

    config.clear();

    config["FSAA"] = cf.getSetting("FSAA",Ogre::BLANKSTRING,"2");
    config["audio_buffers"] = cf.getSetting("audio_buffers",Ogre::BLANKSTRING,"4096");
    config["audio_channels"] = cf.getSetting("audio_channels",Ogre::BLANKSTRING,"2");
    config["audio_rate"] = cf.getSetting("audio_rate",Ogre::BLANKSTRING,"44100");
    config["compositors"] = cf.getSetting("compositors",Ogre::BLANKSTRING,"1");
    config["fullscreen"] = cf.getSetting("fullscreen",Ogre::BLANKSTRING,"No");
    config["last_bg_scene"] = cf.getSetting("last_bg_scene",Ogre::BLANKSTRING,"sky");
    config["music_volume"] = cf.getSetting("music_volume",Ogre::BLANKSTRING,"40");
    config["player_model"] = cf.getSetting("player_model",Ogre::BLANKSTRING,"tom");
    config["player_name"] = cf.getSetting("player_name",Ogre::BLANKSTRING,"player");
    config["render_system"] = cf.getSetting("render_system",Ogre::BLANKSTRING,"OpenGL Rendering Subsystem");
    config["resolution"] = cf.getSetting("resolution",Ogre::BLANKSTRING,"640 x 480");
    config["resource_path"] = cf.getSetting("resource_path",Ogre::BLANKSTRING,"media");
    config["sound_volume"] = cf.getSetting("sound_volume",Ogre::BLANKSTRING,"50");

	return true;
}
