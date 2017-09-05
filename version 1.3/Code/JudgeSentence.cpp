//
// Created by dudu on 9/4/17.
//
#include <set>
#include <vector>
#include "PartWord.h"
#include "JudgeSentence.h"
#include "Exception_dec.h"

bool judgeExpression(vector<worder>& words, const pair<int, int> & p)
{
    for(vector<worder>::iterator it = words.begin() + p.first;it!=words.end()&& it!=words.begin()+p.second;it++)
        if (it->type == JUDGE || it->type == DECLARTION || (it->type == OPERATER && it->name == "=")) return false;
    return expCheck(words, p);
}
bool judgeExpression(vector<worder>& words)
{
    for (auto word : words) {
        if (word.type == JUDGE || word.type == DECLARTION || (word.type == OPERATER && word.name == "=") )
            return false;
    }
    return expCheck(words,pair<int,int>(0,words.size()));
}
bool expCheck(vector<worder>& words,const pair<int,int>& p)
{
    if (words.size() == 0) return false;
    vector<pair<int, int>> jump;
    int count = 0;
    int begin = -1;
    char last = '2';//0:值 1:单目 2:二目
    for (int i = p.first; i < p.second; i++) {
        if (begin != -1) {
            if (words[i].type == OPERATER) {
                if (words[i].name == "(") count++;
                else if (words[i].name == ")") {
                    count--;
                    if (count == 0) {
                        jump.push_back(pair<int, int>(begin, i));
                        begin = -1;
                        last = '0';
                    }
                }
            }
        }
        else if (words[i].type == OPERATER) {
            if (words[i].name == "(") {
                count = 1;
                begin = i + 1;
            }
            else if (words[i].name == ")") throw Other_E("表达式中有不能匹配的)");
            else if (words[i].name == "+" || words[i].name == "-") {
                if (last == '0') {
                    last = '2';
                }
                else {
                    last = '1';
                    if(words[i].name == "+") words[i].name = "z";
                    else words[i].name = "f";
                }
            }
            else if (words[i].name == "z" || words[i].name == "f" || words[i].name == "!") {
                if (last == '0') return false;
                else last = '1';
            }
            else {
                if (last != '0') return false;
                else last = '2';
            }
        }
        else if (last == '0') return false;
        else {
            last = '0';
            if (words[i].type == FUNCTION) {
                int time = 0;
                for (auto vw : processArgumentsOfFunction(words[i])) {
                    if (vw.size() != 0 || time != 0)
                        if (!judgeExpression(vw)) return false;
                    time++;
                }
            }
        }
    }
    if (last != '0') return false;
    if (begin != -1) throw "表达式中有不能匹配的(";
    for (auto it = jump.begin(); it != jump.end(); it++)
        if (!expCheck(words, *it)) return false;
    return true;
}
bool judgeMain(vector<worder> worders)  //判断是否属于main函数
{
    return(worders[0].name == "int" && worders[1].name == "main()") ;
}
int judgeFunction(vector<worder> worders)    //判断是否属于函数语句 返回1表示声明函数，返回2表示调用函数,返回0表示非函数语句
{
    string sentence;
    int result;
    worder funt;
    if(worders[0].type == DECLARTION && worders[1].type == FUNCTION) {
        result = 1;
        funt = worders[1];
    }
    else if(worders[0].type == FUNCTION) {
        result = 2;
        funt = worders[0];
    }
    else return 0;
    vector<worder> wordersOfArgument;
    for(int i = 0; i < funt.name.size() - 1; i++) {
        if(funt.name[i] != '(') continue;
        for(++i; i < funt.name.size() - 1; i++) {
            sentence += funt.name[i];
        }
    }
	if (sentence.empty()) return result;
    wordersOfArgument = complateSepSentence(sentence);
    if(result == 1){
        for(int i = 0; i < wordersOfArgument.size(); i++) {
            if(i % 3 == 0 && wordersOfArgument[i].type != DECLARTION) break;
            if(i % 3 == 1 && wordersOfArgument[i].type != IDENTIFIER) break;
            if(i % 3 == 2 && wordersOfArgument[i].name != ",") break;
            if(i == wordersOfArgument.size() - 1) return 1;
        }
        return 0;
    }
    vector<vector<worder> > arguments = processArgumentsOfFunction(worders[0]);
    for(int i = 0; i < arguments.size(); i++) {
        vector<worder> singleArgument = arguments[i];
        if(!judgeExpression(singleArgument)) return 0;
    }
    return 2;
}
int judgeIfElseWhileReturn(vector<worder> worders)  //判断if或while语句合法性 if返回1,while返回2,else返回3,return返回4,错误返回0,
{
    if(worders.size() == 2 || judgeExpression(worders, pair<int, int>(1, worders.size()-1))) {
        if(worders[0].name == "while" && worders[worders.size() - 1].name == "{") return 2;
        if(worders[0].name == "if" && worders[worders.size()-1].name == "{") return 1;
        if(worders[0].name == "return" && worders[worders.size()-1].name == ";") {
            return 4;
        }
    }
    if(worders[0].name == "else" && worders[1].name == "{" && worders.size() == 2) return 3;
    else return 0;
}
int judgeInAndOut(vector<worder> worders)  //判断语句是否为In或Out，In返回1,Out2,否则返回０
{
    if(worders[0].name == "in") return 1;  //LJN处理
    if(judgeOut(worders)) return 2;
    return 0;
}
bool judgeDeclartion(vector<worder> worders)  //判断是否属于声明语句
{
    if(worders[0].type != DECLARTION || worders[0].name == "void") return false;
    for(int i = 1; i < worders.size() - 1; i++) {
        if(i % 2 != 0 && worders[i].type != IDENTIFIER) {
            cout << "IDENTIFIER error" << endl;
            return false;
        }
        if(i % 2 == 0 && worders[i].name!= ",") {
            cout << "Declat IDENTIFIER error" << endl;
            return false;
        }
    }
    return true;
}
bool judgeAssign(vector<worder> worders)   //判断是否属于赋值语句
{
    wordFeature last = OPERATER;
    int i = 0;
    for (; i < worders.size() - 1; i++) {
        if (last == OPERATER) {
            if (worders[i].type == OPERATER) break;
            else if (worders[i].type == IDENTIFIER) last = worders[i].type;
            else break;
        }
        else if (worders[i].type == IDENTIFIER) return false;
        else if (worders[i].type == OPERATER) {
            if (worders[i].name == "=") last = OPERATER;
            else {
                i--;
                break;
            }
        }
        else return false;
    }
    if (worders.size() - 1 == i) {
        if (worders[i - 1].type == IDENTIFIER ||
            worders[i - 1].type == FUNCTION ||
            worders[i - 1].type == NUMBER ||
            worders[i - 1].type == STRING) return true;
        else return false;
    }
    return judgeExpression(worders, pair<int, int>(i, worders.size() - 1));
}
bool judgeSingleExp(vector<worder> worders)  //判断是否为单个函数语句
{
    if(worders[worders.size() - 1 ].name != ";") return false;
    vector<worder> exp;
    for(int i = 0; i < worders.size() - 1; i++) {
        exp.push_back(worders[i]);
    }
    if(judgeExpression(exp)) return true;
    return false;
}
bool judgeSingleRightBracket(vector<worder> worders)  //判断单个右括号语句
{
    if(worders[0].name == "}" && worders.size() == 1) return true;
    else return false;
}
bool judgeOut(vector<worder> worders)  //判断是否属于输出语句
{
    if(worders[0].name != "out") return false;
    int flag = 0;
    for(int i = 0; i < worders.size(); i++) {
        if(worders[i].name == ",") {
            flag ++;
        }
    }
    if(flag == 0) {
        vector<worder> exp;
        for(int i = 1; i < worders.size()-1; i++) {
            exp.push_back(worders[i]);
        }
        if(!judgeExpression(exp)) return false;
        else return true;
    }
    else if(flag == 1) {
        vector<worder> exp1;
        vector<worder> exp2;
        bool flag = false;
        for(int i = 1; i < worders.size() - 1; i++) {
            if(!flag && worders[i].name != ",") {
                exp1.push_back(worders[i]);
            }
            if(flag || worders[i].name == ",") {
                flag = true;
                if(worders[i].name != ",") exp2.push_back(worders[i]);
            }
        }
        if(!judgeExpression(exp1) || !judgeExpression(exp2)) return false;
        else return true;
    }
    else return false;
}

