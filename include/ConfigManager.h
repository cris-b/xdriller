
#ifndef ConfigManager_H
#define ConfigManager_H

#include <Ogre.h>
#include <OgreSingleton.h>

#include <map>

typedef std::map<std::string,std::string> ConfMap;

class ConfigManager : public Ogre::Singleton<ConfigManager>
{
    public:

    ConfigManager(std::string filename);
    ~ConfigManager();

    int load();
    int save();

	void setValue(Ogre::String key, Ogre::String value);
	bool hasKey(Ogre::String key);

	Ogre::String  getString   (Ogre::String key);
	int     getInt      (Ogre::String key);


    static ConfigManager& getSingleton(void);
    static ConfigManager* getSingletonPtr(void);

	private:

    Ogre::ConfigFile cf;
    ConfMap config;
    Ogre::String filename;



};


#endif
