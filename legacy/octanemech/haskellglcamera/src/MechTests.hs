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

import Tests.GLTests
import HUnit

main = do
  putStrLn "Running Tests"
  runTestTT tests
           
-- End of File