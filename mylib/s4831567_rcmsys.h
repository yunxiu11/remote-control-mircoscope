#ifndef __S4831567_RCMSYS_H
#define __S4831567_RCMSYS_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

void s4831567_rcmsys_init(void);
void rcmsys_task(void);

#endif
