#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
vector<vector<int>> getAnsLocation();
int getMaxOriginLine();
vector <string> ReceiveFile(string pendingFile);
int JudgeFunc(string tobeJudged, int i);
vector <string> DealFile(string pendingFile);
int JudgeLine(string tobeJudged);