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
// connect.c
// 
// - the entry point for the networking code
//
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>              // used for _control


#include "../keys.h"
#include "include/connect.h"
#include "include/clients.h"
#include "network.h"

//
// The network message will keep the user informed
// of what is going on at the bottom of the screen
//
static char network_msg[80] = "Use TAB to switch network modes";
static int _cur_mode = -1;


//
// Super - FUNCTION
// - must be called in main.c(glant.c)
//
void Super_InitNetwork(void)
{
  
  Snapshot_StartTime();
  
  Create_Client_List();
  Create_Client();        // simple inits

} // end of the function 

//
// Super_DeleteNetwork
//
void Super_DeleteNetwork(void)
{
  Delete_Client_List();
  Kill_Client();
  Kill_Server();

} // end of the function 

//
// Get_NetworkMsg
//
void Get_NetworkMsg(char *buffer)
{

  strcpy(buffer, network_msg);

} // end of the function

//
// Set_NetworkMsg(char *buffer)
//
void Set_NetworkMsg(char *buffer)
{
  strcpy(network_msg, buffer);
} // end of func

// we really should use final_str more
//


//
// Start_Service()
// - the only input we need is the string
// with the parameters for starting the service
//
// mode: client or server
void Start_Service(char final_str[4][80], int mode)
{
  char buffer[80];

  strcpy(buffer, final_str[1]);  // get the connect str

  switch(mode)
    {
    case SEL_CLIENT_MODE:

      _cur_mode = SEL_CLIENT_MODE;

      // connect to server, this is so fun!x
      Connect_Server(buffer);
      Set_NetworkMsg("Connecting to Server");
      break;

    case SEL_SERVER_MODE:

      // At this point, the user wants
      // to start a server
      // first get the current ip
      // the user should know already, but it doesnt hurt
      Get_LocalAddress();
      Set_NetworkMsg("Getting Local IP Address");

      _cur_mode = SEL_SERVER_MODE;

      // starting server
      Launch_Server();
      //Kill_Server();

      Set_NetworkMsg("Launching Server");
      break;
      
    default: break;
    };

} // end of the function 

//
// Print_NetRun
// - print if the network is running or not
//
void Print_NetRun(void)
{
  if (_cur_mode == SEL_CLIENT_MODE)
    {
      Print_ClientRun();
    } else if (_cur_mode == SEL_SERVER_MODE) {
      
      Print_ServerRun();
      
    } // end of the if

} // end of the function 
