

#include "HandVisualiser.h"
using std::cout;
using std::endl;






//-------------------------------------------------------------------------------------
HandVisualiser::HandVisualiser(void)
{
	previousFramePitch = previousFrameYaw = previousFrameRoll = 0;
	handOrientationFlag = false;
	

	
}
//-------------------------------------------------------------------------------------
HandVisualiser::~HandVisualiser(void)
{
	
}

//-------------------------------------------------------------------------------------
void HandVisualiser::createScene(void)
{

	
	// leap controller and listener objects
	leapController.addListener(leapSampleListener);
	leapController.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);
	leapSampleListener.onFrame(leapController);
	// end leap



	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
	//hand
	handNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HandNode");
	handNode->setPosition (0, -300, -500);
	// palm
	Ogre::Entity* palm = mSceneMgr->createEntity("palm","cube.mesh");
	palmNode = handNode->createChildSceneNode("PalmNode");
	
	palmNode->attachObject(palm);
	palmNode->scale(.1,.1,.1);
	
	
	// fingers
	//Ogre::Entity* fingers = new Ogre::Entity[5];
	fingersNode = handNode->createChildSceneNode("FingersNode");
	
	Ogre::Entity* bone;
	char str[3];

	for (int i = 0; i < 20; ++i)
	{
		sprintf(str, "%d", i);
		bone = mSceneMgr->createEntity(str,"cube.mesh");
		bonesArr[i] = fingersNode->createChildSceneNode(str);
		bonesArr[i]->attachObject(bone);
		bonesArr[i]->scale(.1,.1,.1);

	
	
	}


 
    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(250, 150, 250));
    pointLight->setDiffuseColour(Ogre::ColourValue::White);
    pointLight->setSpecularColour(Ogre::ColourValue::White);


}

//-------------------------------------------------------------------------------------
void HandVisualiser::createViewports(void)
{
	    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(.2,.1,.3));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));  
}
 
//-------------------------------------------------------------------------------------

bool HandVisualiser::processUnbufferedInput(const Ogre::FrameEvent& evt)
{


	//leap
	static Frame frame;
	frame = leapController.frame();
	

	
	Leap::Hand rightMost = frame.hands().rightmost();

		
		
	previousFramePitch = rightMost.direction().pitch() * RAD_TO_DEG;
	previousFrameYaw = rightMost.direction().yaw() * RAD_TO_DEG;
	previousFrameRoll = rightMost.palmNormal().roll() * RAD_TO_DEG;

	
	
	
	if (!frame.hands().isEmpty() && !handOrientationFlag) 
	{
		
		palmNode->resetOrientation();
		handOrientationFlag = true;	
	}
	else if (handOrientationFlag && frame.hands().isEmpty() )
	{
		handOrientationFlag = false;
	}


	if (!frame.hands().isEmpty())
	{
		

		Leap::Hand rightMost = frame.hands().rightmost();
		palmNode->setPosition(rightMost.palmPosition().x, rightMost.palmPosition().y, rightMost.palmPosition().z); // between 100 and 250

		float pitchValue = rightMost.direction().pitch() * RAD_TO_DEG;
		palmNode->pitch((Ogre::Radian) (pitchValue - previousFramePitch) );

		float rollValue = rightMost.palmNormal().roll() * RAD_TO_DEG;
		palmNode->roll((Ogre::Radian) (rollValue - previousFrameRoll) );

		float yawValue = rightMost.direction().yaw() * RAD_TO_DEG;
		palmNode->yaw((Ogre::Radian) (yawValue - previousFrameYaw) );

		/*char* dummy = (char*) malloc(64);
		sprintf (dummy, "pitch: %f, yaw: %f, roll: %f\n", pitchValue, yawValue, rollValue); 
		OutputDebugString (dummy);
		free(dummy);*/
 
		previousFramePitch = rightMost.direction().pitch() * RAD_TO_DEG;
		previousFrameYaw = rightMost.direction().yaw() * RAD_TO_DEG;
		previousFrameRoll = rightMost.palmNormal().roll() * RAD_TO_DEG;


		// Get fingers
		static FingerList fingers; 
		fingers = rightMost.fingers();
		int i = 0;
		int index = 0; //between 0 and 19 (finger bones)
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) 
		{
			
			static Finger finger;
			finger = *fl;


		  /*char* dummy = (char*) malloc(128);
			sprintf (dummy, "finger id: %d, length: %f, width: %f\n", finger.id(), finger.length(), finger.width()); 
			OutputDebugString (dummy);
			free(dummy);*/

			// Get finger bones
			for (int b = 0; b < 4; ++b, ++index) 
			{
				static Bone::Type boneType;
				boneType = static_cast<Bone::Type>(b);
				static Bone bone;
				bone = finger.bone(boneType);
				bonesArr[i++]->setPosition(bone.center().x, bone.center().y, bone.center().z);

			

			}
		}

	}



 
   

    return true;
}
//-------------------------------------------------------------------------------------
bool HandVisualiser::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
 
    if (!processUnbufferedInput(evt)) return false;
 
    return ret;
}
//-------------------------------------------------------------------------------------


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        HandVisualiser app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif