// Compilers Assignment 1
#include<bits/stdc++.h>
using namespace std;

string line;
int line_num = 1, token_num = 1;
bool multilineComment = false;
map<string, pair<int, string>> symbol_table;

vector<char> del {' ', '+', '-', '*', '/', ',', ';', '>', '<', '=', '(', ')', '[', ']', '{', '}', '%', '!', '\t'};
vector<char> op {'+', '-', '*', '/', '>', '<', '=', '%', '!', '?', ':'};
vector<string> key {"if", "else", "while", "break", "int", "float", "return", "char", "for", "true", "false", "string", "bool"};

bool isDelimiter(char ch) { 
	auto it = find(del.begin(), del.end(), ch);
	return !(it == del.end());
} 


bool isOperator(char ch) { 
	auto it = find(op.begin(), op.end(), ch);
	return !(it == op.end());
} 

bool isKeyword(string str) { 
	auto it = find(key.begin(), key.end(), str);
	return !(it == key.end());
} 

bool isInteger(string str){
	int i = 0;
	if(str[0] == '+' || str[1] == '-')
		i = 1;
	while(i < str.length()){
		if(!isdigit(str[i]))
			return false;
		i++;
	}
	return true;
}

bool isFloat(string str){
	int i = 0;
	if(str[0] == '+' || str[1] == '-')
		i = 1;

	int dec = 0;
	while(i < str.length()){
		if(str[i] == '.')
			dec++;
		else if(!isdigit(str[i]))
			return false;
		i++;
	}
	if(dec != 1)
		return false;
	return true;
}

bool isIdentifier(string str){
	int state = 1;
	int i = 0;
	while(i < str.length()){
		char ch = str[i];
		switch(state){
			case 1: 
				if(!isalpha(ch))
					return false;
				else
					state = 2;
				break;
			case 2:
				if(!isalnum(ch) && ch != '_')
					return false;
		}
		i++;
	}
	return true;

}


string subString(string str, int left, int right) {  
	string s = "";
	for (int i = left; i <= right; i++) 
		s += str[i]; 
	return s; 
}


void printOutput(string subStr, string s){
	int tn = 0, ln = line_num;
	if(symbol_table.find(subStr) == symbol_table.end()){
		symbol_table[subStr] = make_pair(token_num, s);
		tn = token_num;
		token_num++;
	}
	else{
		tn = symbol_table[subStr].first;
	}
	cout << "Token " << tn << ", string " << subStr << ", line number " << ln << "\n";
}

void parse(string str) { 
	int left = 0, right = 0; 
	int len = str.length(); 

	if(len == 0) return;

	while (right < len && left <= right) { 

		if(multilineComment) {
			bool foundEnd = false;
			for(int i=right; i<(len-1); i++) {
				if(str[i] == '*' && str[i+1] == '/') {
					right = i+2;
					left = right;
					foundEnd = true;
					multilineComment = false;
					break;
				}	
			}
			if(foundEnd) continue;
			else break;
		}

		if(str[right] == '/'){
			right++;
			if(right < len) {
				if(str[right] == '/')
					break;
				if(str[right] == '*') {
					multilineComment = true;
					right++;
					left = right;
					continue;
				}			
			}
		}

		if(str[left] == '"'){
			right++;
			while(right < len && str[right] != '"')
				right++;
			right++;
			if(right == len + 1)
				cout << "Lexical error for the string " << subString(str, left, right - 2) << ", line number " << line_num << "\n";
			else
				printOutput(subString(str, left, right - 1), "stringLiteral");
			left = right;
			continue;
		}

		if (!isDelimiter(str[right])) {
			right++; 
			continue;
		}

		if (isDelimiter(str[right]) && left == right) { 
			if (isOperator(str[right])) {
				string subStr = "";
				subStr += str[right];
				printOutput(subStr, "operator");
			}

			if (isDelimiter(str[right]) && str[right] != ' ' && !isOperator(str[right]) && str[right] != '\t') {
				string subStr = "";
				subStr += str[right];
				printOutput(subStr, "delimiter");
			}

			right++; 
			left = right; 
		} 
		else if (isDelimiter(str[right]) && left != right || (right == len && left != right)) { 
			string subStr = subString(str, left, right - 1); 

			if (isKeyword(subStr)){
				printOutput(subStr, "keyword");
			}

			else if (isInteger(subStr)) {
				printOutput(subStr, "integerLiteral");
	
			}

			else if (isFloat(subStr)) {
				printOutput(subStr, "floatLiteral");
			}

			else if (isIdentifier(subStr) && !isDelimiter(str[right - 1])){
				printOutput(subStr, "identifier");
			}

			else if (!isIdentifier(subStr) && !isDelimiter(str[right - 1])){
				int ln = line_num;
				cout << "Lexical error for the string " << subStr << ", line number " << ln << "\n";
			} 
				
			left = right; 
		} 
	} 

	if(right > 0 && (!isDelimiter(str[right - 1]))){
		string subStr = subString(str, left, right - 1); 
		if(subStr == "")
			return;
		if(!isKeyword(subStr) && !isInteger(subStr) && !isFloat(subStr) && !isIdentifier(subStr)){
			int ln = line_num;
			cout << "Lexical error for the string " << subStr << ", line number " << ln << "\n";
		}
		else{
			printOutput(subStr, "keyword");
		}
	}

	return; 
} 

int main() {
	//freopen("input.txt", "r", stdin);
	freopen("input1.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	while(getline(cin, line)){
		parse(line);
		line_num++;
	}
	return 0;
}