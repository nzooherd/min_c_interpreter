#pragma once
#include<vector>
#include<map>
#include<stack>
#include<set>
#include"DS_custom.h"
using namespace std;
class CPU
{
public:
	CPU();
	~CPU();

	bool start(const vector<string>& ops);
private:

	map<string, Function> Functions;//存储所有函数
	vector<Scope> Scopes;//存储运行时产生的产生的自动变量
	vector<stack<Var>> Stacks;//存储运行时产生的临时变量的栈
	int layerNum;//现有的层数
	Var ret;

	void iniStack(stack<Var>& cache,int level);//初始化下一层堆栈空间
	void iniStack(int level);//初始化下一层堆栈空间
	void desStack();//销毁上一层堆栈空间
	
	void decVar(const string& name,char type,int layer);//声明变量
	void assVar(const string& name, Var var, int layer);//给变量赋值
	Var getVar(const string& name,int layer);//取变量
	void IF(const vector<string>& ops,int &start,int layer);//处理if
	void WHILE(const vector<string>&ops, int &start, int layer);//处理while
	Function* newFunc(const string& func);//新建一个函数
	void load(const vector<string>& ops);//加载操作序列实现对全函数的定义的加载
	void run(const vector<string>& ops, int layer);//执行操作序列
	void callFunc(const string& func, int layer);//执行函数
};
