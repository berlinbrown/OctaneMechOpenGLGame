--
-- Berlin Brown
--
module MatrixUtils where

import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT
import Foreign.Storable
import Foreign.Ptr

tunit :: GLfloat
tunit = 0.5
ssize = 2 * tunit

rotateZMatrix degree = map (fromIntegral . round) [ cos r, -sin r, 0, 0, sin r, cos r, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]
  where r = degree*pi/180

multiplyListTuple l  t@(x,y,z) = (rx,ry,z)
  where rx = rcMultiply l t
        ry = rcMultiply (drop 4 l) t
        rcMultiply l (x,y,z) = sum $ zipWith (*) l [x,y,z,1]

