#include "voice.h"


uint8 getVoice(void)
{
    uint8 result;
    result = (voice == 0)? 1 : 0;
    return result;
}