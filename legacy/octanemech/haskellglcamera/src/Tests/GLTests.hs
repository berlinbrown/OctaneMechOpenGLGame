--
--
module Tests.GLTests where

import HUnit
import MatrixUtils
import BasicMechs
import Data.IORef

import Graphics.Rendering.OpenGL

foo :: Int -> (Int, Int)
foo x = (1, x)

exampleTestUtilOne :: Int -> IO (Int, Int)
exampleTestUtilOne v = return (v+2, v+3)

exampleTestUtilTwo :: Int -> IO Bool
exampleTestUtilTwo v = return (v > 5)

exampleTestUtilThree :: IO String
exampleTestUtilThree = return $ show curMechRef

exampleTestUtilMechRef :: IO SimplePlayer
exampleTestUtilMechRef = return curMechRef

updateTestUtilMechRef :: SimplePlayer -> Float -> Float -> IO SimplePlayer
updateTestUtilMechRef simpl posx posy = return $ simpl{posX = posx, posY = posy}

testUtilGetMechRef :: SimplePlayer -> IO String
testUtilGetMechRef simpl = return $ show simpl

--
-- Also see the following for more information types
-- http://www.haskell.org/tutorial/moretypes.html
--
instance Show SimplePlayer where
    show simpleEntity = "<Simple> posX [" ++ show (posX simpleEntity) ++ 
                        "] posY [" ++ show (posY simpleEntity) ++ "]"

--
-- Also, see http://en.wikibooks.org/wiki/Haskell/Type_Declarations
-- The field name f (posX) applies to both constructors in T (SimplePlayer)
curMechRef = SimplePlayer { shape = MechBoxShape, 
                            posX = 0.1, posY = 0.4, 
                            angle = 0 }
                         	                         	
-----------------------------
-- Test Cases
-----------------------------

-- Will Give an Error
test1 = TestCase (assertEqual "Test Case - call foo 3):" 
		  (1, 3) (foo 3))
								
test2 = TestCase (do (x,y) <- exampleTestUtilOne 3
                     assertEqual "Test Case - for the first result of [TestOne]: " 5 x
                     b <- exampleTestUtilTwo y
                     assertBool ("Test Case - [TestTwo]: " ++ show y ++ " failed") b)
                     
testSimpleEntity = TestCase (do curmch <- exampleTestUtilMechRef
                                uu <- updateTestUtilMechRef curmch 0.4 0.22
                                zz <- testUtilGetMechRef uu
                                putStrLn ("" ++ (show zz))
                                assertEqual ("Test Case - [TestTwo]: failed") 2 2)
                                                                
-----------------------------
-- Define the List of Test Cases
-----------------------------

tests = TestList [ TestLabel "Test 1" test1, 
		   TestLabel "Test 2" test2 ,
		   TestLabel "Test Entity" testSimpleEntity ]

-- End of script				

