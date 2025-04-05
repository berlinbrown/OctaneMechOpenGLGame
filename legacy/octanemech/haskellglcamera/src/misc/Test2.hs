import Graphics.Rendering.OpenGL
import Graphics.UI.GLUT
import System.Exit ( exitWith, ExitCode(..) )

myPoints :: [(GLfloat,GLfloat,GLfloat)]
myPoints = map (\k -> (sin(2*pi*k/12),cos(2*pi*k/12),0.0)) [1..12]

main = do
  (progname, _) <- getArgsAndInitialize
  createWindow "Hello World"
  displayCallback $= display
  mainLoop

keyPressed :: KeyboardMouseCallback
keyPressed (Char '\27') Down _ _ = exitWith ExitSuccess
keyPressed _            _    _ _ = return ()

display = do
  -- register the function called when the keyboard is pressed.
  keyboardMouseCallback $= Just keyPressed

  clear [ColorBuffer]
  renderPrimitive Points $ mapM_ (\(x, y, z)->vertex$Vertex3 x y z) myPoints
  flush