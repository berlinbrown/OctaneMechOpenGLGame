/**
 * Copyright (c) 2006-2011 Berlin Brown.  All Rights Reserved
 *
 * http://www.opensource.org/licenses/bsd-license.php
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of the Botnode.com (Berlin Brown) nor
 * the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Date: 8/15/2011
 *
 * Description: Simple OpenGL Mech Game
 *
 * Contact: Berlin Brown <berlin dot brown at gmail.com>
 */


//
// Berlin Brown
// bigbinc@hotmail.com
//
// play.c
//
#include <malloc.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/soundcard.h>
#include <sys/ioctl.h>

#include "sound.h"

Sample snd[NUM_SAMPLES];
char *audio_dev = "/dev/dsp";
int audiofd;
int playingBeam, playingMusic;

int noSound = 0;
int playingTech;
int musicCount;
int soundOn;
int musicOn;


//
// Kill_Sounds
//
void Kill_Sounds(void)
{

  Snd_restore();

} // end of the fnuction 

//
// Turn_SoundOff
//
void Turn_SoundOff(void)
{
  soundOn = 0;
} //  end of the function 

//
// Turn on
//
void Turn_SoundOn(void)
{
  soundOn = 1;
} // end of the function 

//
// Toggle_Sound(void0
//
void Toggle_Sound(void)
{
  soundOn = !soundOn;
} // end of the fnction 

//
// get Sound On
//
int Get_SoundOn(void)
{

  return soundOn;

} // end of the function 

//
// Turn_MusicOff
//
void Turn_MusicOff(void)
{
  musicOn = 0;
} // end of the functino

//
// Turn_MusicOn
//
void Turn_MusicOn(void)
{
  musicOn = 1;
} // end of the function 

//
// Toggle Music
//
void Toggle_Music(void)
{
  musicOn = !musicOn;

} // end of func

//
// Get Music
//
int Get_MusicOn(void)
{
  return musicOn;
} // end of the function 


//
// Load_Audio
//
void Load_Audio(void)
{
#if SOUND_TURN_ON
  struct stat st;

  // load audio
  playingTech = 0;
  musicCount = 0;

  noSound = 1;
  soundOn = 1;
  musicOn = 1;

  if (stat(audio_dev, &st) >= 0) 
    {
      audiofd = open(audio_dev, O_WRONLY);

      if (audiofd >= 0)
	{
	  noSound = 0;
	  close(audiofd);
	  
	} // end of the if 

    } // end of the if 

  if (!noSound)
    {
      
      // load all the sound effects into memory
      Snd_loadRawSample("misc/tech.raw", &snd[0]);
      Snd_loadRawSample("misc/1.raw", &snd[1]);

    } // end of th eif 

  if (Snd_init(NUM_SAMPLES, snd, SAMPLE_RATE, NUM_CHANNELS, audio_dev) == EXIT_FAILURE)
    {

      fprintf(stderr, "Cannot load sound library\n");      
      return;

    } // end of the if 
#endif
  
} // end of the function 

//
// flushSounds
//
void flushSounds(void)
{
  playingTech = 0;

  playingMusic--;

  if (playingMusic < 0)
    {
      playingMusic = 0;
    } // end of the if 

} // end of the functino 

//
// outAudio
//
void outAudio(int sCounter)
{
  if ((sCounter < 0) || (sCounter >= NUM_SAMPLES))
    return;
  
  Snd_effect(sCounter, sCounter);
  
} // end of the functino 

//
// doSound
//
void doSound(int theSound)
{
  if (!noSound)
    {
      
    } // end of the if 

} // end of the function 

//
// Do_FireSound
//
void Do_FireSound(void)
{

#if SOUND_TURN_ON
  outAudio(0);
#endif

} // end of the fnuctino 

//
// Play_Music
//
void Play_Music(void)
{

#if SOUND_TURN_ON
  outAudio(1);
#endif

} // end of the function 
