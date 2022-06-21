#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // thread
#include <pthread.h>    // 

#include "render.h"
#include "event.h"
#include "utility.h"

int main(int, char**);

extern int running;

#endif 