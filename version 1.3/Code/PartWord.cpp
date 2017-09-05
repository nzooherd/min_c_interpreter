//
// Created by dudu on 9/4/17.
//

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include "PartWord.h"
#include "Exception_dec.h"

vector<string> separateSentence(string sentence)        //简单分词
{
    if(sentence[sentence.size()-1] != ' ') sentence += ' ';
    char operateSysmbol[22] = {
            '{','}','(',')','+','-','*','/','>','<','&','$','#','!','|','=',';','"','%',','
    };

    vector<string> wordVec;
    set<char> flag(operateSysmbol, operateSysmbol + 22);
    string word;

    for(int i = 0; i < sentence.size(); i++) {
        if(sentence[i] == '"' && (i == 0 || sentence[i-1] != '\\')) {

            if(!word.empty()) wordVec.push_back(word);

            word.clear();
            word += sentence[i];
            while(sentence[++i] != '"') {
                word += sentence[i];
            }
            word += sentence[i];
            wordVec.push_back(word);
            word.clear();
            continue;
        }

        if(i == sentence.size() - 1) {
            if(!word.empty()) wordVec.push_back(word);
            word.clear();
            break;
        }

        if(sentence[i] == ' ' ) {
            string wordNew = word;
            if(!word.empty()) wordVec.push_back(wordNew);
            word.clear();
        }
        else if(flag.find(sentence[i]) != flag.end()) {
            string wordNew1 = word;
            if(!wordNew1.empty()) wordVec.push_back(wordNew1);
            word.clear();

            word += sentence[i];
            string tempword = word;
            tempword += sentence[i+1];
            if(tempword == "==" || tempword == "<>" || tempword =="||" || tempword == "&&" || tempword == ">=" || tempword == "<=") {
                word += sentence[i+1];
                i++;
            }

            string wordNew2 = word;
            if(!wordNew2.empty()) wordVec.push_back(wordNew2);
            word.clear();

        }
        else {
            word += sentence[i];
        }
    }
    return wordVec;
}
vector<worder> mergeFunction(vector<worder> worders)   //合并函数表达式
{
    vector<worder> result;
    int flag = 0;
    for(int i = 0; i < worders.size(); i++) {
        worder word = worders[i];
        if(word.type != IDENTIFIER && word.name != "main") {
            result.push_back(word);
            continue;
        }
        if(i < worders.size() -1 && worders[i+1].name == "(") {
            flag++;
            word.name += "(";
            i++;
            while(flag != 0) {
                i++;
                if(worders[i].name == "(") flag++;
                if(worders[i].name == ")") flag--;
                word.name += worders[i].name;
                if(flag != 0) word.name += " ";
            }
            word.type = FUNCTION;
            result.push_back(word);
        }
        else result.push_back(word);
    }
    return result;
}

bool judgeNumber(string word)  //判断是否为数字
{
    bool flag = false;
    for(int i = 0; i < word.size(); i++) {
        if(word[i] == '.' && flag) return false;
        else if(word[i] == '.') flag = true;
        if((word[i] < '0' || word[i] > '9') && word[i] != '.') return false;
    }
    return true;
}
bool judgeIdentifier(string word)  //判断是否为标识符
{
    for(int i = 0; i < word.size(); i++) {
        char c = word[i];
        if(c >= '0' && c <= '9' && i == 0) return false;
        if(c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <='Z') continue;
        else return false;
    }
    return true;
}
bool judgeString(string word)  //判断是否为字符串
{
    return (word[0] == '"' && word[word.size() -1]) ;
}

vector<worder> judgeWord(vector<string> wordVec)  //标记分词属性
{
    vector<worder> result;

    string flag[24] = {
            "{","}","(",")","+","-","*","/",">","<","&&","$","#","!","||","=",";","\"","%","<>",">=","<=",",","=="
    };

    set<string> operateSysmbol(flag, flag + 24);
    set<string> declarSysmbol;
    set<string> keyWord;

    declarSysmbol.insert("int");
    declarSysmbol.insert("real");
    declarSysmbol.insert("string");
    declarSysmbol.insert("void");

    keyWord.insert("in");
    keyWord.insert("out");
    keyWord.insert("if");
    keyWord.insert("else");
    keyWord.insert("while");
    keyWord.insert("main");
    keyWord.insert("return");

    for(int i = 0; i < wordVec.size(); i++) {
        if(declarSysmbol.find(wordVec[i]) != declarSysmbol.end()) {
            worder word;
            word.name = wordVec[i];
            word.type = DECLARTION;
            result.push_back(word);
        }
        else if(keyWord.find(wordVec[i]) != keyWord.end()) {
            worder word;
            word.name = wordVec[i];
            word.type = JUDGE;
            result.push_back(word);
        }
        else if(operateSysmbol.find(wordVec[i]) != operateSysmbol.end()) {
            worder word;
            word.name = wordVec[i];
            word.type = OPERATER;
            result.push_back(word);
        }
        else if(judgeIdentifier(wordVec[i])) {
            worder word;
            word.name = wordVec[i];
            word.type = IDENTIFIER;
            result.push_back(word);
        }
        else if(judgeNumber(wordVec[i])) {
            worder word;
            word.name = wordVec[i];
            word.type = NUMBER;
            result.push_back(word);
        }
        else if(judgeString(wordVec[i])) {
            worder word;
            word.name = wordVec[i];
            word.type = STRING;
            result.push_back(word);
        }
        else {
            cout << "ERROR WORD:" << wordVec[i] << endl;
            throw Compile_E("ERROR WORD");
        }
    }
    return result;
}
vector<worder> complateSepSentence(string sentence)   //完整分词，封装上面三个函数
{
    vector<worder> result = mergeFunction(judgeWord(separateSentence(sentence)));
    return result;
}
vector<vector<worder> > processArgumentsOfFunction(worder word) //将函数的参数合并为表达式
{
    vector<vector<worder> > result;
    vector<int> flagofint;
    vector<worder> wordersofargument;
    vector<worder> exp;
    string sentence;
    int index = 0, flag = 0;
    for(int i = 0; i < word.name.size() - 1; i++) {
        if(word.name[i] != '(') continue;
        flag = i;
        break;
    }
    int numberofbrackettemp = 1;

    for(int i = flag + 1; i < word.name.size(); i++) {
        if(word.name[i] == '(') numberofbrackettemp++;
        if(word.name[i] == ')') numberofbrackettemp--;
        if(numberofbrackettemp == 0) break;
        sentence += word.name[i];
    }

    if(sentence.length() == 0) return vector<vector<worder> >(1);
    wordersofargument = complateSepSentence(sentence);

	flag = -1;
    for(int i = 0; i < wordersofargument.size(); i++) {
        if(wordersofargument[i].name == ",") {
            flagofint.push_back(i);
        }
    }
    
    for(int i = 0; i < flagofint.size(); i++) {
        while(index != flagofint[i]) {
            exp.push_back(wordersofargument[index++]);
        }
        result.push_back(exp);
        exp.clear();
        flag = index;
        index++;
    }
    for(int i = flag + 1; i < wordersofargument.size(); i++) {
        exp.push_back(wordersofargument[i]);
    }
    result.push_back(exp);
    return result;
}


FILE* openFile(char* fileName)
{
    printf("fileName = %s\n", fileName);
    FILE* fd = fopen(fileName, "r");
    if(fd) {
        printf("Open file success\n");
        return fd;
    }
    else {
        printf("Open file failed\n");
        exit(0);
    }
    return fd;
}

vector<string> readFile(FILE* fd)
{
    char strLine[1024];
    vector<string> sents;
    while(!feof(fd)) {
        fgets(strLine, 1024, fd);
        string sentence = strLine;
        if(sentence[sentence.size() - 1] == '\n') sentence.erase(sentence.size() -1);
        sents.push_back(sentence);
    }
    return sents;
}

