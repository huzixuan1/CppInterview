#include <iostream>
using namespace std;
// nullptr可以调用成员函数吗
// 可以的，因为在编译时对象就绑定了函数地址，和指针空不空没关系

// class animal{
// public:
//     void sleep(){ cout << "animal sleep" << endl; }
//     void breathe(){ cout << "animal breathe haha" << endl; }
// };
// class fish :public animal{
// public:
//     void breathe(){ cout << "fish bubble" << endl; }
// };
// int main(){
//     animal *pAn=nullptr;
//     pAn->breathe();   // 输出：animal breathe haha
//     fish *pFish = nullptr;
//     pFish->breathe(); // 输出：fish bubble
//     return 0;
// }  

class animal{
    public:
        void sleep(){
            cout<<"动物睡觉"<<endl;
        }
        void breathe(){
            cout<<"动物呼吸"<<endl;
        }
};

class fish :public animal{
    public:
        void sleep(){
            cout<<"鱼睁着眼睡觉"<<endl;
        }
};

int main(){
    animal *ptr=nullptr;
    ptr->breathe();

    fish *p=nullptr;
    p->sleep();
    return 0;
}