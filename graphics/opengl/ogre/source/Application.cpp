#include "Application.h"

#include <OgreArchiveManager.h>
#include <OgreSceneLoaderManager.h>
#include <OgreSceneNode.h>
#include <OgreSGTechniqueResolverListener.h>
#include <OgreTextureManager.h>

#include <RenderSystems/GL3Plus/OgreGL3PlusPlugin.h>

#ifdef __SWITCH__
#include <switch.h>
#endif // __SWITCH__

Application::Application() : mRoot(0), mCamera(0), mCameraNode(0),
    mSceneMgr(0), mWindow(0), mViewport(0), mOgreNode(0)
{
#ifdef __SWITCH__
    mOperationMode = appletGetOperationMode();
#endif // __SWITCH__
}

Application::~Application()
{
    delete mRoot;
}

bool Application::configure()
{
    mRoot->installPlugin(new Ogre::GL3PlusPlugin);

    if(!mRoot->restoreConfig())
    {
        return false;
    }

    mWindow = mRoot->initialise(true, "Ogre Demo for Nintendo Switch");

    return true;
}

void Application::createScene()
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setPosition(20, 80, 50);

    Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");
    mOgreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mOgreNode->attachObject(ogreEntity);
}

void Application::chooseSceneManager()
{
    mSceneMgr = mRoot->createSceneManager();
}

void Application::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCamera");
    mCamera->setNearClipDistance(5);
    mCamera->setAutoAspectRatio(true);

    mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mCameraNode->attachObject(mCamera);
    mCameraNode->setPosition(0, 0, 140);
}

void Application::createFrameListener()
{
    // Add the Application class as a frame listener
    mRoot->addFrameListener(this);
}

void Application::destroyScene()
{
}

void Application::createViewports(void)
{
    // Create one viewport for the entire window
    mViewport = mWindow->addViewport(mCamera);

    // Set the background color of the viewport to black
    mViewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) /
        Ogre::Real(mViewport->getActualHeight()));
}

void Application::setupResources()
{
    Ogre::ResourceGroupManager& resMan = Ogre::ResourceGroupManager::getSingleton();

    // Create the default resource groups
    resMan.createResourceGroup("Demo", false);

    // Add the game directory to the resource search path
    resMan.addResourceLocation("romfs:/Demo", "FileSystem", "Demo");
}

void Application::createResourceListener()
{
}

void Application::loadResources()
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Application::go()
{
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";

    if(!setup())
    {
        return;
    }

    mRoot->startRendering();

    // Clean up
    destroyScene();
}

bool Application::setup()
{
    // Disable the logging to Ogre.log (in the current working directory).
    bool suppressLogFile = true; // set to false to get your log file
    Ogre::LogManager *logManager = new Ogre::LogManager();
    logManager->createLog("ogre.log", true, true, suppressLogFile);

    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    if(!configure())
        return false;

    chooseSceneManager();

    createCamera();
    createViewports();

    // Set default mipmap level
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();

    // Load resources
    loadResources();

    // Setup the RTSS plugin
    setupRTSS();

    // Create the scene
    createScene();

    // Create the frame listener
    createFrameListener();

    return true;
}

void Application::setupRTSS()
{
    std::string shaderLibPath = "romfs:/Media/RTShaderLib";
    std::string shaderCachePath = "Cache";

    if(Ogre::RTShader::ShaderGenerator::initialize())
    {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            shaderLibPath, "FileSystem");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            shaderLibPath + "/GLSL", "FileSystem");


        // Uncomment to get the shader cache.
        // mShaderGenerator->setShaderCachePath(shaderCachePath);
        mShaderGenerator->addSceneManager(mSceneMgr);

        OgreBites::SGTechniqueResolverListener *materialMgrListener = 0;

        if(!materialMgrListener)
        {
            materialMgrListener = new OgreBites::SGTechniqueResolverListener(mShaderGenerator);
            Ogre::MaterialManager::getSingleton().addListener(materialMgrListener);
        }


        Ogre::LogManager::getSingletonPtr()->logMessage("Initialized RTSS");
    }
    else
    {
        Ogre::LogManager::getSingletonPtr()->logMessage("Failed to init RTSS");
    }
}

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
    {
        return false;
    }

    // Pump the main loop
    if(!appletMainLoop())
    {
        return false;
    }

#ifdef __SWITCH__
    uint8_t operationMode = appletGetOperationMode();

    // If the switch has switched between docked and handheld mode, trigger
    // a notification to change the resolution of the window.
    if(operationMode != mOperationMode)
    {
        mOperationMode = operationMode;
        mWindow->windowMovedOrResized();
    }
#endif // __SWITCH__

    mOgreNode->rotate(Ogre::Vector3::UNIT_Y,
        Ogre::Degree(evt.timeSinceLastFrame * 30.0f));

    return true;
}
