----------------------------------------
{-  Module      : DisplayLoop
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : Display
    TODO        : 
    Date        : 9/20/2007
    File        : DisplayLoop.hs
-}
----------------------------------------

module DisplayLoop where

import System.Exit ( exitWith, ExitCode(..) )

import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT
import Data.IORef
import IO
import Debug.Trace ( trace, putTraceMsg )

import MechGenericObjects

--
-- Keyboard Handler with IO Ref input
-- Including move camera functionality
-- Standard ( wasd keyboard movement )
mechKeyPressed :: IORef Camera -> KeyboardMouseCallback
mechKeyPressed cameraRef (Char 'q')   Down _ _ = do
  -- On key press q, show Camera information
  camera <- readIORef cameraRef
  putStrLn $ "Camera Info=" ++ show(camera)
mechKeyPressed cameraRef (Char 'a')   Down _ _ = do
  camera <- readIORef cameraRef
  cameraRef $= moveCamera MoveL 0.2 camera
mechKeyPressed cameraRef (Char 'd')   Down _ _ = do
  camera <- readIORef cameraRef
  cameraRef $= moveCamera MoveR 0.2 camera
mechKeyPressed cameraRef (Char 'w')   Down _ _ = do
  camera <- readIORef cameraRef
  cameraRef $= moveCamera MoveF 0.2 camera
mechKeyPressed cameraRef (Char 's')   Down _ _ = do
  camera <- readIORef cameraRef
  cameraRef $= moveCamera MoveB 0.2 camera
mechKeyPressed _      (Char '\27') Down _ _ = do exitWith ExitSuccess
mechKeyPressed _      _            _    _ _ = return ()

--
-- Define a Mech Vector Type
type MVector = (Double, Double, Double)

--
-- Camera Data Type with position and angle
-- Vectors.  Note, the use of strict annotation
data Camera = Camera {
      cameraPos         :: !MVector,
      cameraLookAt      :: !MVector,
      cameraAngle       :: !MVector,
      cameraRot         :: !MVector,
      cameraCenterAxis  :: !MVector,
      cameraYaw         :: !MVector,
      cameraPitch       :: !MVector,
      cameraRoll        :: !MVector
    } deriving (Show, Read)

data SimpleMech = SimpleMech {
      mechPos         :: !MVector,
      mechRot         :: !MVector
    } deriving (Read)

instance Show SimpleMech where
    show mech = let (xrot, yrot, zrot) = (mechRot mech)
        in "roty <" ++ (show yrot) ++ ">"

--
-- Convert from a tuple of double to an OpenGL vector
toGLVertex :: MVector -> Vertex3 GLdouble
toGLVertex (a,b,c) =  Vertex3 a b c

-- Convert from a tuple of double to an OpenGL vector
toGLVector :: MVector -> Vector3 GLdouble
toGLVector (a,b,c) =  Vector3 a b c

--------------------------------------------------
-- Camera Operations
--------------------------------------------------

--
-- Using gluLookAt, set the camera lookAt parameters
-- including position and look at.
--
-- gluLookAt(CAMERA->position[0], CAMERA->position[1], CAMERA->position[2], 
--  x, h, y, 0.0f, 1.0f, 0.0f);
-- eyeX, eyeY, eyeZ = Specifies the position of the eye point.
-- centerX, centerY, centerZ = Specifies the position of the reference point.
--
setCameraLookAt :: Camera -> IO ()
setCameraLookAt camera = do
  let curCameraPos = (cameraPos camera)
      curCameraLook = (cameraLookAt camera)
      upVector = (0, 1, 0)
  lookAt (toGLVertex curCameraPos)
             (toGLVertex curCameraLook) 
             (toGLVector upVector)

--
-- Initialize a camera
-- Only set the camera position, camera angle and rotation
-- (Position, Camera Angle, Camera Rotation)
initCamera :: (Real posv) => (posv, posv, posv)-> (posv, posv, posv) -> (posv, posv, posv) -> Camera
initCamera (x1, y1, z1) (x2, y2, z2) (x3, y3, z3) = 
    Camera { 
  cameraPos   = ((realToFrac x1), (realToFrac y1), (realToFrac z1)),
  cameraAngle = ((realToFrac x2), (realToFrac y2), (realToFrac z2)),
  cameraLookAt= ((realToFrac x3), (realToFrac y3), (realToFrac z3)),
  cameraRot        = (0, 0, 0),
  cameraCenterAxis = (0, 0, 0),
  cameraYaw        = (0, 0, 0),
  cameraPitch      = (0, 0, 0),
  cameraRoll       = (0, 0, 0) }

--
-- Move the camera the given direction
-- 1 = Left, 2 = Right, 3 = Up, 4 = Down (along the z axis)
data CameraDir = MoveL | MoveR | MoveF | MoveB deriving (Show, Ord, Eq)

mvCameraX :: Double -> Camera -> Camera
mvCameraX dx camera
    = camera { cameraPos = (x + dx, y, x) }
    where (x,y,z) = cameraPos camera

mvCameraZ :: Double -> Camera -> Camera
mvCameraZ dz camera
    = camera { cameraPos = (x, y, z + dz) }
    where (x,y,z) = cameraPos camera

moveCamera :: CameraDir -> Double -> Camera -> Camera
moveCamera MoveL dpos = mvCameraX (-dpos)
moveCamera MoveR dpos = mvCameraX dpos
moveCamera MoveF dpos = mvCameraZ (-dpos)
moveCamera MoveB dpos = mvCameraZ dpos

--------------------------------------------------
-- Mech Operations
--------------------------------------------------

--
-- Given a angle delta value and a Mech object, 
-- rotate the mech on the Y-axis.
rotateMech :: Double -> SimpleMech -> SimpleMech
rotateMech angle mech =
    mech { mechRot = (xrot, yrot + angle, zrot) }
    where
      (xrot,yrot,zrot) = mechRot mech

initMech :: (Real posv) => (posv, posv, posv)-> (posv, posv, posv) -> SimpleMech
initMech (x1, y1, z1) (x2, y2, z2) = 
    SimpleMech { 
  mechPos = ((realToFrac x1), (realToFrac y1), (realToFrac z1)),
  mechRot = ((realToFrac x2), (realToFrac y2), (realToFrac z2)) }

debugMech :: SimpleMech -> IO ()
debugMech mech = do
    let (xrot, yrot, zrot) = (mechRot mech)
    putTraceMsg $ "mech=" ++ (show mech)

--
-- Core display game loop, including rendering the scene
-- and swapping the GL double buffer.
--
-- Note: write mech ref is not used
displayGameLoop :: IORef Camera -> IORef SimpleMech -> IO ()
displayGameLoop cameraRef mechRef = do
  camera <- readIORef cameraRef
  mech <- readIORef mechRef

  clear [ColorBuffer, DepthBuffer]
  loadIdentity
  setCameraLookAt camera

  -- Render Game Objects (preserving = push/pop)
  let (xrot,yrot,zrot) = mechRot mech
  preservingMatrix $ do
    -- C routine = glRotatef(angle, 0.0f, 1.0f, 0.0f);
    rotate yrot (Vector3 0 1 (0 :: GLdouble))
    mechCube 0.6

  preservingMatrix $ do
    translate (Vector3 0 (-2.2) (0 :: GLdouble))
    renderPlain 1.4

  -- Set new mech position
  mechRef $= rotateMech 0.1 mech
  
  -- OpenGL, swap buffers
  swapBuffers
  flush

--
-- Idle function, do nothing
idle = do
  postRedisplay Nothing
