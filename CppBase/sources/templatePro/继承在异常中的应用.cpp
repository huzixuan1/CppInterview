#include <iostream>
#include <cstring>
using namespace std;

// 异常基类
class BaseMyException{
public:
    virtual void what()=0;
    virtual ~BaseMyException(){}
};

class TargetSpaceNullException:public BaseMyException{
public:
    virtual void what(){
        cout<<"目标空间空！"<<endl;
    }
    ~TargetSpaceNullException(){
    }
};

class SourceSpaceNullException:public BaseMyException{
public:
    virtual void what(){
        cout<<"源文件空间空！"<<endl;
    }
    ~SourceSpaceNullException(){
    }  
};


void copy_str(char *target, char *source)
{
    if (target == nullptr)
    {
        throw TargetSpaceNullException();
        return;
    }

    if (source == nullptr)
    {
        throw SourceSpaceNullException();
    }

    int len=strlen(source)+1;
    while (*source != '\0')
    {
        *target = *source;
        target++;
        source++;
    }
}

int main()
{
    char* source = "abcdefg";
    char buf[1024] = {0};
    try{
        copy_str(buf,source);
    }catch(BaseMyException& ex){
        ex.what();
    }
    
    cout << buf << endl;

    return 0;
}