# **SO Shell Terminal** ![version](https://img.shields.io/badge/version-v0.4-yellow.svg) ![nvm version](https://img.shields.io/badge/tests-passed-green.svg)
<!-- ![nvm version](https://img.shields.io/badge/tests-3%20passed%20/%202%20failed-red.svg) -->


## **Introduction**
Expansion of the Minix system. Main goal is to implement new server that simulates barriers
for incomming processes. If barrier reaches its capacity limit then releases all processes.
It can be used to synchronize all processes.


## **Last changes**
- clean code

(everything works. No issues. Ready to use)

## **Features**
- server `barriers` has been created
- commands `barrier_int`, `barrier_destroy` and `barrier_wait` has been added and fully implemented
- communication between `User Space` and `Server Space`
- completed system with errnos
- communication between `PM` and `BS`
- releasing blocked process from barrier if signal
- remove process from barrier if it ended / exited
- release all barriers if server closes
- full implementation of `/proc/barriers`

## **Changed / Created files**
### UserSpace
- `/src/include/barriers.h`- header for user commands
- `/src/include/Makefile` - header added
- `/src/lib/libc/sys-minix/barriers.c` - script for user commands (`main`) 
- `/src/lib/libc/sys-minix/Makefile.inc` - script added

### ServerSpace
- `/src/servers/Makefile` - makefile for servers - `BS` added
- `/src/servers/pm/const.h` - string const added
- `/src/servers/pm/proto.h` - `tell_bs` added
- `/src/servers/pm/signal.c` - `tell_bs` call addded
- `/src/servers/pm/forkexit.c` - `tell_bs` call added
- `/src/servers/pm/utility.c` - `tell_bs` implementation
- `/src/servers/procfs/Makefile` - `barriers` added t
- `/src/servers/procfs/root.c` - `barriers` added to array
- `/src/servers/procfs/barriers.h` - header for barriers.c
- `/src/servers/procfs/barriers.c` - implementation of `/proc/barriers`
- `/src/include/minix/com.h` - added `barriers server (BS)` section
- `/src/servers/barriers/Makefile` - makefile with installation
- `/src/servers/barriers/barriers.c` - implementation of all user commands on server side
- `/src/servers/barriers/barriers.conf` - server config
- `/src/servers/barriers/inc.h` - main header with all functions
- `/src/servers/barriers/barrier.h` - header with barrier struct
- `/src/servers/barriers/main.c` - main logic (inicialization and signal management)

### Rest
- `/src/kernel/table.c` - `BS` added
- `/src/servers/rs/table.c` - `BS` added

(failed atttempt to add `BS` to rest of servers)


## **Installation and Launch**
### How to clone and install
1. Get image of minix system (`3.2v`) - `Minix_3.2`
2. Install image on `Oracle VM VirtualBox`
3. Clone repository (`git@github.com:ITAnalyst-JU/so-minix-barrierserver-PiJayson.git`)
4. Copy all files to `Virtual Box`

### How to use
- **NONE**


## **Issues**
- **NONE**


## **ToDo:**
- **NONE**