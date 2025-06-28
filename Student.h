#pragma once
#include <iostream>
#include <cstring>
using namespace std;

#define MAN 0
#define WOMEN 1

class Student
{
public:
	Student(const long long& no1 = 32400000000, const string& name1 = "新建学生",
		const double& c = 0, const double& math1 = 0, const int& old1 = 1,
		const bool& sex1 = MAN, const string& adress1= "未填写");
	~Student() = default;
	Student(const Student& stu);
	Student operator=(const Student& stu);
public:
	long long no;//学号
	string name;//姓名
	double cscore;//C++成绩
	double math;//数学成绩
	int old;//年龄
	bool sex;//性别
	string adress;//地址
	static int stunum;//学生数量
	static int nonum;//已生成的学号数
};

