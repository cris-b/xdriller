#include "MaterialInstance.h"

using namespace Ogre;

MaterialInstance::MaterialInstance () {
  mCurrentTransparency = 0.0f;

  mCopyMat.reset();

  mSBT = SBT_TRANSPARENT_ALPHA;
}

MaterialInstance::~MaterialInstance () {
  clearCopyMaterial ();
}

void MaterialInstance::setSceneBlending (SceneBlendType sbt) {
  mSBT = sbt;

  if (mCopyMat) {
    for(const auto& tech : mCopyMat->getTechniques()) {
      for(const auto& pass : tech->getPasses()) {
        pass->setSceneBlending (mSBT);
      }
    }
  }
}

void MaterialInstance::setTransparency (Real transparency) {
  mCurrentTransparency = transparency;
  if (mCurrentTransparency > 0.0f) {
    if (mCurrentTransparency > 1.0f)
      mCurrentTransparency = 1.0f;

    if (!mCopyMat) {
      createCopyMaterial ();
    }

    unsigned short i = 0, j;
    ColourValue sc, dc; // Source colur, destination colour
    for(const auto& tech : mCopyMat->getTechniques()) {
      j = 0;
      for(const auto& pass : tech->getPasses()) {
        sc = mOriginalMat->getTechnique (i)->getPass (j)->getDiffuse ();

        switch (mSBT) {
          case SBT_ADD:
            dc = sc;
            dc.r -= sc.r * mCurrentTransparency;
            dc.g -= sc.g * mCurrentTransparency;
            dc.b -= sc.b * mCurrentTransparency;
            pass->setAmbient (ColourValue::Black);
            break;
          case SBT_TRANSPARENT_ALPHA:
          default:
            dc = sc;
            dc.a = sc.a * (1.0f - mCurrentTransparency);
            pass->setAmbient (mOriginalMat->getTechnique (i)->getPass (j)->getAmbient ());
            break;
        }
        pass->setDiffuse (dc);

        ++j;
      }

      ++i;
    }
  }
  else {
    mCurrentTransparency = 0.0f;
  }
}

MaterialPtr MaterialInstance::getCopyMaterial () {
  return mCopyMat;
}

void MaterialInstance::createCopyMaterial () {
  String name;
  // Avoid name collision
  do {
    name = mOriginalMat->getName () + StringConverter::toString (Math::UnitRandom ());
  } while (MaterialManager::getSingleton ().resourceExists (name));

  mCopyMat = mOriginalMat->clone (name);

  for(const auto& tech : mCopyMat->getTechniques()) {
    for(const auto& pass: tech->getPasses()) {
      pass->setDepthWriteEnabled (false);
      pass->setSceneBlending (mSBT);
    }
  }
}

void MaterialInstance::clearCopyMaterial () {
  if (mCopyMat)
    MaterialManager::getSingleton ().remove (mCopyMat->getName ());

  mCopyMat.reset();
}

