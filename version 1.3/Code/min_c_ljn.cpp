#include"min_c_ljn.h"
#include"Exception_dec.h"
vector <vector<int> > ansLocation;
int errLocation[2];
string errInformation;
string nowLine;
int JudgeFunc (string tobeJudged, int i) {
    for ( ; i < tobeJudged.size(); i++) {
        if ((tobeJudged[i] >= '0' && tobeJudged[i] <='9') || (tobeJudged[i] >= 'a' && tobeJudged[i] <= 'z') || (tobeJudged[i] >= 'A' && tobeJudged[i] <= 'Z'))
            continue;
        if (tobeJudged[i] == ' ') {
            if (tobeJudged[i+1] == '(') {
                i = i + 1;
                break;
            }
            else {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = i + 1;
                errInformation = "Function declear error.";
                return 1;
            }
        }
        if (tobeJudged[i] == '(')
            break;
        errLocation[0] = ansLocation.size() - 1, errLocation[1] = i;
        errInformation = "Function declear error.";
        return 1;
    }
    while (tobeJudged[i] != ')' && i < tobeJudged.size())
        i++;
    if (tobeJudged[i+1] == '{')
        return 0;
    if (tobeJudged[i+1] == ' ' && tobeJudged[i+2] == '{')
        return 0;
    if (tobeJudged[i+1] == ' ') {
        errLocation[0] = ansLocation.size() - 1, errLocation[1] = i + 2;
    }
    else {
        errLocation[0] = ansLocation.size() - 1, errLocation[1] = i + 1;
    }
    errInformation = "Function declear error.";
    return 1;
}

int JudgeLine (string tobeJudged) {
    unsigned long jgSize = tobeJudged.size();
    if (!((tobeJudged[0] >= 'a' && tobeJudged[0] <= 'z') || (tobeJudged[0] >= 'A' && tobeJudged[0] <= 'Z') || tobeJudged[0] == '}')) {
        errLocation[0] = ansLocation.size() - 1, errLocation[1] = 0;
        return 1;
    }
    if (tobeJudged[0] == '}') {
        return 0;
    }                                              //}
    if (jgSize > 4 && tobeJudged[0] == 'i' && tobeJudged[jgSize-1] == '{') {
        if (tobeJudged[1] == 'n' && tobeJudged[2] == 't' && tobeJudged[3] == ' ') {
            if (!(tobeJudged[4] >= 'a' && tobeJudged[4] <= 'z') || (tobeJudged[4] >= 'A' && tobeJudged[4] <= 'Z')) {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 4;
                errInformation = "Function declear error.";
                return 1;
            }
            int flag = JudgeFunc(tobeJudged, 5);
            if (flag)
                return 1;
            if (tobeJudged[4] == 'm' && tobeJudged[5] == 'a' && tobeJudged[6] == 'i' && tobeJudged[7] == 'n') {
                if (tobeJudged[8] == ' ' || tobeJudged[8] == '(') {
                    nowLine.push_back('b');
                    return 0;
                }
            }
            nowLine.push_back('a');
            return 0;
        }
    } //int func
    if (jgSize > 5 && tobeJudged[0] == 'v' && tobeJudged[jgSize-1] == '{') {
        if (tobeJudged[1] == 'o' && tobeJudged[2] == 'i' && tobeJudged[3] == 'd' && tobeJudged[4] == ' ') {
            if (!(tobeJudged[5] >= 'a' && tobeJudged[5] <= 'z') || (tobeJudged[5] >= 'A' && tobeJudged[5] <= 'Z')) {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 5;
                errInformation = "Function declear error.";
                return 1;
            }
            nowLine.push_back('a');
            return JudgeFunc(tobeJudged, 6);
        }
    } //void func
    if (jgSize > 5 && tobeJudged[0] == 'r' && tobeJudged[jgSize-1] == '{') {
        if (tobeJudged[1] == 'e' && tobeJudged[2] == 'a' && tobeJudged[3] == 'l' && tobeJudged[4] == ' ') {
            if (!(tobeJudged[5] >= 'a' && tobeJudged[5] <= 'z') || (tobeJudged[5] >= 'A' && tobeJudged[5] <= 'Z')) {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 5;
                errInformation = "Function declear error.";
                return 1;
            }
            nowLine.push_back('a');
            return JudgeFunc(tobeJudged, 6);
        }
    } //real func
    if (jgSize > 7 && tobeJudged[0] == 's' && tobeJudged[jgSize-1] == '{') {
        if (tobeJudged[1] == 't' && tobeJudged[2] == 'r' && tobeJudged[3] == 'i' && tobeJudged[4] == 'n' && tobeJudged[5] == 'g' && tobeJudged[6] == ' ') {
            if (!(tobeJudged[7] >= 'a' && tobeJudged[7] <= 'z') || (tobeJudged[7] >= 'A' && tobeJudged[7] <= 'Z'))  {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 7;
                errInformation = "Function declear error.";
                return 1;
            }
            nowLine.push_back('a');
            return JudgeFunc(tobeJudged, 8);
        }
    } //string func
    if (jgSize > 4 && tobeJudged[0] == 'i' && tobeJudged[jgSize-1] == ';') {
        if (tobeJudged[1] == 'n' && tobeJudged[2] == 't' && tobeJudged[3] == ' ') {
            nowLine.push_back('l');
            return 0;
        }
    } //int declear
    if (jgSize > 5 && tobeJudged[0] == 'r' && tobeJudged[jgSize-1] == ';') {
        if (tobeJudged[1] == 'e' && tobeJudged[2] == 'a' && tobeJudged[3] == 'l' && tobeJudged[4] == ' ') {
            nowLine.push_back('l');
            return 0;
        }
    } //real declear
    if (jgSize > 7 && tobeJudged[0] == 's' && tobeJudged[jgSize-1] == ';') {
        if (tobeJudged[1] == 't' && tobeJudged[2] == 'r' && tobeJudged[3] == 'i' && tobeJudged[4] == 'n' && tobeJudged[5] == 'g' && tobeJudged[6] == ' ') {
            nowLine.push_back('l');
            return 0;
        }
    } //string declear
    if (jgSize > 3 && tobeJudged[0] == 'i' && tobeJudged[jgSize-1] == ';') {
        if (tobeJudged[1] == 'n' && tobeJudged[2] == ' ') {
            nowLine.push_back('h');
            if (tobeJudged[3] == '"') {
                int flag = 0, i = 4;
                while (i < jgSize) {
                    if (tobeJudged[i] == '"') {
                        flag = 1;
                        i++;
                        break;
                    }
                    i++;
                }
                if (flag == 0) {
                    errLocation[0] = ansLocation.size() - 1, errLocation[1] = i;
                    errInformation = "In syntax error.";
                    return 1;
                }
                if ((tobeJudged[i] == ' ' && tobeJudged[i+1] == ',') || (tobeJudged[i] == ',')) {
                    if (tobeJudged[i] == ' ')
                        i = i + 2;
                    else
                        i = i + 1;
                    if (tobeJudged[i] == ' ') {
                        i = i + 1;
                        if ((tobeJudged[i] >= 'a' && tobeJudged[i] <= 'z') || (tobeJudged[i] >= 'A' && tobeJudged[i] <= 'Z')) {
                            while (i < jgSize - 1) {
                                if ((tobeJudged[i] >= '0' && tobeJudged[i] <='9') || (tobeJudged[i] >= 'a' && tobeJudged[i] <= 'z') || (tobeJudged[i] >= 'A' && tobeJudged[i] <= 'Z')) {
                                    i++;
                                    continue;
                                }
                                if (tobeJudged[i] == ' ') {
                                    if (tobeJudged[i+1] == ';')
                                        break;
                                    else {
                                        errLocation[0] = ansLocation.size() - 1, errLocation[1] = i + 1;
                                        errInformation = "In syntax error.";
                                        return 1;
                                    }
                                }
                                errLocation[0] = ansLocation.size() - 1, errLocation[1] = i;
                                errInformation = "In syntax error.";
                                return 1;
                            }
                            return 0;
                        }
                        else {
                            errLocation[0] = ansLocation.size() - 1, errLocation[1] = i;
                            errInformation = "In syntax error.";
                            return 1;
                        }
                    }
                    else if ((tobeJudged[i] >= 'a' && tobeJudged[i] <= 'z') || (tobeJudged[i] >= 'A' && tobeJudged[i] <= 'Z')) {
                        while (i < jgSize - 1) {
                            if ((tobeJudged[i] >= '0' && tobeJudged[i] <='9') || (tobeJudged[i] >= 'a' && tobeJudged[i] <= 'z') || (tobeJudged[i] >= 'A' && tobeJudged[i] <= 'Z')) {
                                i++;
                                continue;
                            }
                            if (tobeJudged[i] == ' ') {
                                if (tobeJudged[i+1] == ';')
                                    break;
                                else {
                                    errLocation[0] = ansLocation.size() - 1, errLocation[1] = i + 1;
                                    errInformation = "In syntax error.";
                                    return 1;
                                }
                            }
                            errLocation[0] = ansLocation.size() - 1, errLocation[1] = i;
                            errInformation = "In syntax error.";
                            return 1;
                        }
                        return 0;
                    }
                    else {
                        errLocation[0] = ansLocation.size() - 1, errLocation[1] = i;
                        errInformation = "In syntax error.";
                        return 1;
                    }
                }
                else {
                    errLocation[0] = ansLocation.size() - 1, errLocation[1] = i;
                    errInformation = "In syntax error.";
                    return 1;
                }
            }
            else if ((tobeJudged[3] >= 'a' && tobeJudged[3] <= 'z') || (tobeJudged[3] >= 'A' && tobeJudged[3] <= 'Z')) {
                int i = 3;
                while (i < jgSize - 1) {
                    if ((tobeJudged[i] >= '0' && tobeJudged[i] <='9') || (tobeJudged[i] >= 'a' && tobeJudged[i] <= 'z') || (tobeJudged[i] >= 'A' && tobeJudged[i] <= 'Z')) {
                        i++;
                        continue;
                    }
                    if (tobeJudged[i] == ' ') {
                        if (tobeJudged[i+1] == ';')
                            return 0;
                        else {
                            errLocation[0] = ansLocation.size() - 1, errLocation[1] = i + 1;
                            errInformation = "In syntax error.";
                            return 1;
                        }
                    }
                    errLocation[0] = ansLocation.size() - 1, errLocation[1] = i;
                    errInformation = "In syntax error.";
                    return 1;
                }
                return 0;
            }
            else {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 3;
                errInformation = "In syntax error.";
                return 1;
            }
        }
    } //in syntax
    if (jgSize > 4 && tobeJudged[0] == 'o' && tobeJudged[jgSize-1] == ';') {
        if (tobeJudged[1] == 'u' && tobeJudged[2] == 't' && tobeJudged[3] == ' ') {
            nowLine.push_back('i');
            return 0;
        }
    } //out syntax
    if (jgSize > 3 && tobeJudged[0] == 'i' && tobeJudged[jgSize-1] == '{') {
        if (tobeJudged[1] == 'f') {
            nowLine.push_back('c');
            if (tobeJudged[2] != ' ') {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 2;
                errInformation = "If syntax error.";
                return 1;
            }
            if (tobeJudged[3] == '{') {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 3;
                errInformation = "If syntax error.";
                return 1;
            }
            else {
                return 0;
            }
        }
    } //if
    if (jgSize > 4 && tobeJudged[0] == 'e' && tobeJudged[jgSize-1] == '{') {
        if (tobeJudged[1] == 'l' && tobeJudged[2] == 's' && tobeJudged[3] == 'e') {
            nowLine.push_back('f');
            if (tobeJudged[4] == '{')
                return 0;
            else if (tobeJudged[4] == ' ') {
                if (tobeJudged[5] == '{')
                    return 0;
                else {
                    errLocation[0] = ansLocation.size() - 1, errLocation[1] = 5;
                    errInformation = "Else syntax error.";
                    return 1;
                }
            }
            else {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 4;
                errInformation = "Else syntax error.";
                return 1;
            }
        }
    } //else
    if (jgSize > 6 && tobeJudged[0] == 'w' && tobeJudged[jgSize-1] == '{') {
        if (tobeJudged[1] == 'h' && tobeJudged[2] == 'i' && tobeJudged[3] == 'l' && tobeJudged[4] == 'e') {
            nowLine.push_back('d');
            if (tobeJudged[5] != ' ') {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 5;
                errInformation = "While syntax error.";
                return 1;
            }
            if (tobeJudged[6] == '{') {
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = 6;
                errInformation = "While syntax error.";
                return 1;
            }
            else
                return 0;
        }
    } //while
    if (jgSize > 6 && tobeJudged[0] == 'r' && tobeJudged[jgSize-1] == ';') {
        if (tobeJudged[1] == 'e' && tobeJudged[2] == 't' && tobeJudged[3] == 'u' && tobeJudged[4] == 'r' && tobeJudged[5] == 'n') {
            if (tobeJudged[6] == ';' || tobeJudged[6] == ' ') {
                nowLine.push_back('e');
                return 0;
            }
        }
    } //return
    if (jgSize > 1 && tobeJudged[jgSize-1] == ';') {
        for (int i = 0; i < jgSize; i++) {
            if (tobeJudged[i] == '=') {
                nowLine.push_back('j');
                return 0;
            }
            if (tobeJudged[i] == '(') {
                nowLine.push_back('k');
                return 0;
            }
        }
    } //let = and () go
	if (jgSize > 8 && tobeJudged[0] == 'c' && tobeJudged[jgSize-1] == ';') {
		if (tobeJudged[0] == 'c' && tobeJudged[1] == 'o' && tobeJudged[2] == 'n' && tobeJudged[3] == 't' && tobeJudged[4] == 'i' && tobeJudged[5] == 'n' && tobeJudged[6] == 'u' && tobeJudged[7] == 'e') {
			if (tobeJudged[8] == ';') {
				nowLine.push_back('m');
				return 0;
			}
			if (tobeJudged[8] == ' ' && tobeJudged[9] == ';') {
				nowLine.push_back('m');
				return 0;
			}
		}
	} //continue
	if (jgSize > 5 && tobeJudged[0] == 'b' && tobeJudged[jgSize - 1] == ';') {
		if (tobeJudged[1] == 'r' && tobeJudged[2] == 'e' && tobeJudged[3] == 'a' && tobeJudged[4] == 'k') {
			if (tobeJudged[5] == ';') {
				nowLine.push_back('n');
				return 0;
			}
			if (tobeJudged[5] == ' ' && tobeJudged[6] == ';') {
				nowLine.push_back('n');
				return 0;
			}
		}
	} //break
    errLocation[0] = ansLocation.size() - 1, errLocation[1] = 0;
    errInformation = "Invalid syntax.";
    return 1;
}

vector <string> DealFile (string pendingFile) {
	if (pendingFile.length() == 0)
		throw Compile_E("输入了空文件");
    vector <string> ans;
    vector <int> location;
    unsigned long pfSize = pendingFile.length();
    int now = 0, lineNum = 1;
    int parenthesesNum = 0, braceNum = 0, errorFlag = 0;
    while (now < pfSize) {
        if (pendingFile[now] == ' ') {
            if (nowLine.length() != 0 && nowLine[nowLine.length()-1] != ' ') {
                nowLine.push_back(pendingFile[now]);
                location.push_back(lineNum);
            }
            now = now + 1;
            continue;
        }
        if (pendingFile[now] == '\n' || pendingFile[now] == '\r' || pendingFile[now] == '\t') {
            if (pendingFile[now] == '\n')
                lineNum = lineNum + 1;
            now = now + 1;
            continue;
        }
        if (pendingFile[now] == '/') {
            now = now + 1;
            if (now == pfSize) {
                location.push_back(lineNum);
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = location.size() - 1;
                errInformation = "Invalid syntax.";
                errorFlag = 1;
                break;
            }
            if (pendingFile[now] == '/') {
                now = now + 1;
                while (now < pfSize) {
                    if (pendingFile[now] == '\n')
                        break;
                    now = now + 1;
                }
            }
            else {
                nowLine.push_back('/');
                location.push_back(lineNum);
            }
            continue;
        }
        if (pendingFile[now] == ';') {
            if (parenthesesNum != 0) {
                location.push_back(lineNum);
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = location.size() - 1;
                errInformation = "Invalid syntax.";
                errorFlag = 1;
                break;
            }
            nowLine.push_back(pendingFile[now]);
            location.push_back(lineNum);
            errorFlag = JudgeLine(nowLine);
            if (errorFlag)
                break;
            ans.push_back(nowLine);
            ansLocation.push_back(location);
            nowLine.clear();
            location.clear();
            now = now + 1;
            continue;
        }
        if (pendingFile[now] == '{') {
            if (parenthesesNum != 0) {
                location.push_back(lineNum);
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = location.size() - 1;
                errInformation = "Invalid syntax.";
                errorFlag = 1;
                break;
            }
            braceNum = braceNum + 1;
            nowLine.push_back(pendingFile[now]);
            location.push_back(lineNum);
            errorFlag = JudgeLine(nowLine);
            if (errorFlag)
                break;
            ans.push_back(nowLine);
            ansLocation.push_back(location);
            nowLine.clear();
            location.clear();
            now = now + 1;
            continue;
        }
        if (pendingFile[now] == '}') {
            if (nowLine.length() != 0 || braceNum == 0) {
                location.push_back(lineNum);
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = location.size() - 1;
                errInformation = "Invalid syntax.";
                errorFlag = 1;
                break;
            }
            braceNum = braceNum - 1;
            nowLine = "}g";
            location.push_back(lineNum);
            ans.push_back(nowLine);
            ansLocation.push_back(location);
            nowLine.clear();
            location.clear();
            now = now + 1;
            continue;
        }
        if (pendingFile[now] == '(') {
            parenthesesNum = parenthesesNum + 1;
            nowLine.push_back(pendingFile[now]);
            location.push_back(lineNum);
            now = now + 1;
            continue;
        }
        if (pendingFile[now] == ')') {
            if (parenthesesNum == 0) {
                location.push_back(lineNum);
                errLocation[0] = ansLocation.size() - 1, errLocation[1] = location.size() - 1;
                errInformation = "Invalid syntax.";
                errorFlag = 1;
                break;
            }
            parenthesesNum = parenthesesNum - 1;
            nowLine.push_back(pendingFile[now]);
            location.push_back(lineNum);
            now = now + 1;
            continue;
        }
        if (pendingFile[now] == '"') {
            nowLine.push_back(pendingFile[now]);
            location.push_back(lineNum);
            now = now + 1;
            while (now < pfSize) {
                nowLine.push_back(pendingFile[now]);
                location.push_back(lineNum);
                if (pendingFile[now] == '"') {
                    now = now + 1;
                    break;
                }
                now = now + 1;
            }
        }
        else {
            nowLine.push_back(pendingFile[now]);
            location.push_back(lineNum);
            now = now + 1;
        }
    }
    if (parenthesesNum || braceNum || errorFlag || nowLine.size()) {
        ans.clear();
        nowLine.clear();
		if (!errInformation.size()) {
			errInformation = "Invalid syntax.";
		}
    }
    return ans;
}

vector <string> ReceiveFile (string pendingFile) {
    vector <string> ans = DealFile(pendingFile);
	if (ans.size() == 0)
		throw Compile_E(ansLocation[errLocation[0]][errLocation[1]], errInformation);
    return ans;
}

vector<vector<int>> getGlobal() {
	return ansLocation;
}