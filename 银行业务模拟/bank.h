#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <conio.h> 

#define MONEY 5000 //单次业务交易金额上限
#define OK 1 
#define ERROR 0 
#define OVERFLOW -1

typedef struct {     //队列1元素 
	int ArriveTime; //到达时间 
	int OccurTime;  //事件发生时间 
	int Flag;       //存取钱标志 
	int DealTime;   //办理业务时间 
	int money;      //交易金额 
}Event1, ElemType1;

typedef struct {     //队列2元素 
	int ArriveTime;  //到达时间
	int DealTime;    //办理业务时间
	int money;       //交易金额
}Event2, ElemType2;

typedef struct QNode1 {  //队列1结点类型 
	ElemType1 data;      //数据域
	struct QNode1 *next;
}QNode1, *Queue1;

typedef struct QNode2 {  //队列2结点类型 
	ElemType2 data;      //数据域
	struct QNode2 *next;
}QNode2, *Queue2;

typedef struct {  //链队列类型 
	Queue1 front; //队头 
	Queue1 rear;  //队尾 
}LinkQueue1;

typedef struct {  //链队列类型
	Queue2 front; //队头
	Queue2 rear;  //队尾
}LinkQueue2;
