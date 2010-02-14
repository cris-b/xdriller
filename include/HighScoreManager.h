#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <Ogre.h>
#include <OgreSingleton.h>

#define SCORES_PER_PAGE     10

class HighScore
{
    public:
        HighScore() {time=0;lives=0;depth=0;}
        ~HighScore() {};

        Ogre::String name;
        int time;
        int lives;
        int depth;
};


class HighScoreManager : public Ogre::Singleton<HighScoreManager>
{
    public:

    HighScoreManager(Ogre::String filename);
    ~HighScoreManager();

    int load();
    int save();

    int addScore(Ogre::String mode, Ogre::String level, Ogre::String name, int time = 0, int lives = 0, int depth = 0);

    HighScore *getScore(Ogre::String mode, Ogre::String level, int num);

    static HighScoreManager& getSingleton(void);
    static HighScoreManager* getSingletonPtr(void);

    struct LastScore
    {
        HighScore score;
        Ogre::String mode;
        Ogre::String level;
    } lastScore;

    private:

    void sortPage(Ogre::String mode, Ogre::String level);

    HighScore *adventureScores;
    HighScore *infiniteScores;
    HighScore *timeAttackScores;



    int last_highscore_index;

    Ogre::String filename;
};

#endif // HIGHSCOREMANAGER_H
