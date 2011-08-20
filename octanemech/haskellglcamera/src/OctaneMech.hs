----------------------------------------
{-  Module      : OctaneMech Main
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : Octane Mech Game
    TODO        : 
    Date        : 9/20/2007
-}
----------------------------------------
module Main where

import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT

import MechGLScreen

main = do
  putStrLn "running OctaneMech"
  mechLaunchGLWindow
           
-- End of File