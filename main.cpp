#include <latch>
#include <iostream>
#include <syncstream>
#include <chrono>
#include <thread>

using namespace std;

void f(char x, int y)
{
    this_thread::sleep_for(chrono::milliseconds(1000));
    osyncstream(cout) << "Completed action " << y << " of the set " << x << endl;
}

int main()
{
    int nt = 5, a = 8, b = 5, c = 5, d = 5, e = 5, F = 7, g = 4, h = 4, i = 5, j = 9, k = 4, m = 4, n = 3;
    latch stepDG(a), stepHI(b+e), stepK(d+h+j), stepN(F+g+i);
    vector<jthread> calc;
    
    auto start = chrono::system_clock::now();
    osyncstream(cout) << "Calculation has begun." << endl;
    calc.emplace_back([&](){
        for (int x = 1; x <= a/2; ++x) {f('a', x);}
        stepDG.arrive_and_wait(a/2);
        
        for (int y = 1; y <= g; ++y)
        {
            f('g', y);
            stepN.count_down();
        }

        for (int z = 1; z <= d / 2; ++z)
        {
            f('d', z);
            stepK.count_down();
        }
        stepK.wait();

        for (int aa = 1; aa <= k/2; ++aa) {f('k', aa);}
    });

    calc.emplace_back([&a, &d, &e, &k, &stepDG, &stepHI, &stepK](){
        for (int x = 5; x <= a; ++x)
        {
            f('a', x);
            stepDG.count_down();
        }

        for (int y = 1; y <= e; ++y)
        {
            f('e', y);
            stepHI.count_down();
        }

        for (int z = (d/2) + 1; z <= d; ++z)
        {
            f('d', z);
            stepK.count_down();
        }
        stepK.wait();

        for (int aa = (k/2) + 1; aa <= k; ++aa) {f('k', aa);}
    });

    calc.emplace_back([&b, &F, &h, &n, &stepHI, &stepK, &stepN](){
        for (int x = 1; x <= 3; ++x)
        {
            f('b', x);
            stepHI.count_down();
        }
        stepHI.wait();

        for (int y = 1; y <= h; ++y)
        {
            f('h', y);
            stepK.count_down();
        }

        for (int z = 1; z <= (F/3) + 1; ++z)
        {
            f('f', z);
        }
        stepN.arrive_and_wait((F/3) + 1);

        f('n', n/3);
    });

    calc.emplace_back([&b, &F, &i, &n, &stepHI, &stepN](){
        for (int x = 4; x <= b; ++x)
        {
            f('b', x);
            stepHI.count_down();
        }
        stepHI.wait();

        for (int y = 1; y <= i; ++y)
        {
            f('i', y);
            stepN.count_down();
        }

        for (int z = F/2 + 1; z < F-1; ++z)
        {
            f('f', z);
        }
        stepN.arrive_and_wait(F/3);

        f('n', n-1);
    });

    calc.emplace_back([&c, &F, &j, &n, &stepK, &stepN](){
        for (int x = 1; x <= c; ++x) {f('c', x);}

        for (int y = 1; y <= j; ++y)
        {
            f('j', y);
            stepK.count_down();
        }

        for (int z = F-1; z <= F; ++z)
        {
            f('f', z);
        }
        stepN.arrive_and_wait(F/3);

        f('n', n);
    });

    for (int i = 0; i < nt; ++i) {calc[i].join();};
    auto end = chrono::system_clock::now();
    osyncstream(cout) << "Calculation complete. Time elapsed: " << chrono::duration<double>(end - start) << endl;
    return 0;
}