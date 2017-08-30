
// out
void out(short a) {
	printf("%d", a);
}

void out(float a) {
	printf("%f", a);
}

void out(string s) {
	cout << s;
}

void out(int a, string s) {
	while (a--) {
		cout << s;
	}
}

void out(string s1, string s2) {
	cout << s1 << s2;
}





//in
short in(short a) {
	scanf("%d", &a);
	return a;
}

float in(float a) {
	scanf("%f", &a);
	return a;
}

string in(string s) {
	scanf("%s", s);
	return s;
}

short in(string s, short a) {
	cout << s;
	scanf("%d", &a);
	return a;
}

float in(string s, float a) {
	cout << s;
	scanf("%f", &a);
	return a;
}

string in(string s1, string s2) {
	cout << s1;
	scanf("%s", s2);
	return s1;
}


