//
// Created by dudu on 9/4/17.
//
#include <map>
#include "PartWord.h"
#include "JudgeSentence.h"
#include "ProcessSentence.h"
#include "Exception_dec.h"

namespace process {
	map<string, int> operPrec = { 
		{ "z", 8 },{ "f", 8 },{ "!", 8 },
		{ "^", 7 },
		{ "*", 6 },{ "/", 6 },{ "%", 6 },
		{ "+", 5 },{ "-", 5 },
		{ "$", 4 },{ "#", 4 },
		{ "<", 3 },{ "<=", 3 },{ ">", 3 },{ ">=", 3 },
		{ "==", 2 },{ "<>", 2 },
		{ "&&", 1 },{ "||", 0 }

	};
}

bool judgePreceRate(worder a, worder b)
{
	using process::operPrec;

	if (a.name == "^" && b.name == "^") return false;
	if ((*operPrec.find(a.name)).second - (*operPrec.find(b.name)).second >= 0) return true;
	else return false;
}

vector<worder> infixToPostfix(vector<worder> worders)  //����ת������ʽ
{
	vector<worder> post;
	stack<worder> sk;

	if (!judgeExpression(worders))
		throw Other_E("���ܽ�����ı��ʽתΪ��׺���ʽ");

	for (int i = 0; i < worders.size(); i++) {
		if (worders[i].type != OPERATER) {
			post.push_back(worders[i]);
			continue;
		}
		if (worders[i].type == OPERATER) {
			if (worders[i].name == "(") {
				sk.push(worders[i]);
			}
			else if (worders[i].name == ")") {
				while (!sk.empty() && sk.top().name != "(") {
					post.push_back(sk.top());
					sk.pop();
				}

				if (sk.empty()) {
					printf("Error brackets");

				}
				else sk.pop();
			}
			else {
				while (!sk.empty() && sk.top().name != "(" &&judgePreceRate(sk.top(), worders[i])) {
					post.push_back(sk.top());
					sk.pop();
				}
				sk.push(worders[i]);
			}
		}
	}
	while (!sk.empty()) {
		post.push_back(sk.top());
		sk.pop();
	}
	return post;
}

vector<string> processDeclartion(vector<worder> worders)  //�����������
{
	vector<string> result;
	for (int i = 1; i < worders.size(); i++) {
		if (i % 2 == 1) {
			string opt;
			opt += "q_";
			opt += worders[0].name[0];
			opt += worders[i].name;
			result.push_back(opt);
		}
	}
	return result;
}
vector<string> processIfElseWhileReturn(vector<worder> worders, int flag)   //����if,else,while,return���
{
	vector<string> result;
	if (flag == 1 || flag == 4) {
		if (worders.size() >= 3) {
			string flagIfOrReturn;
			if (flag == 1) flagIfOrReturn += "0i";
			if (flag == 4) flagIfOrReturn += "1r";
			vector<worder> exp;
			vector<string> retOfexp;
			for (int i = 1; i < worders.size() - 1; i++) {
				exp.push_back(worders[i]);
			}
			if (exp.size() == 1 && exp[0].type != FUNCTION) {
				flagIfOrReturn += exp[0].name;
			}
			else {
				retOfexp = processExpression(infixToPostfix(exp));
				for (int i = 0; i < retOfexp.size(); i++) {
					result.push_back(retOfexp[i]);
				}
				flagIfOrReturn += "?";
			}
			result.push_back(flagIfOrReturn);
		}
		else {
			string flagReturnVoid;
			flagReturnVoid += "0r";
			result.push_back(flagReturnVoid);
		}
	}
	else if (flag == 2) {
		string flagPre;
		string flagApd;
		flagPre += "0w";
		result.push_back(flagPre);
		if (worders.size() != 3) {
			vector<worder> exp;
			vector<string> retOfexp;

			for (int i = 1; i < worders.size() - 1; i++) {
				exp.push_back(worders[i]);
			}
			retOfexp = processExpression(infixToPostfix(exp));
			for (int i = 0; i < retOfexp.size(); i++) {
				result.push_back(retOfexp[i]);
			}
			flagApd += "0W";
		}
		else {
			flagApd += "0W";
			flagApd += worders[1].name;
		}
		result.push_back(flagApd);
	}
	else if (flag == 3) {
		string flagElse;
		flagElse += "0e";
		result.push_back(flagElse);
	}
	return result;
}
vector<string> processRightBigBracket(vector<worder> worders)  //����������
{
	vector<string> result;
	string word = "0}";
	result.push_back(word);
	return result;
}
vector<string> processFunction(vector<worder> worders, int flag)   //��������flag = 1��ʾ��������, flag = 2 ��ʾ���ú�����ջ��flag = 3��ʾ���ú�������ջ
{
	string op = "q";
	vector<string> add;
	if (flag == 1) {
		op.push_back('@');
		int i = 0;
		while (worders[1].name[i] != '(') op.push_back(worders[1].name[i++]);
		op.push_back(',');
		op.push_back(worders[0].name[0]);
		for (auto words : processArgumentsOfFunction(worders[1])) {
			if (words.size() == 0) break;
			op.push_back(words[0].name[0]);
			add.push_back(words[1].name);
		}
		op.push_back(',');
		for (auto it = add.begin(); it != add.end(); it++) {
			op += *it;


			op.push_back(',');
		}
		return vector<string>{op};
	}
	else {
		if (flag == 2) op.push_back(';');
		else op.push_back(':');
		int i = 0;
		while (worders[0].name[i] != '(')op.push_back(worders[0].name[i++]);
		op.push_back(',');
		vector<string> ret;
		for (auto each_arg : processArgumentsOfFunction(worders[0])) {
			if (each_arg.size() == 0) return vector<string>{op};
			else if (each_arg.size() == 1 && each_arg[0].type != FUNCTION)
				op += each_arg[0].name;
			else {
				add = processExpression(infixToPostfix(each_arg));
				ret.insert(ret.end(), add.begin(), add.end());
				op.push_back('?');
			}
			op.push_back(',');
		}
		ret.push_back(op);
		return ret;
	}
}
vector<string> processExpression(vector<worder> worders) //������ʽ
{
	vector<string> result;
	stack<worder> idention, operation;
	if (worders.empty()) return result;
	if (worders.size() == 1 && worders[0].type == FUNCTION) return processFunction(worders, 2);
	if (worders.size() == 1) {
		string opt = "1p";
		opt += worders[0].name;
		result.push_back(opt);
	}
	for (int i = 0; i < worders.size(); i++) {
		worder word = worders[i];
		if (word.type != OPERATER) {
			idention.push(word);
		}
		else {
			string opt;
			if (word.name == "z" || word.name == "f" || word.name == "!") {
				opt += "1";
				opt += word.name;
				worder topword = idention.top();
				if (topword.type == FUNCTION) {
					vector<string> temp = processFunction(vector<worder>(1, topword), 2);
					result.insert(result.end(), temp.begin(), temp.end());
					opt += "?";
				}
				if (topword.type != FUNCTION) opt += topword.name;
			}
			else {
				if (word.name != "==" && word.name != "<>" && word.name != "<=" && word.name != ">=") opt += word.name[0];
				else {
					if (word.name == "==") opt += "E";
					else if (word.name == "<>") opt += "N";
					else if (word.name == "<=") opt += "(";
					else if (word.name == ">=") opt += ")";
				}
				worder top1word = idention.top();
				idention.pop();
				worder top2word = idention.top();
				idention.pop();
				vector<string> temp_;
				if (top2word.type == FUNCTION) {
					temp_ = processFunction(vector<worder>(1, top2word), 2);
					result.insert(result.end(), temp_.begin(), temp_.end());
				}
				if (top1word.type == FUNCTION) {
					temp_ = processFunction(vector<worder>(1, top1word), 2);
					result.insert(result.end(), temp_.begin(), temp_.end());
				}
				if (top2word.type == FUNCTION) opt.push_back('?');
				else opt += top2word.name;
				opt.push_back(',');
				if (top1word.type == FUNCTION) opt.push_back('?');
				else opt += top1word.name;
			}
			result.push_back(opt);
			idention.push(worder("?", OPERATER));
		}
	}
	return result;
}
vector<string> processIn(const vector<worder>& worders)  //�����������
{
	vector<string> result;
	string singleOpt = "1I";
	if (worders[1].type == IDENTIFIER)
		singleOpt += worders[1].name;
	else {
		vector<worder> worderOfout;
		worderOfout.push_back(worder("out",JUDGE));
		worderOfout.push_back(worders[1]);
		worderOfout.push_back(worder(";",OPERATER));

		for (auto str : processOut(worderOfout))
			result.push_back(str);
		singleOpt += worders[3].name;
	}
	result.push_back(singleOpt);
	return result;
}
vector<string> processOut(const vector<worder>& worders)  //����������
{
	vector<string> result;
	string process = "O";
	int flag = 0;
	for (int i = 0; i < worders.size(); i++)
		if (worders[i].name == ",") flag++;
	if (flag == 0) {
		vector<worder> singleOpt(worders.begin() + 1,worders.end()-1);
		process.insert(0, "1");
		if (singleOpt.size() == 1 && singleOpt[0].type != FUNCTION)
			process += singleOpt[0].name;
		else {
			for (auto exp : processExpression(infixToPostfix(singleOpt)))
				result.push_back(exp);
			process.push_back('?');
		}
	}
	else {
		flag = 0;
		vector<worder> exp1;
		vector<worder> exp2;
		for (int i = 1; i < worders.size() - 1; i++) {
			if (!flag && worders[i].name != ",") {
				exp1.push_back(worders[i]);
			}
			if (flag || worders[i].name == ",") {
				flag = 1;
				if (worders[i].name != ",") exp2.push_back(worders[i]);
			}
		}
		bool flag1 = false, flag2 = false;
		if (!(exp1.size() == 1 && exp1[0].type != FUNCTION)) {
			vector<string> processOpt1 = processExpression(infixToPostfix(exp1));
			for (int i = 0; i < processOpt1.size(); i++) {
				result.push_back(processOpt1[i]);
			}
			flag1 = true;
		}
		if (!(exp2.size() == 1 && exp2[0].type != FUNCTION)) {
			vector<string> processOpt2 = processExpression(infixToPostfix(exp2));
			for (int i = 0; i < processOpt2.size(); i++) {
				result.push_back(processOpt2[i]);
			}
			flag2 = true;
		}
		if (flag1) process.push_back('?');
		else process += exp1[0].name;
		process.push_back(',');
		if (flag2) process.push_back('?');
		else process += exp2[0].name;
	}
	result.push_back(process);
	return result;
}
vector<string> processAssign(vector<worder> worders)
{
	vector<string> result;
	vector<worder> idents;
	vector<worder> exp;
	int flag = 0;
	for (int i = 0; i <= worders.size(); i++) {
		if (worders[i].type == IDENTIFIER && worders[i + 1].name == "=") {
			idents.push_back(worders[i]);
			i++;
		}
		else {
			flag = i;
			break;
		}
	}
	for (int i = flag; i < worders.size() - 1; i++) {
		exp.push_back(worders[i]);
	}
	if (exp.size() == 1 && exp[0].type != FUNCTION) {
		for (int i = idents.size() - 1; i >= 0; i--) {
			string singleOpt;
			singleOpt += "=";
			singleOpt += idents[i].name;
			singleOpt += ",";
			if (i == idents.size() - 1) singleOpt += exp[0].name;
			else singleOpt += idents[i + 1].name;
			result.push_back(singleOpt);
		}

	}
	else {
		vector<string> resultOfExp = processExpression(infixToPostfix(exp));
		for (int i = 0; i < resultOfExp.size(); i++) {
			result.push_back(resultOfExp[i]);
		}
		for (int i = idents.size() - 1; i >= 0; i--) {
			string singleOpt;
			singleOpt += "=";
			singleOpt += idents[i].name;
			singleOpt += ",";
			if (i == idents.size() - 1) singleOpt += "?";
			else singleOpt += idents[i + 1].name;
			result.push_back(singleOpt);
		}
	}
	return result;
}

vector<string> program(string sentence)
{
	char flag = sentence[sentence.size() - 1];
	sentence = sentence.substr(0, sentence.size() - 1);
	vector<worder> worders = complateSepSentence(sentence);
	if (flag == 'a') {
		if (judgeFunction(worders) != 1)
			throw Compile_E("The Sentence is not a DECLARAT FUNCTION");
		return processFunction(worders, 1);
	}
	else if (flag == 'b') {
		if (!judgeMain(worders))
			throw Compile_E("The Sentence is not MAIN FUNCTION");
		return processFunction(worders, 1);
	}
	else if (flag == 'c') {
		if (judgeIfElseWhileReturn(worders) != 1)
			throw Compile_E("The Sentence is not IF Sentence");
		return processIfElseWhileReturn(worders, 1);
	}
	else if (flag == 'd') {
		if (judgeIfElseWhileReturn(worders) != 2)
			throw Compile_E("The Sentence is not WHILE Sentence");
		return processIfElseWhileReturn(worders, 2);
	}
	else if (flag == 'e') {
		if (judgeIfElseWhileReturn(worders) != 4)
			throw Compile_E("The Sentence is not RETUEN Sentence");
		return processIfElseWhileReturn(worders, 4);
	}
	else if (flag == 'f') {
		if (judgeIfElseWhileReturn(worders) != 3)
			throw Compile_E("The Sentence is not ElSE Sentence");
		return processIfElseWhileReturn(worders, 3);
	}
	else if (flag == 'g') {
		if (!judgeSingleRightBracket(worders))
			throw Compile_E("The Sentence is not RIGHT BRACKET Sentence");
		return processRightBigBracket(worders);
	}
	else if (flag == 'h') {
		if (judgeInAndOut(worders) != 1)
			throw Compile_E("The Sentence is not IN Sentence");
		return processIn(worders);
	}
	else if (flag == 'i') {
		if (judgeInAndOut(worders) != 2)
			throw Compile_E("The Sentence is not OUT Sentence");
		return processOut(worders);
	}
	else if (flag == 'j') {
		if (!judgeAssign(worders))
			throw Compile_E("The Sentence is not ASSIGN Sentence");
		return processAssign(worders);
	}
	else if (flag == 'k') {
		if (judgeFunction(worders) != 2)
			throw Compile_E("The Sentence is not FUNCTION Sentence");
		return processFunction(worders, 3);
	}
	else if (flag == 'l') {
		if (!judgeDeclartion(worders))
			throw Compile_E("The Sentence is not DECLARATION Sentence");
		return processDeclartion(worders);
	}
	else if (flag == 'm')
		return vector<string>{"0c"};
	else if (flag == 'n')
		return vector<string>{ "0b" }; 
	else if (flag == 'o')
		return processDo(worders);
	else if (flag == 'p') {
		if (!judgeUntil(worders))
			throw Compile_E("The Sentence is not UNTIL Sentence");
		return processUntil(worders);
	}
	throw Compile_E(string("Unknown Sentence:")+sentence);
}

vector<string> processDo(vector<worder> worders)
{
	vector<string> result;
	result.push_back("0d");
	return result;
}
vector<string> processUntil(vector<worder> worders)
{
	vector<string> result;
	result.push_back("0u");
	vector<worder>::iterator k = worders.begin();
	worders.erase(k);
	worders.pop_back();
	vector<string> rsOfExp = processExpression(infixToPostfix(worders));
	for (int i = 0; i < rsOfExp.size(); i++)
		result.push_back(rsOfExp[i]);
	result.push_back("0U");
	return result;

}