----------------------------------------
{-  Module      : Display
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : Display
    TODO        : 
    Date        : 9/20/2007
-}
----------------------------------------

module MechDisplay (displayGameLoop, idle) where

import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT

import MechTerrain
import BasicMechs

--
-- Core display game loop, including rendering the scene
-- and swapping the GL double buffer.
displayGameLoop mechObjRef rotationMatrix = do
  clear [ColorBuffer]
  curMech <- get mechObjRef
  prevMatrix <- get rotationMatrix
  loadIdentity

  multMatrix prevMatrix
  curMatrix <- get ((matrix $ Just $ Modelview 0)::StateVar(GLmatrix GLdouble)) 

  loadIdentity
  scale 0.06 0.06 (0.06::GLfloat)
  multMatrix curMatrix

  -- Render the terrain, including the flat plain
  renderTerrain

  -- Render the Mechs
  renderBasicMech curMech

  -- OpenGL, swap buffers
  swapBuffers

--
-- Idle function, do nothing
idle = do
  postRedisplay Nothing