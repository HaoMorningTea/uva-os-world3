/*
for nes0 (image in-kernel), minimum functions. only gfx, no input control
-------------
Principle: minimum OS requirement. NO dependency on:
    -- thread (CLONE_VM, spinlock, semaphore, etc)
    -- non-blocking IO (O_NONBLOCK) 
    -- SDL_xxx (libminisdl) functions (hence directly writes to /dev/fb)

Known issues: minor flickring & glitches at the canvas top (TBD)

---- Below: comment from NJU OS project ---- 
https://github.com/NJU-ProjectN/LiteNES/blob/master/src/hal.c

This file presents all abstractions needed to port LiteNES.
  (The current working implementation uses allegro library.)

To port this project, replace the following functions by your own:
1) nes_hal_init()
    Do essential initialization work, including starting a FPS HZ timer.

2) nes_set_bg_color(c)
    Set the back ground color to be the NES internal color code c.

3) nes_flush_buf(*buf)
    Flush the entire pixel buf's data to frame buffer.

4) nes_flip_display()
    Fill the screen with previously set background color, and
    display all contents in the frame buffer.

5) wait_for_frame()
    Implement it to make the following code is executed FPS times a second:
        while (1) {
            wait_for_frame();
            do_something();
        }
*/
#include "hal.h"
#include "fce.h"
#include "common.h"
#include "../user.h"

static int cur_id = 0; // id of fb being rendered to, 0 or 1 (if FB_FLIP==1)

static char *vtx = 0;  // byte-to-byte mirror of hw fb (inc row padding - pitch)
static int vtx_sz = 0; // in bytes

/* Wait until next timer event is fired. */
void wait_for_frame()
{
#ifdef NES_PROFILE  // print fps every sec, to stdout
    // as wait_for_frame() is called every frame. we can measure the actual FPS
    static int fps = 0;     // measured fps
    static int total=0, nframes=0, last=0;
    int t0 = uptime();  // ms

    total += (t0 - last); nframes ++; last = t0; 

    if (total > 1000) { // print every ~1sec 
        fps = 1000/(total/nframes); 
        printf("%s: FPS %d\n", __func__, fps);
        nframes = total = 0; 
    }
#endif

    // render as fast as possible
    // in the future, can lock FPS via sleep() 

    /* project idea: fork here, so that we have mutiple mario on screen.
    processes will have to adjus their fb offset. e.g. the parent can still keep
    the original lcoations, while child shifts  */

    /* Project idea: support graceful exit. For a special key press (e.g. 'q'),
    returns from this func and goes back to fce_run which further exits the
    program but before that, need to notify the timer task & event task to quit
    too; the main task shall wait() them to quit.    */
}

// id: fb 0 or 1
static inline void setpixel(int id, char *buf, int x, int y, int pit, PIXEL p) {
    assert(x>=0 && y>=0); 
    y += (id*SCREEN_HEIGHT);
    *(PIXEL *)(buf + y*pit + x*PIXELSIZE) = p; 
}

static inline PIXEL getpixel(int id, char *buf, int x, int y, int pit) {
    assert(x>=0 && y>=0); 
    y += (id*SCREEN_HEIGHT);
    return *(PIXEL *)(buf + y*pit + x*PIXELSIZE); 
}

#define fcecolor_to_pixel(color) \
(((char)color.r<<16)|((char)color.g<<8)|((char)color.b))

/* Set background color to vtx. RGB value of c is defined in fce.h */
void nes_set_bg_color(int c)
{
    // int pitch = dispinfo[PITCH];
    int pitch = cfg.pitch;
    PIXEL p = fcecolor_to_pixel(palette[c]);
    for (int y = 0; y < SCREEN_HEIGHT; y++) 
        for (int x = 0; x < SCREEN_WIDTH; x++)
            setpixel(cur_id,vtx,x,y,pitch,p); 
}

/* Flush the pixel buffer. Materialize nes's drawing (unordered pixels) to fb.
         this lays out pixels in memory (in hw format)
   ver1: draw to an app fb, which is to be write to /dev/fb in one shot
   ver2: draw to a "back" fb (which will be made visible later)

    fb basics
    https://github.com/fxlin/p1-kernel/blob/master/docs/exp3/fb.md */
// quest: mario
void nes_flush_buf(PixelBuf *buf) {
    int pitch = cfg.pitch; //in bytes
    
    for (int i = 0; i < buf->size; i ++) {
        Pixel *p = &buf->buf[i];
        int x = p->x, y = p->y;
        pal color = palette[p->c];    
        PIXEL c = fcecolor_to_pixel(color); /* TODO: replace this */

        // Pixel could have coorindates x<0 (looks like fce shifts drawn
        //  pixels by applying offsets to them). These pixels shall be
        //  invisible on fb. 
        assert(x<SCREEN_WIDTH && y>=0 && y<SCREEN_HEIGHT);
        if (x>=0)
            setpixel(cur_id,vtx,x,y,pitch,c); /* TODO: replace this */
    }
}

/* Initialization: (1) start a 1/FPS Hz timer. (2) create tasks that produce
    timer/kb events & dispatch them to the main task over pipes */
void nes_hal_init() {
    assert(cfg.vwidth >= SCREEN_WIDTH);
    assert(cfg.vheight >= SCREEN_HEIGHT);

    vtx_sz = cfg.pitch * cfg.vheight; /* TODO: replace this */
    vtx = malloc(vtx_sz);
    if (!vtx) {printf("failed to alloc vtx\n"); exit(1);}
    printf("fb alloc ...ok\n"); 
}

/* Update screen at FPS rate by drawing function. 
   Timer ensures this function is called FPS times a second. */
// quest: mario
void nes_flip_display()
{
    assert(vtx && vtx_sz); 
    // printf("draw...\n");    
    memmove(cfg.fb,vtx,vtx_sz); /* TODO: replace this */
}

/* Query a button's state. b: the button idx. 
   Returns 1 if button #b is pressed. */
int nes_key_state(int b)
{
    return 0;  
}

