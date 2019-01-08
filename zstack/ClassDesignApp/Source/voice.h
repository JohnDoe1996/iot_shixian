#ifndef __VOICE_H__
#define __VOICE_H__

#include "OSAL.h"
#include "hal_mcu.h"
#include <zcomdef.h>

#define voice P0_1

uint8 getVoice(void);

#endif