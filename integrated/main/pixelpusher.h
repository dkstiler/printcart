#define PP_NYANCAT 0
#define PP_COLOR 1
#define PP_MONA 2
#define PP_MINE 3
#define PP_MONA_IMU 4
#define PP_WHEE 5

//shift fixed points to right by this to get int val
#define FP_SHIFT 16


void pixel_pusher_init();
void pixel_pusher_set_speed_pos(int x, int y, int vx, int vy, int ena);
void pixel_pusher_set_type(int type);
void pixel_pusher_color_set(int r, int g, int b, int pos);
void pixel_pusher_mona_set_stripe(int stripe);