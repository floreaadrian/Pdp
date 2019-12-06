#include <iostream>
#include <thread>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <mutex>

using namespace std;

struct elem
{
    int val;
    mutex mtx;
};

int  n = 50, noThreads = 50, activeThreads = 0;
vector<int> a(n);
vector<elem> b(n);
vector<int> c(n);
vector<thread> threads;

void assignRandomValues()
{
    for (int i = 0; i < n; i++)
    {
        int element = rand() % 50 + 1;
        a[i] = b[i].val = element;
    }
}

void increasePartialSum(vector<elem>& arr, int k, int i)
{
    arr[i].mtx.lock();
    arr[i].val += arr[i - k].val;
    arr[i].mtx.unlock();
}

void partialSums()
{
    int k;
    for (k = 1; k < n; k = k * 2)
    {
        for (int i = 2 * k - 1; i < n; i += 2 * k)
        {
            if (activeThreads < noThreads)
            {
                threads.push_back(thread(increasePartialSum, ref(b), k, i));
                activeThreads++;
            }
            else
            {
                increasePartialSum(b, k, i);
            }
        }
    }

    activeThreads -= threads.size();
    for (unsigned int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
    threads.clear();

    k = k / 4;
    for (; k > 0; k = k / 2)
    {
        for (int i = 3 * k - 1; i < n; i += 2 * k)
        {
            if (activeThreads < noThreads)
            {
                threads.push_back(thread(increasePartialSum, ref(b), k, i));
                activeThreads++;
            }
            else
            {
                increasePartialSum(b, k, i);
            }
        }
    }

    for (unsigned int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
    threads.clear();
}

void sumK()
{
    c[0] = a[0];
    for (int i = 1; i < n; i++)
    {
        c[i] = c[i - 1] + a[i];
    }
}

int main()
{
    srand(time(NULL));

    assignRandomValues();

    auto startTime0 = std::chrono::high_resolution_clock::now();
    sumK();
    auto finishTime0 = std::chrono::high_resolution_clock::now();

    auto startTime1 = std::chrono::high_resolution_clock::now();
    partialSums();
    auto finishTime1 = std::chrono::high_resolution_clock::now();

    for_each(a.begin(), a.end(), [](int el) { cout << el << " "; });
    cout << "\n";
    for_each(c.begin(), c.end(), [](int el) { cout << el << " "; });
    cout << "\n";
    for (unsigned int i = 0; i < b.size(); i++)
        cout << b[i].val << " ";

    std::chrono::duration<double> timeElapsed0 = finishTime0 - startTime0;
    cout << "\nTotal execution [basic version]: " << timeElapsed0.count() * 1000 << " ms\n";

    std::chrono::duration<double> timeElapsed1 = finishTime1 - startTime1;
    cout << "\nTotal execution [thread version]: " << timeElapsed1.count() * 1000 << " ms\n";

    return 0;
}
