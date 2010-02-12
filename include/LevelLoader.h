#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <Ogre.h>
#include <OgreSingleton.h>

#include <vector>



class LevelLoader : public Ogre::Singleton<LevelLoader>
{
    public:

        enum Difficulty {EASY, MEDIUM,HARD};

        LevelLoader();

        ~LevelLoader();



        void setLevelNum(int levelNum);
        void setLevelName(Ogre::String levelName);


        int getNumLevels();




        void setBoardNum(int num) {boardNum = num;}
        int loadBoard();

        int getNumBoards() {return numBoards;}
        int getHeight() {return height;}
        int getWidth() {return width;}

        Ogre::String getLevelName(void) {return levelName;}
        Ogre::String getLevelName(int level_num);

        int getLevelNum(Ogre::String name);
        int getLevelNum(void) {return levelNum;}

        Ogre::String getLongName(void);
        Ogre::String getValue(Ogre::String opt);

        int getBrickType(int index) {return levelData[index];}

        void nextLevel();
        void prevLevel();

        int getGameMode() {return gameMode;}
        void setGameMode(int gameMode);

        int getDifficulty() {return levelDifficulty;}
        void setDifficulty(int diff) {levelDifficulty = diff;}

        static LevelLoader& getSingleton(void);
        static LevelLoader* getSingletonPtr(void);

    private:

        Ogre::String levelName,longName;
        int boardNum, numBoards;
        int levelNum, numLevels;
        int gameMode, levelDifficulty;

        Ogre::ConfigFile cf;

        int height, width;

        Ogre::Image *levelImage;

        std::vector<int> levelData;
        std::vector<Ogre::String> levelNames;

};

#endif // LEVELLOADER_H
