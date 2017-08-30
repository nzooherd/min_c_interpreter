#include"DS_custom_dec.h"
#include<iostream>
Var::Var(char _type, _Var _var) :type(_type), init(true), var(_var) {};
Var::Var(char _type) :type(_type), init(false) {};

Function::Function(vector<string> _para, char ret) :para(_para), ret_type(ret), ops() {};

bool isNum(const Var& v) {
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
	throw "不能将字符型转为数值型";
}
char minNumType(const Var& v1, const Var& v2) {
	if (isNum(v1, v2)) {
		if (v1.type == 'i' || v2.type == 'i') return 'i';
		else return 'r';
	}
	throw "不能将字符型转为数值型";
}
bool isSame(const Var& v1, const Var& v2) {
	return !(isNum(v1) ^ isNum(v2));
}

short Int(const Var& v) {
	return (short)Float(v);
}
float Float(const Var& v) {
	if (v.type == 'i') return v.var.Int;
	else if (v.type == 'r') return v.var.Real;
	else
		throw "字符型不能转换成数值型";
}
bool Bool(const Var& v) {
	return Int(v) != 0?true : false;
}

Var vInt(short i) {
	_Var ansVar;
	ansVar.Int = i;
	return Var('i', ansVar);
}
Var vBool(const Var& v) {
	return Bool(Int(v) != 0);
}
Var vBool(bool BOOL) {
	_Var ansVar;
	ansVar.Int = (short)(BOOL? 1 : 0);
	return Var('i', ansVar);
}

Var Plus(const Var& v1, const Var& v2) {
	char ansType = maxNumType(v1, v2);
	_Var ansVar;
	if (ansType == 'i') ansVar.Int = v1.var.Int + v2.var.Int;
	else ansVar.Real = Float(v1) + Float(v2);
	return Var(ansType, ansVar);
}
Var Minus(const Var& v1, const Var& v2) {
	char ansType = maxNumType(v1, v2);
	_Var ansVar;
	if (ansType == 'i') ansVar.Int = v1.var.Int - v2.var.Int;
	else ansVar.Real = Float(v1) - Float(v2);
	return Var(ansType, ansVar);
}
Var Mul(const Var& v1, const Var& v2) {
	char ansType = maxNumType(v1, v2);
	_Var ansVar;
	if (ansType == 'i') ansVar.Int = v1.var.Int * v2.var.Int;
	else ansVar.Real = Float(v1) * Float(v2);
	return Var(ansType, ansVar);
}
Var Div(const Var& v1, const Var& v2) {
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
	if (v1.type == 's' && v2.type == 's') {
		_Var ansVar;
		ansVar.str_p = &(*v1.var.str_p + *v2.var.str_p);
		return Var('s', ansVar);
	}
	else
		throw "数值型不能进行$运算";
}
Var Delete(const Var& v1, const Var& v2) {
	///
	if (isNum(v1, v2))
		throw "不能从数值型转换为字符串型";
	int n = v1.var.str_p->length();
	bool* bp = new bool[n];
	int i = 0, start = 0;
	for (i = 0; i < n; i++)
		bp[i] = true;
	while (start != string::npos) {
		start = v1.var.str_p->find(*v2.var.str_p, start);
		for (i = 0; i < v2.var.str_p->length(); i++) bp[start+i] = false;
	}
	_Var ansVar;
	ansVar.str_p = &string();
	for (i = 0; i < n; i++) 
		if(bp[i])
			ansVar.str_p->push_back(v1.var.str_p->at(i));
	return Var('s', ansVar);
}

Var Equal(const Var& v1, const Var& v2) {
	_Var var;
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