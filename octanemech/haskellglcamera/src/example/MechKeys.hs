----------------------------------------
{-  Module      : Basic Keyboard Actions
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : Basic Keyboard Actions
    TODO        : 
    Date        : 9/20/2007
-}
----------------------------------------

module MechKeys (mechKeyboardHandler) where

import Graphics.UI.GLUT
import System.Exit ( exitWith, ExitCode(..) )

import MatrixUtils
import BasicMechs

import Debug.Trace ( trace )

--
-- Mech Keyboard Action
-- mechKeyboardAct :: Map.Map k a -> Key -> KeyState -> SimplePlayer
mechKeyboardAct mech (SpecialKey KeyLeft) Down   = mech{posX = (posX mech) - ssize}
mechKeyboardAct mech (SpecialKey KeyRight) Down  = mech{posX = (posX mech) + ssize}
mechKeyboardAct mech (SpecialKey KeyUp) Down     = mech{posY = (posY mech) + ssize}
mechKeyboardAct mech (SpecialKey KeyDown) Down   = mech{posY = (posY mech) - ssize}

mechKeyboardAct mech (Char 'w') Down   = mech
mechKeyboardAct mech (Char 'a') Down   = mech
mechKeyboardAct mech (Char 's') Down   = mech
mechKeyboardAct mech (Char 'd') Down   = mech

mechKeyboardAct mech (Char '\27') Down           = escapeKeyHandler mech
mechKeyboardAct mech                         _ _ = mech

--
-- Debug trace escape key handler
escapeKeyHandler :: SimplePlayer -> SimplePlayer
escapeKeyHandler mechRef =
    trace ("INFO: exiting")
    mechRef

--
-- Mech Keyboard Handler, given the mech object reference and the
-- [ key, state, modifiers, and keyboard pos ]
mechKeyboardHandler mechRef key state modifiers position = do
  mech <- get mechRef
  mechRef $= mechKeyboardAct mech key state

