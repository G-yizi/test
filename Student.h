#pragma once
#include <iostream>
#include <cstring>
using namespace std;

#define MAN 0
#define WOMEN 1

class Student
{
public:
	Student(const long long& no1 = 32400000000, const string& name1 = "�½�ѧ��",
		const double& c = 0, const double& math1 = 0, const int& old1 = 1,
		const bool& sex1 = MAN, const string& adress1= "δ��д");
	~Student() = default;
	Student(const Student& stu);
	Student operator=(const Student& stu);
public:
	long long no;//ѧ��
	string name;//����
	double cscore;//C++�ɼ�
	double math;//��ѧ�ɼ�
	int old;//����
	bool sex;//�Ա�
	string adress;//��ַ
	static int stunum;//ѧ������
	static int nonum;//�����ɵ�ѧ����
};

