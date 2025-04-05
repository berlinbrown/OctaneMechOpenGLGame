--
--
module Tests.GLTests where

import HUnit

foo :: Int -> (Int, Int)
foo x = (1, x)

exampleTestUtilOne :: Int -> IO (Int, Int)
exampleTestUtilOne v = return (v+2, v+3)

exampleTestUtilTwo :: Int -> IO Bool
exampleTestUtilTwo v = return (v > 5)

-----------------------------
-- Test Cases
-----------------------------

-- Will Give an Error
test1 = TestCase (assertEqual "Test Case - call foo 3):" 
								(1,2) (foo 3))

test2 = TestCase (do (x,y) <- exampleTestUtilOne 3
                     assertEqual "Test Case - for the first result of [TestOne]: " 5 x
                     b <- exampleTestUtilTwo y
                     assertBool ("Test Case - [TestTwo]: " ++ show y ++ " failed") b)

tests = TestList [	TestLabel "Test 1" test1, 
					TestLabel "Test 2" test2 ]
