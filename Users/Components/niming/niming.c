#include "niming.h"
#include "main.h"
#include "usart.h"

uint8_t data_to_send[100];



//void ANO_DT_Send_F1(uint16 _a, uint16 _b, uint16 _c, uint16 _d)   //F1֡  4�� uint16 ����
//{
//    uint8 _cnt = 0;
//    uint8 sumcheck = 0; //��У��
//    uint8 addcheck = 0; //���Ӻ�У��
//    uint8 i=0;
//    data_to_send[_cnt++] = 0xAA;
//    data_to_send[_cnt++] = 0xFF;
//    data_to_send[_cnt++] = 0xF1;
//    data_to_send[_cnt++] = 8; //���ݳ���
//    data_to_send[_cnt++] = BYTE1(_a);
//    data_to_send[_cnt++] = BYTE0(_a);
//	
//    data_to_send[_cnt++] = BYTE1(_b);
//    data_to_send[_cnt++] = BYTE0(_b);
//	
//    data_to_send[_cnt++] = BYTE1(_c);
//    data_to_send[_cnt++] = BYTE0(_c);
//	
//    data_to_send[_cnt++] = BYTE1(_d);
//    data_to_send[_cnt++] = BYTE0(_d);

//	
//	  for ( i = 0; i < data_to_send[3]+4; i++)
//    {
//        sumcheck += data_to_send[i];
//        addcheck += sumcheck;
//    }

//    data_to_send[_cnt++] = sumcheck;
//    data_to_send[_cnt++] = addcheck;
//		
//		HAL_UART_Transmit(&huart3,data_to_send,_cnt,0xFFFF); //���߼ǵ��޸Ĵ��ڷ��ͺ���
//}




void ANO_DT_Send_F2(int16_t _a, int16_t _b, int16_t _c, int16_t _d)   //F2֡  4��  int16 ����
{
    uint8_t _cnt = 0;
    uint8_t sumcheck = 0; //��У��
    uint8_t addcheck = 0; //���Ӻ�У��
    uint8_t i=0;
   data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xFF;
    data_to_send[_cnt++] = 0xF2;
    data_to_send[_cnt++] = 8; //���ݳ���
    data_to_send[_cnt++] = BYTE0(_a);
    data_to_send[_cnt++] = BYTE1(_a);
	
    data_to_send[_cnt++] = BYTE0(_b);
    data_to_send[_cnt++] = BYTE1(_b);
	
    data_to_send[_cnt++] = BYTE0(_c);
    data_to_send[_cnt++] = BYTE1(_c);
	
    data_to_send[_cnt++] = BYTE0(_d);
    data_to_send[_cnt++] = BYTE1(_d);
	
	  for ( i = 0; i < data_to_send[3]+4; i++)
    {
        sumcheck += data_to_send[i];
        addcheck += sumcheck;
    }

    data_to_send[_cnt++] = sumcheck;
    data_to_send[_cnt++] = addcheck;
		
	HAL_UART_Transmit(&huart3,data_to_send,_cnt,0xFFFF);//���߼ǵ��޸Ĵ��ڷ��ͺ���
}
