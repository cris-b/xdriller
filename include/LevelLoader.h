#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <Ogre.h>
#include <OgreSingleton.h>

#include <vector>

class LevelLoader : public Ogre::Singleton<LevelLoader>
{
    public:
        LevelLoader();

        ~LevelLoader();



        void setLevelNum(int levelNum);
        void setLevelName(Ogre::String levelName);



        void setBoardNum(int num) {boardNum = num;}
        int loadBoard();

        int getNumBoards() {return numBoards;}
        int getHeight() {return height;}
        int getWidth() {return width;}
        Ogre::String getLevelName(void) {return levelName;}
        Ogre::String getLongName(Ogre::String levelName);
        Ogre::String getLevelInfo(Ogre::String levelName);

        int getBrickType(int index) {return levelData[index];}

        void nextLevel();
        void prevLevel();



        static LevelLoader& getSingleton(void);
        static LevelLoader* getSingletonPtr(void);

    private:

        Ogre::String levelName,longName;
        int boardNum, numBoards;
        int levelNum, numLevels;

        Ogre::ConfigFile cf;

        int height, width;

        Ogre::Image *levelImage;

        std::vector<int> levelData;

};

#endif // LEVELLOADER_H
