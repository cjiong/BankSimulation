#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <conio.h> 

#define MONEY 5000 //����ҵ���׽������
#define OK 1 
#define ERROR 0 
#define OVERFLOW -1

typedef struct {     //����1Ԫ�� 
	int ArriveTime; //����ʱ�� 
	int OccurTime;  //�¼�����ʱ�� 
	int Flag;       //��ȡǮ��־ 
	int DealTime;   //����ҵ��ʱ�� 
	int money;      //���׽�� 
}Event1, ElemType1;

typedef struct {     //����2Ԫ�� 
	int ArriveTime;  //����ʱ��
	int DealTime;    //����ҵ��ʱ��
	int money;       //���׽��
}Event2, ElemType2;

typedef struct QNode1 {  //����1������� 
	ElemType1 data;      //������
	struct QNode1 *next;
}QNode1, *Queue1;

typedef struct QNode2 {  //����2������� 
	ElemType2 data;      //������
	struct QNode2 *next;
}QNode2, *Queue2;

typedef struct {  //���������� 
	Queue1 front; //��ͷ 
	Queue1 rear;  //��β 
}LinkQueue1;

typedef struct {  //����������
	Queue2 front; //��ͷ
	Queue2 rear;  //��β
}LinkQueue2;
