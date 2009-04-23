
#ifndef ConfigManager_H
#define ConfigManager_H

#include <Ogre.h>
#include <OgreSingleton.h>

#include <string>
#include <map>



typedef std::map<std::string,std::string> ConfMap;

class ConfigManager : public Ogre::Singleton<ConfigManager>
{
    public:

    ConfigManager(std::string filename);
    ~ConfigManager();

    int load();
    int save();

	void setValue(std::string key, std::string value);
	bool hasKey(std::string key);

	std::string  getString   (std::string key);
	int     getInt      (std::string key);


    static ConfigManager& getSingleton(void);
    static ConfigManager* getSingletonPtr(void);

    private:

	private:
		ConfMap config;
		std::string filename;



};


#endif
