#include <iostream>
#include <vector>
#include <string>
using namespace std;


vector<int> filter(vector<vector<string>> tags, string target) {
	vector<int> res;
	for (int i = 0; i < tags.size(); i++) {
		for (int j = 0; j < tags[i].size(); j++) {
			if (tags[i][j] == target) {
				res.push_back(i);
			}
		}
	}
	return res;
}

int main() {
	vector<vector<string>> vec;
	vec = {
		{"���","������","ð��"},// 0
		{"���","����","��ʥ�ٻ�","����Ԫ","����"}, // 1
		{"����Ԫ","����"} // 2
	};
	vector<int> res;
	string target;
	cin >> target;
	res = filter(vec, target);
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i] << endl;
	}
	return 0;
}