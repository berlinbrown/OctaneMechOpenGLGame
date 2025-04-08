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
#include <sys/wait.h>

#include "sound.h"

struct Mix
{
  unsigned char *Vclippedbuf;
  int *Vunclipbuf;
  int Vsize;
};
typedef struct Mix Mix;

struct Channel
{
  unsigned char *Vstart, *Vcurrent; // ptr's into a playing sample
  int Vlen;                         // length of sample in bytes
  int Vleft;                        // bytes left of sample to play
  int Snum;
};
typedef struct Channel Channel;

static int sampleMixerStatus = 0;

static const Sample *S_sounds = NULL;
static int S_num_sounds = 0;
static int S_fd_snddev = -1;
static int S_fd_pipe[2] = {-1, -1};
static int S_son_pid = -1;
static const char *S_snddev = NULL;
static int S_num_channels = 6;
static int S_playback_freq = 0;

int Snd_init_dev();
int Snd_restore_dev();
void Chan_reset(Channel *chan);
void Chan_assign(Channel *chan, const Sample *snd, int sound_num);
int Chan_mixAll(Mix *mix, Channel *ch);
int Chan_copyIn(Channel *chan, Mix *mix);
int Chan_mixIn(Channel *chan, Mix *mix);

int Chan_finalMixIn(Channel *chan, Mix *mix);

void Chan_resetSound(Channel *chan, int nr);

void Mix_alloc(Mix *mix, int size);
void Mix_dealloc(Mix *mix);

//
// dump_snd_list
//
void dump_snd_list()
{
  int i = 0;

  for (i = 0; i < S_num_sounds; i++)
  {
    printf("snd %d: len = %d \n", i, S_sounds[i].len);
  }
} // end of func

//
// Snd_Init
//
int Snd_init(int num_snd, const Sample *sa, int frequency,
             int channels, const char *dev)
{
  int result;

  S_num_sounds = num_snd;
  S_sounds = sa; /* array of sound samples*/
  S_playback_freq = frequency;
  S_num_channels = channels;
  S_snddev = dev; /* sound device, eg /dev/dsp*/

  if (S_sounds == NULL)
    return EXIT_FAILURE;

  result = Snd_init_dev();

  if (result == EXIT_SUCCESS)
  {
    sampleMixerStatus = 1;
  }
  else
  {
    sampleMixerStatus = 0;
  }

  return result;

} // end of  the function

//
// Snd_restore
//
int Snd_restore()
{
  int result;

  if (!sampleMixerStatus)
    return EXIT_FAILURE;

  result = Snd_restore_dev();

  if (result == EXIT_SUCCESS)
  {
    sampleMixerStatus = 0;
  }
  else
  {
    sampleMixerStatus = 0;
  }

  return result;
}

//
// sndeffect
//
int Snd_effect(int sound_num, int channel)
{
  if (!sampleMixerStatus)
    return EXIT_FAILURE;

  if (S_sounds[sound_num].data != NULL)
  {
    write(S_fd_pipe[1], &sound_num, sizeof(sound_num));
    write(S_fd_pipe[1], &channel, sizeof(channel));
  }
  else
    fprintf(stderr, "Referencing NULL sound entry\n");

  return EXIT_SUCCESS;

} // end of the function

//
// Snd init dev
//
int Snd_init_dev()
{
  int whoami;
  S_fd_snddev = -1;

  S_son_pid = 0;

  if (access(S_snddev, W_OK) != 0)
  {
    perror("No access to sound device");
    return EXIT_FAILURE;
  }

  S_fd_snddev = open(S_snddev, O_WRONLY);

  if (S_fd_snddev < 0)
  {
    fprintf(stderr, "int_snddev: Cannot open sound device \n");
    return EXIT_FAILURE;
  }

  close(S_fd_snddev);

  if (pipe(S_fd_pipe) < 0)
  {
    fprintf(stderr, "Cannot create pipe for sound control \n");
    return EXIT_FAILURE;
  }

  if ((whoami = fork()) < 0)
  {
    fprintf(stderr, "Cannot fork sound driver\n");
    return EXIT_FAILURE;
  }

  if (whoami != 0)
  {
    close(S_fd_pipe[0]);
    S_son_pid = whoami;
    return EXIT_SUCCESS;
  }

  /* Here is the code for the son... */
  {
    int sound_num, ch, i;
    struct timeval tval = {0L, 0L};
    fd_set readfds, dsp;

    Mix mix;

    int frag, fragsize;

    Channel *chan = (Channel *)malloc(sizeof(Channel) * S_num_channels);

    for (i = 0; i < S_num_channels; i++)
      Chan_reset(chan + i);

    S_fd_snddev = open(S_snddev, O_WRONLY);
    if (S_fd_snddev < 0)
    {
      perror("Cannot open sound device: ");
      exit(1);
    }

    frag = FRAG_SPEC;

    ioctl(S_fd_snddev, SNDCTL_DSP_SETFRAGMENT, &frag);

    if (ioctl(S_fd_snddev, SNDCTL_DSP_SPEED, &S_playback_freq) == -1)
      perror("Sound driver ioctl ");

    fragsize = 0;
    if (ioctl(S_fd_snddev, SNDCTL_DSP_GETBLKSIZE, &fragsize) == -1)
      perror("Sound driver ioctl ");

    // init mixer
    Mix_alloc(&mix, fragsize);

    close(S_fd_pipe[1]);

    FD_ZERO(&dsp);
    FD_SET(S_fd_snddev, &dsp);

    FD_ZERO(&readfds);
    FD_SET(S_fd_pipe[0], &readfds);

    printf("Sound driver initialized.\n");

    for (;;)
    {
      FD_SET(S_fd_pipe[0], &readfds);
      tval.tv_sec = 0L;
      tval.tv_usec = 0L;
      select(S_fd_pipe[0] + 1, &readfds, NULL, NULL, &tval);

      if (FD_ISSET(S_fd_pipe[0], &readfds))
      {
        if (read(S_fd_pipe[0], &sound_num, sizeof(int)) == 0)
          break;

        read(S_fd_pipe[0], &ch, sizeof(int));

        /* printf("chan=%d snd=%d len=%d \n", ch, sound_num, S_sounds[sound_num].len ); */
        /* Find free channel for sample */
        if (ch == -1)
        {
          printf("Reset %d\n", sound_num);
          Chan_resetSound(chan, sound_num);
        }
        else
          for (i = 0; i < S_num_channels; i++)
          {
            if (chan[i].Vleft == 0)
            {
              Chan_assign(&(chan[i]), &(S_sounds[sound_num]), sound_num);
              break;
            }
          }
      }

      Chan_mixAll(&mix, chan);
      write(S_fd_snddev, mix.Vclippedbuf, fragsize);
    }

    Mix_dealloc(&mix);
    printf("Sound process exiting..\n");
    close(S_fd_pipe[0]);
    close(S_fd_pipe[1]);
    exit(0);
  }

} // end of the function

//
// Snd Restore
//
int Snd_restore_dev()
{
  close(S_fd_pipe[0]);
  close(S_fd_pipe[1]);

  /* wait for child process to die*/
  wait(NULL);
  return EXIT_SUCCESS;
} // end of the function

//
//   CHANNEL MIXING FUNCTIONS
//
void Chan_reset(Channel *chan)
{
  chan->Vstart = NULL;
  chan->Vcurrent = NULL;
  chan->Vlen = 0;
  chan->Vleft = 0;
}

//
// Chan assign
void Chan_assign(Channel *chan, const Sample *snd, int sound_num)
{
  chan->Vstart = snd->data;
  chan->Vcurrent = chan->Vstart;
  chan->Vlen = snd->len;
  chan->Vleft = snd->len;
  chan->Snum = sound_num;
}

/*==========================================================================*/
int Chan_copyIn(Channel *chan, Mix *mix)
{
  int i, *p = mix->Vunclipbuf, result, min;

  result = (chan->Vleft > 0) ? 1 : 0;
  min = (chan->Vleft < mix->Vsize) ? chan->Vleft : mix->Vsize;

  for (i = 0; i < min; i++)
  {
    *p++ = (int)*chan->Vcurrent++;
  }
  chan->Vleft -= i;

  /* fill the remaining (if any) part of the mix buffer with silence */
  while (i < mix->Vsize)
  {
    *p++ = 128;
    i++;
  }
  return result;

} // end of the functnio

//
// Chan_mixIn
//
int Chan_mixIn(Channel *chan, Mix *mix)
{
  int i, *p = mix->Vunclipbuf, result, min;

  result = (chan->Vleft > 0) ? 1 : 0;
  min = (chan->Vleft < mix->Vsize) ? chan->Vleft : mix->Vsize;

  for (i = 0; i < min; i++)
  {
    *p++ += (int)(*chan->Vcurrent++) - 128;
  }

  chan->Vleft -= i;
  return result;

} // end of function

//
// clip
//
static inline unsigned char clip(int i)
{
  return (i < 0) ? 0 : ((i > 255) ? 255 : i);
} // end of the function

//
// Chan_Finalmix
//
int Chan_finalMixIn(Channel *chan, Mix *mix)
{
  register int i;
  int *p = mix->Vunclipbuf, result, min;
  unsigned char *final = mix->Vclippedbuf;

  result = (chan->Vleft > 0) ? 1 : 0;
  min = (chan->Vleft < mix->Vsize) ? chan->Vleft : mix->Vsize;

  for (i = 0; i < min; i++)
  {
    *p += (int)(*chan->Vcurrent++) - 128;
    *final++ = clip(*p++);
  }
  chan->Vleft -= i;

  /* copy rest of Vunclipbuf over to Vclippedbuf */
  while (i < mix->Vsize)
  {
    *final++ = clip(*p++);
    i++;
  }

  return result;
}

//
// mix alloc
void Mix_alloc(Mix *mix, int size)
{
  mix->Vclippedbuf = (unsigned char *)calloc(sizeof(char), size);
  mix->Vunclipbuf = (int *)calloc(sizeof(int), size);
  mix->Vsize = size;

  if ((mix->Vclippedbuf == NULL) || (mix->Vunclipbuf == NULL))
  {
    fprintf(stderr, "Unable to allocate memory for mixer buffer\n");
    exit(-1);
  } // end of the if

} // end of the functino

//
// Mix dealloc
//
void Mix_dealloc(Mix *mix)
{
  if (mix->Vclippedbuf)
    free(mix->Vclippedbuf);
  if (mix->Vunclipbuf)
    free(mix->Vunclipbuf);
} // end of cuntino

// Mixes together the channels into one sound.
//   Returns # of channels currently playing *any* sound
//   Therefore, return 0 means to channels have a sample, therefore no
//   sound is playing
int Chan_mixAll(Mix *mix, Channel *chan)
{
  int result = 0, i = 0;

  result = Chan_copyIn(chan, mix);

  for (i = 2; i < S_num_channels; i++)
  {

    result += Chan_mixIn(++chan, mix);
  }

  result += Chan_finalMixIn(++chan, mix);

  return result;

} // end of function

//
// reseting sound
//
void Chan_resetSound(Channel *chan, int nr)
{
  int i;
  printf("kill here  ");

  for (i = 0; i < S_num_channels; i++)
  {
    if (chan->Snum == nr)
    {
      Chan_reset(chan);
      printf("KILLL OK!!!\n");
    }
    chan++;
  }

} // end of unction

//
// Snd_LoadRawSample
//
int Snd_loadRawSample(const char *file, Sample *sample)
{
  FILE *fp;

  sample->data = NULL;
  sample->len = 0;

  fp = fopen(file, "r");

  if (fp == NULL)
    return -1;

  sample->len = lseek(fileno(fp), 0, SEEK_END);
  lseek(fileno(fp), 0, SEEK_SET);

  sample->data = (unsigned char *)malloc(sample->len);

  if (sample->data == NULL)
  {
    fclose(fp);
    return -2;
  }

  fread(sample->data, 1, sample->len, fp);

  fclose(fp);

  return 0;

} // end of function
