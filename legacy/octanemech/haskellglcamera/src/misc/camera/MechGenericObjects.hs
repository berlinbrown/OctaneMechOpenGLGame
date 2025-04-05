----------------------------------------
{-  Module      : Mech Generic Objects
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

module MechGenericObjects where

import Graphics.Rendering.OpenGL

mechRed    = Color3 (1.0::GLfloat) 0.0 0.0
mechGreen  = Color3 (0.0::GLfloat) 1.0 0.0
mechBlue   = Color3 (0.0::GLfloat) 0.0 1.0
mechYellow = Color3 (1.0::GLfloat) 1.0 0.0
mechViolet = Color3 (1.0::GLfloat) 1.0 0.0
mechTeal   = Color3 (1.0::GLfloat) 0.0 1.0
mechWhite  = Color3 (1.0::GLfloat) 1.0 1.0

renderPlain :: Double -> IO ()
renderPlain w = do
  color mechWhite
  renderPrimitive Quads $ do
  vertex $ Vertex3   w    0   w
  vertex $ Vertex3 (-w)   0   w
  vertex $ Vertex3 (-w)   0 (-w)
  vertex $ Vertex3   w    0 (-w)

mechCube :: Double -> IO ()
mechCube w = do
  color mechRed
  renderPrimitive Quads $ do
    -- Top of cube
    color mechRed
    vertex $ Vertex3   w   w (-w)
    vertex $ Vertex3 (-w)  w (-w)
    vertex $ Vertex3 (-w)  w   w
    vertex $ Vertex3   w   w   w

    -- Bottom of cube
    color mechYellow
    vertex $ Vertex3  w   (-w)   w
    vertex $ Vertex3 (-w) (-w)   w
    vertex $ Vertex3 (-w) (-w) (-w)
    vertex $ Vertex3  w   (-w) (-w)

    -- Front of cube
    color mechBlue
    vertex $ Vertex3   w    w  w
    vertex $ Vertex3 (-w)   w  w
    vertex $ Vertex3 (-w) (-w) w
    vertex $ Vertex3   w  (-w) w

    -- Back of cube
    color mechViolet
    vertex $ Vertex3   w  (-w) (-w)
    vertex $ Vertex3 (-w) (-w) (-w)
    vertex $ Vertex3 (-w)  w   (-w)
    vertex $ Vertex3   w   w   (-w)

    -- Left of cube
    color mechGreen
    vertex $ Vertex3 (-w)   w    w
    vertex $ Vertex3 (-w)   w  (-w)
    vertex $ Vertex3 (-w) (-w) (-w)
    vertex $ Vertex3 (-w) (-w)   w

    -- Right of cube
    color mechTeal
    vertex $ Vertex3 w   w  (-w)
    vertex $ Vertex3 w   w    w
    vertex $ Vertex3 w (-w)   w
    vertex $ Vertex3 w (-w) (-w)

-- End of File
