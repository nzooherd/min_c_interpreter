//
// Created by dudu on 9/4/17.
//

#ifndef CLIONPROJECT_PROCESSSENTENCE_H
#define CLIONPROJECT_PROCESSSENTENCE_H
#include "PartWord.h"
bool judgePreceRate(worder a, worder b);
vector<worder> infixToPostfix(vector<worder> worders);
vector<string> processDeclartion(vector<worder> worders);
vector<string> processIfElseWhileReturn(vector<worder> worders, int flag);
vector<string> processRightBigBracket(vector<worder> worders);
vector<string> processFunction(vector<worder> worders, int flag);
vector<string> processExpression(vector<worder> worders);
vector<string> processIn(const vector<worder>& worders);
vector<string> processOut(const vector<worder>& worders);
vector<string> processAssign(vector<worder> worders);
vector<string> program(string sentence);

#endif //CLIONPROJECT_PROCESSSENTENCE_H
