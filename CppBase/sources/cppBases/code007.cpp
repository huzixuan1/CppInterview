#include <map>
#include <set>
#include <iostream>
using namespace std;

int main()
{
    set<int> sets;
    sets.insert(5);
    sets.insert(3);
    sets.insert(4);
    sets.insert(8);
    sets.insert(3);

    for (auto it = sets.begin(); it != sets.end(); ++it)
    {
        cout << *it << " ";
    }

    cout << endl;

    map<int,string> base;
    base.insert(make_pair<int,string>(1,"tom"));
    base.insert(make_pair<int,string>(2,"lady"));
    for(auto it=base.begin();it!=base.end();++it){
        cout<<it->first<<" "<<it->second<<endl;
    }
    

    return 0;
}