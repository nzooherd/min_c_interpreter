#include<iostream>
#include<sstream>
#include"Exception_dec.h"
#include"DS_custom_dec.h"
Var::Var(char _type, _Var _var) :type(_type), init(true), var(_var) {};
Var::Var(char _type) :type(_type), init(false) { if (_type == 's') this->var.str_p = new string(); };

Function::Function(vector<string> _para, char ret) :para(_para), ret_type(ret), ops(){};

bool isNum(const Var& v) {
	if (v.type == 'v')
		throw Simple_E("void 型不能被使用");
	return v.type != 's';
}
bool isNum(const Var& v1, const Var& v2) {
	return isNum(v1) && isNum(v2);

}
char maxNumType(const Var& v1, const Var& v2) {
	if (isNum(v1, v2)) {
		if (v1.type == 'r' || v2.type == 'r') return 'r';
		else return 'i';
	}
	throw Simple_E("不能将字符型转为数值型");
}
char minNumType(const Var& v1, const Var& v2) {
	if (isNum(v1, v2)) {
		if (v1.type == 'i' || v2.type == 'i') return 'i';
		else return 'r';
	}
	throw Simple_E("不能将字符串转为数值型");
}
bool isSame(const Var& v1, const Var& v2) {
	return !(isNum(v1) ^ isNum(v2));
}

short Int(const Var& v) {
	return (short)Float(v);
}
float Float(const Var& v) {
	InitCheck(v);
	if (isNum(v)) {
		if (v.type == 'i') return v.var.Int;
		else return v.var.Real;
	}
	else
		throw Simple_E("字符串不能转换成数值型");
}
bool Bool(const Var& v) {
	return Int(v) != 0?true : false;
}
string String(const Var& v) {
	InitCheck(v);
	if (v.type == 'v')
		throw "void型不能被使用";
	std::stringstream ss;
	if (v.type == 's') return string(*v.var.str_p);
	else if (v.type == 'i')
		ss << v.var.Int;
	else
		ss << v.var.Real;
	return ss.str();
}

void InitCheck(const Var& v) {
	if (!v.init)
		throw "使用了未初始化的变量";

}

Var vFloat(const Var& v) {
	InitCheck(v);
	return vFloat(Float(v));
}
Var vFloat(float v) {
	_Var ansVar;
	ansVar.Real = v;
	return Var('r', ansVar);
}
Var vInt(const Var& v) {
	InitCheck(v);
	return vInt(Int(v));
}
Var vInt(short v) {
	_Var ansVar;
	ansVar.Int = v;
	return Var('i', ansVar);
}
Var vBool(const Var& v) {
	InitCheck(v);
	return vBool(Int(v) != 0);
}
Var vBool(bool v) {
	_Var ansVar;
	ansVar.Int = (short)(v? 1 : 0);
	return Var('i', ansVar);
}
Var vString(const Var& v) {
	InitCheck(v);
	return vString(String(v));
}
Var vString(const string& v) {
	_Var ansVar;
	ansVar.str_p = new string(v);
	return Var('s', ansVar);
}

Var Plus(const Var& v1, const Var& v2) {
	InitCheck(v2);
	InitCheck(v2);
	char ansType = maxNumType(v1, v2);
	_Var ansVar;
	if (ansType == 'i') ansVar.Int = v1.var.Int + v2.var.Int;
	else ansVar.Real = Float(v1) + Float(v2);
	return Var(ansType, ansVar);
}
Var Minus(const Var& v1, const Var& v2) {
	InitCheck(v1);
	InitCheck(v2);
	char ansType = maxNumType(v1, v2);
	_Var ansVar;
	if (ansType == 'i') ansVar.Int = v1.var.Int - v2.var.Int;
	else ansVar.Real = Float(v1) - Float(v2);
	return Var(ansType, ansVar);
}
Var Mul(const Var& v1, const Var& v2) {
	InitCheck(v1);
	InitCheck(v2);
	char ansType = maxNumType(v1, v2);
	_Var ansVar;
	if (ansType == 'i') ansVar.Int = v1.var.Int * v2.var.Int;
	else ansVar.Real = Float(v1) * Float(v2);
	return Var(ansType, ansVar);
}
Var Div(const Var& v1, const Var& v2) {
	InitCheck(v1);
	InitCheck(v2);
	char ansType = maxNumType(v1, v2);
	_Var ansVar;
	if (ansType == 'i') ansVar.Int = v1.var.Int / v2.var.Int;
	else ansVar.Real = Float(v1) / Float(v2);
	return Var(ansType, ansVar);
}
Var Mod(const Var& v1, const Var& v2) {
	return vInt(Int(v1) % Int(v2));
}

Var Connect(const Var& v1, const Var& v2) {
	return vString(String(v1) + String(v2));
}
Var Delete(const Var& v1, const Var& v2) {
	return Delete(String(v1), String(v2));
}
Var Delete(const string& s1, const string& s2) {

	int n = s1.length();
	bool* bp = new bool[n];
	int i = 0, start = 0;
	for (i = 0; i < n; i++)
		bp[i] = true;
	while (start != string::npos) {
		start = s1.find(s2, start);
		for (i = 0; i < s2.length(); i++) bp[start + i] = false;
	}

	string ns;
	for (i = 0; i < n; i++)
		if (bp[i])
			ns.push_back(s1[i]);
	return vString(ns);
}

Var Equal(const Var& v1, const Var& v2) {
	InitCheck(v1);
	InitCheck(v2);
	if (isSame(v1, v2)) {
		if (v1.type == 's')
			return vBool(*v1.var.str_p == *v2.var.str_p);
		else 
			return vBool(Float(v1) == Float(v2));
	}
	else
		throw "不同类型不能判断是否相等";
}
Var NEqual(const Var& v1, const Var& v2) {
	return Not(Equal(v1, v2));
}
Var Greater(const Var& v1, const Var& v2) {
	return vBool(Float(v1) > Float(v2));
}
Var Less(const Var& v1, const Var& v2) {
	return vBool(Float(v1) < Float(v2));
}
Var GEqual(const Var& v1, const Var& v2) {
	return vBool(Float(v1) >= Float(v2));
}
Var LEqual(const Var& v1, const Var& v2) {
	return vBool(Float(v1) <= Float(v2));
}

Var And(const Var& v1, const Var& v2) {
	return vBool(Bool(v1) && Bool(v2));
}
Var Or(const Var& v1, const Var& v2) {
	return vBool(Bool(v1) || Bool(v2));
}
Var Not(const Var& v) {
	Var var = vBool(v);
	var.var.Int = 1 - var.var.Int;
	return var;
}

void Print(const Var& v) {
	if (v.type == 's') std::cout << *v.var.str_p;
	else if (v.type == 'i') std::cout << v.var.Int;
	else std::cout << v.var.Real;
}
short toInt(const string& str) {
	short ret = 0;
	int i = 0;
	bool neg = false;
	if (str[0] == '-') {
		neg = true;
		i++;
	}
	else if (str[0] == '+')
		i++;
	for(;i<str.length();i++){
		ret *= 10;
		ret += str[i] - '0';
	}
	return ret*(neg?-1:1);
}
float toFloat(const string& str) {
	float ret_int = 0;
	float ret_float = 0;
	int i = 0;
	bool neg = false;
	if (str[0] == '-') {
		neg = true;
		i++;
	}
	else if(str[0] == '+')
		i++;
	for (; str[i] != '.'; i++) {
		ret_int *= 10;
		ret_int += str[i] - '0';
	}
	for (int j = str.length() - 1; j > i; j--) {
		ret_float += str[j] - '0';
		ret_float /= 10;
	}
	return (ret_int + ret_float)*(neg?-1:1);
}
Var vNum(const string& str) {
	for (int i = 0; i < str.length(); i++)
		if (str[i] == '.')
			return vFloat(toFloat(str));
	return vInt(toInt(str));
}
string Escape(const string& str) {
	string ret;
	for (auto i = str.begin(); i != str.end();i++){
		if (*i == '\\') {
			if (*(i + 1) == '\\')
				ret.push_back('\\');
			else if (*(i + 1) == '\"')
				ret.push_back('\"');
			else if (*(i + 1) == 'a')
				ret.push_back('\a');
			else if (*(i + 1) == 'b')
				ret.push_back('\b');
			else if (*(i + 1) == 'f')
				ret.push_back('\f');
			else if (*(i + 1) == 'n')
				ret.push_back('\n');
			else if (*(i + 1) == 'r')
				ret.push_back('\r');
			else if (*(i + 1) == 't')
				ret.push_back('\t');
			else if (*(i + 1) == 'v')
				ret.push_back('\v');
			else
				throw "无此种转义字符";
			i++;
		}
		else ret.push_back(*i);
	}
	return ret;
}

void Pos(const Var& v) {
	InitCheck(v);
	if (!isNum(v))
		throw Simple_E("字符串不能换为数值型的值");
}
Var Neg(const Var& v) {
	if (v.type == 'i')
		return vInt(-Int(v));
	else
		return vFloat(-Float(v));
}