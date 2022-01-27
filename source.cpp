#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::getline;
using std::ofstream;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::steady_clock;

#include "util.h"

const long long BASE = 257; // prime base for rolling hash
const long long MOD = 1e9 + 9; // prime modulus for rolling hash

/*Hash a string using a rolling hash*/
long long hash(const string& str, const int& len) {
	long long hash = 0;

	// hash = (str[0] * base + str[1] * base + ... + str[n] * base) % mod        
	for (int i = 0; i < len; ++i)
		hash = (BASE * hash + int(str[i])) % MOD;

	return hash;
}

/*Roll the hash along the data string by one index*/
long long rehash(long long& hash, const int& first, const int& last, const long long& pow) {
	hash = (BASE * (hash - first * pow) + last) % MOD; // drops first hashed character and adds a new last one to the existing hash

	if (hash < 0)
		hash = (hash + MOD); // negative hash is avoied by adding modulus

	return hash;
}

/*Boyer-Moore-Horspool string matching algorithm*/
vector <size_t> search_bmh(const string& pat, const string& data) {
	int pat_len = pat.size();
	int text_len = data.size();
	vector <size_t> pos;

	int skip[256]; 	// array of ints representing an alphabet of 256 symbols to tell how far to skip for each
	for (int i = 0; i < 256; ++i)
		skip[i] = pat_len; // skip by pattern length by default
	for (int i = 0; i < pat_len; ++i)
		skip[int(pat[i])] = (pat_len - 1) - i; // symbol is in the pattern, skip by how many characters far it is from the end of the pattern

	for (int i = 0; i <= text_len - pat_len; ++i) {
		int j;
		int s = skip[int(data[i + pat_len - 1])]; // rightmost character in substring that is being compared to pattern

		if (s != 0) { // character doesn't match, skip forwards
			i += s - 1;
			continue;
		}
		for (j = 0; j < pat_len; ++j) { // character matches, compare the rest
			if (data[i + j] != pat[j]) // doesn't match 
				break;
		}
		if (j == pat_len) // match found
			pos.push_back(i);
	}

	return pos;
}

/*Rabin Karp string matching algorithm*/
vector <size_t> search_rk(const string& pat, const string& data) {
	int pat_len = pat.size();
	int text_len = data.size();
	vector <size_t> pos;

	// hash the pattern and first substring of the data
	long long pat_h = hash(pat, pat_len);
	long long text_h = hash(data, pat_len);

	long long pow_b = 1;
	// prime base raised to the power of pat_len - 1, used for recalculating hash
	for (int i = 0; i < pat_len - 1; ++i)
		pow_b = (pow_b * BASE) % MOD;

	for (int i = 0; i <= text_len - pat_len; ++i) {
		int j;

		if (pat_h == text_h) { // hash values match, compare strings
			for (j = 0; j < pat_len; ++j) {
				if (data[i + j] != pat[j]) {// doesn't match (hash collision) 
					break;
				}
			}
			if (j == pat_len) // match found
				pos.push_back(i);
		}

		// recalculate hash value for the next substring
		if (i < text_len - pat_len) {
			rehash(text_h, int(data[i]), int(data[i + pat_len]), pow_b);
		}
	}

	return pos;
}

int main() {
	string filepath, data, pat;

	cout << "Leave input fields empty to find instances of 192.168.1.123 in conn.csv.\n";

	// get user input
	cout << "\nEnter a filepath: ";
	getline(cin, filepath);
	cout << "Enter a pattern: ";
	getline(cin, pat);

	// preset values
	if (filepath.empty())
		filepath = "files/conn.csv";
	if (pat.empty())
		pat = "192.168.1.123";

	load_data(filepath, data);

	// if file is not found ask for another filepath
	while (data.empty()) {
		cout << "Enter a another one: ";
		getline(cin, filepath);
		if (filepath.empty())
			filepath = "files/conn.csv";
		load_data(filepath, data);
	}

	if (pat.size() <= data.size()) {
		// run and measure runtime of both algorithms
		steady_clock::time_point start = steady_clock::now();
		vector <size_t> pos = search_bmh(pat, data);
		steady_clock::time_point end = steady_clock::now();

		auto time = duration_cast<microseconds>(end - start).count();
		cout << "\nBoyer-Moore-Horspool runtime: " << time << " microseconds.\n";

		start = steady_clock::now();
		pos = search_rk(pat, data);
		end = steady_clock::now();

		time = duration_cast<microseconds>(end - start).count();
		cout << "Rabin Karp runtime: " << time << " microseconds.\n";

		cout << "\nFound " << pos.size() << " instances of " << pat << " in given file.\n";

		// output positions to the user
		// (not used here as there are many instances of a pattern, consider doing operations on the returned vector if user needs to do more than just find how often a pattern appears)	
		/*if (!pos.empty())
		{
			for (int i : pos)
				cout << "Found '" << pat << "' at index " << i << ":\n";
		}*/
	}
	else {
		cout << "\nPattern length exceeds given data length!\n";
	}

	system("pause");
	return 0;
}