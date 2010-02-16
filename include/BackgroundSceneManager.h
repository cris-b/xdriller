#ifndef BackgroundSceneManager_H
#define BackgroundSceneManager_H

#include <Ogre.h>
#include <OgreSingleton.h>

#include <map>


class BackgroundSceneManager : public Ogre::Singleton<BackgroundSceneManager>
{
    public:
        BackgroundSceneManager();
        virtual ~BackgroundSceneManager();

        struct Scene
        {
            Ogre::String name;
            Ogre::String long_name;
            Ogre::String scene_filename;
            bool fog;
            int fog_start;
            int fog_end;
            Ogre::ColourValue background_color;
            Ogre::String skybox;
            Ogre::Quaternion skybox_quaternion;
            Ogre::String particle_effect;
        };

        std::map<Ogre::String,Scene> scenes;

        int getNumScenes() {return num_scenes;}


        static BackgroundSceneManager& getSingleton(void);
        static BackgroundSceneManager* getSingletonPtr(void);

    private:

        Ogre::ConfigFile cf;

        int num_scenes;

};

#endif // BackgroundSceneManager_H
