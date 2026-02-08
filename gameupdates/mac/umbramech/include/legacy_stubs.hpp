/**
 * Legacy stubs and compatibility helpers for missing symbols.
 */
#pragma once

#include <cstddef>

#ifndef MED_RED
#define MED_RED glColor3f(0.9f, 0.2f, 0.2f)
#endif

#ifndef MED_GREEN
#define MED_GREEN glColor3f(0.2f, 0.9f, 0.2f)
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
#define GET_NORMAL ((void)0)
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
