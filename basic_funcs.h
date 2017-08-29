#include<iostream>
#include<string>
#include<sstream>
using namespace std;

short add(short a, short b) {
	return a + b;
}

float add(short a, float b) {
	return a + b;
}

float add(float a, short b) {
	return a + b;
}

float add(float a, float b) {
	return a + b;
}

short sub(short a, short b) {
	return a - b;
}

float sub(short a, float b) {
	return a - b;
}

float sub(float a, short b) {
	return a - b;
}

float sub(float a, float b) {
	return a - b;
}

short multiple(short a, short b) {
	return a * b;
}

float multiple(short a, float b) {
	return a * b;
}

float multiple(float a, short b) {
	return a * b;
}

float multiple(float a, float b) {
	return a * b;
}


short divide(short a, short b) {
	return a / b;
}

float divide(short a, float b) {
	return a / b;
}

float divide(float a, short b) {
	return a / b;
}

float divide(float a, float b) {
	return a / b;
}

//%
short remainder(short a, short b) {
	return a % b;
}

//$
string attach(string a, string b) {
	return a + b;
}

string attach(short a, string b) {
	stringstream ss;
	ss << a;
	return ss.str() + b;
}

string attach(float a, string b) {
	stringstream ss;
	ss << a;
	return ss.str() + b;
}

string attach(string a, short b) {
	stringstream ss;
	ss << b;
	return  a + ss.str();
}

string attach(string a, float b) {
	stringstream ss;
	ss << b;
	return a + ss.str();
}

// #
string delete_sub(string s, string sub) {
	int pos = s.find(sub, 0);
	while (pos != string::npos) {
		s.erase(pos, sub.size());
		pos = s.find(sub, pos);
	}
	return s;
}

// []
string index(string s, short a) {
	return s.substr(a, 1);
}

string index(string s, short a, short b) {
	return s.substr(a, b - a + 1);
}

// && || !
short and(short a, short b) {
	return a && b;
}

short or (short a, short b) {
	return a || b;
}

short not(short a) {
	return !a;
}

//<
short smaller(short a, short b) {
	return a < b;
}

short smaller(float a, short b) {
	return a < b;
}

short smaller(short a, float b) {
	return a < b;
}

short smaller(float a, float b) {
	return a < b;
}

//>
short bigger(short a, short b) {
	return a > b;
}

short bigger(float a, short b) {
	return a > b;
}

short bigger(short a, float b) {
	return a > b;
}

short bigger(float a, float b) {
	return a > b;
}

// <=
short smaller_and(short a, short b) {
	return a <= b;
}

short smaller_and(float a, short b) {
	return a <= b;
}

short smaller_and(short a, float b) {
	return a <= b;
}

short smaller_and(float a, float b) {
	return a <= b;
}

//>=
short bigger_and(short a, short b) {
	return a >= b;
}

short bigger_and(float a, short b) {
	return a >= b;
}

short bigger_and(short a, float b) {
	return a >= b;
}

short bigger_and(float a, float b) {
	return a >= b;
}

// == 
short equal(short a, short b) {
	return a == b;
}

short equal(float a, short b) {
	return a == b;
}

short equal(short a, float b) {
	return a == b;
}

short equal(float a, float b) {
	return a == b;
}

// <>
short not_equal(short a, short b) {
	return a != b;
}

short not_equal(float a, short b) {
	return a != b;
}

short not_equal(short a, float b) {
	return a != b;
}

short not_equal(float a, float b) {
	return a != b;
}


// out
void out(short a) {
	printf("%d", a);
}

void out(float a) {
	printf("%f", a);
}

void out(string s) {
	cout << s;
}

void out(int a, string s) {
	while (a--) {
		cout << s;
	}
}

void out(string s1, string s2) {
	cout << s1 << s2;
}

//in
short in(short a) {
	scanf("%d", &a);
	return a;
}

float in(float a) {
	scanf("%f", &a);
	return a;
}

string in(string s) {
	scanf("%s", s);
	return s;
}

short in(string s, short a) {
	cout << s;
	scanf("%d", &a);
	return a;
}

float in(string s, float a) {
	cout << s;
	scanf("%f", &a);
	return a;
}

string in(string s1, string s2) {
	cout << s1;
	scanf("%s", s2);
	return s1;
}


