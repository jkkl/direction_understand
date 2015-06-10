#include <iostream>

#include "algo.h"

using namespace std;
/*
** �������������Լ��
**	in: a 
**  in: b
**	out: gdc ���Լ�� -1 error
*/

int gdc(int a, int b)
{
	if(a == 0 || b==0 ){
		cout << "error inupt 0\n";
		return -1;
	}
	int out,r,temp;
	out = a > b ? a : b;
	r = a > b ? b : a;
	while (r)
	{
		temp = r;
		r = out%r;
		out = temp;
	}
	return out;
}