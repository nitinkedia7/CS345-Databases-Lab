#include <bits/stdc++.h>
using namespace std;

int main() {
    cout << 1 << endl;
    int a, b;
    cin >> a >> b;
    cout << a << endl;
    cout << b << endl;
    int x, r;
    cin >> x;
    srand(time(NULL));
    for (int i = 0; i < x; i++) {
        r = rand() % (10*x+1);
        cout << 2 << endl;
        cout << r << endl;
    }
    cout << 4 << endl;
    cout << -1 << endl;
}