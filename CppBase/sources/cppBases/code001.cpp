#include <string>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    int arr[5] = {1, 3, 5, 7, 9};
    cout << *arr + 10 << endl;
    cout << *(arr + 4) << endl;

    return 0;
}
