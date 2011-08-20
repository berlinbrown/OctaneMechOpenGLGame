----------------------------------------
{-  Module      : Basic Mechs
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : Generic GL Object Definitions
    TODO        : 
    Date        : 9/20/2007
-}
----------------------------------------

module BasicMechs where

import Graphics.Rendering.OpenGL

import MechGenericObjects
import MatrixUtils
--
-- Basic Mech shape data type
data MechShape = MechBoxShape

data SimplePlayer = SimplePlayer { 
      shape :: MechShape,
      angle :: GLfloat,
      posX :: GLfloat,
      posY :: GLfloat
    }

-- Converts SimplePlayer Data to a readable message.
--instance Show SimplePlayer where
--    show pos = "posX (" ++ (show pos) ++ ")"

--
-- Actually render the piece based on position, angle
renderBasicMech :: SimplePlayer -> IO ()
renderBasicMech piece = preservingMatrix $ do
  translate $ Vector3 (posX piece) (posY piece) 0
  rotate (angle piece) $ Vector3 0 0 (1::GLfloat)
  renderShape $ shape piece

renderShape :: MechShape -> IO ()
renderShape shape = mapM_ (\v -> preservingMatrix $ do
  translate v
  mechCube tunit
  ) $ piecePoints shape

--
-- piecePoints :: TetrisShape -> [Vector3 GLfloat GLfloat GLfloat]
piecePoints = map (\(x,y,z) -> Vector3 x y z) . pointTriplets

--
-- pointTriplets :: TetrisShape -> [(GLfloat,GLfloat,GLfloat)]
--
pointTriplets MechBoxShape = [ (-ssize,0,0) ]