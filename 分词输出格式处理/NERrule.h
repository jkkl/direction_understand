#include<fstream>
#include<iostream>
#include<string>
#include<cliext\vector>
#using <system.dll>

using namespace System;
using namespace System::IO;
using namespace std;
using namespace cliext;
using namespace System::Text::RegularExpressions;

//��������ʽƥ
//r1:��/v ��/p ����/t �ŵ�/m ��/p ��/rr ��/vi ��/ng ��/ng ��/v ��/p ����/n ��/f ��/wj 
Boolean ProcessRule(String^ flags,array<String^> ^lineSplit);

//���͹��򣺸�/p ��/rr һ��/mq ����/n ��/v ��/p ����/n ��/f ��/wj 
//					��/v ��/rr ��/v ��/q ƻ��/n ��/v ��/p ����/n ��/f ��/wj 
Boolean ProcessRule4(String^ flags,array<String^> ^lineSplit);

//���͹��򣺣���/v ��/p ����������/t ����/m ������/pba ��/n ��/v ��/vf ��/wj  
//û�з������
Boolean ProcessRule2(String^ flags,array<String^> ^lineSplit);

//���͹��򣺣���/v ��/p ����������/t ����/m ���� ɹ/v ����/n ��/wj  
//û�з������ q�����⴦����/q
Boolean ProcessRule3(String^ flags,array<String^> ^lineSplit);