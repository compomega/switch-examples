#ifndef __Application_h__
#define __Application_h__

#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreConfigFile.h>

#include <OgreViewport.h>
#include <OgreLogManager.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

#include <OgreFrameListener.h>

#include <RTShaderSystem/OgreShaderGenerator.h>

class Application : public Ogre::FrameListener
{
public:
    Application();
    virtual ~Application();

    virtual void go();

protected:
    virtual bool setup();
    virtual bool configure();
    virtual void chooseSceneManager();
    virtual void createCamera();
    virtual void createFrameListener();
    virtual void createScene();
    virtual void destroyScene();
    virtual void createViewports();
    virtual void createResourceListener();
    virtual void loadResources();

    virtual void setupResources();
    virtual void setupRTSS();

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    Ogre::Root *mRoot;
    Ogre::Camera *mCamera;
    Ogre::SceneNode *mCameraNode;
    Ogre::SceneManager *mSceneMgr;
    Ogre::RenderWindow *mWindow;
    Ogre::Viewport* mViewport;
    Ogre::SceneNode *mOgreNode;

    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;

    Ogre::RTShader::ShaderGenerator *mShaderGenerator;

    uint8_t mOperationMode;
};

#endif // #ifndef __Application_h_
