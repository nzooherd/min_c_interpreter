#include<iostream>
#include"CPU_dec.h"
#include"Exception_dec.h"
using namespace std;
CPU::CPU() : Functions(), Scopes(), Stacks(), MainOps(), 
layerNum(0), ret('i'), op_line(0),
returnFlag(false),continueFlag(false),breakFlag(false),
wait(), tempWait(-2), nowLine(-1), runnable(true), origin_ops(),
line_map_fm(), line_map_fo(), line_map_of(),
maxOriginLine(0),lastOriginLine(-1)
{
}

CPU::~CPU()
{
}

void CPU::addOp(const vector<string>& ops) {
	for (int i = 0; i < ops.size(); i++) {
		line_map_fm.insert(pair<int, int>(origin_ops.size(), op_line));
		origin_ops.push_back(ops[i]);
	}
	op_line++;
}
int CPU::getLineByLine(int line) {
	auto ret = line_map_fo.find(line);
	if (ret == line_map_fo.end())
		return -1;
	else
		return ret->second;
}
int CPU::start() {
	
	load();//类似于编译完成
	
	if (MainOps.size() == 0)
		throw Compile_E("未定义main函数");
	
	iniStack(0);
	run(MainOps);
	desStack();
	if(!ret.init)
		throw Compile_E("main 函数没有return语句 或 不是所有分支都有return语句");
	return Int(ret);
}
void CPU::load(){
	const vector<string>& ops = origin_ops;
	map<string, Function>::iterator func_it = Functions.end();
	int count;
	for (int i = 0; i < ops.size(); i++) {
		if (func_it == Functions.end()) {
			if (ops[i][0] == 'q' && ops[i][1] == '@') {
				if (ops[i].substr(2, 5) == "main,") {
					if (MainOps.size() != 0)
						throw Compile_E(i,"main函数不允许重定义或重载");
					if (ops[i].length() != 9)
						throw Compile_E(i,"main函数不允许有参数");
					if (ops[i][7] != 'i')
						throw Compile_E("main函数需要返回int型值");
					count = 1; i++;
					for (; i < ops.size(); i++) {
						MainOps.push_back(pair<int,string>(i,ops[i]));
						if (ops[i][0] == '0') {
							if (ops[i][1] == '}')
								count--;
							else if (ops[i][1] == 'i' ||
								ops[i][1] == 'e' ||
								ops[i][1] == 'w' ||
								ops[i][1] == 'd')
								count++;
							if (count == 0)//main函数声明结束
								break;
						}
					}
				}
				else {
					func_it = newFunc(ops[i]);
					count = 1;
				}
			}
			else throw Other_E(i,string("不能识别的操作:")+ops[i]);
		}
		else {
			func_it->second.ops.push_back(pair<int,string>(i,ops[i]));
			if (ops[i][0] == '0') {
				if (ops[i][1] == 'i' || ops[i][1] == 'e' || ops[i][1] == 'w' || ops[i][1] == 'd') count++;
				else if (ops[i][1] == '}') {
					count--;
					if (count == 0)
						func_it = Functions.end();
				}
			}
		}
	}
}
void CPU::trueReturn(char ret_type,const string& funcSign) {
	if (ret_type == 'v') {
		if (ret.type != 'v')
			throw Simple_E(string("函数(") + funcSign + ")不应当有返回值");
	}
	else if (ret.type =='v')
		throw Simple_E(string("函数(") + funcSign + ")应当有返回值");
	else if (ret.type == 's') ret = vString(ret);
	else if (ret_type == 'i') ret = vInt(ret);
	else ret = vFloat(ret);
}
void CPU::callFunc(const string& func) {
	//form the layer [layer] call function with string:
	//^<func_name>,<para1_name>,<para2_name>,<...>,
	vector<string> element(0);
	string paras_type = "";
	Var* t_var;
	stack<Var> cache;
	int t_index = 1;

	for (int index = 2; index < func.length(); index++)
		if (func[index] == ',') {
			element.push_back(func.substr(t_index + 1, index - t_index - 1));
			t_index = index;
		}

	for (vector<string>::iterator it = element.begin() + 1; it != element.end(); it++) {
			t_var = &getVar(*it);

		cache.push(*t_var);
		paras_type.push_back(t_var->type);
	}
	string signature = element[0] + '|' + paras_type;
	map<string, Function>::iterator func_it = Functions.find(signature);
	if (func_it == Functions.end())
		throw Simple_E(string("调用了未定义的函数") + element[0]);
	else {
		//初始化堆栈空间
		iniStack(cache,2);
		//在layer+1层运行函数
		run(func_it->second.ops);
		desStack();
		trueReturn(func_it->second.ret_type,signature);
	}

}
void CPU::pushCall(const string& func) {
	callFunc(func);
	if (ret.type == 'v')
		throw Simple_E("void值不能被使用");
	Stacks[layerNum - 1].push(ret);
	ret.type = 'v';
}
map<string, Function>::iterator CPU::newFunc(const string& func) {
	//q@[f_name],[r_type][p_type][p_type],[p_name],[p_name],
	vector<string> element(0);
	int t_index = 1;

	for (int index = 2; index < func.length(); index++)
		if (func[index] == ',') {
			element.push_back(func.substr(t_index + 1, index - t_index - 1));
			t_index = index;
		}
	string signature = element[0] + '|' + element[1].substr(1);
	map<string, Function>::iterator it = Functions.find(signature);
	if (it == Functions.end()) {
		Functions.insert(pair<string, Function>(signature, Function(vector<string>(element.begin() + 2, element.end()), element[1][0])));
		it = Functions.find(signature);
		//在函数中声明参数 
		//在函数中给参数赋值
		//第 i[1,n]个参数，类型element[1][i] 名称element[1+i]
		for (int i = 1; i < element[1].length(); i++)
		{
			
			it->second.ops.push_back(pair<int,string>(-1,string("q_") + element[1][i] + element[i + 1]));
			it->second.ops.push_back(pair<int, string>(-1, string("=") + element[i + 1] + ",?"));
		}
		return it;
	}
	else
		throw Simple_E(string("重定义函数：") + element[0]);

}

Var& CPU::decVar(const string& name, char type) {
	if (type == 'v')
		throw Compile_E("不能声明void型的变量");
	//declare var in layer
	int layer = layerNum - 1;
	map<string, Var >::iterator it = Scopes[layer].vars.find(name);
	if (it != Scopes[layer].vars.end())
		throw Simple_E(string("重定义变量:") + name);
	else 
		Scopes[layer].vars.insert(pair<string, Var>(name, Var(type)));
	return Scopes[layer].vars.find(name)->second;
}
Var CPU::getVar(const string& name) {
	Var ret = getVar_(name);
	return ret;
}
Var& CPU::getVar_(const string&name) {
	int layer = layerNum - 1;
	if (name[0] == '?')
		return popStack();
	else if (name[0] <= '9' && name[0] >= '0') {
		return vNum(name);
	}
	else if (name[0] == '\"') {
		//字符串
		return vString(Escape(name.substr(1, name.length() - 2)));
	}
	layer = findVar(name);
	if (layer == -1)
		return decVar(name, 'i');
	else
		return Scopes[layer].vars.find(name)->second;
}

void CPU::iniStack(stack<Var> & cache, char level) {
	iniStack(level);
	while (!cache.empty()) {
		Stacks[layerNum-1].push(cache.top());
		cache.pop();
	}
}
void CPU::iniStack(char level) {
	if (layerNum == Scopes.size()) {
		Scopes.push_back(Scope());
		Stacks.push_back(stack<Var>());
	}

	Scopes[layerNum].level = level;
	layerNum++;
}
void CPU::desStack() {
	layerNum--;
	for (auto var : Scopes[layerNum].vars)
		if (var.second.type == 's')
			delete var.second.var.str_p;
	Scopes[layerNum].vars.clear();
	while (!Stacks[layerNum].empty()) {
		throw Simple_E("我感觉运行到这就算是错了,这一个作用域的程序都运行完了，还有临时的变量，那怕是GG.");
		Stacks[layerNum].pop();
	}
}
Var CPU::popStack() {
	if (layerNum < 1)
		throw Exe_E("栈被异常清空");
	if (Stacks[layerNum - 1].empty()) {
		throw Exe_E("尝试从空栈取栈顶。");
		return Var('v');
	}
	Var var = Stacks[layerNum - 1].top();
	Stacks[layerNum - 1].pop();
	return var;
}

void CPU::IF(const vector<pair<int,string>>& ops, int& start) {
	bool ifFlag;
	bool hasElse;
	int if_ = start,else_ = ops.size();
	vector<pair<int,string>> nextOps(0);
	ifFlag = Bool(getVar(ops[start].second.substr(2)));
	int count = 1;
	for (start++; start < ops.size(); start++) {
		if (ops[start].second[0] == '0') {
			if (ops[start].second[1] == 'i'||
				ops[start].second[1] == 'e'||
				ops[start].second[1] == 'w'||
				ops[start].second[1] == 'd') count++;
			else if (ops[start].second[1] == '}') {
				count--;
				if (count == 0) break;
			}
		}
	}
	start++;
	if (ops[start].second[0] == '0' && ops[start].second[1] == 'e') {//有else
		else_ = start;
		hasElse = true;
		count = 1;
		for (start++; start < ops.size(); start++) {
			if (ops[start].second[0] == '0') {
				if (ops[start].second[1] == 'i' ||
					ops[start].second[1] == 'e' ||
					ops[start].second[1] == 'w' ||
					ops[start].second[1] == 'd') count++;
				else if (ops[start].second[1] == '}') {
					count--;
					if (count == 0) break;
				}
			}
		}
	}
	else {
		start--;//无else
		hasElse = false;
	}
	int begin = if_ + 1, end = start;
	if (ifFlag) {
		if (hasElse)
			end = else_ - 1;
	}
	else {
		if (hasElse)
			begin = else_ + 1;
		else
			return;
	}
	while(begin<end)
		nextOps.push_back(ops[begin++]);
	iniStack(1);
	run(nextOps);
	desStack();

}
void CPU::WHILE(const vector<pair<int,string>>&ops, int &start) {
	//while 分作两部分看
	//part 1 ()里的部分 + r‘
	//part 2 {}里的部分
	// 操作序列应该是 W part1 w part2 }
	//总是在当前栈 请求新栈 执行完后 取栈顶值做判断决定是否执行part 2
	vector<pair<int, string>> part1;
	vector<pair<int, string>> part2;
	start++;
	while (start < ops.size() && !(ops[start].second[0] == '0' && ops[start].second[1] == 'W'))
		part1.push_back(ops[start++]);
	string* name = nullptr;
	if (ops[start].second.length() != 2)
		name = new string(ops[start].second.substr(2));

	start++;
	int count = 1;
	for (; start < ops.size(); start++) {
		if (ops[start].second[0] == '0') {
			if (ops[start].second[1] == 'i' || 
				ops[start].second[1] == 'e' ||
				ops[start].second[1] == 'w' ||
				ops[start].second[1] == 'd') count++;
			else if (ops[start].second[1] == '}') {
				count--;
				if (count == 0) break;
			}
		}
		part2.push_back(pair<int,string>(ops[start].first,ops[start].second));
	}
	if (start >= ops.size())
		throw Other_E("while 嵌套异常。");
	bool whileFlag = true;
	while (true) {
		iniStack(3);
		run(part1);
		if (name == nullptr)
			whileFlag = Bool(popStack());
		else
			whileFlag = Bool(getVar(*name));
		desStack();
		if (whileFlag) {
			iniStack(3);
			run(part2);
			desStack();
		}
		else break;
		if (continueFlag)
			continueFlag = false;
		if (breakFlag) {
			breakFlag = false;
			break;
		}
	}
	delete name;
}
void CPU::DO(const vector<pair<int, string>>&ops, int & start) {
	//part1 是do里的部分;
	//part2 是until里的部分;
	vector<pair<int, string>> part1;
	vector<pair<int, string>> part2;
	//ops[start] == "0d{"
	//find until

	int count = 1;
	start++;
	for (; start < ops.size(); start++) {
		if (ops[start].second[0] == '0') {
			if (ops[start].second[1] == 'i' ||
				ops[start].second[1] == 'e' ||
				ops[start].second[1] == 'w' ||
				ops[start].second[1] == 'd') count++;
			else if (ops[start].second[1] == '}') {
				count--;
				if (count == 0) break;
			}
		}
		part1.push_back(ops[start]);
	}
	//ops[start] = "0}"
	start++;
	if (ops[start].second != "0u")
		throw Compile_E("缺少一个until语句");
	start++;
	while (start < ops.size() && !(ops[start].second[0] == '0' && ops[start].second[1] == 'U'))
		part2.push_back(ops[start++]);
	bool untilFlag;
	do
	{
		iniStack(3);
		run(part1);
		desStack();
		if (continueFlag)
			continueFlag = false;
		if (breakFlag) {
			breakFlag = false;
			break;
		}
		iniStack(3);
		run(part2);
		untilFlag = Bool(popStack());
		desStack();
	} while (!untilFlag);

}
int CPU::firstInte(const string& op) {
	bool in = false;
	for (int i = 1; i < op.length(); i++) {
		if (in) {
			if (op[i] == '\"' && op[i - 1] != '\\')
				in = false;
		}
		else if (op[i] == '\"')
			in = true;
		else if (op[i] == ',') return i;
	}
	return -1;
}

void CPU::run(const vector<pair<int,string>>& ops) {

	for (int i = 0,layer = layerNum - 1; i < ops.size(); i++) {
		try {

			nowLine = ops[i].first;
			Wait(nowLine);
			if (returnFlag){
				if (Scopes[layer].level%2==0)
					returnFlag = false;
				break;
			}
			if (continueFlag||breakFlag) {
				break;
			}
			if (ops[i].second[0] == '0') {
				switch(ops[i].second[1])
				{
				case 'i':
					IF(ops, i);
					break;
				case 'w':
					WHILE(ops, i);
					break;
				case 'r':
					ret.type = 'v';
					returnFlag = true;
					break;
				case '}':
					// 当前是 main 0 的话 报错
					// 当前是 func 2 的话 报错
					// 当前是 if-eles 1 的话 判断之后是否还有语句 有则报错 无则跳出
					// 同上
					if (Scopes[layer].level % 2 == 0)
						throw Compile_E("函数必须有return语句");
					else //if(i!=ops.size()-1)
						throw Compile_E("有不能匹配的{或}");
					break;
				case 'b':
					if (Scopes[layer].level%2 == 0)
						throw Compile_E("break语句不能在非循环中。");
					else
						 breakFlag = true;
					break;
				case 'c':
					if (Scopes[layer].level%2 == 0)
						throw Compile_E("continue语句不能在非循环中。");
					else
						continueFlag = true;
					break;
				case 'W':
					throw Other_E("while 部分处理异常.");
					break;
				case 'e':
					throw Runtime_E("有不能匹配到if的else");
					break;
				case 'd':
					DO(ops, i);
					break;
				case 'u':
					throw Runtime_E("有单独的until语句");
					break;
				case 'U':
					throw Other_E("do~until 部分处理异常");
					break;
				default:
					throw Other_E(string("不能识别的操作：")+ops[i].second);
				}
			}
			else
				run(ops[i].second);
		}
		catch (Simple_E e) {
			throw Runtime_E(ops[i].first,e);
		}
		catch (Compile_E e) {
			if (e.line == -1)
				e.line = ops[i].first;
			throw e;
		}
		catch (Runtime_E e) {
			if (e.line == -1)
				e.line = ops[i].first;
			throw e;
		}
		catch (Other_E e) {
			if (e.line == -1)
				e.line = ops[i].first;
			throw e;
		}
		catch (Exe_E e) {
			throw e;
		}
		catch (...) {
			throw Exe_E("出现了完全未知的bug");
		}
	}
}
void CPU::assVar(Var& v1, const Var & v2) {
	InitCheck(v2);
	if (v1.type == 'i') v1 = vInt(v2);
	else if (v1.type == 'r') v1 = vFloat(v2);
	else v1 = vString(v2);
	v1.init = true;
}

void CPU::OpWith2(const string& op) {
	stack<Var>& stk = Stacks[layerNum - 1];
	int i = firstInte(op); 
	Var v2 = getVar(op.substr(i + 1));
	if (op[0] == '=')//赋值特殊处理
	{
		assVar(getVar_(op.substr(1, i - 1)), v2);
		return;
	}
	Var v1 = getVar(op.substr(1, i - 1));
	switch (op[0])
	{
	case '+':// +
		stk.push(Plus(v1, v2));
		break;
	case '-':// -
		stk.push(Minus(v1, v2));
		break;
	case '*':// *
		stk.push(Mul(v1, v2));
		break;
	case '/':// /
		stk.push(Div(v1, v2));
		break;
	case '%': // %
		stk.push(Mod(v1, v2));
		break;
	case '^':// power
		stk.push(Pow(v1, v2));
		break;
	case '$': // $
		stk.push(Connect(v1, v2));
		break;
	case '#': // #
		stk.push(Delete(v1, v2));
		break;
	case ')': // >=
		stk.push(GEqual(v1, v2));
		break;
	case '<': // <
		stk.push(Less(v1, v2));
		break;
	case '(': // <=
		stk.push(LEqual(v1, v2));
		break;
	case '>': // >
		stk.push(Greater(v1, v2));
		break;
	case 'E': // ==
		stk.push(Equal(v1, v2));
		break;
	case 'N': // ！=
		stk.push(NEqual(v1, v2));
		break;
	case '&': // &&
		stk.push(And(v1, v2));
		break;
	case '|': // ||
		stk.push(Or(v1, v2));
		break;
	case 'O': //
		Out(v1, v2);
		break;
	default:
		throw Other_E(string("不能识别的操作："+op));
	}

}
void CPU::OpWith1(const string& op) {
	//输入特别处理
	if (op[1] == 'I') {
		In(getVar_(op.substr(2)));
		return;
	}
	Var v = getVar(op.substr(2));
	stack<Var>& stk = Stacks[layerNum - 1];
	static int i = 0;
	switch (op[1])
	{
	case 'z':
		Pos(v);
		stk.push(v);
		break;
	case 'f':
		stk.push(Neg(v));
		break;
	case '!':
		stk.push(Not(v));
		break;

	case 'O':
		Out(v);
		break;
	case 'r':			
		ret = v;
		returnFlag = true;
		break;
	case 'p':
		stk.push(v);
		break;
	default:
		throw Other_E(string("不能识别的操作:")+op);
		break;
	}
}
void CPU::OpWithq(const string&op) {
	switch (op[1])
	{
	case '@':
		throw Runtime_E("不允许函数内声明函数");
		break;
	case '_':
		decVar(op.substr(3), op[2]);
		break;
	case ':':
		callFunc(op);
		break;
	case ';':
		pushCall(op);
		break;
	default:
		throw Other_E(string("不能识别的操作:") + op);
		break;
	}
}


void CPU::run(const string&op) {
	stack<Var>& stk = Stacks[layerNum - 1];
	switch (op[0])
	{
	case '1':
		OpWith1(op);
		break;
	case 'q':
		OpWithq(op);
		break;
	case '0':
		throw Exe_E("if-else,while,函数,三者嵌套出现异常");
		break;
	default:
		OpWith2(op);
		break;
	}
}
void CPU::clearWait() {
	wait.clear();
}
void CPU::delWait(int line) {
	auto final_it = line_map_of.find(line);
	if (final_it == line_map_of.end())
		throw Exe_E("不能删除位于原文第" + toString(line) + "行的断点，因为这一行对于程序而言是空行.");
	auto wait_it = wait.find(final_it->second);
	if (wait_it == wait.end())
		throw Exe_E("不能删除位于原文第" + toString(line) + "行的断点，因为这一行上没有断点.");
	else
		wait.erase(wait_it);
}
void CPU::addWait(int line) {
	//添加的原文行号
	auto final_it = line_map_of.find(line);
	if (final_it == line_map_of.end())
		throw Exe_E("不能于原文第" + toString(line) + "行添加断点，因为这一行对于程序而言是空行.");
	else if (origin_ops[final_it->second][0] == 'q' && origin_ops[final_it->second][1] == '@') {
		cout << "Warning:不能于原文第" + toString(line) + "行添加断点，因为这一行是声明函数，不会被执行，所以已经改为尝试将断点设置在下一行\n";
		if (line == maxOriginLine)
			cout << "Warning:由于当前行是最后一行，所以并未在下一行添加断点\n";
		else
			addWait(line + 1);
		return;
	}
	auto wait_it = wait.find(final_it->second);
	if (wait_it == wait.end())
		wait.insert(final_it->second);
	else
		throw Exe_E("不能于原文第" + toString(line) + "行添加断点，因为这一行已经有一个断点.");

}
void CPU::nextLine() {
	lastOriginLine = getNowLine();
	runnable = true;
	tempWait = -1;
}
void CPU::nextStep() {
	tempWait = nowLine + 1;
	runnable = true;
}
void CPU::nextPoint() {
	runnable = true;
	tempWait = -1;
}
Var* CPU::getValue(const string& name,int layer) {
	if (layer >= layerNum)
		throw Simple_E("提供了错误的layer.");
	layer = findVar(name, layer);
	if (layer == -1)
		return nullptr;
	else
		return &Scopes[layer].vars.find(name)->second;
}
int CPU::getMaxLayer() {
	return layerNum - 1;
}
int CPU::findVar(const string& name) {
	return findVar(name, layerNum - 1);
}
int CPU::findVar(const string& name,int layer){
	int ori_layer = layer;
	map<string, Var>::iterator it;

	do{
		it = Scopes[layer].vars.find(name);
		if (it != Scopes[layer].vars.end())
			return layer;
		else if (Scopes[layer].level & 1)
			layer--;
		else
			return -1;
	} while (true);
}
bool CPU::needWait(int line) {
	if (line == -1) return false;
	else if (lastOriginLine != -1 && lastOriginLine != getNowLine()) {
		lastOriginLine = -1;
		return true;
	}
	else if (wait.find(line) != wait.end()) return true;
	else return line == tempWait;
}
void CPU::Wait(int line) {
	if (needWait(line)) {
		//while (!runnable) _sleep(500);
		cout << "已暂停于第"<<getNowLine()<<"行前，若要查看变量的值，输入 \"v n 变量名1 变量名 ,...\"(其中n表示要查看变量的数目)\n"
			<< "若要执行下一行 输入 \"l\",若要执行到下一个断点 输入 \"b\",若要执行该行下一步骤 输入\"s\"\n"
			<< "暂时不提供运行过程中断点的添加或删除\n"
			<< "若要退出程序 输入 \"q\"\n";
		do {

			char input;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> input;
			if (input == 'l') {
				nextLine();
				break;
			}
			else if (input == 'q')
				throw Runtime_E("用户要求退出。");
			else if (input == 'b') {
				nextPoint();
				break;
			}
			else if (input == 'v') {
				int n = 0;
				cin >> n;
				string name;
				Var* v;
				for (int i = 0; i < n; i++) {
					cin >> name;
					v = getValue(name,getMaxLayer());
					cout << "name:" << name << "\t";
					if (v == nullptr) {
						cout <<"未定义\n";
						continue;
					}
					cout << "\ttype:";
					if (v->type == 'i') cout << "int";
					else if (v->type == 'r') cout << "real";
					else if (v->type == 's') cout << "string";
					else cout << "void或不能识别";
					cout << "\t是否已初始化：" << v->init;
					cout << "\t值：";
					if (v->init) {
						if (v->type == 'i') cout << v->var.Int;
						else if (v->type == 'r') cout << v->var.Real;
						else if (v->type == 's') cout << *v->var.str_p;
						else cout << "未定义";

					}
					else cout << "未定义";
					cout << endl;
				}
			}
			else if (input == 's') {
				nextStep();
				break;
			}
			else cout << "未知的命令,请重新";
			cout << "输入:\n";
		} while (true);

		runnable = false;
	}
	else;
}

void CPU::setLineMap(const vector<vector<int>> & lineMap) {
	//lineMap -> lineMap[midLine][0] = origin_line
	// f->m m->o
	map<int, int>::iterator mid_it;
	int originLine;
	for (int finalLine = 0; finalLine < origin_ops.size(); finalLine++){
		
		mid_it = line_map_fm.find(finalLine);
		if (mid_it == line_map_fm.end())
			throw Exe_E(string("执行行:") + toString(finalLine) + "不能找到对应中间行");
		originLine = lineMap[mid_it->second][0];
		maxOriginLine = maxOriginLine > originLine ? maxOriginLine : originLine;
		line_map_fo.insert(pair<int, int>(finalLine, originLine));
	}

	map<int, int>::iterator origin_it;
	map<int, int>::iterator final_it;
	for (int finalLine = 0; finalLine < origin_ops.size(); finalLine++){
		origin_it = line_map_fo.find(finalLine);
		if(origin_it == line_map_fo.end())
			throw Exe_E(string("执行行:") + toString(finalLine) + "不能找到对应原始行");

		final_it = line_map_of.find(origin_it->second);
		if (final_it == line_map_of.end())
			line_map_of.insert(pair<int, int>(origin_it->second, finalLine));
	}
}

int CPU::getNowLine() {
	return getLineByLine(nowLine);
}