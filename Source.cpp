#include<iostream>
#include<stack>
#include<string>
#include<vector>
#include<cstdint>
#include<set>
#include<algorithm>
typedef std::string string;
typedef std::pair<int32_t, int32_t> pair;

class  Regexp_reverse_polish {
private:
	std::string regexp_reverse_polish_notation;
	std::set<char> alphabets{'a', 'b', 'c', '1'};
public:
	Regexp_reverse_polish(string& regexp) : regexp_reverse_polish_notation(regexp) {};
	string search_max_prefix(const char letter, const  int32_t deqree);
};

void insert_element_from_alphabets(std::stack<pair>& regexp, const char letter, const char symbol) {
	if (symbol == '1') {
		regexp.push(std::make_pair(-1, -1));
	}
	else {
		regexp.push(std::make_pair(symbol == letter, 0));
	}
}

void power_operation(std::stack<pair>& regexp, const int32_t degree) {
	pair new_regexp = regexp.top();
	regexp.pop();

	if (new_regexp.first > 0) {
		new_regexp.first = degree;
	}
	else {
		new_regexp.first = -1;
	}

	regexp.push(new_regexp);
}

void choice_operation(std::stack<pair>& regexp) {
	pair right_regexp = regexp.top();
	regexp.pop();
	pair left_regexp = regexp.top();
	regexp.pop();

	pair new_regexp(0, 0);
	new_regexp.first = std::max(left_regexp.first, right_regexp.first);
	new_regexp.second = std::max(left_regexp.second, right_regexp.second);
	if ((right_regexp.first == -1 || left_regexp.first == -1) && new_regexp.first == 0) {
		new_regexp.first = -1;
	}

	regexp.push(new_regexp);
}	

void concatenate_operation(std::stack<pair>& regexp, const pair& epsilon_word) {
	pair right_regexp = regexp.top();
	regexp.pop();
	pair left_regexp = regexp.top();
	regexp.pop();
	pair new_regexp(0, 0);

	if (left_regexp == epsilon_word) {
		regexp.push(right_regexp);
	}
	else {
		if (right_regexp == epsilon_word) {
			regexp.push(left_regexp);
		}
	}
	if (left_regexp.first != -1 && left_regexp.first != 0 && right_regexp.first != -1 && right_regexp.first != 0) {
		new_regexp.first = right_regexp.first + left_regexp.first;
	}
	else {
		if (left_regexp.first != 0 && right_regexp.first != 0) {
			new_regexp.first = std::max(left_regexp.first, right_regexp.first);
		}
	}
	new_regexp.second = std::max(left_regexp.second, right_regexp.second + left_regexp.first);
	regexp.push(new_regexp);
}

void work_with_operation(std::stack<pair>& regexp, pair epsilon_word, char symbol, const int32_t degree) {
	if (symbol == '*') {
		power_operation(regexp, degree);
	}
	if (symbol == '+') {
		choice_operation(regexp);
	}
	if (symbol == '.') {
		concatenate_operation(regexp, epsilon_word);
	}
	if (symbol != '.' && symbol != '+' && symbol != '*') {
		throw "unexpected symbol";
	}
}

string Regexp_reverse_polish::search_max_prefix(const char letter, const int32_t degree) {
	pair epsilon_word = std::make_pair(-1, -1);
	std::stack<pair> normal_regexp;
	int32_t length_regexp = regexp_reverse_polish_notation.size();
	for (int32_t i = 0; i < length_regexp; ++i) {
		try {
			if (alphabets.find(regexp_reverse_polish_notation[i]) != alphabets.end()) {
				insert_element_from_alphabets(normal_regexp, letter, regexp_reverse_polish_notation[i]);
			}
			else {
				work_with_operation(normal_regexp, epsilon_word, regexp_reverse_polish_notation[i], degree);
			}
		}
		catch (...) {
			return "ERROR";
		}
	}
	if (normal_regexp.size() > 1) {
		return "ERROR";
	}
	else {
		if (std::max(normal_regexp.top().first, normal_regexp.top().second) >= degree) {
			return "YES";
		}
		else {
			return "NO";
		}
	}
}


int main() {
	string regexp;
	char letter;
	int32_t degree;
	std::cin >> regexp >> letter >> degree;
	Regexp_reverse_polish regexp_reverse(regexp);
	std::cout << regexp_reverse.search_max_prefix(letter, degree);

	return 0;
}