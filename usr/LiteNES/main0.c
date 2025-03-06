/*

for nes0 (image in-kernel), minimum functions. only gfx, no input control

---- Below: comment from NJU OS project ---- 

LiteNES originates from Stanislav Yaglo's mynes project:
  https://github.com/yaglo/mynes

LiteNES is a "more portable" version of mynes.
  all system(library)-dependent code resides in "hal.c" and "main.c"
  only depends on libc's memory moving utilities.

How does the emulator work?
  1) read file name at argv[1]
  2) load the rom file into array rom
  3) call fce_load_rom(rom) for parsing
  4) call fce_init for emulator initialization
  5) call fce_run(), which is a non-exiting loop simulating the NES system
  6) when SIGINT signal is received, it kills itself (TBD)
*/

#include "fce.h"   
#include "common.h"   
#include "../user.h"

#define stderr 2

#include "mario-rom.h"    // built-in rom buffer with Super Mario

struct config cfg; 

// expected args: from exec
// arg0 - prog name (convention)
// arg1 - user VA for mapped fb
// arg2 - fb.vwidth
// arg3 - fb.vheight
// arg4 - fb.pitch
// arg5 - offsetx
// arg6 - offsety
// has to check against SCREEN_HEIGHT, SCREEN_WIDTH
//quest: mario
int main(int argc, char *argv[])
{
    printf("got to main0\n");
    for (int i=0; i<argc; i++)
      printf("arg %d: %s\n", i, argv[i]); // dbg

    //unsigned long fb = strtol(argv[1], 0, 16); /* TODO: replace this */ doesn't work because no libc
    // we don't have sscanf/atol16 (no libc). 
    // we expect config.fb is around 0x3c00_0000, so it SHOULD not overflow
    // sanity check 
    unsigned long fb = 0;
    char *s = argv[1];
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;

    for (; *s; s++) {
        fb *= 16;
        if (*s >= '0' && *s <= '9') 
            fb += (*s - '0');
        else if (*s >= 'a' && *s <= 'f')
            fb += (*s - 'a' + 10); // ✅ Fixed here
        else if (*s >= 'A' && *s <= 'F')
            fb += (*s - 'A' + 10); // ✅ Fixed here
        else {
            fprintf(stderr, "Invalid hex char: %c\n", *s);
            exit(1);
        }
    }

    //printf("fb = 0x%lx\n", fb);

    assert(fb > 0x30000000 && fb < 0x40000000); 
    cfg.fb = (char *)fb; 
    /* TODO: your code here */
    cfg.vwidth = atoi(argv[2]);
    cfg.vheight = atoi(argv[3]);
    cfg.pitch = atoi(argv[4]);
    cfg.offsetx = atoi(argv[5]);
    cfg.offsety = atoi(argv[6]);
        
    if (fce_load_rom(rom) != 0) { // will load the built-in rom
        fprintf(stderr, "Invalid or unsupported rom.\n");
        exit(1);
    }
    printf("load rom...ok\n"); 

    fce_init();

    printf("running fce ...\n"); 
    
    // project idea: can test stdin. "press a key to start..."
    fce_run();
    return 0;
}
