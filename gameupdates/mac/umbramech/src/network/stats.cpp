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
// stats.c
//
// berlin _dot__ brown  __at_ g_mail _ dot_ com
//
// - simple networking stats
//
// Notice: I used long names for functions
// I dont want to use very often
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>

#include "network.h"
#include "include/msg.h"

static struct timeval start_time;
static struct timeval end_time;
static double packet_time;


struct sockaddr *client_addie = NULL;

//
// start_net_time
//
void Start_Net_Time(void)
{
  gettimeofday(&start_time, NULL);
  
} // end of the function 

//
// End_Net_Time
//
void End_Net_Time(void)
{
 

  // record response
  gettimeofday(&end_time, NULL);

  end_time.tv_sec -= start_time.tv_sec;
  end_time.tv_usec -= start_time.tv_usec;
  
  if (end_time.tv_usec < 0L)
    {

      end_time.tv_usec += 1000000L;
      end_time.tv_sec -= 1;

    } // end of the if 

  // This may be off...I am sleepy
  packet_time = (double)end_time.tv_sec + 
    ((double)end_time.tv_usec/1000000.0L);

  packet_time *= 1000.0L;

} // end of the function 

//
// Print_Net_Time
void Print_Net_Time(void)
{
  printf("64byte packet: %0.3fms\n", packet_time);
} // end of the if

//
// Set_SockAddr

//
// Send_Net_Packet
//
void Send_Net_Packet(int sock, int serv_sock)
{
  int res;
  char buffer[80];
  int h;

  memset(buffer, 0xff, 64*sizeof(char));
  buffer[64] = '\0';
  
        // send array of data
#if defined(UDP_PROTOCOL)
  
  res = strlen(buffer);
  
      h = sendto(serv_sock, buffer, res, 0, 
	     (struct sockaddr *)client_addie, sizeof(struct sockaddr));

      if (h < 0)
	{
	  fprintf(stderr, "sendto() packet: failed %d err: %d\n", res, h);
	} // end of if 

#else
      res = strlen(buffer);
      send(sock, buffer, res, 0);
#endif      

      // start the race......
      Start_Net_Time();
      

} // end of func

//
// Recv_Net_Packet
void Recv_Net_Packet(int sock, int serv_sock)
{
  struct sockaddr *client_addr;
  int len;
  char buffer[80];
  int res;

  

#if defined(UDP_PROTOCOL)

      len = sizeof(client_addr);
      
      res = recvfrom(serv_sock, buffer, 80, 0,
		     (struct sockaddr *)&client_addr, &len);
#else 
      // get the message      
      res = recv(sock, buffer, 80, 0);      
#endif
      

      End_Net_Time();
      Print_Net_Time();


} // end of the functuion
