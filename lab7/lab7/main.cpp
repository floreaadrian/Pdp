#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <assert.h>
#include <cassert>
#include <chrono>

using namespace std;

const int maxlg = 22;
const int maxn = 5000005;

int n;
long long dp[maxlg][maxn], sum[maxn], psum[maxn];
long long b[maxn];

const int T = 5;

//#define debug

inline void alg() {
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t k=1; k<n; k = k*2) {
        for(size_t i=2*k-1; i<n; i+=2*k) { // in parallel
            b[i] += b[i-k];
        }
    }
    // Then, compute each partial sum as a sum of 2^j groups:
    int k = n/4;
    for(;k>0; k = k/2) {
        for(size_t i=3*k-1 ; i<n ; i+=2*k) { // in parallel
            b[i] += b[i-k];
        }
    }
    
    auto finish = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = finish - start;
    cout << "From online of an array with " << n
        << " elements took me " << elapsed.count()<< " seconds\n";
}

inline void doIt(int idx) {
  for(int i = idx; i < n; i += T) {
    int act = 0;
    int now = i + 1;
    for(int bit = 0; (1 << bit) <= now; ++ bit) {
      if(now & (1 << bit)) {
        sum[i] += dp[bit][act];
        act += (1 << bit);
      }
    }
  }
}

void generateArray(int size) {
    for(int i=0;i<size;++i) {
        dp[0][i] = rand()%100+1;
        b[i] = dp[0][i];
    }
}

void solveWithThreads() {
    auto start = std::chrono::high_resolution_clock::now();
    for(int k = 1; (1 << k) < maxn; ++ k) {
      for(int i = 0; i < n; ++ i) {
        dp[k][i] = dp[k - 1][i] + dp[k - 1][i + (1 << (k - 1))];
      }
    }

    vector <thread> th;
    for(int i = 0; i < min(T, n); ++ i) {
      th.push_back(thread(doIt, i));
    }
    for(int i = 0; i < th.size(); ++ i) {
      th[i].join();
    }
    auto finish = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = finish - start;
    cout << "(Threaded) Computing partial sums of an array with " << n
        << " elements took me " << elapsed.count()<< " seconds\n";
}

void solveWithoutThreads(){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<n;++i)
        sum[i]+=dp[0][i];
    auto finish = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = finish - start;
    cout << "Computing partial sums of an array with " << n
        << " elements took me " << elapsed.count()<< " seconds\n";
}

int main() {
    n = 1000000;
//
//  ifstream fin("/Users/adrianflorea/Codes/Pdp/lab7/lab7/sums.in");
//
//  fin >> n;
//  for(int i = 0; i < n; ++ i) {
//    fin >> dp[0][i];
//    psum[i] = psum[i - 1] + dp[0][i];
//  }
  /// dp[k][i] = sum(a[j]) where i <= j <= i + 2^k - 1
  // dp[k][i] =
    solveWithThreads();
    generateArray(n);
    for(int i=0;i<n;++i)
        sum[i] = 0;
    solveWithoutThreads();
    generateArray(n);
    alg();
  return 0;
}
