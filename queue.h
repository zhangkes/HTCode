#include<iostream>
using namespace std;

#define OK 1
#define ERROR 0
/*���������ݶ���*/
#define QElemType HBiTreeNode*
typedef  struct QNode {// �������
	QElemType      data;
	struct QNode  *next;
}QNode,*QueuePtr;
typedef struct{
	QueuePtr front;		   //��ͷָ��
	QueuePtr rear;		   //��βָ��
}LinkQueue;
/*�����еĲ���*/
int InitQueue(LinkQueue &Q) {
	// ����һ���ն���Q������ͷָ��Ͷ�βָ�붼ָ��ͷ��㣩
	Q.front = Q.rear =(QueuePtr)malloc(sizeof(QNode));
	if (!Q.front) exit(0);  //�洢����ʧ��
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
{ // ����Ԫ��eΪQ���µĶ�βԪ��              3-3-11
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	if (!p)  exit(OVERFLOW);   //�洢����ʧ��
	p->data = e;   p->next = NULL;  //�������½��
	Q.rear->next = p;                        //�ڲ����β
	Q.rear = p;                          //���޸Ķ�βָ��ָ���β
	return OK;
}
int DeQueue(LinkQueue &Q, QElemType &e)
{ // �����в��գ���ɾ��Q�Ķ�ͷԪ�أ��� e ������ֵ��������OK
	//���򷵻�ERROR                                                  3-3-12-1       3-3-12-2
	if (Q.front == Q.rear)    return ERROR;//���п�
	QueuePtr p = Q.front->next;   e = p->data;         //����e����
	Q.front->next = p->next;   //���޸�ͷָ��ʼ��ָ�����Ԫ��
	if (Q.rear == p)  Q.rear = Q.front;//�������������ն�
	free(p);                       //���ͷŶ��׽��
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

