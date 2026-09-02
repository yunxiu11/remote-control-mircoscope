#ifndef __RCMSYS_H
#define __RCMSYS_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

void rcmsys_init(void);
void rcmsys_task(void);

#endif
