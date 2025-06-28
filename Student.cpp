#include "Student.h"

int Student::stunum = 0;
int Student::nonum = 0;

Student::Student(const long long& no1, const string& name1, const double& c, const double& math1, const int& old1, const bool& sex1, const string& adress1)
	:no(no1), name(name1), cscore(rand()%101), math(rand()%101), old(old1), sex(sex1), adress(adress1)
{
}

Student::Student(const Student& stu)
{
	no = stu.no;
	name = stu.name;
	cscore = stu.cscore;
	math = stu.math;
	old = stu.old;
	sex = stu.sex;
	adress = stu.adress;
}

Student Student::operator=(const Student& stu)
{
	no = stu.no;
	name = stu.name;
	cscore = stu.cscore;
	math = stu.math;
	old = stu.old;
	sex = stu.sex;
	adress = stu.adress;
	return *this;
}
