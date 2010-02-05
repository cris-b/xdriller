#include "HighScoreTable.h"
#include "HighScoreManager.h"

#include "Gettext.h"
#include "Tools.h"

#define BIG_CHAR_HEIGHT 0.06
#define SMALL_CHAR_HEIGHT 0.05

using namespace Ogre;

HighScoreTable::HighScoreTable()
{
    ColourValue color_1 = ColourValue(0,1,0);
    ColourValue color_2 = ColourValue(0.8,0.8,0.8);


    mOverlayMgr   = OverlayManager::getSingletonPtr();

    if(mOverlayMgr->getByName("HighScoreTableOverlay") == 0)
    {
        mOverlay = mOverlayMgr->create("HighScoreTableOverlay");
    }
    else
    {
        mOverlay = mOverlayMgr->getByName("HighScoreTableOverlay");
    }


    mPanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "HighScoreTable_panel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);


    mPanel->setPosition(0, 0);
    mPanel->setDimensions(1, 1);
    mPanel->setTransparent(true);

    Ogre::TextAreaOverlayElement *mName = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "HighScoreTable_name"));
    mName->setMetricsMode(Ogre::GMM_RELATIVE);
    mName->setHorizontalAlignment(Ogre::GHA_CENTER);
    mName->setVerticalAlignment(Ogre::GVA_CENTER);
    mName->setPosition(-0.30, -0.15);
    mName->setDimensions(1, 0.2);
    mName->setCharHeight(BIG_CHAR_HEIGHT);
    mName->setFontName("CoolFont");
    mName->setColour(color_1);
    mName->setAlignment(TextAreaOverlayElement::Center);
    mName->setCaption(_("Name"));

    Ogre::TextAreaOverlayElement *mTime = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "HighScoreTable_time"));
    mTime->setMetricsMode(Ogre::GMM_RELATIVE);
    mTime->setHorizontalAlignment(Ogre::GHA_CENTER);
    mTime->setVerticalAlignment(Ogre::GVA_CENTER);
    mTime->setPosition(0.0, -0.15);
    mTime->setDimensions(1, 0.2);
    mTime->setCharHeight(BIG_CHAR_HEIGHT);
    mTime->setFontName("CoolFont");
    mTime->setColour(color_1);
    mTime->setAlignment(TextAreaOverlayElement::Center);
    mTime->setCaption(_("Time"));

    Ogre::TextAreaOverlayElement *mDepth = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "HighScoreTable_depth"));
    mDepth->setMetricsMode(Ogre::GMM_RELATIVE);
    mDepth->setHorizontalAlignment(Ogre::GHA_CENTER);
    mDepth->setVerticalAlignment(Ogre::GVA_CENTER);
    mDepth->setPosition(0.30, -0.15);
    mDepth->setDimensions(1, 0.2);
    mDepth->setCharHeight(BIG_CHAR_HEIGHT);
    mDepth->setFontName("CoolFont");
    mDepth->setColour(color_1);
    mDepth->setAlignment(TextAreaOverlayElement::Center);
    mDepth->setCaption(_("Depth"));

    /*Ogre::TextAreaOverlayElement *mPoints = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "HighScoreTable_points"));
    mPoints->setMetricsMode(Ogre::GMM_RELATIVE);
    mPoints->setHorizontalAlignment(Ogre::GHA_CENTER);
    mPoints->setVerticalAlignment(Ogre::GVA_CENTER);
    mPoints->setPosition(0.35, -0.15);
    mPoints->setDimensions(1, 0.2);
    mPoints->setCharHeight(BIG_CHAR_HEIGHT);
    mPoints->setFontName("CoolFont");
    mPoints->setColour(color_1);
    mPoints->setAlignment(TextAreaOverlayElement::Center);
    mPoints->setCaption(_("Points"));*/


    Ogre::TextAreaOverlayElement *mTable = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "HighScoreTable_table"));
    mTable->setMetricsMode(Ogre::GMM_RELATIVE);
    mTable->setHorizontalAlignment(Ogre::GHA_CENTER);
    mTable->setVerticalAlignment(Ogre::GVA_CENTER);
    mTable->setPosition(0.0, -0.05);
    mTable->setDimensions(1, 0.2);
    mTable->setCharHeight(SMALL_CHAR_HEIGHT);
    mTable->setFontName("Monospace");
    mTable->setColour(color_2);
    mTable->setAlignment(TextAreaOverlayElement::Center);
    mTable->setCaption("");

    mPanel->addChild(mName);
    mPanel->addChild(mTime);
    mPanel->addChild(mDepth);
    //mPanel->addChild(mPoints);
    mPanel->addChild(mTable);


    mOverlay->add2D(mPanel);

    mOverlay->show();
}

HighScoreTable::~HighScoreTable()
{
    mPanel->removeChild("HighScoreTable_name");
    mPanel->removeChild("HighScoreTable_time");
    mPanel->removeChild("HighScoreTable_depth");
    //mPanel->removeChild("HighScoreTable_points");
    mPanel->removeChild("HighScoreTable_table");

    mOverlay->remove2D(mPanel);

    mOverlayMgr->destroyOverlayElement("HighScoreTable_name");
    mOverlayMgr->destroyOverlayElement("HighScoreTable_time");
    mOverlayMgr->destroyOverlayElement("HighScoreTable_depth");
    //mOverlayMgr->destroyOverlayElement("HighScoreTable_points");
    mOverlayMgr->destroyOverlayElement("HighScoreTable_table");
    mOverlayMgr->destroyOverlayElement("HighScoreTable_panel");

}


void HighScoreTable::updateTable(Ogre::String mode, Ogre::String level)
{
    String tableText;

    Ogre::TextAreaOverlayElement *mTable;

    mTable = static_cast<TextAreaOverlayElement*>(
    mOverlayMgr->getOverlayElement("HighScoreTable_table"));


    HighScore* highScore;

    for(int i = 0; i < SCORES_PER_PAGE; i++)
    {
        highScore = HighScoreManager::getSingleton().getScore(mode,level,i);

        String name = highScore->name;
        String time = secsToClockString(highScore->time);
        String depth = StringConverter::toString(highScore->depth);
        //String points = StringConverter::toString(highScore->points);

        if(name == "")
        {
            tableText += "    -                -               -    \n";
        }
        else
        {

            int spaces = 12-name.size();

            tableText += name;

            for(int j=0;j<spaces; j++)
            {
                tableText += " ";
            }

            spaces = 6-time.size();

            for(int j=0;j<spaces; j++)
            {
                tableText += " ";
            }

            tableText += time;

            spaces = 10-depth.size();

            for(int j=0;j<spaces; j++)
            {
                tableText += " ";
            }

            tableText += depth;

            /*spaces = 14-points.size();

            for(int j=0;j<spaces; j++)
            {
                tableText += " ";
            }

            tableText += points;*/

            tableText += "\n";
        }

    }

    mTable->setCaption(tableText);
}











