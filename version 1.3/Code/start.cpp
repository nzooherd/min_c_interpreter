#include<iostream>
#include<string>
#include<fstream>
#include <sstream> 
#include"CPU_dec.h"
#include"DS_custom_dec.h"
#include"Exception_dec.h"
#include"JudgeSentence.h"
#include"PartWord.h"
#include"ProcessSentence.h"
#include"min_c_ljn.h"
using namespace std;
string fileInput() {
	string filename;
	stringstream buffer;
	ifstream fin;
	cout << "请输入文件路径:";
	cin >> filename;
	fin.open(filename, ios::in);

	if (fin.is_open()) {
		buffer << fin.rdbuf();
		fin.close();
		return buffer.str();
	}
	else
		throw Other_E("文件打开失败。\n");
	
}
int main() {
	string origin_text;
	vector<string> sentences;
	vector<vector<int>> ansLocation;
	CPU executioner;
	int result;
	int breakpoint;
	try {
		origin_text = fileInput();
		sentences = ReceiveFile(origin_text);
		ansLocation = getGlobal();
		for (int i = 0; i < sentences.size(); i++) {
			try {
				executioner.addOp(program(sentences[i]));
			}
			catch (Compile_E e) {
				if (e.line == -1)
					e.line = i;
				throw e;
			}
		}
		executioner.setLineMap(ansLocation);
		cout << "文件打开成功。\n";
		cout << "输入要增加断点的行号，并在输入0后结束输入。\n";
		do{
			cin >> breakpoint;
			if (breakpoint != 0)
				executioner.addWait(breakpoint);
			else
				break;
		} while (true);
	}
	catch (Other_E e) {
		cout << e.reason<<endl;
		return 0;
	}
	catch (Compile_E e) {
		cout << "编译失败:" << e.reason;
		if (e.line != -1)
			cout << " 于原文第"
			<< ansLocation[e.line][0]
			<< "行";
		cout << endl;
		return 0;
	}
	catch (Exe_E e) {
		cout << "出现了错误：" << e.reason << endl;
		return 0;
	}
	catch (...) {
		cout << "出现了未知的异常。\n";
		return 0;
	}
	try {
		result = executioner.start();
	}
	catch (Compile_E e) {
		cout << "编译失败:" << e.reason;
		if (e.line != -1)
			cout << " 于原文第"
			<< executioner.getLineByLine(e.line)
			<< "行";
		cout << endl;

		return 0;
	}
	catch (Runtime_E e) {
		cout << "运行时出现了错误:" << e.reason;
		if (e.line != -1)
			cout << " 于原文第"
			<< executioner.getLineByLine(e.line)
			<< "行";
		cout << endl;

		return 0;
	}
	catch (Other_E e) {
		cout << "出现了未预料到的错误:" << e.reason;
		if (e.line != -1)
			cout << " 于原文第"
			<< executioner.getLineByLine(e.line)
			<< "行";
		cout << endl;

		return 0;
	}
	catch (Exe_E e) {
		cout << "出现了未预料到的严重错误:" << e.reason << endl;
		cout << "这表明这个解释器的执行部分存在巨大漏洞。\n";

		return 0;
	}
	catch (...) {

		cout << "出现了未知的异常。\n";
		return 0;
	}
	if (result != 0)
		cout << "main 函数没有返回0，而是返回了:" << result << endl;

	return 0;
}