#include"CPU_dec.h"
CPU::CPU() : Functions(),Scopes(),Stacks(),layerNum(0){
}

CPU::~CPU()
{
}

void CPU::load(const vector<string>& ops){
	Function* fp = nullptr;
	int count;
	for (int i = 0; i < ops.size(); i++) {
		if(fp == nullptr) {
			if (ops[i][0] == '@') {
				fp = newFunc(ops[i]);
				count = 1;
			}
			else throw "有不能识别的代码";
		}
		else if (ops[i][0] == '}') {
			count--;
			if (count == 0)//函数声明结束
				fp = nullptr;
			else
				fp->ops.push_back("}");
		}
		else fp->ops.push_back(ops[i]);
	}
}
bool CPU::start(const vector<string>& ops) {
	load(ops);//类似于编译完成
	callFunc(":main,", -1);//执行
	return ret.type == 'i' && ret.var.Int == 0;
}
void CPU::callFunc(const string& func, int layer) {
	//form the layer [layer] call function with string:
	//:<func_name>,<para1_name>,<para2_name>,<...>,
	vector<string> element(0);
	string paras_type = "";
	Var* t_var;
	stack<Var> cache;
	int t_index = 0;

	for (int index = 0; index < func.length; index++)
		if (func[index] == ',') {
			element.push_back(func.substr(t_index + 1, index));
			t_index = index;
		}

	for (vector<string>::iterator it = element.begin() + 1; it != element.end(); it++) {
		if (*it == "?") {
			t_var = &Stacks[layer].top();
			Stacks[layer].pop();
		}
		else
			t_var = &getVar(*it, layer);
		cache.push(*t_var);
		paras_type.push_back(t_var->type);
	}
	string signature = element[0] + '|' + paras_type;
	map<string, Function>::iterator func_it = Functions.find(signature);
	if (func_it == Functions.end())
		throw string("调用了未定义的函数") + element[0];
	else {
		//初始化堆栈空间
		iniStack(cache,Scopes[layer].level);
		//在layer+1层运行函数
		run(func_it->second.ops, layer + 1);
	}

}
void CPU::decVar(const string& name, char type,int layer) {
	//declare var in layer
	map<string, Var >::iterator it = Scopes[layer].vars.find(name);
	if (it != Scopes[layer].vars.end())
		throw string("重定义变量:") + name;
	else 
		Scopes[layer].vars.insert(pair<string, Var>(name, Var(type)));
}
void CPU::assVar(const string& name, Var var, int layer) {
	//assignment var in layer
	map<string, Var >::iterator it = Scopes[layer].vars.find(name);
	if (it != Scopes[layer].vars.end())
		it->second = var;
	else if (var.type == 's')
		throw string("不能对int型变量")+name+"，赋string值";
	else if (var.type == 'r') {
		var.type = 'i';
		var.var.Int = (int)var.var.Real;
		Scopes[layer].vars.insert(pair<string, Var>(name, var));
	}
	else
		Scopes[layer].vars.insert(pair<string, Var>(name, var));
}
Var CPU::getVar(const string&name, int layer) {
	map<string, Var >::iterator it = Scopes[layer].vars.find(name);
	if (it != Scopes[layer].vars.end() && it->second.init)
		return it->second;
	else if (Scopes[layer].level > 0)
		return getVar(name, layer - 1);
	throw string("使用了未初始化的变量") + name;
}
void CPU::IF(const vector<string>& ops, int& start,int layer) {
	Var* t_var = &Stacks[layer].top();Stacks[layer].pop();
	bool ifFlag;
	vector<string> nextOps(0);
	if (t_var->type == 's')
		throw "不能从string转换成int型";
	else if (t_var->type == 'i')
		ifFlag = (t_var->var.Int != 0);
	else
		ifFlag = (t_var->var.Real != 0);
	int count = 1, end = start + 1;
	for (; end < ops.size(); end++) {
		if (ops[end][0] == 'i' ||
			ops[end][0] == 'e' ||
			ops[end][0] == 'w') count++;
		else if (ops[end][0] == '}') {
			count--;
			if (count == 0) break;
		}
	}
	if (!ifFlag) {
		if (ops[end+1][0] == 'e') {//有else
			start = end + 1;
			count = 1;
			for (end = start + 1; end < ops.size(); end++) {
				if (ops[end][0] == 'i' ||
					ops[end][0] == 'e' ||
					ops[end][0] == 'w') count++;
				else if (ops[end][0] == '}') {
					count--;
					if (count == 0) break;
				}
			}
		}
		else return;//无else 且不执行if部分
	}
	for (start = start + 1; start < end; start++)
		nextOps.push_back(ops[start]);
	iniStack(Scopes[layer].level + 1);
	run(nextOps, layer + 1);

}
void CPU::iniStack(stack<Var>& cache, int level) {
	iniStack(level);
	while (!cache.empty()) {
		Stacks[layerNum-1].push(cache.top());
		cache.pop();
	}
}
void CPU::iniStack(int level) {
	if (layerNum == Scopes.size()) {
		Scopes.push_back(Scope());
		Stacks.push_back(stack<Var>());
	}
	else {
		Scopes[layerNum].vars.clear();
		Stacks.clear();
	}

	Scopes[layerNum].level = level;
	layerNum++;
}
void CPU::desStack() {
	layerNum--;
}
void CPU::WHILE(const vector<string>&ops, int &start, int layer) {
	//while 分作两部分看
	//part 1 ()里的部分 + r‘
	//part 2 {}里的部分
	// 操作序列应该是 W part1 w part2 }
	//总是在当前栈 请求新栈 执行完后 取栈顶值做判断决定是否执行part 2
	vector<string> part1;
	vector<string> part2;
	start++;
	while (start < ops.size() && ops[start][0] != 'w')
		part1.push_back(ops[start++]);
	part1.push_back("r");
	start++;
	while (start < ops.size() && ops[start][0] != '}')
		part2.push_back(ops[start++]);
	if (start >= ops.size())
		throw "while 嵌套异常。";
	Var* t_var;
	bool whileFlag;
	while (true) {
		iniStack(Scopes[layer].level + 1);
		run(part1, layer + 1);
		t_var = &Stacks[layer].top(); Stacks[layer].pop();
		if(t_var->type == 's')
			throw "不能从string转换成int型";
		else if (t_var->type == 'i')
			whileFlag = (t_var->var.Int != 0);
		else
			whileFlag = (t_var->var.Real != 0);
		if (whileFlag) {
			iniStack(Scopes[layer].level + 1);
			run(part2, layer + 1);
		}
		else break;
	}
}
/////////////////
Function* CPU::newFunc(const string& func) {
	//@[f_name],[r_type][p_type][p_type],[p_name],[p_name],
	vector<string> element(0);
	int t_index = 0;

	for (int index = 0; index < func.length; index++)
		if (func[index] == ',') {
			element.push_back(func.substr(t_index + 1, index));
			t_index = index;
		}
	string signature = element[0] + '|' + element[1].substr(1);
	map<string, Function>::iterator it = Functions.find(signature);
	if (it == Functions.end()) {
		Function* fp = &Function(vector<string>(element.begin() + 2, element.end()), element[1][0]);
		Functions.insert(pair<string, Function>(signature, *fp));
		return fp;
	}
	else
		throw string("重定义函数：") + element[0];
	
	
}
//////////////////////////
void CPU::run(const vector<string>&ops, int layer) {
	//操作: 调用	声明	赋值	+	-	*	/	%	$	#	if	else	
	//符号: :	_	=	+	-	*	/	%	$	#	i	e		
	//		while前	while后	}	>	<	>=	<=	==	<>	!	&&	||
	//		W		w		}	>	<	]	[	E	N	!	&	|
	//		出栈	入栈		返回
	//		?	(自动)	r

	int j;
	bool notreturn = true;
	for (int i = 0; i < ops.size() && notreturn; i++) {
		switch (ops[i][0])
		{
		case ':':
			callFunc(ops[i], layer);
		case '_':
			decVar(ops[i].substr(2), ops[i][1], layer);
		case '=':
			for (j = 0; i < ops[i].length; j++)
				if (ops[i][j] == ',') break;
			assVar(ops[i].substr(1, j), getVar(ops[i].substr(j + 1),layer), layer);
		case 'i':IF(ops, i, layer + 1); break;
		case 'e':throw "存在匹配不到if的else";break;
		case '}':throw "存在匹配不到if的else"; break;
		case 'W':WHILE(ops, i, layer + 1); break;
		case 'r':
			if (layer == 0)
				ret = getVar("?", layer);
			else
				Stacks[layer - 1].push(getVar("?", layer));
			break;
		//待实现:E N > < ( ) | & ! + - * / % $ #
		default:throw "Unknown error";
		}
	}
	desStack();
}