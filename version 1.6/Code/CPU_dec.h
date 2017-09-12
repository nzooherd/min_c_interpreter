#pragma once

#include<vector>
#include<map>
#include<stack>
#include<string>
#include<set>
#include"DS_custom_dec.h"
using namespace std;
class CPU
{
public:
	CPU();
	~CPU();
	
	void addOp(const vector<string>& op);
	int getLineByLine(int line);
	int start();

	void clearWait();
	void addWait(int line);
	void delWait(int line);
	void nextLine();
	void nextPoint();
	void nextStep();
	Var* getValue(const string& name,int layer);
	int getMaxLayer();
	int getNowLine();

	void setLineMap(const vector<vector<int>>& lineMap);

private:
	map<int, int> line_map_fm;//key: final_line value: mid_line
	map<int, int> line_map_fo;//key: final_line value: origin_line
	map<int, int> line_map_of;//key: origin_line  value: the first final_line
	map<string, Function> Functions;//�洢���к���
	vector<Scope> Scopes;//�洢����ʱ�����Ĳ������Զ�����
	vector<stack<Var>> Stacks;//�洢����ʱ��������ʱ������ջ
	int layerNum;//���еĲ���
	Var ret;
	bool returnFlag;
	bool continueFlag;
	bool breakFlag;
	vector<pair<int ,string>> MainOps;
	set<int> wait;
	int tempWait;
	int nowLine;
	bool runnable;
	vector<string> origin_ops;
	int op_line;
	int maxOriginLine;
	int lastOriginLine;

	void Wait(int line);
	bool needWait(int line);

	void iniStack(stack<Var>& cache,char level);//��ʼ����һ���ջ�ռ�
	void iniStack(char level);//��ʼ����һ���ջ�ռ�
	void desStack();//������һ���ջ�ռ�
	Var popStack();//ȡջ��Ԫ��
	
	Var& decVar(const string& name,char type);//��������
	void assVar(Var& v1, const Var & v2);//��������ֵ
	Var getVar(const string& name);//ȡ����
	int findVar(const string& name);
	int findVar(const string& name,int layer);
	Var& getVar_(const string& name);

	void IF(const vector<pair<int,string>>& ops,int &start);//����if
	void WHILE(const vector<pair<int,string>>&ops, int &start);//����while
	void DO(const vector<pair<int, string>>&ops, int &start);

	map<string, Function>::iterator newFunc(const string& func);//�½�һ������
	void load();//���ز�������ʵ�ֶ�ȫ�����Ķ���ļ���

	void run(const vector<pair<int,string>>& ops);//ִ�в�������
	void run(const string& op);

	void callFunc(const string& func);//ִ�к���

	int firstInte(const string& op);//�ҵ�һ��,
	void trueReturn(char ret_type,const string& funcSign);//ת���ɺ��ʵķ���ֵ
	void pushCall(const string& op);



	void OpWithq(const string& op);
	void OpWith1(const string& op);
	void OpWith2(const string& op);
};
