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



        void setLevelName(Ogre::String levelName);
        void setBoardNum(int num) {boardNum = num;}
        int loadBoard();

        int getNumBoards() {return numBoards;}
        int getHeight() {return height;}
        int getWidth() {return width;}

        int getBrickType(int index) {return levelData[index];}

        static LevelLoader& getSingleton(void);
        static LevelLoader* getSingletonPtr(void);

    private:

        Ogre::String levelName;
        int boardNum, numBoards;

        Ogre::ConfigFile cf;

        int height, width;

        Ogre::Image *levelImage;

        std::vector<int> levelData;

};

#endif // LEVELLOADER_H
