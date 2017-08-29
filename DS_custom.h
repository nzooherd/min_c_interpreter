#pragma once
union _Var {
	string* str_p;
	short Int;
	float Real;
};
struct Var {
	char type;//i r s
	bool init;
	_Var var;
	Var(char _type) :type(_type), init(false) {};
	Var(char _type, _Var _var) :type(_type), init(true), var(_var) {};
};
struct Scope {
	map<string, Var> vars;
	short level;
};
struct Function {
	vector<string> para;
	vector<string> ops;
	char ret_type;
	Function(vector<string> _para, char ret) :para(_para), ret_type(ret) {}
};