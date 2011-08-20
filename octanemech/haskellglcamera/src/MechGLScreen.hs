----------------------------------------
{-  Module      : GL Screen
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : Screen GL inits
    TODO        : 
    Date        : 9/20/2007
-}
----------------------------------------
module MechGLScreen where

import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT
import Data.IORef

import MechDisplay
import BasicMechs
import MechTerrain
import MatrixUtils
import MechKeys

mechWindowTitle = "OctaneMech"

mechScreenWidth = 640
mechScreenHeight = 480

--
-- Perform GL Initializations and launch window
mechLaunchGLWindow = do
  getArgsAndInitialize
  initialDisplayMode $= [DoubleBuffered]
  initialWindowSize $= Size mechScreenWidth mechScreenHeight
  createWindow mechWindowTitle
          
  -- Build the initial mech object
  curMech <- newIORef (SimplePlayer {
                         shape = MechBoxShape, 
                         posX = 0, 
                         posY = terrainYSize-ssize, 
                         angle = 0})

  -- Set initial rotation
  loadIdentity
  let xaxis = (Vector3 1 0 0)
      yaxis = (Vector3 0 1 0)
      zaxis = (Vector3 0 0 1)
  -- yaxis <- reverseRotateXVector ((fromIntegral yM::GLfloat)/5) (Vector3 0 1 0) 
  rotate (fromIntegral 60::GLfloat) xaxis
  rotate (fromIntegral 10::GLfloat) yaxis
  rotate (fromIntegral 10::GLfloat) zaxis
  -- End of initial rotation

  -- Get the current and rotation matrices
  curMatrix <- get ((matrix $ Just $ Modelview 0)::StateVar(GLmatrix GLdouble))
  rotationMatrix <- newIORef curMatrix
  -- Keep track of camera movement
  cameraRotPosition <- newIORef (0::GLint, 0)

  -- Set the OpenGL callbacks
  displayCallback $= displayGameLoop curMech rotationMatrix

  -- Set the keyboard callback for handling actions
  keyboardMouseCallback $= Just (mechKeyboardHandler curMech)

  idleCallback $= Just idle            
  mainLoop

-- End of File
