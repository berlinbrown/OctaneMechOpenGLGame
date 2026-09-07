#import <AVFoundation/AVFoundation.h>
#include <mach-o/dyld.h>
#include <filesystem>
#include <cstdio>
#include "audio.hpp"

static AVAudioPlayer *music, *motor;
static AVAudioPlayer *shots[8];
static unsigned nextShot = 0;
static bool muted = false, isPaused = true;
static std::filesystem::path audioDirectory;

static AVAudioPlayer* Load(const char* name, bool loop, float volume)
{
  const auto path = audioDirectory / name;
  NSError* error = nil;
  AVAudioPlayer* player = [[AVAudioPlayer alloc]
    initWithContentsOfURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:path.c_str()]]
    error:&error];
  if (!player) { fprintf(stderr, "Audio unavailable: %s (%s)\n", name, [[error localizedDescription] UTF8String]); return nil; }
  player.numberOfLoops = loop ? -1 : 0;
  player.volume = volume;
  [player prepareToPlay];
  return player;
}

void InitAudio()
{
  @autoreleasepool {
    char executable[4096]; uint32_t size = sizeof(executable);
    if (_NSGetExecutablePath(executable, &size) == 0)
      audioDirectory = std::filesystem::path(executable).parent_path() / "resources/audio";
    if (!std::filesystem::exists(audioDirectory / "music.mp3"))
      audioDirectory = std::filesystem::path(UMBRA_RESOURCE_DIR) / "audio";
    music = Load("music.mp3", true, 0.12f);
    motor = Load("motor.wav", true, 0.20f);
    for (auto& shot : shots) shot = Load("shoot.wav", false, 0.45f);
    [music play];
  }
}

void UpdateAudio(bool paused, bool moving)
{
  isPaused = paused;
  music.volume = muted ? 0.0f : (paused ? 0.08f : 0.16f);
  if (!muted && !paused && moving) { if (!motor.playing) [motor play]; }
  else [motor pause];
  if (paused || muted) for (auto shot : shots) [shot stop];
}

void PlayShot()
{
  if (muted || isPaused) return;
  AVAudioPlayer* shot = shots[nextShot++ % 8];
  [shot stop]; shot.currentTime = 0; [shot play];
}

void ToggleAudioMute() { muted = !muted; }
