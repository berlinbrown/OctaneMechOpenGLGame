----------------------------------------
{-  Module      : Mech Terrain
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : 
    TODO        : 
    Date        : 9/20/2007
-}
----------------------------------------
module MechTerrain where

import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT
import Data.Map hiding (map)

import ColorDefs
import MatrixUtils

terrainXSize = 12 * ssize
terrainYSize = 12 * ssize

plainXSize = 10 * ssize
plainYSize = 10 * ssize

renderTerrain = renderOutline >> renderPlain

renderOutline = 
    renderPrimitive LineLoop $ do
      let x = terrainXSize-tunit
          y = terrainYSize-tunit
      color mechWhite
      vertex $ Vertex3 x y 0
      vertex $ Vertex3 x (-y) 0
      vertex $ Vertex3 (-x) (-y) 0
      vertex $ Vertex3 (-x) y 0

--
-- Render a terrain plain on the y axis
renderPlain =
    renderPrimitive Quads $ do
      color mechWhite
      let xSize = plainXSize-tunit
          ySize = plainYSize-tunit
      color mechWhite
      vertex $ Vertex3 xSize    ySize    0
      vertex $ Vertex3 xSize    (-ySize) 0
      vertex $ Vertex3 (-xSize) (-ySize) 0
      vertex $ Vertex3 (-xSize) ySize    0

-- End of File