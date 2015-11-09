#include<iostream>
using namespace std;

#define OK 1
#define ERROR 0
/*单队列数据定义*/
#define QElemType HBiTreeNode*
typedef  struct QNode {// 结点类型
	QElemType      data;
	struct QNode  *next;
}QNode,*QueuePtr;
typedef struct{
	QueuePtr front;		   //队头指针
	QueuePtr rear;		   //队尾指针
}LinkQueue;
/*单队列的操作*/
int InitQueue(LinkQueue &Q) {
	// 构造一个空队列Q，（队头指针和队尾指针都指向头结点）
	Q.front = Q.rear =(QueuePtr)malloc(sizeof(QNode));
	if (!Q.front) exit(0);  //存储分配失败
	Q.front->next = NULL;
	return 0;
} /*end of InitQeueue*/
/*DestroyQueue*/
int DestroyQueue(LinkQueue &Q){
	QueuePtr p=NULL;
	while (Q.front){
		p = Q.front;
		Q.front = p->next;
		free(p);
	}
	return OK;
} /*end of DestroyQueue*/
int EnQueue(LinkQueue &Q, QElemType e)
{ // 插入元素e为Q的新的队尾元素              3-3-11
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	if (!p)  exit(OVERFLOW);   //存储分配失败
	p->data = e;   p->next = NULL;  //①生成新结点
	Q.rear->next = p;                        //②插入队尾
	Q.rear = p;                          //③修改队尾指针指向队尾
	return OK;
}
int DeQueue(LinkQueue &Q, QElemType &e)
{ // 若队列不空，则删除Q的队头元素，用 e 返回其值，并返回OK
	//否则返回ERROR                                                  3-3-12-1       3-3-12-2
	if (Q.front == Q.rear)    return ERROR;//①判空
	QueuePtr p = Q.front->next;   e = p->data;         //②用e返回
	Q.front->next = p->next;   //③修改头指针始终指向队首元素
	if (Q.rear == p)  Q.rear = Q.front;//④特殊情况处理空队
	free(p);                       //⑤释放队首结点
	return OK;
}
/*Traverse*/
int QueueTravese(LinkQueue &Q){
	QueuePtr p = Q.front->next;
	while (p){
		cout << "data is" << p->data << endl;
		p = p->next;
	}
	return OK;
}
bool IsEmpty(LinkQueue &Q)
{
	if (Q.front==Q.rear)return true;
	else return false;
}

