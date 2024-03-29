#include "BackgroundSceneManager.h"

using namespace Ogre;

template<> BackgroundSceneManager* Ogre::Singleton<BackgroundSceneManager>::msSingleton = 0;
BackgroundSceneManager* BackgroundSceneManager::getSingletonPtr(void)
{
    return msSingleton;
}
BackgroundSceneManager& BackgroundSceneManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}

BackgroundSceneManager::BackgroundSceneManager()
{
    num_scenes = 0;

    cf.loadFromResourceSystem("scenes.cfg","General");

    const auto &settings = cf.getSettingsBySection();

    Ogre::String secName, optName, optValue;

    for(const auto& sec : settings)
    {

        secName = sec.first;

        scenes[secName].name = secName;

        //LogManager::getSingleton().logMessage("SceneManager: Scene " + StringConverter::toString(num_scenes) + " = " + secName);

        //inicializa las variables

        if(optName == "long_name") scenes[secName].long_name = "";
        if(optName == "scene_filename") scenes[secName].scene_filename = "";
        if(optName == "fog") scenes[secName].fog = false;
        if(optName == "fog_start") scenes[secName].fog_start = 2000;
        if(optName == "fog_end") scenes[secName].fog_end = 3000;
        if(optName == "background_color") scenes[secName].background_color = ColourValue::ZERO;
        if(optName == "skybox") scenes[secName].skybox = "";
        if(optName == "skybox_quaternion") scenes[secName].skybox_quaternion = Quaternion::IDENTITY;
        if(optName == "particle_effect") scenes[secName].particle_effect = "";

        for(const auto& set: sec.second)
        {
            optName = set.first;
            optValue = set.second;

            if(optName == "long_name") scenes[secName].long_name = optValue;
            if(optName == "scene_filename") scenes[secName].scene_filename = optValue;
            if(optName == "fog") scenes[secName].fog = StringConverter::parseBool(optValue);
            if(optName == "fog_start") scenes[secName].fog_start = StringConverter::parseInt(optValue);
            if(optName == "fog_end") scenes[secName].fog_end = StringConverter::parseInt(optValue);
            if(optName == "background_color") scenes[secName].background_color = StringConverter::parseColourValue(optValue);
            if(optName == "skybox") scenes[secName].skybox = optValue;
            if(optName == "skybox_quaternion") scenes[secName].skybox_quaternion = StringConverter::parseQuaternion(optValue);
            if(optName == "particle_effect") scenes[secName].particle_effect = optValue;
        }

       num_scenes++;
    }


    LogManager::getSingleton().logMessage("SceneManager: Loaded scenes.cfg. Number of Scenes: "
                                                + StringConverter::toString(num_scenes));

    /*for(int i = 0; i<num_scenes; i++)
    {
        LogManager::getSingleton().logMessage("SceneManager: Scene " + StringConverter::toString(i) + " = " + scenes[i].name);
    }*/


}

BackgroundSceneManager::~BackgroundSceneManager()
{
    //dtor
}
