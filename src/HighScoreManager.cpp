#include "HighScoreManager.h"
#include "LevelLoader.h"

#include "tinyxml.h"

#define SCORES_PER_PAGE     10
#define NUM_MODES            3

using namespace Ogre;

template<> HighScoreManager* Ogre::Singleton<HighScoreManager>::ms_Singleton = 0;
HighScoreManager* HighScoreManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
HighScoreManager& HighScoreManager::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}


HighScoreManager::HighScoreManager(Ogre::String filename)
{
    this->filename = filename;

    int num_levels = LevelLoader::getSingleton().getNumLevels();

    scores = new HighScore[NUM_MODES*num_levels*SCORES_PER_PAGE];

    last_highscore_index = 0;

}

HighScoreManager::~HighScoreManager()
{
    delete scores;
}


int HighScoreManager::load()
{
    int num_levels = LevelLoader::getSingleton().getNumLevels();

	TiXmlDocument doc(filename.c_str());
	bool loadOkay = doc.LoadFile();
	if (!loadOkay)
	{
	    LogManager::getSingletonPtr()->logMessage("ERROR: Failed to load highscores.xml");
        return 1;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);


    pElem=hDoc.FirstChildElement().Element();

    if (!pElem)
    {
        LogManager::getSingletonPtr()->logMessage("ERROR: Failed to load highscores: File Corrupted.");
        return 1;
    }


    hRoot=TiXmlHandle(pElem);

    TiXmlElement* mode;
    mode=hRoot.FirstChild( "mode" ).Element();

    for( ; mode; mode=mode->NextSiblingElement())
    {
        const char *mode_name= mode->Attribute("name");
        int mode_num = 0;

        if(strcmp(mode_name,"Survive") == 0) mode_num = 0;
        if(strcmp(mode_name,"Time Attack") == 0) mode_num = 1;
        if(strcmp(mode_name,"Adventure") == 0) mode_num = 1;

        TiXmlElement* level = TiXmlHandle(mode).FirstChild("level").Element();

        for( ; level; level=level->NextSiblingElement())
        {
            const char *level_name = level->Attribute("name");

            int level_num = LevelLoader::getSingleton().getLevelNum(level_name);

            int score_num = 0;

            TiXmlElement* score = TiXmlHandle(level).FirstChild("score").Element();

            for( ; score; score=score->NextSiblingElement())
            {
                const char *name = score->Attribute("name");
                int time; score->QueryIntAttribute("time",&time);
                int points; score->QueryIntAttribute("points",&points);
                int lives; score->QueryIntAttribute("lives",&lives);
                int depth; score->QueryIntAttribute("depth",&depth);

                int score_index = mode_num*(SCORES_PER_PAGE*num_levels)+level_num*SCORES_PER_PAGE+score_num;

                scores[score_index].name = name;
                scores[score_index].time = time;
                scores[score_index].points = points;
                scores[score_index].lives = lives;
                scores[score_index].depth = depth;

                score_num++;
            }

        }


    }




    return 0;
}

int HighScoreManager::save()
{
    int num_levels = LevelLoader::getSingleton().getNumLevels();

	TiXmlDocument doc;

 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement *root = new TiXmlElement( "highscores" );
	doc.LinkEndChild( root );

	TiXmlComment *comment = new TiXmlComment();
	comment->SetValue(" DO NOT EDIT. AUTOMATICALLY GENERATED " );
	root->LinkEndChild( comment );

    for(int k = 0; k<NUM_MODES; k++)
    {
        TiXmlElement *mode = new TiXmlElement( "mode" );
        root->LinkEndChild( mode );

        if(k == 0) mode->SetAttribute("name","Survive");
        if(k == 1) mode->SetAttribute("name","Time Attack");
        if(k == 2) mode->SetAttribute("name","Adventure");

        for(int i = 0; i<num_levels; i++)
        {
            const char *level_name;

            level_name = LevelLoader::getSingleton().getLevelName(i).c_str();

            TiXmlElement *level = new TiXmlElement( "level" );
            mode->LinkEndChild( level );
            level->SetAttribute("name",level_name);

            for(int j = 0; j<SCORES_PER_PAGE; j++)
            {
                int score_index = k*(SCORES_PER_PAGE*num_levels)+i*SCORES_PER_PAGE+j;

                TiXmlElement *scoreElm = new TiXmlElement( "score" );

                level->LinkEndChild( scoreElm );

                scoreElm->SetAttribute("name",scores[score_index].name.c_str());
                scoreElm->SetAttribute("time",scores[score_index].time);
                scoreElm->SetAttribute("points",scores[score_index].points);
                scoreElm->SetAttribute("lives",scores[score_index].lives);
                scoreElm->SetAttribute("depth",scores[score_index].depth);
            }

        }
    }


	doc.SaveFile( filename.c_str() );

    return 0;
}

int HighScoreManager::addScore(Ogre::String mode, Ogre::String level, Ogre::String name, int time, int points, int lives, int depth)
{
    int mode_num = 0;

    int num_levels = LevelLoader::getSingleton().getNumLevels();


    if(mode == "Survive") mode_num = 0;
    if(mode == "Time Attack") mode_num = 1;
    if(mode == "Adventure") mode_num = 2;

    int level_num = LevelLoader::getSingleton().getLevelNum(level);

    int score_index = mode_num*(SCORES_PER_PAGE*num_levels)+level_num*SCORES_PER_PAGE;

    if(mode_num == 0)
    {
        bool _isHighScore = false;

        int highest_time = scores[score_index].time;
        int highest_time_index = score_index;

        for(int i = 0; i<10; i++)
        {
            if(scores[score_index+i].name == "")
            {
                highest_time = scores[score_index+i].time;
                highest_time_index = score_index+i;

                _isHighScore = true;

                break;
            }
            else if(scores[score_index+i].time > highest_time)
            {
                _isHighScore = true;

                highest_time = scores[score_index+i].time;
                highest_time_index = score_index+i;
            }

        }

        if(_isHighScore)
        {
            scores[highest_time_index].name = name;
            scores[highest_time_index].time = time;
            scores[highest_time_index].points = points;
            scores[highest_time_index].lives = lives;
            scores[highest_time_index].depth = depth;

            return 1;
        }
    }

    return 0;
}
