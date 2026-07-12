/**
 * Legacy stubs and compatibility helpers for missing symbols.
 */
#pragma once

#include <cstddef>
#include <cmath>
#include "DriverBots.hpp"

#ifndef MED_RED
#define MED_RED glColor3f(0.9f, 0.2f, 0.2f)
#endif

#ifndef MED_GREEN
#define MED_GREEN glColor3f(0.62f, 0.62f, 0.62f)
#endif

#ifndef MED_BLUE
#define MED_BLUE glColor3f(0.2f, 0.2f, 0.9f)
#endif

#ifndef MED_PURPLE
#define MED_PURPLE glColor3f(0.6f, 0.2f, 0.7f)
#endif

#ifndef MED_YELLOW
#define MED_YELLOW glColor3f(0.9f, 0.9f, 0.2f)
#endif

#ifndef MED_CYAN
#define MED_CYAN glColor3f(0.2f, 0.9f, 0.9f)
#endif

#ifndef CLR_0
#define CLR_0 glColor3f(0.7f, 0.7f, 0.7f)
#endif
#ifndef CLR_1
#define CLR_1 glColor3f(0.3f, 0.6f, 0.9f)
#endif
#ifndef CLR_2
#define CLR_2 glColor3f(0.9f, 0.5f, 0.2f)
#endif
#ifndef CLR_3
#define CLR_3 glColor3f(0.2f, 0.9f, 0.6f)
#endif

#ifndef GET_NORMAL
inline void Compute_Normal_From_Vertices(const float vertices[3][3], float normal[3])
{
  const float ux = vertices[1][0] - vertices[0][0];
  const float uy = vertices[1][1] - vertices[0][1];
  const float uz = vertices[1][2] - vertices[0][2];

  const float vx = vertices[2][0] - vertices[0][0];
  const float vy = vertices[2][1] - vertices[0][1];
  const float vz = vertices[2][2] - vertices[0][2];

  normal[0] = (uy * vz) - (uz * vy);
  normal[1] = (uz * vx) - (ux * vz);
  normal[2] = (ux * vy) - (uy * vx);

  const float length = std::sqrt((normal[0] * normal[0]) + (normal[1] * normal[1]) +
                                 (normal[2] * normal[2]));
  if (length > 0.0f)
  {
    normal[0] /= length;
    normal[1] /= length;
    normal[2] /= length;
  }
}

#define GET_NORMAL Compute_Normal_From_Vertices(v, n)
#endif

#ifndef MOVE_FORWARD
#define MOVE_FORWARD 0x01
#endif
#ifndef MOVE_BACKWARD
#define MOVE_BACKWARD 0x02
#endif
#ifndef MOVE_TURNLEFT
#define MOVE_TURNLEFT 0x04
#endif
#ifndef MOVE_TURNRIGHT
#define MOVE_TURNRIGHT 0x08
#endif

struct Msg {
  int id;
  int object_id;
  float x;
  float y;
  float pos_x;
  float pos_y;
  float heading;
  int type;
};

#ifndef DWORD
using DWORD = unsigned long;
#endif

inline void Do_FireSound() {}
inline void Build_MoveMsg(float, float, float, int) {}
inline Msg* Get_Messages(int* count, int* objectid)
{
  if (count) *count = 0;
  if (objectid) *objectid = 0;
  return nullptr;
}
inline void Reset_MessageCount() {}
inline void Perform_Snapshots(void) {}
inline void Display_NetSel(char (*)[80]) {}
inline void Get_NetworkMsg(char*) {}
inline void printConnections() {}
inline void Print_NetRun() {}

#ifndef N_0
#define N_0 ((void)0)
#endif
#ifndef N_1
#define N_1 ((void)0)
#endif
#ifndef N_2
#define N_2 ((void)0)
#endif
#ifndef N_3
#define N_3 ((void)0)
#endif

DriverBotPtr CreateBot(int bot_id);
void LoadBotParms(DriverBotPtr bot_ptr);
void ResetBot(DriverBotPtr bot_ptr);
void DestroyBot(DriverBotPtr bot_ptr);
void Generate_Command(DriverBotPtr bot, int cmd);
