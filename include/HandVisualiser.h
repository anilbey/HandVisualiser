
#ifndef __HandVisualiser_h_
#define __HandVisualiser_h_

#include "BaseApplication.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


#include "BaseApplication.h"
#include "SampleListener.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


class HandVisualiser : public BaseApplication
{

private:

	Ogre::SceneNode* bonesArr[20]; //array that stores the bones
	SampleListener leapSampleListener;
	Controller leapController;
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);
	Ogre::SceneNode* handNode;
	Ogre::SceneNode *palmNode;
	Ogre::SceneNode* fingersNode;
	float previousFramePitch;
	float previousFrameYaw;
	float previousFrameRoll;
	bool handOrientationFlag;

public:
    HandVisualiser(void);
    virtual ~HandVisualiser(void);

protected:
    virtual void createScene(void);
	virtual void createViewports(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	
};

#endif // #ifndef __HandVisualiser_h_
