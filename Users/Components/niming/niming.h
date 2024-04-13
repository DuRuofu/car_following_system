#ifndef __NIMING_H_
#define __NIMING_H_
#include "main.h"

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、int32等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp) (*(char *)(&dwTemp))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))
//void	ANO_DT_Send_F1(uint16 _a, uint16 _b, uint16 _c, uint16 _d)；
void ANO_DT_Send_F2(int16_t _a,int16_t _b,int16_t _c,int16_t _d);
#endif
