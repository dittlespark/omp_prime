#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <ctime>
#include <cassert>
#include <climits>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <bitset>
#include <algorithm>
#include <omp.h>
#define MST(a, b) memset(a, b, sizeof(a))
#define REP(i, a) for (int i = 0; i < int(a); i++)
#define REPP(i, a, b) for (int i = int(a); i <= int(b); i++)
#define NUM_THREADS 4
using namespace std;
const int N = 1e8;
int vis[N], p[N], tot;
void prime(int n) {
	for (int i = 2; i < n; i++) {
		if (!vis[i]) p[tot++] = i;
		for (int j = 0; j < tot && i * p[j] < n; j++) {
			vis[i * p[j]] = p[j];
			if (i % p[j] == 0) break;
		}
	}
}
int main() {
	ios::sync_with_stdio(0);
	clock_t st, ed, start;
	//串行
	st = clock();
	prime(N);
	ed = clock();
	cout << fixed << setprecision(10) << "串行: " << 1.0 * (ed - st) / CLOCKS_PER_SEC << endl;
	cout << "Prime total: " << tot << endl;
	//输出到文件
	ofstream fout("Prime", ios::out);
	REPP(i, 2, N - 1) {
		if (!vis[i]) {
			fout << i << endl;
		}
	}
	//初始化
	MST(vis, 0), tot = 0;
	//并行
	omp_set_num_threads(NUM_THREADS);
	 st = clock();
	prime(10000);
	//ed = clock();
	//cout << fixed << setprecision(10) << "并行第一部分: " << 1.0 * (ed - st) / CLOCKS_PER_SEC
		//<< endl; 
	//st = clock();
	int i, j;
#pragma omp parallel shared (p, vis) private (i, j)
	{
#pragma omp for schedule (dynamic)
		for (i = 0; i < tot; i++) {
			for (j = p[i] << 1; j < N; j += p[i]) {
				vis[j] = 1;
			}
		}
	}
	ed = clock();
	//cout << fixed << setprecision(10) << "并行第二部分: " << 1.0 * (ed - st) / CLOCKS_PER_SEC
		//<< endl;
	cout << fixed << setprecision(10) << "并行: " << 1.0 * (ed - st) / CLOCKS_PER_SEC <<
		endl;
	tot = 0;
	for (int i = 2; i < N; i++) if (!vis[i]) {
		p[tot++] = i;
	}
	cout << "Prime total: " << tot << endl;
	system("pause");
	return 0;
}