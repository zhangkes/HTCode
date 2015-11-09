// HTCode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdafx.h"

#include<list>
#include<deque>
#include<map>
#include<algorithm>
#include<functional>

#include<string>
#include<stdlib.h>
#include<iostream>
#include<fstream>

using namespace std;
#ifndef Element
#define ElementType char 
#endif	

#define MAXCHARACTER 1024
#define MAXLETTER 127
ElementType* DATABUFFER = NULL;
/*统计信息*/
int COMPRESS = 0;
int CODELENGHT = 0;

/*定义字符和相应的编码*/
struct Encoding{
	ElementType c;			              /*字符*/
	char code[MAXLETTER - 1];			  /*编码*/
};
/*定义Huffman树节点结构*/
typedef struct HBiTreeNode{
	ElementType	c;					   /*字符*/
	int    weight;						/*权重*/
	struct HBiTreeNode *lchild, *rchild;
	bool operator <(const HBiTreeNode &e) const
	{
		return weight < e.weight;
	}
}HTNode, *HuffmanTree;

/*统计字符权重*/
void Statistics_weight(char* txt, deque<HBiTreeNode> &VHTNode, map<char, int>  &LWMap)
{
	ElementType *DATA = txt;				/*接口*/
	//cout << endl << "文本长度： " << strlen(DATA) << endl;
	for (int i = 0; i < strlen(DATA); i++){
		LWMap[DATA[i]]++;        
	}
	map<char, int>::iterator  iter;
	for (iter = LWMap.begin(); iter != LWMap.end(); iter++)
	{
		HBiTreeNode HTNode;
		HTNode.c = iter->first;
		HTNode.weight = iter->second;
		HTNode.lchild = NULL;
		HTNode.rchild = NULL;
		VHTNode.push_back(HTNode);
	}
} /*end of Statistics_weight*/

void SystemInput(){
	//int num;    /*编码字符个数*/
	DATABUFFER = (ElementType*)malloc(sizeof(char)*(MAXCHARACTER + 1));
	memset(DATABUFFER, 0, sizeof(char)*(MAXCHARACTER + 1));	  /*初始化空间值为0*/
	cout << endl << "请输入一段文字！"<<endl;
	cin >> DATABUFFER;
	fflush(stdin);
	DATABUFFER[MAXCHARACTER] = '#';  /*以标识符#结尾*/
}

void see(HBiTreeNode &s)
{
	cout << s.c << "的权重:" << s.weight << endl;
}
void value_copy(deque<HBiTreeNode> &VHTNode, HBiTreeNode*  HT){
	deque<HBiTreeNode>::iterator outputIterator = VHTNode.begin();
	HT->c = (*outputIterator).c;
	HT->weight = (*outputIterator).weight;
	HT->lchild = (*outputIterator).lchild;
	HT->rchild = (*outputIterator).rchild;
}
/*构建哈夫曼树*/
HBiTreeNode*  Build_Tree( deque<HBiTreeNode> &VHTNode){
	HBiTreeNode* HT=NULL;
	sort(VHTNode.begin(), VHTNode.end());
	/*cout << "*****************************" << endl;
	for_each(VHTNode.begin(), VHTNode.end(), see);
	cout << "*****************************" << endl;   */
	if (VHTNode.size() == 1)
	{
		HT = (HBiTreeNode*)malloc(sizeof(HTNode));
		value_copy(VHTNode, HT);
	}
	while (VHTNode.size() >= 2)
	{
		HBiTreeNode*  HT1 = (HBiTreeNode*)malloc(sizeof(HTNode));
		HBiTreeNode*  HT2 = (HBiTreeNode*)malloc(sizeof(HTNode));
		HBiTreeNode*  TempNode = (HBiTreeNode*)malloc(sizeof(HTNode));

		value_copy(VHTNode, HT1);
		VHTNode.pop_front();

		value_copy(VHTNode, HT2);
		VHTNode.pop_front();

		TempNode->weight = HT1->weight + HT2->weight;
		TempNode->c = '#';
		TempNode->lchild = HT2;
		TempNode->rchild = HT1;
		VHTNode.push_front(*TempNode);
		HT = TempNode;
		//cout << endl << "HT[" << HT2->c << "] and HT[" << HT1->c << "] create";
		//cout << " HT[" << TempNode->c << "], weight=" << TempNode->weight;
		sort(VHTNode.begin(), VHTNode.end());
	}
	VHTNode.clear();
	return HT;
}
/************************************************************************************************/
/*对字符进行编码*/
void encoding(HBiTreeNode* T, char* code,  map<char, string>  &PassWordMap)
{
	if (T) {
		if ((!T->lchild) && (!T->rchild) && T->c != '#')
		{	 //count++;     // 对叶子结点计数
			PassWordMap[T->c] = code;
			//cout << T->c << " 的编码：" << code << endl;
			COMPRESS += T->weight*strlen(code);							/*计算压缩后文本大小*/
		}
		code[strlen(code)] = '0';
		encoding(T->lchild, code, PassWordMap);
		code[strlen(code) - 1] = '\0';
		code[strlen(code)] = '1';
		encoding(T->rchild, code, PassWordMap);
		code[strlen(code) - 1] = '\0';
	} // if
}
/*对文本进行编码*/
void EncodingTxt(char *EncodeTxt, char* DecodeTxt, map<char, string>  PassWordMap)	{
	int count = 0;
	if (EncodeTxt != NULL){
		while (*EncodeTxt != '\0')
		{
			memcpy(DecodeTxt + count, PassWordMap[*EncodeTxt].data(), PassWordMap[*EncodeTxt].size());
			count += strlen(PassWordMap[*EncodeTxt].data());
			EncodeTxt++;
		}
	}
}
/****************************************************************************************************/
/*对单字符进行解码*/
int flag = 1;
void decoding(HBiTreeNode* T, char *EncodeTxt, char *c, int& count)	{
	if (T) {
		if ((!T->lchild) && (!T->rchild) && T->c != '#')
		{
			*c = T->c;	                  /*解码的字符*/
			flag = 0;					   /*设置标识符，跳回根节点，以便重新解码*/
			return;
		}
		if (*EncodeTxt == '0'&&flag != 0)
		{
			EncodeTxt++;					   /*密文向后移一位*/
			count++;						   /*计算径长度*/
			decoding(T->lchild, EncodeTxt, c, count);
		}
		if (*EncodeTxt == '1'&&flag != 0){
			EncodeTxt++;
			count++;
			decoding(T->rchild, EncodeTxt, c, count);
		}
	}
}
/*对文本进行解码*/
void decoding1(HBiTreeNode* T, char *EncodeTxt, char *DecodeTxt){
	/*对ENCodeTxt进行解码放入DecodexTxt中*/
	int count = 0;	                         /*偏移位数*/
	while (EncodeTxt[count] != '\0'){
		flag = 1;
		decoding(T, EncodeTxt + count, DecodeTxt++, count);
	}
}
int Depth(HBiTreeNode* T){ // 返回二叉树的深度
	int depthLeft, depthRight, depthval;
	if (T) {
		depthLeft = Depth(T->lchild);
		depthRight = Depth(T->rchild);
		depthval = 1 + (depthLeft > depthRight ? depthLeft : depthRight);
	}
	else  depthval = 0;
	return depthval;
}
void PreorderTraversal(HBiTreeNode* T)
{
	if (T) {
		cout << T->c << "->";
		PreorderTraversal(T->lchild);
		PreorderTraversal(T->rchild);
	}
}
#include"queue.h"
void LevelorderTraversal(HBiTreeNode* HT)
{
	LinkQueue Q;
	HuffmanTree BT;
	if (!HT) return; /* 若是空树则直接返回 */
	InitQueue(Q); /* 创建空队列Q */
	EnQueue(Q, HT);
	while (!IsEmpty(Q)) {
		DeQueue(Q, BT);
		cout << "HT[" << BT->c << "]  and wegith: " << BT->weight  << endl;
		//printf("%s ", T->data); /* 访问取出队列的结点 */
		if (BT->lchild)   EnQueue(Q, BT->lchild);
		if (BT->rchild)   EnQueue(Q, BT->rchild);
	}
}
void DestoryTree(HBiTreeNode* HT)
{
	LinkQueue Q;
	HBiTreeNode* BT;
	if (!HT) return; /* 若是空树则直接返回 */
	InitQueue(Q); /* 创建空队列Q */
	EnQueue(Q, HT);
	while (!IsEmpty(Q)) {
		DeQueue(Q, BT);
		if (BT->lchild)   EnQueue(Q, BT->lchild);
		if (BT->rchild)   EnQueue(Q, BT->rchild);
		free(BT);
	}
}

size_t ReadFile(FILE *fp, void *ReadBuffer, int ReadSize, int offset = 0, int count = 1, int origin = SEEK_SET){
	fseek(fp, offset, origin);
	if (ReadSize*count == (fread(ReadBuffer, ReadSize, count, fp)))return 1;
	else return -1;
}
size_t WirteFile(FILE *fp, void *WirteBuffer, int WirteSize, int offset = 0, int count = 1, int origin = SEEK_SET){
	fseek(fp, offset, origin);
	if (WirteSize*count == (fread(WirteBuffer, WirteSize, count, fp)))return 1;
	else return -1;
}
FILE* OpenFile(const char* FileName, const char* mode){
	FILE *fp = NULL;
	/*if ((fp = fopen(FileName, "w")) == NULL){
	cout << "open " << FileName << "failed" << endl;
	exit(0);
	} 	*/
	return fp;
}
void EncodingTxt(char* DecodeFile, char* EncodeFile){
	fstream in(DecodeFile,ios::in );
	fstream out(EncodeFile, ios::out);
	fstream inTree("HT.txt", ios::in);
	fstream outTree("HT.txt", ios::out);
	if (!in){
		cout << "open this failed in" << endl;
		exit(0);
	}
	if (!out){
		cout << "open this failed out" << endl;
		exit(0);
	}	
	char ch;
	//if (in.good()){ cout << "not good" << endl; exit(0); }
	while (in) {
		deque<HBiTreeNode> VHTNode;
		map<char, int> 	   LWMap;            /*字符和相应的权重*/
		map<char, string>  PassWordMap;		 /*字符和相应的编码*/
		HBiTreeNode* HT = NULL;

		char TxtBuffer[MAXLETTER*2];				 /*文本缓冲区*/
		char SecretTxt[MAXLETTER*4];					     /*密文*/
		memset(TxtBuffer, '\0', sizeof(TxtBuffer));
		memset(SecretTxt, '\0', sizeof(SecretTxt) );
		
		in.unsetf(ios::skipws);
		in.read(TxtBuffer, 52);
		//in.getline(TxtBuffer, 52, '\n');

		//cout << endl << "TxtBuffer : " << TxtBuffer << endl;					   /*输出*/
		
		Statistics_weight(TxtBuffer, VHTNode, LWMap);	  /*统计字符*/
	
		HT = Build_Tree(VHTNode);		   /*构建哈夫曼树*/
		
		char strcode[2*MAXLETTER-1];		      /*临时数组，储存单个字符编码信息*/
		memset(strcode, '\0', sizeof(strcode));
		encoding(HT, strcode, PassWordMap);
		EncodingTxt(TxtBuffer, SecretTxt, PassWordMap);	  /*编码*/
		
		//cout << endl << "MiTXT : " << SecretTxt << endl;					   /*输出*/
		out.write(SecretTxt, strlen(SecretTxt));
		
		char Txt[MAXLETTER];				 /*文本缓冲区*/
		memset(Txt, '\0', sizeof(char) * MAXLETTER);
		Encoding TreeInfor[MAXLETTER*2];
		map<char, string>::iterator  iter;
		for (iter = PassWordMap.begin(); iter != PassWordMap.end(); iter++)
		{
			//TreeInfor.c = iter->first;
			//memcpy(&TreeInfor.code, iter->second.data(), iter->second.size());
	
		}
		//outTree.write(TreeInfor, sizeof(Encoding));
		decoding1(HT, SecretTxt, Txt);
		
		cout << Txt;
		DestoryTree(HT);
	}
	in.close();
	out.close();
}
void t()
{

	HBiTreeNode* HT1 = NULL;
	deque<HBiTreeNode> VHTNode;			 /*字符和相应的权重,和LWMap顺序相反*/
	map<char, int> 	   LWMap;            /*字符和相应的权重*/
	map<char, string>  PassWordMap;		 /*字符和相应的编码*/

	cout << endl << "HuffmanCoding.cpp";
	cout << endl << "=================" << endl;
	SystemInput();
	Statistics_weight(DATABUFFER, VHTNode, LWMap);   /*统计字符频率*/
	HT1 = Build_Tree(VHTNode);		   /*构建哈夫曼树*/

	cout << endl << "=================" << endl;
	PreorderTraversal(HT1);
	if (HT1 != NULL){
		cout << "print huffamntree" << endl;
		cout << "depth" << Depth(HT1) << endl;
	}

	char strcode[MAXLETTER];		      /*临时数组，储存单个字符编码信息*/
	memset(strcode, '\0', sizeof(char)*MAXLETTER);
	encoding(HT1, strcode, PassWordMap);
	LevelorderTraversal(HT1);
	cout << "****************将文本信息编码****************" << endl;
	char SecretTxt[10240];					     /*密文*/
	memset(SecretTxt, '\0', sizeof(char) * 10240);
	EncodingTxt(DATABUFFER, SecretTxt, PassWordMap);
	cout << SecretTxt << endl;

	cout << "****************将文本信息解码****************" << endl;
	char Txt[10240];					     /*明文*/
	memset(Txt, '\0', sizeof(char) * 10240);
	decoding1(HT1, SecretTxt, Txt);
	cout << "原文信息" << DATABUFFER << endl << " ------VS------" << endl << "解码信息" << Txt << endl;

	cout << "****************压缩程度统计****************" << endl;
	cout << "原文大小：" << strlen(DATABUFFER) * 4 << endl;
	cout << "压缩后大小：" << COMPRESS << endl;
	cout << endl << "...OK!...";
	DestoryTree(HT1);

}
void main()            		//main() function
{
	//t();
	char * myfile = "zhangke.txt";
	char * mycode = "vk.txt";
	EncodingTxt(myfile, mycode);
	//goto l;

} //main() end



