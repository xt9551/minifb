#include <MiniFB.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


#define kUnused(var)    (void) var

#define WIDTH      1024
#define HEIGHT     600
static unsigned char g_buffer[WIDTH * HEIGHT*3];
static bool         g_active = true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <time.h>
typedef struct
{
    unsigned int tv_sec;
    unsigned int tv_usec;
}timeval_minifb;


timeval_minifb minifb_start;

static void minifb_gettimeofday(timeval_minifb *currTime)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    currTime->tv_sec = clock;
    currTime->tv_usec = wtm.wMilliseconds * 1000;
}

static unsigned int minifb_GetTimePassed(timeval_minifb startTime)
{

    timeval_minifb endTime;
    unsigned int diff;
    minifb_gettimeofday(&endTime);

    diff = 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec / 1000 - startTime.tv_usec / 1000;
    // printf("time used: %d uS\n",diff);
    return diff;
}

static void
active(struct mfb_window *window, bool isActive) {
    const char *window_title = "";
    if(window) {
        window_title = (const char *) mfb_get_user_data(window);
    }
    fprintf(stdout, "%s > active: %d\n", window_title, isActive);
    g_active = isActive;
}

static void
resize(struct mfb_window *window, int width, int height) {
    uint32_t x = 0;
    uint32_t y = 0;
    const char *window_title = "";
    if(window) {
        window_title = (const char *) mfb_get_user_data(window);
    }

    fprintf(stdout, "%s > resize: %d, %d\n", window_title, width, height);
//    if(width > WIDTH) {
//        x = (width - WIDTH) >> 1;
//        width = WIDTH;
//    }
//    if(height > HEIGHT) {
//        y = (height - HEIGHT) >> 1;
//        height = HEIGHT;
//    }
    mfb_set_viewport(window, x, y, width, height);
}

static bool
close(struct mfb_window *window) {
    const char* window_title = "";
    if (window) {
        window_title = (const char*)mfb_get_user_data(window);
    }
	fprintf(stdout, "%s > close\n", window_title);
    return true;    // true => confirm close
                    // false => don't close 
}

static void
keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool isPressed) {
    const char *window_title = "";
    if(window) {
        window_title = (const char *) mfb_get_user_data(window);
    }
    fprintf(stdout, "%s > keyboard: key: %s (pressed: %d) [key_mod: %x]\n", window_title, mfb_get_key_name(key), isPressed, mod);
    if(key == KB_KEY_ESCAPE) {
        mfb_close(window);
    }
}

static void
char_input(struct mfb_window *window, unsigned int charCode) {
    const char *window_title = "";
    if(window) {
        window_title = (const char *) mfb_get_user_data(window);
    }
    fprintf(stdout, "%s > charCode: %d\n", window_title, charCode);
}

static void
mouse_btn(struct mfb_window *window, mfb_mouse_button button, mfb_key_mod mod, bool isPressed) {
    const char *window_title = "";
    int x, y;
    if(window) {
        window_title = (const char *) mfb_get_user_data(window);
    }
    x = mfb_get_mouse_x(window);
    y = mfb_get_mouse_y(window);
    fprintf(stdout, "%s > mouse_btn: button: %d (pressed: %d) (at: %d, %d) [key_mod: %x]\n", window_title, button, isPressed, x, y, mod);
}

static void
mouse_move(struct mfb_window *window, int x, int y) {
    kUnused(window);
    kUnused(x);
    kUnused(y);
    const char *window_title = "";
    if(window) {
        window_title = mfb_get_user_data(window);
    }
    fprintf(stdout, "%s > mouse_move: %d, %d\n", window_title, x, y);
}

static void
mouse_scroll(struct mfb_window *window, mfb_key_mod mod, float deltaX, float deltaY) {
    const char *window_title = "";
    if(window) {
        window_title = (const char *) mfb_get_user_data(window);
    }
    fprintf(stdout, "%s > mouse_scroll: x: %f, y: %f [key_mod: %x]\n", window_title, deltaX, deltaY, mod);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int
main()
{
    int noise, carry, seed = 0xbeef;
    timeval_minifb minifb_start;
    struct mfb_window *window = mfb_open_ex("Input Events Test", WIDTH, HEIGHT, WF_RESIZABLE);
    if (!window)
        return 0;
    fprintf(stdout, " window: %x\n", (unsigned int )window);
    mfb_set_active_callback(window, active);
    mfb_set_resize_callback(window, resize);
    mfb_set_close_callback(window, close);
    mfb_set_keyboard_callback(window, keyboard);
    mfb_set_char_input_callback(window, char_input);
    mfb_set_mouse_button_callback(window, mouse_btn);
//    mfb_set_mouse_move_callback(window, mouse_move);
//    mfb_set_mouse_scroll_callback(window, mouse_scroll);

    mfb_set_user_data(window, (void *) "Input Events Test");
    float scalex,scaley;
    mfb_get_monitor_scale(window,&scalex,&scaley);
    fprintf(stdout, "scale: %f %f\n",scalex,scaley);
    do {
        int              i;
        mfb_update_state state;

        if(g_active)
        {
            memset(g_buffer,0x0,WIDTH*HEIGHT*3);
            for (i = 0; i < WIDTH * HEIGHT*3; i+=3)
            {
//                noise = seed;
//                noise >>= 3;
//                noise ^= seed;
//                carry = noise & 1;
//                noise >>= 1;
//                seed >>= 1;
//                seed |= (carry << 30);
//                noise &= 0xFF;
//                g_buffer[i] = MFB_RGB(noise, noise, noise);
               if((i%6)==0)
               {
                   g_buffer[i] = 0xff;
                   g_buffer[i+1] = 0xff;
                   g_buffer[i+2] = 0xff;
               }

            }
//          memset(g_buffer,0x55,WIDTH*HEIGHT*4);
//            fprintf(stdout, "window: %x\n", (unsigned int )window);

//            minifb_gettimeofday(&minifb_start);
            state = mfb_update(window, g_buffer);
//            fprintf(stdout, "t=%d\n", minifb_GetTimePassed(minifb_start));

        }
        else {
            state = mfb_update_events(window);
        }
        if (state != STATE_OK) {
            window = 0x0;
            break;
        }
    } while(mfb_wait_sync(window));

    return 0;
}
