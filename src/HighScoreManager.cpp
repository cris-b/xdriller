#include "HighScoreManager.h"
#include "LevelLoader.h"
#include "Globals.h"

#include "tinyxml.h"


#include <algorithm> //para std::sort()


using namespace Ogre;

//HighScore

/*HighScore& HighScore::operator = (HighScore &p)
{
    this->name   =  p.name;
    this->time   =  p.time;
    //this->points =  p.points;
    this->lives  =  p.lives;
    this->depth  =  p.depth;

    return *this;
}*/


//HighScoreManager


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

    adventureScores = new HighScore[num_levels*SCORES_PER_PAGE];
    infiniteScores = new HighScore[3*SCORES_PER_PAGE]; //3 = EASY+MEDIUM+HARD
    timeAttackScores = new HighScore[num_levels*SCORES_PER_PAGE];

    last_highscore_index = 0;

}

HighScoreManager::~HighScoreManager()
{
    delete adventureScores;
    delete infiniteScores;
    delete timeAttackScores;
}


int HighScoreManager::load()
{


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
        HighScore *scores;

        if(strcmp(mode_name,"Adventure") == 0) scores = adventureScores;
        if(strcmp(mode_name,"Infinite") == 0) scores = infiniteScores;
        if(strcmp(mode_name,"Time Attack") == 0) scores = timeAttackScores;

        int num_levels = 0;

        if(strcmp(mode_name,"Infinite") == 0)
        {
            num_levels = 3;
        }
        else
        {
            num_levels = LevelLoader::getSingleton().getNumLevels();
        }

        TiXmlElement* level = TiXmlHandle(mode).FirstChild("level").Element();

        for( ; level; level=level->NextSiblingElement())
        {
            const char *level_name = level->Attribute("name");

            int level_num = 0;

            if(strcmp(mode_name,"Infinite") == 0)
            {
                if(strcmp(level_name,"Easy") == 0)  level_num = 0;
                if(strcmp(level_name,"Medium") == 0) level_num = 1;
                if(strcmp(level_name,"Hard") == 0)   level_num = 2;
            }
            else
            {
                level_num = LevelLoader::getSingleton().getLevelNum(level_name);
            }

            int score_num = 0;

            TiXmlElement* score = TiXmlHandle(level).FirstChild("score").Element();

            for( ; score; score=score->NextSiblingElement())
            {
                const char *name = score->Attribute("name");
                int time; score->QueryIntAttribute("time",&time);
                //int points; score->QueryIntAttribute("points",&points);
                int lives; score->QueryIntAttribute("lives",&lives);
                int depth; score->QueryIntAttribute("depth",&depth);

                int score_index = level_num*SCORES_PER_PAGE+score_num;

                scores[score_index].name = name;
                scores[score_index].time = time;
                //scores[score_index].points = points;
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


	TiXmlDocument doc;

 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement *root = new TiXmlElement( "highscores" );
	doc.LinkEndChild( root );

	TiXmlComment *comment = new TiXmlComment();
	comment->SetValue(" DO NOT EDIT. AUTOMATICALLY GENERATED " );
	root->LinkEndChild( comment );

    for(int k = 0; k<NUM_GAME_MODES; k++)
    {
        TiXmlElement *mode = new TiXmlElement( "mode" );
        root->LinkEndChild( mode );

        HighScore *scores;

        if(k == 0) scores = adventureScores;
        if(k == 1) scores = infiniteScores;
        if(k == 2) scores = timeAttackScores;

        if(k == 0) mode->SetAttribute("name","Adventure");
        if(k == 1) mode->SetAttribute("name","Infinite");
        if(k == 2) mode->SetAttribute("name","Time Attack");

        int num_levels = 0;

        if(k == 1)
        {
            num_levels = 3;
        }
        else
        {
            num_levels = LevelLoader::getSingleton().getNumLevels();
        }

        for(int i = 0; i<num_levels; i++)
        {
            String level_name;

            if(k == 1)
            {
                if(i == 0)  level_name = "Easy";
                else if(i == 1)  level_name = "Medium";
                else if(i == 2)  level_name = "Hard";
                else break;
            }
            else
            {
                level_name = LevelLoader::getSingleton().getLevelName(i);
                if(level_name == "")
                    LogManager::getSingleton().logMessage("HighScoreManager: Failed to get levelName for mode " +
                    StringConverter::toString(k) + ", level " + StringConverter::toString(i));

            }



            TiXmlElement *level = new TiXmlElement( "level" );
            mode->LinkEndChild( level );
			level->SetAttribute("name",level_name.c_str());

            for(int j = 0; j<SCORES_PER_PAGE; j++)
            {
                int score_index = i*SCORES_PER_PAGE+j;

                TiXmlElement *scoreElm = new TiXmlElement( "score" );

                level->LinkEndChild( scoreElm );

                scoreElm->SetAttribute("name",scores[score_index].name.c_str());
                scoreElm->SetAttribute("time",scores[score_index].time);
                //scoreElm->SetAttribute("points",scores[score_index].points);
                scoreElm->SetAttribute("lives",scores[score_index].lives);
                scoreElm->SetAttribute("depth",scores[score_index].depth);
            }

        }
    }


	doc.SaveFile( filename.c_str() );

    return 0;
}

void HighScoreManager::sortPage(Ogre::String mode, Ogre::String level)
{
    //Gnome sort: http://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Gnome_sort

    HighScore *scores;

    if(mode == "Adventure") scores = adventureScores;
    if(mode == "Infinite") scores = infiniteScores;
    if(mode == "Time Attack") scores = timeAttackScores;

    int level_num = 0;

    if(mode == "Infinite")
    {
        if(level == "Easy")   level_num = 0;
        if(level == "Medium") level_num = 1;
        if(level == "Hard")   level_num = 2;
    }
    else
    {
        level_num = LevelLoader::getSingleton().getLevelNum(level);
    }

    int index = level_num*SCORES_PER_PAGE;

    //el modo aventura ordena de menor a mayor tiempo
    if(mode == "Adventure")
    {
        for ( int i = 1; i < SCORES_PER_PAGE; )
        {

            if ( scores[index+i-1].time <= scores[index+i].time)
            {
                ++i;
            }
            else
            {
                std::swap( scores[index+i-1] , scores[index+i] );
                --i;
                if ( i == 0 )
                {
                    i = 1;
                }
            }
        }
    } //el infinito de mayor a menor profundidad
    else if(mode == "Infinite")
    {
        for ( int i = 1; i < SCORES_PER_PAGE; )
        {

            if ( scores[index+i-1].depth >= scores[index+i].depth)
            {
                ++i;
            }
            else
            {
                std::swap( scores[index+i-1] , scores[index+i] );
                --i;
                if ( i == 0 )
                {
                    i = 1;
                }
            }
        }
    }

    //repite el algoritmo para poner al final todos los sin nombre.
    //TODO: ¿se podria hacer metiendo un if en el enterior? no se me ocurre como

    for ( int i = 1; i < SCORES_PER_PAGE; )
    {
        //si A no tiene nombre y B si, los cambia
        if ( scores[index+i-1].name == "" && scores[index+i].name != "")
        {
            std::swap( scores[index+i-1] , scores[index+i] );
            --i;
            if ( i == 0 )
            {
                i = 1;
            }
        }
        else
        {
            ++i;
        }
    }

}

int HighScoreManager::addScore(Ogre::String mode, Ogre::String level, Ogre::String name, int time, int lives, int depth)
{

    //LogManager::getSingleton().logMessage("HighScoreManager::addScore = " + mode + " " + level);

    HighScore *scores;

    if(mode == "Adventure") scores = adventureScores;
    if(mode == "Infinite") scores = infiniteScores;
    if(mode == "Time Attack") scores = timeAttackScores;

    int level_num = 0;

    if(mode == "Infinite")
    {
        if(level == "Easy")   level_num = 0;
        if(level == "Medium") level_num = 1;
        if(level == "Hard")   level_num = 2;
    }
    else
    {
        level_num = LevelLoader::getSingleton().getLevelNum(level);
    }


    int score_index = level_num*SCORES_PER_PAGE;

    if(mode == "Adventure")
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
            else if(scores[score_index+i].time > time)
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
            //scores[highest_time_index].points = points;
            scores[highest_time_index].lives = lives;
            scores[highest_time_index].depth = depth;

            sortPage(mode,level);

            return 1;
        }
    }
    else if(mode == "Infinite")
    {
        bool _isHighScore = false;

        int lowest_depth = scores[score_index].depth;
        int lowest_depth_index = score_index;

        for(int i = 0; i<10; i++)
        {
            if(scores[score_index+i].name == "")
            {
                lowest_depth = scores[score_index+i].depth;
                lowest_depth_index = score_index+i;

                _isHighScore = true;

                break;
            }
            else if(scores[score_index+i].depth < depth)
            {
                _isHighScore = true;

                lowest_depth = scores[score_index+i].depth;
                lowest_depth_index = score_index+i;

            }

        }

        if(_isHighScore)
        {
            scores[lowest_depth_index].name = name;
            scores[lowest_depth_index].time = time;
            //scores[highest_depth_index].points = points;
            scores[lowest_depth_index].lives = lives;
            scores[lowest_depth_index].depth = depth;

            sortPage(mode,level);

            return 1;
        }
    }
    return 0;
}

HighScore* HighScoreManager::getScore(Ogre::String mode, Ogre::String level, int num)
{
    HighScore *scores;

    if(mode == "Adventure") scores = adventureScores;
    if(mode == "Infinite") scores = infiniteScores;
    if(mode == "Time Attack") scores = timeAttackScores;

    int level_num = 0;

    if(mode == "Infinite")
    {
        if(level == "Easy")   level_num = 0;
        if(level == "Medium") level_num = 1;
        if(level == "Hard")   level_num = 2;
    }
    else
    {
        level_num = LevelLoader::getSingleton().getLevelNum(level);
    }

    int score_index = level_num*SCORES_PER_PAGE+num;

    return &scores[score_index];

}
