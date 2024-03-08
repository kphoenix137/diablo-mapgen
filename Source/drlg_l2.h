/**
 * @file drlg_l2.h
 *
 * Interface of the catacombs level generation algorithms.
 */
#ifndef __DRLG_L2_H__
#define __DRLG_L2_H__

void LoadL2Dungeon(const char *sFileName, int vx, int vy);
void LoadPreL2Dungeon(const char *sFileName, int vx, int vy);
int CreateL2Dungeon(DWORD rseed, int entry, bool breakOnSuccess);

#endif /* __DRLG_L2_H__ */
