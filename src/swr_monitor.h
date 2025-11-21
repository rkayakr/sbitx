#ifndef SWR_MONITOR_H
#define SWR_MONITOR_H

#include <stdio.h>

/*
 * max_vswr is in real SWR units (e.g. 3.0 means 3.0:1)
 * The repository uses integer vswr values equal to (SWR * 10).
 */
extern float max_vswr;
extern int vswr_tripped;

/* Check the integer 'vswr' (SWR*10) and take protective actions if needed */
void check_and_handle_vswr(int vswr);

/* Reset any tripped state and clear UI */
void reset_vswr_tripped(void);

#endif // SWR_MONITOR_H
