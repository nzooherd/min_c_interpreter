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
	cout << "�������ļ�·��:";
	cin >> filename;
	fin.open(filename, ios::in);

	if (fin.is_open()) {
		buffer << fin.rdbuf();
		fin.close();
		return buffer.str();
	}
	else
		throw Other_E("�ļ���ʧ�ܡ�\n");
	
}
int main() {
	string origin_text;
	vector<string> sentences;
	vector<vector<int>> ansLocation;
	CPU executioner;
	int result;
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

	}
	catch (Other_E e) {
		cout << e.reason<<endl;
		return 0;
	}
	catch (Compile_E e) {
		cout << "����ʧ��:" << e.reason;
		if (e.line != -1)
			cout << " ��ԭ�ĵ�"
			<< ansLocation[e.line][0]
			<< "��";
		cout << endl;
		return 0;
	}
	catch (...) {
		cout << "������δ֪���쳣��\n";
		return 0;
	}
	try {
		result = executioner.start();
	}
	catch (Compile_E e) {
		cout << "����ʧ��:" << e.reason;
		if (e.line != -1)
			cout << " ��ԭ�ĵ�"
			<< ansLocation[executioner.getLineByLine(e.line)][0]
			<< "��";
		cout << endl;

		return 0;
	}
	catch (Runtime_E e) {
		cout << "����ʱ�����˴���:" << e.reason;
		if (e.line != -1)
			cout << " ��ԭ�ĵ�"
			<< ansLocation[executioner.getLineByLine(e.line)][0]
			<< "��";
		cout << endl;

		return 0;
	}
	catch (Other_E e) {
		cout << "������δԤ�ϵ��Ĵ���:" << e.reason;
		if (e.line != -1)
			cout << " ��ԭ�ĵ�"
			<< ansLocation[executioner.getLineByLine(e.line)][0]
			<< "��";
		cout << endl;

		return 0;
	}
	catch (Exe_E e) {
		cout << "������δԤ�ϵ������ش���:" << e.reason << endl;
		cout << "����������������ִ�в��ִ��ھ޴�©����\n";

		return 0;
	}
	catch (...) {

		cout << "������δ֪���쳣��\n";
		return 0;
	}
	if (result != 0)
		cout << "main ����û�з���0�����Ƿ�����:" << result << endl;

	return 0;
}