#ifndef __PLL_H__
#define __PLL_H__

#include "main.h"
#ifndef MFANG
    #include <stdio.h>
#endif

void SelRCHFToPLL(uint32_t rchf, uint32_t clock);
void SelXTHFToPLL(uint32_t clock);

#endif
