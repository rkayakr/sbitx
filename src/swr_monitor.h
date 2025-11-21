#ifndef SWR_MONITOR_H
#define SWR_MONITOR_H

// VSWR threshold and status
extern float max_vswr;
extern int vswr_tripped;

// Main VSWR monitoring function
// vswr parameter uses project convention: vswr = SWR * 10 (e.g., 30 == 3.0)
void check_and_handle_vswr(int vswr);

// Reset VSWR trip status and clear UI
void reset_vswr_tripped(void);

#endif // SWR_MONITOR_H
