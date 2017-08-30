#pragma once
#include<vector>
#include<string>
#include<map>
using std::vector;
using std::string;
using std::map;
union _Var {
	string* str_p;
	short Int;
	float Real;
};
struct Var {
	char type;//i r s
	bool init;
	_Var var;
	Var(char _type);
	Var(char _type, _Var _var);
};

struct Scope {
	map<string, Var> vars;
	short level;
};
struct Function {
	vector<string> para;
	vector<string> ops;
	char ret_type;
	Function(vector<string> _para, char ret);
};
bool isNum(const Var& v);
bool isNum(const Var& v1, const Var& v2);
char maxNumType(const Var& v1, const Var& v2);
char minNumType(const Var& v1, const Var& v2);
bool isSame(const Var& v1, const Var& v2);

float Float(const Var& v);
short Int(const Var& v);
bool Bool(const Var& v);

Var vBool(const Var& v);
Var vBool(bool BOOL);

Var Plus(const Var& v1, const Var& v2);
Var Minus(const Var& v1, const Var& v2);
Var Mul(const Var& v1, const Var& v2);
Var Div(const Var& v1, const Var& v2);
Var Mod(const Var& v1, const Var& v2);

Var Connect(const Var& v1, const Var& v2);
Var Delete(const Var& v1, const Var& v2);

Var Equal(const Var& v1, const Var& v2);
Var NEqual(const Var& v1, const Var& v2);
Var Greater(const Var& v1, const Var& v2);
Var Less(const Var& v1, const Var& v2);
Var GEqual(const Var& v1, const Var& v2);
Var LEqual(const Var& v1, const Var& v2);

Var And(const Var& v1, const Var& v2);
Var Or(const Var& v1, const Var& v2);
Var Not(const Var& v);

void Print(const Var& v);