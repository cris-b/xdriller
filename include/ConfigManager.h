
#ifndef ConfigManager_H
#define ConfigManager_H

#include <Ogre.h>
#include <OgreSingleton.h>

#include <string>
#include <map>


using namespace std;
using namespace Ogre;

typedef std::map<string,string> ConfMap;

class ConfigManager : public Ogre::Singleton<ConfigManager>
{
    public:

    ConfigManager(string filename);
    ~ConfigManager();

    int load();
    int save();

	void setValue(string key, string value);
	bool hasKey(string key);

	string  getString   (string key);
	int     getInt      (string key);


    static ConfigManager& getSingleton(void);
    static ConfigManager* getSingletonPtr(void);

    private:

	private:
		ConfMap config;
		string filename;



};


#endif
