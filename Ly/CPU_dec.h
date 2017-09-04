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

	bool start(const vector<string>& ops);
	void clearWait();
	void addWait(int line);
	void delWait(int line);
	void nextLine();
	void nextPoint();
	Var* getValue(const string& name,int layer);
	int getMaxLayer();
private:

	map<string, Function> Functions;//存储所有函数
	vector<Scope> Scopes;//存储运行时产生的产生的自动变量
	vector<stack<Var>> Stacks;//存储运行时产生的临时变量的栈
	int layerNum;//现有的层数
	Var ret;
	bool returnFlag;
	vector<pair<int ,string>> MainOps;
	set<int> wait;
	int tempWait;
	int nowLine;
	bool runnable;

	void Wait(int line);

	void iniStack(stack<Var>& cache,char level);//初始化下一层堆栈空间
	void iniStack(char level);//初始化下一层堆栈空间
	void desStack();//销毁上一层堆栈空间
	Var popStack();//取栈顶元素
	
	Var& decVar(const string& name,char type);//声明变量
	void assVar(Var& v1, const Var & v2);//给变量赋值
	Var getVar(const string& name);//取变量
	map<string, Var>::iterator findVar(const string& name);
	map<string, Var>::iterator findVar(const string& name,int layer);
	Var& getVar_(const string& name);

	void IF(const vector<pair<int,string>>& ops,int &start);//处理if
	void WHILE(const vector<pair<int,string>>&ops, int &start);//处理while

	map<string, Function>::iterator newFunc(const string& func);//新建一个函数
	void load(const vector<string>& ops);//加载操作序列实现对全函数的定义的加载

	void run(const vector<pair<int,string>>& ops);//执行操作序列
	void run(const string& op);

	void callFunc(const string& func);//执行函数

	int firstInte(const string& op);//找第一个,
	void trueReturn(char ret_type,const string& funcSign);//转换成合适的返回值
	void pushCall(const string& op);



	void OpWithq(const string& op);
	void OpWith1(const string& op);
	void OpWith2(const string& op);
};
