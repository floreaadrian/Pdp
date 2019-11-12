#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <future>
#include <queue>
#include <tuple>
#include <chrono>
#include <mutex>
#include <condition_variable>

#define maxn 100

using namespace std;
int numberElemLine, a[maxn][maxn], b[maxn][maxn], c[maxn][maxn], rez[maxn][maxn];
bool finished[maxn];

mutex mx[10];
condition_variable cv[10];
queue<tuple<int, int, int>> q;

inline void producer(int line, int T)
{
  for (int i = line; i < numberElemLine; i += T)
  {
    for (int j = 0; j < numberElemLine; ++j)
    {
      int aux_i_j = 0;
      for (int k = 0; k < numberElemLine; ++k)
      {
        aux_i_j += a[i][k] * b[k][j];
      }
      {
        lock_guard<mutex> lk(mx[line]);
        q.push(make_tuple(i, j, aux_i_j));
      }
    }
  }
    cout<<"producer -> "<<line<<"\n";
  finished[line] = true;
  cv[line].notify_all();
}

inline void consumer(int tid, int T)
{
    while (true)
  {
    unique_lock<mutex> lk(mx[tid]);
    cv[tid].wait(lk, [tid]{return finished[tid] == true || !q.empty();});
      cout<<"consumer -> "<<tid<<"\n";
    if(q.empty())
      break;
    tuple<int, int, int> el = q.front();
    q.pop();
    int i = get<0>(el);
    int j = get<1>(el);
    int x = get<2>(el);
    for (int k = 0; k < numberElemLine; ++k)
    {
      rez[i][k] += x * c[j][k];
    }
  }
}

void initMat(int k[][3]){
    k[0][0]=1;
    k[0][1]=2;
    k[0][2]=3;
    k[1][0]=1;
    k[1][1]=2;
    k[1][2]=3;
    k[2][0]=1;
    k[2][1]=2;
    k[2][2]=3;
}

double mainFunc(int nr, int numberOfThreads)
{
//    initMat(a);
//    initMat(b);
//    initMat(c);
    numberElemLine = nr;
  auto start = chrono::high_resolution_clock::now();
    for(int i = 0;i<nr;++i){
        finished[i] = false;
    }
  vector<thread> producers, consumers;
  for (int i = 0; i < min(numberElemLine, numberOfThreads); ++i)
  {
    producers.push_back(thread(producer, i, numberOfThreads));
  }
  for (int i = 0; i < min(numberElemLine, numberOfThreads); ++i)
  {
    consumers.push_back(thread(consumer, i, numberOfThreads));
  }
  for (int i = 0; i < consumers.size(); ++i)
  {
    consumers[i].join();
  }
  for (int i = 0; i < producers.size(); ++i)
  {
    producers[i].join();
  }
  auto finish = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed4 = finish - start;
//    for(int i=0;i<numberElemLine;++i){
//        for(int j=0;j<numberElemLine;++j){
//            cout<<rez[i][j]<<" ";
//        }
//        cout<<'\n';
//    }
  return elapsed4.count();
}

void statistics(int nr, int numberOfThreads)
{
  double nrNormal = 0;
  int iterationNumber = 20;
  for (int i = 0; i < iterationNumber; ++i)
  {
    nrNormal += mainFunc(nr, numberOfThreads);
  }
  cout << "---------------------------\n";
  cout << numberOfThreads << "       |  " << nrNormal / iterationNumber << "\n";
}

int main()
{
  int nrOfEl = 3;
  cout << "Threads | Time\n";
    mainFunc(100,2);

  return 0;
}
