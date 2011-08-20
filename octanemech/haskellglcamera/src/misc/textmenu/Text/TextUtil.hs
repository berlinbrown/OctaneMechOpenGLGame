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

module Text.TextUtil where

import ScreenSettings (mechScreenWidth, mechScreenHeight)
import Text.ByteFontData

import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT

import Data.Word
import Data.Array

-- Text Utility in conjuction with preserving matrix.
-----------------------------------------------------
-- | (from preservingMatrix Docs) Push the current matrix stack down by one, duplicating the current matrix,
-- excute the given action, and pop the current matrix stack, replacing the
-- current matrix with the one below it on the stack (i.e. restoring it to its
-- previous state). The returned value is that of the given action.
renderGLText :: IO a -> IO ()
renderGLText action = do  
  -- Call text begin and then invoke the 'action'
  -- Use of two calls to preserving matrix.
  matrixMode $= Projection
  preservingMatrix $ do
    -- (1) First matrix push
    loadIdentity
    ortho 0.0 640 480 0.0 (-1.0) (1.0 :: GLdouble)
    matrixMode $= Modelview 0
    preservingMatrix $ do
                        -- (2) Second matrix push
                        loadIdentity
                        --glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT)
                        -- C Version: glBindTexture(GL_TEXTURE_2D, fontID)
                        [texName] <- genObjectNames 1

                        -- Set our new texture as OpenGL's current texture. 
                        -- This ensures that all texture operations that occur 
                        -- from this point on will happen to this texture.
                        textureBinding Texture2D $= Just texName
                        textureWrapMode Texture2D S $= (Repeated, Repeat)
                        textureWrapMode Texture2D T $= (Repeated, Repeat)

                        -- C Version: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
                        -- C Version: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)  
                        textureFilter Texture2D $= ((Nearest, Nothing), Nearest)

                        --glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)                 
                        -- Enable 2D Texturing
                        -- Tells OpenGL that we want to use 2D textures
                        texture Texture2D $= Enabled
                        -- Always Draw in Front
                        --glDisable(GL_DEPTH_TEST)
                        --glDisable(GL_CULL_FACE)
                        lighting $= Disabled

                        (do action)
                        
                        -- (1) Pop matrix
                        matrixMode $= Projection
    -- (2) Pop matrix                   
    matrixMode $= Modelview 0

-- End of File