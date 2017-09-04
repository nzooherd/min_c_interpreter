//
// Created by dudu on 9/4/17.
//
#include"Exception_dec.h"
Runtime_E::Runtime_E(int _line, Simple_E _reason) : line(_line), reason(_reason.reason) {}
Runtime_E::Runtime_E(string _reason) : line(-1), reason(_reason) {}
Runtime_E::Runtime_E(int _line, string _reason) : line(_line), reason(_reason) {}
Runtime_E::~Runtime_E() {}

Compile_E::Compile_E(int _line, Simple_E _reason) : line(_line), reason(_reason.reason) {}
Compile_E::Compile_E(string _reason) : line(-1), reason(_reason) {}
Compile_E::Compile_E(int _line,string _reason):line(_line),reason(_reason){}
Compile_E::~Compile_E() {}


Other_E::Other_E(int _line, Simple_E _reason) : line(_line), reason(_reason.reason) {}
Other_E::Other_E(string _reason) : line(-1), reason(_reason) {}
Other_E::Other_E(int _line, string _reason) : line(_line), reason(_reason) {}
Other_E::~Other_E() {}

Simple_E::Simple_E(string _reason) : reason(_reason) {}
Simple_E::~Simple_E() {}

Exe_E::Exe_E(string _reason) : reason(_reason) {}
Exe_E::~Exe_E(){}
