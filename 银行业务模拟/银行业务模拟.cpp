// 银行业务模拟.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <conio.h> 
#include "bank.h"
 

int TotalMoney = 10000; //银行现有资金总额
int CloseTime = 600;    //银行营业时间  
int TotalTime;          //客户逗留总时间，用来计算平均值
int TimeNow;            //当前时间，判断是否超过营业时间
int number;             //业务办理次序
int TimeUse;            //每位顾客所用时间 
int money;              //每位顾客办理的金额数 
int INTERTIME;          //客户到达的时间间隔 
int DEALTIME;           //业务办理所需的时间 

LinkQueue1 Q1;
LinkQueue2 Q2;
Event1 ev;
Event2 ew;

//初始化队列1 
int InitQueue1() {
	Q1.front = Q1.rear = (Queue1)malloc(sizeof(QNode1));
	if (!Q1.front) 
		exit(OVERFLOW);
	Q1.front->next = NULL;
	return OK;
}

//初始化队列 2 
int InitQueue2() {
	Q2.front = Q2.rear = (Queue2)malloc(sizeof(QNode2));
	if (!Q2.front)
		exit(OVERFLOW);
	Q2.front->next = NULL;
	return OK;
}

//销毁队列1
int DestroyQueue1() {		
	while (Q1.front) {
		Q1.rear = Q1.front->next;
		free(Q1.front);
		Q1.front = Q1.rear;
	}
	return OK;
}

//销毁队列2 
int DestroyQueue2() {      
	while (Q2.front) {
		Q2.rear = Q2.front->next;
		free(Q2.front);
		Q2.front = Q2.rear;
	}
	return OK;
}

//删除Q1的队头元素，用ev返回
int DeQueue1() {		 
	Queue1 p; 
	if (Q1.front == Q1.rear) 
		return ERROR;
	p = Q1.front->next;
	ev.ArriveTime = p->data.ArriveTime;
	ev.OccurTime = p->data.OccurTime;
	ev.Flag = p->data.Flag;
	ev.DealTime = p->data.DealTime;
	ev.money = p->data.money;
	Q1.front->next = p->next;
	if (Q1.rear == p) 
		Q1.rear = Q1.front;	//当Q1只有一个元素时 
	free(p);   //释放结点
	return OK;
}

//删除Q2的队头元素，用ew返回
int DeQueue2() {			 
	Queue2 p;
	if (Q2.front == Q2.rear) 
		return ERROR;
	p = Q2.front->next;
	ew.ArriveTime = p->data.ArriveTime;
	ew.DealTime = p->data.DealTime;
	ew.money = p->data.money;
	Q2.front->next = p->next;
	if (Q2.rear == p) 
		Q2.rear = Q2.front;  //当Q2只有一个元素时
	free(p);    //释放结点
	return OK;
}

//Q1入队操作
int EnQueue1() {		 
	Queue1 p, r, t;
	t = NULL;
	p = (Queue1)malloc(sizeof(QNode1));  //分配存储空间
	if (!p)    
		exit(OVERFLOW);             //分配失败
	p->data.ArriveTime = ev.ArriveTime;
	p->data.OccurTime = ev.OccurTime;
	p->data.Flag = ev.Flag;
	p->data.DealTime = ev.DealTime;
	p->data.money = ev.money;
	r = Q1.front->next;
	while (r) {   //r非空
		if (p->data.ArriveTime < r->data.ArriveTime) {
			if (r == Q1.front->next) {
				p->next = r;
				Q1.front->next = p;
			}
			else{
				t->next = p;
				p->next = r;
			}
		}
		t = r;
		r = r->next;
	}
	if (!r) {  //r为空
		if (Q1.front->next == NULL) {
			Q1.front->next = p;
			Q1.rear = p;
			Q1.rear->next = NULL;
		}
		else{
			p->next = NULL;
			Q1.rear->next = p;
			Q1.rear = p;
		}
	}
	return OK;
}

//Q2入队操作
int EnQueue2() {		
	Queue2 p;
	p = (Queue2)malloc(sizeof(QNode2)); //分配存储空间
	if (!p)
		exit(OVERFLOW);     //分配失败
	p->data.ArriveTime = ew.ArriveTime;
	p->data.DealTime = ew.DealTime;
	p->data.money = ew.money;
	p->next = NULL;
	Q2.rear->next = p;
	Q2.rear = p;
	return OK;
}

//营业时间到，关闭银行，清空队列
void CloseBank() {  
	DestroyQueue1();
	DestroyQueue2();
}

//产生随机数据
int rand_1(int *intertime, int *Flag, int *DealTime, int *money) {    
	*intertime = rand() % INTERTIME + 1;  //下个客户到达的时间间隔
	*Flag = rand() % 2;                   //存款或者取款
	*DealTime = rand() % DEALTIME + 1;    //客户办理业务所需时间
	*money = rand() % MONEY + 1;          //客户办理业务的交易金额
	return OK;
}

//初始化操作
void InitBank() {      
	printf("	请输入最大到达时间间隔 (1~600) ：");
	scanf_s("%d", &INTERTIME);     
	printf("	请输入最大的处理时间 (1~600) ：");
	scanf_s("%d", &DEALTIME);
	TotalTime = 0;		 
	number = 0;		     
	InitQueue1();   //初始化队列 1 
	InitQueue2();	//初始化队列 2 
	ev.ArriveTime = 0;
	ev.OccurTime = 0;
	ev.Flag = 0;
	ev.money = 0;
	ev.DealTime = 0;
	EnQueue1();  //事件进入队列
}

//
void CustomerArrived() {
	Queue1 p;
	int intertime, i = 0;
	TimeNow = ev.OccurTime;
	rand_1(&intertime, &(ev.Flag), &(ev.DealTime), &(ev.money));		 //初始化客户数据
	ev.Flag++;
	p = Q1.front->next;
	while (p != NULL) {
		if (p->data.Flag != 0)
			i = p->data.OccurTime; //获取时间
		p = p->next;
	}
	if (i == 0)		//第一位客户 
		ev.OccurTime = ev.ArriveTime + ev.DealTime;
 	else if (i >= ev.ArriveTime)		//本事件到达时上一事件还未结束 
		ev.OccurTime = i + ev.DealTime;		 //此事件的离开时间=上一事件的离开时间+本事件处理时间 
	else				//上一事件离开之后，本事件才到达 
		ev.OccurTime = ev.ArriveTime + ev.DealTime;		//此事件的离开时间=本事件到达时间+本事件处理时间 
	EnQueue1();			//下一用户到达事件插入队列 1  
	ev.ArriveTime = ev.ArriveTime + intertime;		//下一客户到达时间 
	ev.OccurTime = ev.ArriveTime;
	ev.Flag = 0; //暂时赋值 
	ev.money = 0;
	ev.DealTime = 0;
	EnQueue1();
}

//检查Q2中是否有客户已经达到可以处理其所需业务
int CheckQ2() {   
	int i = 0, j, z = 0;
	Queue2 p;
	p = Q2.front->next;
	while (p) {          //Q2的长度
		i++;
		p = p->next;
	} 
	for (j = 1; j <= i; j++) {
		DeQueue2();
		if (ew.money <= TotalMoney) //若Q2出队元素的要求交易的金额小于或等于银行现存金额，可以办理
		{
			if (TimeNow > CloseTime)  //营业时间到
			{
				printf("--\t%d\t%d\t%d\t%d\t%d\n", z, TimeUse, number, z, (ew.ArriveTime), ew.money);
			}
			else
			{
				TimeNow = TimeNow + ew.DealTime; //更新系统当前时间 
				TimeUse = TimeNow - ew.ArriveTime; //当前客户处理业务所用时间
				TotalTime += TimeUse; //更新逗留时间 
				TotalMoney -= ew.money; //更新资金总额 
				number++; //更新实现交易的客户数 
				printf("%d\t%d\t\t%d\t\t%d\t\t-%d\n", number, (ew.ArriveTime), TimeUse, (ew.money), TotalMoney);
			}
		}
		else
		{      //若Q2出队元素的要求交易的金额大于银行现存金额，不能办理 
			if (TimeNow > CloseTime) {
				printf("--\t%d\t%d\t%d\t%d\t%d\n", z, TimeUse, number, z, (ew.ArriveTime), ew.money);
			}
			else{
				EnQueue2(); //插入Q2的队尾，继续等待
			}
		}
	}
	return OK;
}

void CustomerLeave() {
	Queue1 p, q, r;
	int i;
	i = ev.Flag;	//判断业务类型，1表示存款，2表示取款 
	TimeNow = ev.OccurTime - ev.DealTime;
	if (i == 1){	//办理存款 
		if (ev.OccurTime>CloseTime)	//营业结束,全部客户离开银行 
			CloseBank();
		else{	//营业时间没有结束，继续办理 	 
			TimeUse = ev.OccurTime - ev.ArriveTime;
			TotalTime += TimeUse;
			TotalMoney = TotalMoney + ev.money;
			number++;
			TimeNow = ev.OccurTime;
			printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", number, ev.ArriveTime, TimeUse, ev.money, TotalMoney);
			CheckQ2(); //检查队列2是否有满足条件者 
		}
	}
	else {//办理取款 
		if (ev.money > TotalMoney) { //办理取款，当银行剩余存款不能满足申请金额时，离开Q1，进入Q2等待
			ElemType1 ev;
			ev.OccurTime = 0; 
			ev.ArriveTime = 0;  
			ev.DealTime = DEALTIME;
			p = Q1.front->next;
			r = Q1.front;
			if (p){
				if (p->data.Flag != 0) {
					q = p->next;
					r->next = q;
					ev.ArriveTime = p->data.ArriveTime; //到达时间 
   					ev.OccurTime = p->data.OccurTime - ev.DealTime; //事件发生时间 
					ev.Flag = p->data.Flag; //事件类型 
					ev.DealTime = p->data.DealTime; //办理业务时间 
					ev.money = p->data.money; //交易金额 
					free(p);
					EnQueue1();
				}
			}
			ew.ArriveTime = ev.ArriveTime;
			ew.DealTime = ev.DealTime;
			ew.money = ev.money;
			EnQueue2(); //进入队列2继续等待 
		}
		else //办理取款,当能满足所申请金额时进行队列1 
		{
			if (ev.OccurTime>CloseTime) //营业结束,全部客户离开银行 
				CloseBank();
			else {
				TimeUse = ev.OccurTime - ev.ArriveTime;//顾客所用时间 = 事件发生时间 - 事件到达时间 
				TotalTime += TimeUse;
				TotalMoney -= ev.money;
				TimeNow = ev.OccurTime;
				number++;
				printf("%d\t%d\t\t%d\t\t-%d\t\t%d\n", number, ev.ArriveTime, TimeUse, ev.money, TotalMoney);
			}
		}
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	int a;
	printf("	--------------------------------------------------------- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	--------------    班级：软件工程14(1)班    -------------- \n");
	printf("	--------------    学号：3114006171         -------------- \n");
	printf("	--------------    姓名：陈炯               -------------- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	--------------       银行业务模拟程序      -------------- \n");
	printf("	-----      1.开始模拟               0.退出程序      ----- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	请输入功能数字： ");
	scanf_s("%d", &a);
	while (a == 1)
	{
		InitBank(); //初始化
		printf("--------------------------------------------------------------------\n");
		printf("序号\t到达时间\t办理时间\t交易金额\t银行当前金额\n");
		{
			while (Q1.front)
			{
				DeQueue1(); //Q1出队 
				if (ev.Flag == 0) //标志：0表示客户到达，1表示客户离开 
					CustomerArrived(); //处理客户到达事件 
				else
					CustomerLeave(); //处理客户离开事件，业务类型标志：1表示存款，2表示取款 
			}
			printf("1.营业结束后银行拥有的资金: %ld\n", TotalMoney);
			printf("2.营业时间完成处理的客户数: %d\n", number);
			printf("3.客户在银行的平均逗留时间: %f\n", (float)TotalTime / (float)number);
			printf("---------------------------------------------------------------------------\n");
		}
		printf(" \n\n\t输入“1”再次模拟，输入其他任意数字退出：");
		scanf_s("%d", &a);
		if (a != 1) break;

	}
	return 0;
}

