//
// Created by dudu on 9/4/17.
//

#ifndef CLIONPROJECT_EXCEPTION_DEC_H
#define CLIONPROJECT_EXCEPTION_DEC_H
#include<exception>
#include<string>
using std::string;
using std::exception;
class Simple_E :public exception {
public:
    Simple_E(string _reason);
    ~Simple_E();
    string reason;
};
class Runtime_E :public exception {
public:
    Runtime_E(int _line, Simple_E _reason);
    Runtime_E(int _line, string _reason);
    Runtime_E(string _reason);
    ~Runtime_E();
    string reason;
    int line;

};
class Compile_E:public exception
{
public:
    Compile_E(int line, Simple_E reason);
    Compile_E(string _reason);
    Compile_E(int line, string _reason);
    ~Compile_E();
    string reason;
    int line;

};
class Other_E :public exception
{
public:
    Other_E(int _line, Simple_E _reason);
    Other_E(int _line, string _reason);
    Other_E(string _reason);
    ~Other_E();
    string reason;
    int line;
};
class Exe_E :public exception
{
public:
    Exe_E(string _reason);
    string reason;
    ~Exe_E();
};

#endif //CLIONPROJECT_EXCEPTION_DEC_H
