module Main where
import Prelude hiding (catch)

import Network (connectTo, withSocketsDo, PortID(..))
import System.IO
import System.IO.Error (isEOFError)
import System.Environment (getArgs)
import Control.Exception (finally, catch, Exception(..))
import Control.Concurrent 
import Control.Concurrent.STM

main = withSocketsDo $ do 
    [host, portStr] <- getArgs
    let port = fromIntegral (read portStr :: Int)
    sock <- connectTo host $ PortNumber port
    start sock `catch` handler `finally` hClose sock 
        where 
            handler (IOException e)
                    | isEOFError e = return ()
            handler e = putStrLn $ show e
            
start :: Handle -> IO ()
start sock = do 
    netChan  <- atomically newTChan
    userChan <- atomically newTChan
    netTID   <- spawn $ listenLoop (hGetLine sock) netChan
    userTID  <- spawn $ listenLoop getLine userChan
    mainLoop sock netChan userChan

spawn :: IO () -> IO ThreadId
spawn act = do
    mainTID <- myThreadId
    forkIO $ act `catch` throwTo mainTID

listenLoop :: IO a -> TChan a -> IO ()
listenLoop act chan = 
    sequence_ (repeat (act >>= atomically . writeTChan chan))

---Recursive version
---listenLoop = do
---    v <- action
---    atomically $ writeTChan chan v
---    listenLoop action chan

mainLoop :: Handle -> TChan String -> TChan String -> IO ()
mainLoop sock netChan userChan = do
    input <- atomically $ select netChan userChan
    case input of
          -- from netChan, to user
          Left str  -> putStrLn str >> hFlush stdout
          -- from userChan, to net
          Right str -> hPutStrLn sock str >> hFlush sock
    mainLoop sock netChan userChan

select :: TChan a ->  TChan b -> STM (Either a b) 
select ch1 ch2 = do 
                 a <- readTChan ch1; return (Left a) 
                 `orElse` do 
                 b <- readTChan ch2; return (Right b)
-- End of File                 

    