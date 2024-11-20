#include <iostream>
using namespace std;

/**
 * 友元的三种实现:
 *      1、全局函数做友元
 *      2、类做友元
 *      3、成员函数做友元
*/

// 1、全局函数做友元
// class Building
// {
// 	//告诉编译器 goodGay全局函数 是 Building类的好朋友，可以访问类中的私有内容
// 	friend void goodGay(Building * building);

// public:

// 	Building()
// 	{
// 		this->m_SittingRoom = "客厅";
// 		this->m_BedRoom = "卧室";
// 	}


// public:
// 	string m_SittingRoom; //客厅

// private:
// 	string m_BedRoom; //卧室
// };


// void goodGay(Building * building)
// {
// 	cout << "好基友正在访问： " << building->m_SittingRoom << endl;
// 	cout << "好基友正在访问： " << building->m_BedRoom << endl;
// }


// int main(){

//     Building b;
// 	goodGay(&b);

// 	return 0;
// }




// 2、类作友元

// class Building;     // 将被访问的类申请在前面
// class goodGay
// {
// public:

// 	goodGay();  // goodGay的类构造函数  
// 	void visit();

// private:
// 	Building *building;     // 给被访问的类放在访问类的成员变量中
// };


// class Building
// {
// 	//告诉编译器 goodGay类是Building类的好朋友，可以访问到Building类中私有内容
// 	friend class goodGay;

// public:
// 	Building();

// public:
// 	string m_SittingRoom; //客厅
// private:
// 	string m_BedRoom;//卧室
// };

// Building::Building()
// {
// 	this->m_SittingRoom = "客厅";
// 	this->m_BedRoom = "卧室";
// }

// goodGay::goodGay()      // 实现构造函数
// {
// 	building = new Building;
// }

// void goodGay::visit()
// {
// 	cout << "好基友正在访问" << building->m_SittingRoom << endl;
// 	cout << "好基友正在访问" << building->m_BedRoom << endl;
// }


// int main(){

// 	goodGay gg;
// 	gg.visit();

// 	return 0;
// }



// 3、成员函数做友元


class Building;
class goodGay
{
public:
	goodGay();
	void visit(); // 只让visit函数作为Building的好朋友，可以发访问Building中私有内容
	void visit2(); 

private:
	Building *building;
};


class Building
{
	//告诉编译器  goodGay类中的visit成员函数 是Building好朋友，可以访问私有内容
	friend void goodGay::visit();

public:
	Building();

public:
	string m_SittingRoom; //客厅
private:
	string m_BedRoom;//卧室
};

Building::Building()
{
	this->m_SittingRoom = "客厅";
	this->m_BedRoom = "卧室";
}

goodGay::goodGay()
{
	building = new Building;
}

void goodGay::visit()
{
	cout << "好基友正在访问" << building->m_SittingRoom << endl;
	cout << "好基友正在访问" << building->m_BedRoom << endl;
}

void goodGay::visit2()
{
	cout << "好基友正在访问" << building->m_SittingRoom << endl;
	//cout << "好基友正在访问" << building->m_BedRoom << endl;
}



int main(){
    
    goodGay  gg;
	gg.visit();

	return 0;
}



