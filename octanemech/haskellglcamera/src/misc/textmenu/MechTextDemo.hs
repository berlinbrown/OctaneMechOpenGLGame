----------------------------------------
{-  Module      : Main
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : Octane Mech Camera Demo
    TODO        : 
    Date        : 9/20/2007
    File        : MechCamera.hs
-}
----------------------------------------
module Main where

import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT
import Data.IORef

import Text.TextUtil
import DisplayLoop

import ScreenSettings

--
-- Initialize OpenGL setup including shade models and depth modes.
-- The following initializations can be performed.
--------------------------------------------------
-- * Enable Smooth Shading
-- Set Shade Model GL_SMOOTH
-- Set Clear Color 0.0f, 0.0f, 0.0f, 0.0f
-- Set Clear Depth 1.0f
-- Set Enable = GL_NORMALIZE
-- Set PolygonMode = GL_FRONT_AND_BACK, GL_FILL
-- Set ShadeModel = GL_SMOOTH
-- Set Enable = GL_DEPTH_TEST
-- Set DepthFunc = GL_LEQUAL
-- * Really Nice Perspective Calculations
-- Set Hint = GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST
--------------------------------------------------
mechInitGL :: IO ()
mechInitGL = do
  putStrLn "Performing GL Initialization..."
  initialDisplayMode $= [DoubleBuffered]
  clear [ColorBuffer, DepthBuffer]
  shadeModel $= Smooth
  hint PerspectiveCorrection $= Fastest
  hint PolygonSmooth $= Fastest

  clearColor $= Color4 0 0 0 0
  clearDepth $= 1
  depthFunc $= Just Less
  shadeModel $= Smooth
  --cursor $= None  
  
  -- Revert to projection matrix mode and
  -- set the perspective, aspect ratio of the window
  -- In C: gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 
  --          0.1f, 100.0f);
  -- (see glant.c ResizeGLScene)
  -- This is important to setting the game view mode.
  matrixMode $= Projection
  loadIdentity
  perspective 45.0 (fromIntegral mechScreenWidth / fromIntegral mechScreenHeight)
                  1.0 mechZPerspective
  matrixMode $= Modelview 0
  viewport $= ((Position 0 0), Size mechScreenWidth mechScreenHeight)
         
  -- GL_MODELVIEW is about having different 
  -- objects being pushed into a "world space".
  matrixMode $= Modelview 0
  flush
 
--
-- Invoke GL Initializations and launch window
mechLaunchGLWindow = do
  getArgsAndInitialize
  -- Initialize with the following settings
  -- Double buffer, RGBA color
  -- Alpha components supported, Depth buffer
  initialDisplayMode $= [ DoubleBuffered, RGBAMode, WithDepthBuffer, 
                             WithAlphaComponent ]

  initialWindowSize $= Size mechScreenWidth mechScreenHeight
  createWindow mechWindowTitle
               
  mechInitGL
  let mainCamera = initCamera (1.4, 4.2, -4.5) (0,0,0) (0,0,0)
      mainMech = initMech (0, 0, 0) (0, 0, 0)
  mainCameraRef <- newIORef(mainCamera)
  mainMechRef <- newIORef(mainMech)
            
  -- Set the OpenGL callbacks
  displayCallback $= displayGameLoop mainCameraRef mainMechRef

  -- Set the keyboard callback for handling actions
  keyboardMouseCallback $= Just (mechKeyPressed mainCameraRef)

  idleCallback $= Just idle            
  mainLoop

--------------------------------------------------
-- Main Entry Point
--------------------------------------------------
main = do
  putStrLn "INFO: Running Mech Camera Demo"
  mechLaunchGLWindow
           
-- End of File