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
		{"奇幻","大世界","冒险"},// 0
		{"奇幻","卡牌","七圣召唤","二次元","老婆"}, // 1
		{"二次元","动作"} // 2
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