----------------------------------------
{-  Module      : Octane
    Copyright   : BSD License
    License     : 
    Maintainer  : Berlin Brown
    Stability   : 
    Portability : 
    Description : Octane Web Server
    TODO        : 
-}
----------------------------------------

module ProxyServer (launchServer) where

import System.Exit ( exitWith, ExitCode(..) )

import Prelude hiding (catch)	
import Network (listenOn, accept, sClose, Socket,
                 withSocketsDo, PortID(..))
import System.IO
import System.Environment (getArgs)
import Control.Exception (finally, catch)
import Control.Concurrent 
import Control.Concurrent.STM
import Control.Monad (forM, filterM, liftM, when)

import Data.Char

import System.Log.Logger
import System.Log.Handler.Syslog 

trim :: String -> String
trim = f . f where f = reverse . dropWhile isSpace

launchServer = withSocketsDo $ do
                 -- Copy everything to syslog
                 s <- openlog "SyslogOctane.log" [PID] USER DEBUG
                 updateGlobalLogger rootLoggerName (addHandler s)
                 updateGlobalLogger "Octane" (setLevel DEBUG)
                 let port = fromIntegral (9980 :: Int)
                 servSock <- listenOn $ PortNumber port
                 
                 infoM "Octane" $ "INFO: listening on " ++ show port
                 start servSock `finally` sClose servSock

start servSock = do
  acceptChan <- atomically newTChan
  forkIO $ acceptLoop servSock acceptChan
  mainLoop servSock acceptChan []

type Client = (TChan String, Handle)

acceptLoop :: Socket -> TChan Client -> IO ()
acceptLoop servSock chan = do
  (cHandle, host, port) <- accept servSock
  cChan <- atomically newTChan
  cTID  <- forkIO $ clientLoop cHandle cChan
  atomically $ writeTChan chan (cChan, cHandle)
  acceptLoop servSock chan

clientLoop :: Handle -> TChan String -> IO ()
clientLoop handle chan =
    listenLoop (hGetLine handle) chan
                   `catch` (const $ return ())
                   `finally` hClose handle 

listenLoop :: IO a -> TChan a -> IO ()
listenLoop act chan = 
    sequence_ (repeat (act >>= atomically . writeTChan chan))

--
-- Function to exit out of the server system, 
-- invoked when a ":quit" command is sent
exitCurrentServer :: String -> IO ()
exitCurrentServer msg = do
	putStrLn "Shutting down server..."
	putStrLn msg
	exitWith ExitSuccess
	return ()
	
mainLoop :: Socket -> TChan Client -> [Client] -> IO ()
mainLoop servSock acceptChan clients = do
    r <- atomically $ (Left `fmap` readTChan acceptChan)
                      `orElse`
                      (Right `fmap` tselect clients)
    case r of
          Left (ch,h)    -> do
               putStrLn "From Server: New client connection"
               mainLoop servSock acceptChan $ (ch,h):clients
          Right (line,_) -> do
               putStrLn $ "Data from client: " ++ line
               if ((trim line) == ":quit") then exitCurrentServer ("msg:Client invoked quit command") else putStr ""
               
               clients' <- forM clients $ 
                            \(ch,h) -> do 
                                hPutStrLn h ("from client: " ++ line)
                                hFlush h
                                return [(ch,h)] 
                            `catch` const (hClose h >> return [])
               let dropped = length $ filter null clients'
               when (dropped > 0) $ 
                   putStrLn ("clients lost: " ++ show dropped)
               mainLoop servSock acceptChan $ concat clients'

tselect :: [(TChan a, t)] -> STM (a, t)
tselect = foldl orElse retry
          . map (\(ch, ty) -> (flip (,) ty) `fmap` readTChan ch)
                
-- End of Source

