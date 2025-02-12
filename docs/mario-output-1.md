## After being able to launch the program, before its rendering is done
````bash
xzl@FelixLin-XPS15 (main)[p1-kernel-lab3]$ ./run-rpi3qemu.sh 
------ kernel boot ------  core 0
VA_START ffff000000000000
build time (kernel.c) Nov 10 2024 22:10:25
phys mem: 00000000 -- 3f000000
         kernel img: 00080000 -- 00126000
         paging mem: 00126000 -- 3b900000
                 959M 245722 pages
         malloc mem: 3b900000 -- 3c100000
                 8M
         reserved for framebuffer: 3c100000 -- 3f000000
mbox.c:379 OK. fb pa: 0x3c100000 w 640 h 640 vw 640 vh 640 pitch 2560 isrgb 0
timer.c:139 cycles_per_us 507 cycles_per_ms 507614
kernel.c:120 entering init
Kernel process started at EL 1, pid 1
sched.c:742 5767 bytes to load from img
vm.c:618 demand paging at user va 0x7fffff0, elr 0x90
It's me, Mario!
arg 0: nes0
arg 1: 0x3c100000
arg 2: 640
arg 3: 640
arg 4: 2560
arg 5: 0
arg 6: 0
load rom...ok
fb alloc ...ok
running fce ...
vm.c:618 demand paging at user va 0xab7000, elr 0x5c98
vm.c:618 demand paging at user va 0xab8400, elr 0x5c98
vm.c:618 demand paging at user va 0xab9000, elr 0x5c98
(... there could be memory faults below ...)
````

## After the rendering runs OK

````bash
xzl@FelixLin-XPS15 (main)[p1-kernel-lab3]$ ./run-rpi3qemu.sh 
------ kernel boot ------  core 0
VA_START ffff000000000000
build time (kernel.c) Nov 10 2024 22:13:30
phys mem: 00000000 -- 3f000000
         kernel img: 00080000 -- 00126000
         paging mem: 00126000 -- 3b900000
                 959M 245722 pages
         malloc mem: 3b900000 -- 3c100000
                 8M
         reserved for framebuffer: 3c100000 -- 3f000000
mbox.c:379 OK. fb pa: 0x3c100000 w 640 h 640 vw 640 vh 640 pitch 2560 isrgb 0
timer.c:139 cycles_per_us 450 cycles_per_ms 452488
kernel.c:120 entering init
Kernel process started at EL 1, pid 1
sched.c:742 5767 bytes to load from img
vm.c:618 demand paging at user va 0x7fffff0, elr 0x90
It's me, Mario!
arg 0: nes0
arg 1: 0x3c100000
arg 2: 640
arg 3: 640
arg 4: 2560
arg 5: 0
arg 6: 0
load rom...ok
fb alloc ...ok
running fce ...
wait_for_frame: FPS 25
wait_for_frame: FPS 31
wait_for_frame: FPS 28
....
````
