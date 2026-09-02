#ifdef MYCONFIG
#include "myconfig.h"
#ifdef MYRADIOCHAN
	#define DEFAULT_RF_CHANNEL MYRADIOCHAN
	#define default_addr myradiotxaddr
#endif
#else
#define MYRADIOCHAN		52						//Default radio channel
uint8_t myradiotxaddr[] = {0x30, 0x10, 0x00, 0x00, 0x52};	//Default address with MSB first
 
#endif