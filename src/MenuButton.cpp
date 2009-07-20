#include "MenuButton.h"

int menuButtonCount = 0;

using namespace Ogre;

MenuButton::MenuButton(UTFString caption, int align, bool hasOption, bool hasArrows)
{
    this->_hasOption = hasOption;
    this->_hasArrows = hasArrows;
    this->align = align;

    String name = "MenuButton_" + StringConverter::toString(menuButtonCount);
    menuButtonCount++;

    mNode = Root::getSingleton().getSceneManager( "ST_GENERIC" )->getRootSceneNode()->createChildSceneNode(name + "_Node");
    mTextNode = mNode->createChildSceneNode();

    text = new MovableText(name,caption);


    mTextNode->attachObject(text);

    mTextNode->scale(0.5,0.5,0.5);

    if(hasOption)
    {
        optionText = new MovableText(name + "_option",".");

        mOptionNode = mNode->createChildSceneNode();

        mOptionNode->attachObject(optionText);

        AxisAlignedBox aabb = optionText->GetAABB();

        mOptionNode->scale(0.5,0.5,0.5);

        mOptionNode->setPosition(8 -(aabb.getMinimum().x+aabb.getMaximum().x)/2,0,0);


    }


    AxisAlignedBox aabb = text->GetAABB();

    if(align == ALIGN_CENTER)
        mTextNode->setPosition(-(aabb.getMinimum().x+aabb.getMaximum().x)/4.0,0,0);
    //else

        //mTextNode->translate(-(aabb.getMinimum().x+aabb.getMaximum().x)/3.0,0,0);

    //LogManager::getSingleton().logMessage(StringConverter::toString(this->text->getWidth()));

    // Create a manual object for 2D
    frame = Root::getSingleton().getSceneManager( "ST_GENERIC" )->createManualObject(name + "_frame");

    // Use identity view/projection matrices
    //frame->setUseIdentityProjection(true);
    //frame->setUseIdentityView(true);

    frame->begin("button", RenderOperation::OT_TRIANGLE_STRIP);

    frame->position(-3, -0.4, -0);
    frame->textureCoord(0, 0);
    frame->position( 3, -0.4, -0);
    frame->textureCoord(1, 0);
    frame->position( 3,  0.4, -0);
    frame->textureCoord(1, 1);
    frame->position(-3,  0.4, -0);
    frame->textureCoord(0, 1);

    frame->index(0);
    frame->index(1);
    frame->index(2);
    frame->index(3);
    frame->index(0);

    frame->end();


    frame->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);
    text->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);



    //mTextNode->needUpdate();

    mNode->attachObject(frame);
    //mNode->needUpdate();



    //mNode->showBoundingBox(true);
    //mTextNode->showBoundingBox(true);

    state = BSTATE_INACTIVE;
    dest_reached = false;

    speed = Vector3(0,0,0);

}

float MenuButton::getWidth()
{
    AxisAlignedBox aabb = text->GetAABB();

    return (aabb.getMinimum().x+aabb.getMaximum().x)/45.0+0.06;
}

float MenuButton::getOptionWidth()
{
    //return 0;
    if(!_hasOption) return 0.0;

    AxisAlignedBox aabb = optionText->GetAABB();

    return (aabb.getMinimum().x+aabb.getMaximum().x)/45.0+0.06;
}

Ogre::Vector2 MenuButton::getScreenPosition()
{

    Vector2 pos;

    pos.y = dest.y*-0.118 + 0.003;

    pos.x = 0;

    return pos;



    //return aabb.getCenter();
}

Ogre::Vector2 MenuButton::getOptionScreenPosition()
{

    if(!_hasOption) return Vector2(0,0);

    //AxisAlignedBox aabb = optionText->GetAABB();

    Vector2 pos;

    pos.y = dest.y*-0.118 + 0.003;

    pos.x = 0.41-getOptionWidth();

    return pos;



    //return aabb.getCenter();
}

void MenuButton::setOptionCaption(String caption)
{
    mOptionNode->detachObject(optionText);

    delete optionText;

    optionText = new MovableText("MenuButton_" + StringConverter::toString(menuButtonCount) + "_option",caption);

    optionText->setColor(ColourValue(1.0,0.3,0.0));

    mOptionNode->attachObject(optionText);

    AxisAlignedBox aabb = optionText->GetAABB();

    mOptionNode->setPosition(0,0,0);

    mOptionNode->setScale(1,1,1);

    mOptionNode->scale(0.5,0.5,0.5);

    mOptionNode->setPosition(8 -(aabb.getMinimum().x+aabb.getMaximum().x)/2,0,0);

}

void MenuButton::setCaption(String caption)
{
    text->setCaption(caption);


    AxisAlignedBox aabb = text->GetAABB();


    if(align == ALIGN_CENTER)
        mTextNode->setPosition(-(aabb.getMinimum().x+aabb.getMaximum().x)/4.0,0,0);

}

MenuButton::~MenuButton()
{

    mTextNode->detachAllObjects();
    mNode->detachAllObjects();
    delete text;
    if(_hasOption) delete optionText;
    Root::getSingleton().getSceneManager( "ST_GENERIC" )->destroyManualObject( frame );
    mNode->removeAndDestroyAllChildren();
    mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());

    menuButtonCount--;

}

void MenuButton::setState(int state)
{
    this->state = state;

    if(state == BSTATE_ACTIVE)
    {
        text->setColor(ColourValue(1,1,0));
    }
    else if(state == BSTATE_INACTIVE)
    {
        text->setColor(ColourValue::White);
    }

}

void MenuButton::update(unsigned long lTimeElapsed)
{
    if(dest_reached == false)
    {
        Vector3 pos = mNode->getPosition();
        Vector3 dir = dest-pos;

        dir.normalise();

        speed += dir*lTimeElapsed*0.0001;

        if(speed.length() > Vector3(pos-dest).length())
        {
            speed = Vector3(0,0,0);
            dest_reached = true;
            mNode->setPosition(dest);
        }
        else
        {
            mNode->setPosition(pos+speed);
        }

    }

}

void MenuButton::setDest(Vector3 dest)
{
    this->dest = dest;
    dest_reached = false;
}

void MenuButton::setDest(Real x,Real y ,Real z)
{
    Vector3 dest(x,y,z);
    this->dest = dest;
    dest_reached = false;
}








