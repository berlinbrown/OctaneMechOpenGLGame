"""Exercise the actual pause/camera handlers without a GLUT window."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]

def function(path, signature):
    source = (root / path).read_text()
    start = source.index(signature)
    opening = source.index('{', start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]

constants = []
for header in ('menu.hpp', 'bot.hpp'):
    constants += [line for line in (root / 'include' / header).read_text().splitlines()
                  if line.startswith('#define ') and any(line.split()[1] == name for name in
                  ('MENU_TITLE_MODE', 'MENU_RUN_MODE', 'MENU_HELP_MODE', 'FIRST_TIME_TRUE',
                   'FIRST_TIME_FALSE', 'PLAYER_0', 'DEAD_STATE', 'ALIVE_STATE',
                   'THIRD_PERSON_MODE', 'CLOSE_THIRD_MODE', 'FIRST_PERSON_MODE', 'DEMO_MODE'))]
constants += [line for line in (root / 'include/fireants.hpp').read_text().splitlines()
              if line.startswith('constexpr int PLAYER_0 =')]
source = '\n'.join(constants) + '''
#include <cassert>
#include <cstdlib>
struct Globals { int paused, menu_mode, _menu_state; } globals;
Globals* ant_globals = &globals;
struct Bot { int id, alive, view_mode; } bot;
Bot* camera_bot = &bot;
'''
source += function('src/main.cpp', 'static void HandlePause(void)')
source += function('src/camera.cpp', 'void ToggleViewMode(void)')
source += function('src/main.cpp', 'static void HandleEsc(void)')
source += '''
static bool gKeys[512] = {}, gGameReady = true;
static float newYPos = 0, newXPos = 0, gCamYawDeg = 0, gCamDistance = 40;
static int menuActivations = 0;
void PrintCurrentCameraView() {}
void ToggleAudioMute() {}
bool Set_MenuMode() { ++menuActivations; return false; }
'''
source += function('src/main.cpp', 'static float ClampFloat(')
source += function('src/main.cpp', 'static void KeyDown(')
source += function('src/main.cpp', 'static void KeyUp(')
source += '''
int main() {
  globals = {1, MENU_TITLE_MODE, FIRST_TIME_TRUE};
  bot = {PLAYER_0, ALIVE_STATE, CLOSE_THIRD_MODE};
  ToggleViewMode(); assert(bot.view_mode == FIRST_PERSON_MODE);
  assert(globals.paused == 1 && globals.menu_mode == MENU_TITLE_MODE);
  ToggleViewMode(); assert(bot.view_mode == DEMO_MODE);
  ToggleViewMode(); assert(bot.view_mode == THIRD_PERSON_MODE);
  ToggleViewMode(); assert(bot.view_mode == CLOSE_THIRD_MODE);
  HandlePause(); assert(!globals.paused && globals.menu_mode == MENU_RUN_MODE);
  assert(globals._menu_state == FIRST_TIME_FALSE);
  HandlePause(); assert(globals.paused);
  HandlePause(); assert(!globals.paused);
  globals = {1, MENU_HELP_MODE, FIRST_TIME_FALSE};
  HandlePause(); assert(!globals.paused && globals.menu_mode == MENU_RUN_MODE);
  // Lost key-up: a stale held bit must not prevent the next P press resuming.
  globals = {1, MENU_TITLE_MODE, FIRST_TIME_TRUE};
  gKeys['p'] = true;
  KeyDown('p', 0, 0); assert(!globals.paused && globals.menu_mode == MENU_RUN_MODE);
  KeyDown('p', 0, 0); assert(globals.paused);
  KeyDown('p', 0, 0); assert(!globals.paused);
  KeyUp('p', 0, 0); assert(!gKeys['p']);
  KeyDown('P', 0, 0); assert(globals.paused);
  KeyDown(13, 0, 0); assert(!globals.paused && menuActivations == 0);
  KeyDown(27, 0, 0); assert(globals.paused && globals.menu_mode == MENU_TITLE_MODE);
  KeyDown(27, 0, 0); assert(!globals.paused);
  globals = {1, MENU_TITLE_MODE, FIRST_TIME_TRUE};
  KeyDown(27, 0, 0); assert(!globals.paused);
  globals = {1, MENU_TITLE_MODE, FIRST_TIME_TRUE};
  KeyDown(13, 0, 0); assert(menuActivations == 1);
  bot.view_mode = CLOSE_THIRD_MODE;
  gKeys[9] = true;
  KeyDown(9, 0, 0); assert(bot.view_mode == FIRST_PERSON_MODE && globals.paused);
  camera_bot = nullptr; ToggleViewMode();
  ant_globals = nullptr; HandlePause();
}
'''
with tempfile.TemporaryDirectory(prefix='umbra-input-') as directory:
    cpp = Path(directory) / 'input.cpp'
    binary = Path(directory) / 'input'
    cpp.write_text(source)
    subprocess.run(['clang++', '-std=c++17', str(cpp), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
print('Pause and camera handler checks passed.')
