// ����ҵ��ģ��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <conio.h> 
#include "bank.h"
 

int TotalMoney = 10000; //���������ʽ��ܶ�
int CloseTime = 600;    //����Ӫҵʱ��  
int TotalTime;          //�ͻ�������ʱ�䣬��������ƽ��ֵ
int TimeNow;            //��ǰʱ�䣬�ж��Ƿ񳬹�Ӫҵʱ��
int number;             //ҵ��������
int TimeUse;            //ÿλ�˿�����ʱ�� 
int money;              //ÿλ�˿Ͱ���Ľ���� 
int INTERTIME;          //�ͻ������ʱ���� 
int DEALTIME;           //ҵ����������ʱ�� 

LinkQueue1 Q1;
LinkQueue2 Q2;
Event1 ev;
Event2 ew;

//��ʼ������1 
int InitQueue1() {
	Q1.front = Q1.rear = (Queue1)malloc(sizeof(QNode1));
	if (!Q1.front) 
		exit(OVERFLOW);
	Q1.front->next = NULL;
	return OK;
}

//��ʼ������ 2 
int InitQueue2() {
	Q2.front = Q2.rear = (Queue2)malloc(sizeof(QNode2));
	if (!Q2.front)
		exit(OVERFLOW);
	Q2.front->next = NULL;
	return OK;
}

//���ٶ���1
int DestroyQueue1() {		
	while (Q1.front) {
		Q1.rear = Q1.front->next;
		free(Q1.front);
		Q1.front = Q1.rear;
	}
	return OK;
}

//���ٶ���2 
int DestroyQueue2() {      
	while (Q2.front) {
		Q2.rear = Q2.front->next;
		free(Q2.front);
		Q2.front = Q2.rear;
	}
	return OK;
}

//ɾ��Q1�Ķ�ͷԪ�أ���ev����
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
		Q1.rear = Q1.front;	//��Q1ֻ��һ��Ԫ��ʱ 
	free(p);   //�ͷŽ��
	return OK;
}

//ɾ��Q2�Ķ�ͷԪ�أ���ew����
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
		Q2.rear = Q2.front;  //��Q2ֻ��һ��Ԫ��ʱ
	free(p);    //�ͷŽ��
	return OK;
}

//Q1��Ӳ���
int EnQueue1() {		 
	Queue1 p, r, t;
	t = NULL;
	p = (Queue1)malloc(sizeof(QNode1));  //����洢�ռ�
	if (!p)    
		exit(OVERFLOW);             //����ʧ��
	p->data.ArriveTime = ev.ArriveTime;
	p->data.OccurTime = ev.OccurTime;
	p->data.Flag = ev.Flag;
	p->data.DealTime = ev.DealTime;
	p->data.money = ev.money;
	r = Q1.front->next;
	while (r) {   //r�ǿ�
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
	if (!r) {  //rΪ��
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

//Q2��Ӳ���
int EnQueue2() {		
	Queue2 p;
	p = (Queue2)malloc(sizeof(QNode2)); //����洢�ռ�
	if (!p)
		exit(OVERFLOW);     //����ʧ��
	p->data.ArriveTime = ew.ArriveTime;
	p->data.DealTime = ew.DealTime;
	p->data.money = ew.money;
	p->next = NULL;
	Q2.rear->next = p;
	Q2.rear = p;
	return OK;
}

//Ӫҵʱ�䵽���ر����У���ն���
void CloseBank() {  
	DestroyQueue1();
	DestroyQueue2();
}

//�����������
int rand_1(int *intertime, int *Flag, int *DealTime, int *money) {    
	*intertime = rand() % INTERTIME + 1;  //�¸��ͻ������ʱ����
	*Flag = rand() % 2;                   //������ȡ��
	*DealTime = rand() % DEALTIME + 1;    //�ͻ�����ҵ������ʱ��
	*money = rand() % MONEY + 1;          //�ͻ�����ҵ��Ľ��׽��
	return OK;
}

//��ʼ������
void InitBank() {      
	printf("	��������󵽴�ʱ���� (1~600) ��");
	scanf_s("%d", &INTERTIME);     
	printf("	���������Ĵ���ʱ�� (1~600) ��");
	scanf_s("%d", &DEALTIME);
	TotalTime = 0;		 
	number = 0;		     
	InitQueue1();   //��ʼ������ 1 
	InitQueue2();	//��ʼ������ 2 
	ev.ArriveTime = 0;
	ev.OccurTime = 0;
	ev.Flag = 0;
	ev.money = 0;
	ev.DealTime = 0;
	EnQueue1();  //�¼��������
}

//
void CustomerArrived() {
	Queue1 p;
	int intertime, i = 0;
	TimeNow = ev.OccurTime;
	rand_1(&intertime, &(ev.Flag), &(ev.DealTime), &(ev.money));		 //��ʼ���ͻ�����
	ev.Flag++;
	p = Q1.front->next;
	while (p != NULL) {
		if (p->data.Flag != 0)
			i = p->data.OccurTime; //��ȡʱ��
		p = p->next;
	}
	if (i == 0)		//��һλ�ͻ� 
		ev.OccurTime = ev.ArriveTime + ev.DealTime;
 	else if (i >= ev.ArriveTime)		//���¼�����ʱ��һ�¼���δ���� 
		ev.OccurTime = i + ev.DealTime;		 //���¼����뿪ʱ��=��һ�¼����뿪ʱ��+���¼�����ʱ�� 
	else				//��һ�¼��뿪֮�󣬱��¼��ŵ��� 
		ev.OccurTime = ev.ArriveTime + ev.DealTime;		//���¼����뿪ʱ��=���¼�����ʱ��+���¼�����ʱ�� 
	EnQueue1();			//��һ�û������¼�������� 1  
	ev.ArriveTime = ev.ArriveTime + intertime;		//��һ�ͻ�����ʱ�� 
	ev.OccurTime = ev.ArriveTime;
	ev.Flag = 0; //��ʱ��ֵ 
	ev.money = 0;
	ev.DealTime = 0;
	EnQueue1();
}

//���Q2���Ƿ��пͻ��Ѿ��ﵽ���Դ���������ҵ��
int CheckQ2() {   
	int i = 0, j, z = 0;
	Queue2 p;
	p = Q2.front->next;
	while (p) {          //Q2�ĳ���
		i++;
		p = p->next;
	} 
	for (j = 1; j <= i; j++) {
		DeQueue2();
		if (ew.money <= TotalMoney) //��Q2����Ԫ�ص�Ҫ���׵Ľ��С�ڻ���������ִ�����԰���
		{
			if (TimeNow > CloseTime)  //Ӫҵʱ�䵽
			{
				printf("--\t%d\t%d\t%d\t%d\t%d\n", z, TimeUse, number, z, (ew.ArriveTime), ew.money);
			}
			else
			{
				TimeNow = TimeNow + ew.DealTime; //����ϵͳ��ǰʱ�� 
				TimeUse = TimeNow - ew.ArriveTime; //��ǰ�ͻ�����ҵ������ʱ��
				TotalTime += TimeUse; //���¶���ʱ�� 
				TotalMoney -= ew.money; //�����ʽ��ܶ� 
				number++; //����ʵ�ֽ��׵Ŀͻ��� 
				printf("%d\t%d\t\t%d\t\t%d\t\t-%d\n", number, (ew.ArriveTime), TimeUse, (ew.money), TotalMoney);
			}
		}
		else
		{      //��Q2����Ԫ�ص�Ҫ���׵Ľ����������ִ�����ܰ��� 
			if (TimeNow > CloseTime) {
				printf("--\t%d\t%d\t%d\t%d\t%d\n", z, TimeUse, number, z, (ew.ArriveTime), ew.money);
			}
			else{
				EnQueue2(); //����Q2�Ķ�β�������ȴ�
			}
		}
	}
	return OK;
}

void CustomerLeave() {
	Queue1 p, q, r;
	int i;
	i = ev.Flag;	//�ж�ҵ�����ͣ�1��ʾ��2��ʾȡ�� 
	TimeNow = ev.OccurTime - ev.DealTime;
	if (i == 1){	//������ 
		if (ev.OccurTime>CloseTime)	//Ӫҵ����,ȫ���ͻ��뿪���� 
			CloseBank();
		else{	//Ӫҵʱ��û�н������������� 	 
			TimeUse = ev.OccurTime - ev.ArriveTime;
			TotalTime += TimeUse;
			TotalMoney = TotalMoney + ev.money;
			number++;
			TimeNow = ev.OccurTime;
			printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", number, ev.ArriveTime, TimeUse, ev.money, TotalMoney);
			CheckQ2(); //������2�Ƿ������������� 
		}
	}
	else {//����ȡ�� 
		if (ev.money > TotalMoney) { //����ȡ�������ʣ���������������ʱ���뿪Q1������Q2�ȴ�
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
					ev.ArriveTime = p->data.ArriveTime; //����ʱ�� 
   					ev.OccurTime = p->data.OccurTime - ev.DealTime; //�¼�����ʱ�� 
					ev.Flag = p->data.Flag; //�¼����� 
					ev.DealTime = p->data.DealTime; //����ҵ��ʱ�� 
					ev.money = p->data.money; //���׽�� 
					free(p);
					EnQueue1();
				}
			}
			ew.ArriveTime = ev.ArriveTime;
			ew.DealTime = ev.DealTime;
			ew.money = ev.money;
			EnQueue2(); //�������2�����ȴ� 
		}
		else //����ȡ��,����������������ʱ���ж���1 
		{
			if (ev.OccurTime>CloseTime) //Ӫҵ����,ȫ���ͻ��뿪���� 
				CloseBank();
			else {
				TimeUse = ev.OccurTime - ev.ArriveTime;//�˿�����ʱ�� = �¼�����ʱ�� - �¼�����ʱ�� 
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
	printf("	--------------    �༶���������14(1)��    -------------- \n");
	printf("	--------------    ѧ�ţ�3114006171         -------------- \n");
	printf("	--------------    �������¾�               -------------- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	--------------       ����ҵ��ģ�����      -------------- \n");
	printf("	-----      1.��ʼģ��               0.�˳�����      ----- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	--------------------------------------------------------- \n");
	printf("	�����빦�����֣� ");
	scanf_s("%d", &a);
	while (a == 1)
	{
		InitBank(); //��ʼ��
		printf("--------------------------------------------------------------------\n");
		printf("���\t����ʱ��\t����ʱ��\t���׽��\t���е�ǰ���\n");
		{
			while (Q1.front)
			{
				DeQueue1(); //Q1���� 
				if (ev.Flag == 0) //��־��0��ʾ�ͻ����1��ʾ�ͻ��뿪 
					CustomerArrived(); //����ͻ������¼� 
				else
					CustomerLeave(); //����ͻ��뿪�¼���ҵ�����ͱ�־��1��ʾ��2��ʾȡ�� 
			}
			printf("1.Ӫҵ����������ӵ�е��ʽ�: %ld\n", TotalMoney);
			printf("2.Ӫҵʱ����ɴ���Ŀͻ���: %d\n", number);
			printf("3.�ͻ������е�ƽ������ʱ��: %f\n", (float)TotalTime / (float)number);
			printf("---------------------------------------------------------------------------\n");
		}
		printf(" \n\n\t���롰1���ٴ�ģ�⣬�����������������˳���");
		scanf_s("%d", &a);
		if (a != 1) break;

	}
	return 0;
}

