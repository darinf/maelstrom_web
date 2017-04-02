
/* Game Logic interface routines and variables */

#include "netlogic.h"

/* From logic.cpp */
extern void LogicUsage(void);
extern int  InitLogicData(void);
extern int  LogicParseArgs(const char ***argvptr, int *argcptr);
extern int  InitLogic(void);
extern int  InitPlayerSprites(void);
extern void HaltLogic(void);
extern void SetControl(unsigned char which, int toggle);
extern int  SpecialKey(SDL_keysym key);
extern int GetScore(void);

/* From game.cpp */
extern void NewGame_Init(void);
extern void NewGame_Iteration(void);
extern void NewGame_Shutdown(void);

/* From about.cpp */
extern void DoAbout(void);

/* From blit.cpp (fastlogic) player.cpp (netlogic) */
extern Uint8 gPlayerShotColors[];
extern SDL_Surface *gPlayerShot;
extern Uint8 gEnemyShotColors[];
extern SDL_Surface *gEnemyShot;

