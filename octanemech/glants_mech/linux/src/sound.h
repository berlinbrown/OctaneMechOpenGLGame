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

#ifndef _SOUND_H_
#define _SOUND_H_

#include <stdlib.h>
#include <stdio.h>

#define FRAG_SPEC 0x00020007
#define SOUND_TURN_ON         1 

#define NUM_SAMPLES          2
#define SAMPLE_RATE          11000
#define NUM_CHANNELS         8

struct Sample
    {
    unsigned char *data;	
    int len;			
    };

typedef struct Sample Sample;


int
Snd_loadRawSample( const char *file, Sample *sample );


//
// init sound device, etc..                                                 
// num_snd  = the number of samples in the sample array *sa                 
// sa       = the sample array						    
// freq     = the rate (Hz) to play back the samples                        
// channels = # of channels to mix                                          
// sound_device = a char string for the sound device, eg, "/dev/dsp"        
// returns: 0=success, -1=failure.
//
int Snd_init( int num_snd, const Sample *sa, int freq, 
          int channels, const char *sound_device );

int 
Snd_restore();

int 
Snd_effect( int nr, int channel );

void flushSounds(void);
void outAudio(int sCouner);
void Load_Audio(void);

void Do_FireSound(void);
void Play_Music(void);
	
#endif
