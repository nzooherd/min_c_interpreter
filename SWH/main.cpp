#include "PartWord.h"
#include "ProcessSentence.h"
#include "Exception_dec.h"
int ansLocation[100][100];
int main()
{
    for(int i = 1; i < 2; i++) {
        char* fileName = (char*)malloc(sizeof(char) * 60);
        scanf("%s", fileName);
        FILE* fd = openFile(fileName);
        vector<string> sents = readFile(fd);
        for(int i = 0; i < sents.size() - 1; i++) {
            try {
                vector<string> result = program(sents[i]);
                for (int i = 0; i < result.size(); i++) {
                    cout << result[i] << endl;
                }
            } catch (Compile_E E){
                  cout << E.reason << endl;
            }
        }
    }
    return 0;
}