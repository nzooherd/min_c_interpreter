//
// Created by dudu on 9/4/17.
//

#ifndef CLIONPROJECT_JUDGESENTENCE_H
#define CLIONPROJECT_JUDGESENTENCE_H
#include "PartWord.h"

bool judgeExpression(vector<worder>& words, const pair<int, int> & p);
bool judgeExpression(vector<worder>& words);
bool expCheck(vector<worder>& words,const pair<int,int>& p);
bool judgeMain(vector<worder> worders);
int judgeFunction(const vector<worder>& worders);
int judgeIfElseWhileReturn(vector<worder> worders);
int judgeInAndOut(vector<worder> worders);
bool judgeDeclartion(vector<worder> worders);
bool judgeAssign(vector<worder> worders);
bool judgeSingleRightBracket(vector<worder> worders);
bool judgeOut(vector<worder> worders);
bool judgeUntil(vector<worder> worderes);


#endif //CLIONPROJECT_JUDGESENTENCE_H
