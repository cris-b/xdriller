#include "ConfigManager.h"

#include <iostream>
#include <fstream>

using namespace Ogre;

template<> ConfigManager* Ogre::Singleton<ConfigManager>::ms_Singleton = 0;
ConfigManager* ConfigManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ConfigManager& ConfigManager::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
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
    file << "#xdriller config file";


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

    config["FSAA"] = cf.getSetting("FSAA",StringUtil::BLANK,"2");
    config["audio_buffers"] = cf.getSetting("audio_buffers",StringUtil::BLANK,"4096");
    config["audio_channels"] = cf.getSetting("audio_channels",StringUtil::BLANK,"2");
    config["audio_rate"] = cf.getSetting("audio_rate",StringUtil::BLANK,"44100");
    config["compositors"] = cf.getSetting("compositors",StringUtil::BLANK,"1");
    config["fullscreen"] = cf.getSetting("fullscreen",StringUtil::BLANK,"No");
    config["last_bg_scene"] = cf.getSetting("last_bg_scene",StringUtil::BLANK,"sky");
    config["music_volume"] = cf.getSetting("music_volume",StringUtil::BLANK,"40");
    config["player_model"] = cf.getSetting("player_model",StringUtil::BLANK,"tom");
    config["player_name"] = cf.getSetting("player_name",StringUtil::BLANK,"player");
    config["render_system"] = cf.getSetting("render_system",StringUtil::BLANK,"OpenGL Rendering Subsystem");
    config["resolution"] = cf.getSetting("resolution",StringUtil::BLANK,"640 x 480");
    config["resource_path"] = cf.getSetting("resource_path",StringUtil::BLANK,"media");
    config["sound_volume"] = cf.getSetting("sound_volume",StringUtil::BLANK,"50");

	return true;
}
