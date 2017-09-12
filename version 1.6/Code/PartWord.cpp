//
// Created by dudu on 9/4/17.
//

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include "PartWord.h"
#include "Exception_dec.h"
namespace partword {
	set<char> operateChar{ '{','}','(',')','+','-','*','/','>','<','&','$','#','!','|','=',';','\"','%',',','^' };
	set<string> operateSysmbol{ "{","}","(",")","+","-","*","/",">","<","&&","$","#","!","||","=",";","\"","%","<>",">=","<=",",","==","^" };
	set<string> declarSysmbol{ "int","real","string","void" };
	set<string> keyWord{ "in","out","if","else","while","main","return","continue","break","do","until" };

}
wd::wd(string _name, wordFlag _type) :name(_name), type(_type) {};

vector<string> separateSentence(string sentence)        //简单分词
{
    if(sentence[sentence.size()-1] != ' ') sentence += ' ';
	using partword::operateChar;

    vector<string> wordVec;
    string word;

    for(int i = 0; i < sentence.size(); i++) {
        if(sentence[i] == '"' && (i == 0 || sentence[i-1] != '\\')) {
            if(!word.empty()) wordVec.push_back(word);
            word.clear();
            word.push_back(sentence[i]);
            while(sentence[++i] != '"') word.push_back(sentence[i]);
            word.push_back(sentence[i]);
            wordVec.push_back(word);
            word.clear();
        }
        else if(i == sentence.size() - 1) {
            if(!word.empty()) wordVec.push_back(word);
            break;
        }
        else if(sentence[i] == ' ' ) {
            if(!word.empty()) wordVec.push_back(word);
            word.clear();
        }
        else if(operateChar.find(sentence[i]) != operateChar.end()) {
            if(!word.empty()) wordVec.push_back(word);
            word.clear();
            word.push_back(sentence[i]);
            string tempword = word;
			tempword.push_back(sentence[i + 1]);
            if(tempword == "==" || tempword == "<>" || tempword =="||" || tempword == "&&" || tempword == ">=" || tempword == "<=")
				word.push_back(sentence[++i]);

            if(!word.empty()) wordVec.push_back(word);
            word.clear();

        }
        else word.push_back(sentence[i]);
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

vector<worder> judgeWord(const vector<string>& wordVec)  //标记分词属性
{
    vector<worder> result;
	using namespace partword;


	wordFlag wordType;
    for(auto word:wordVec) {
		if (declarSysmbol.find(word) != declarSysmbol.end())
			wordType = DECLARTION;
        else if(keyWord.find(word) != keyWord.end()) {
			wordType = JUDGE;
        }
        else if(operateSysmbol.find(word) != operateSysmbol.end())
			wordType = OPERATER;
        else if(judgeIdentifier(word))
			wordType = IDENTIFIER;
        else if(judgeNumber(word))
			wordType = NUMBER;
        else if(judgeString(word))
			wordType = STRING;
        else
            throw Compile_E(string("ERROR WORD:"+word));
		result.push_back(worder(word, wordType));
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



