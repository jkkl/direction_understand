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

Boolean ProcessRule(String^ flags,array<String^> ^lineSplit)
{
	//ʶ���ע��ľ��ӣ�
	String^ NERSentence = gcnew String("");
	//��������ʽƥ
	//r1:��/v ��/p ����/t �ŵ�/m ��/p ��/rr ��/vi ��/ng ��/ng ��/v ��/p ����/n ��/f ��/wj 
	String^ pattern1 = "^vptm(p|v)(rr|nr)(vi|v|a)(m|mq|ng|rz)(n|ng|nz)(v|vp)(n|nf|s)";//û����ʮ�ĵ㡢����ŵ�롢�˵���ʮ���������һ���Ƿֿ���ע����ô���涨��λ�û�ȡʵ��̫����ˡ�
	//������ʽ��
	Regex^ regex = gcnew Regex(pattern1);

	//�����򣬱�עʵ��
	if ( regex->IsMatch(flags) )
	{
		//cout << "Match";
		for (int i = 0; i< lineSplit->Length; i++)
		{
			if (lineSplit[i]->EndsWith("t") && lineSplit[i+1]->EndsWith("m"))
			{// ʶ��ʱ�����
				lineSplit[i] += " B-T";
				lineSplit[++i] += " I-T";
			}
			else if (lineSplit[i]->EndsWith("rr") || lineSplit[i]->EndsWith("nr"))
			{//ʶ�������� ָ�����
				lineSplit[i] += " B-H";
				lineSplit[++i] += " B-S";
			}
			else if ( (lineSplit[i]->EndsWith("m")||lineSplit[i]->EndsWith("ng")||lineSplit[i]->EndsWith("rz")) && 
							(lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("ng") || lineSplit[i+1]->EndsWith("nz")) )
			{//ʶ��Ŀ�����1 ����
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( lineSplit[i]->EndsWith("q") && 
							(lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("ng")) )
			{//ʶ��Ŀ�����1 һ����
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( (lineSplit[i]->EndsWith("p")||lineSplit[i]->EndsWith("v")) &&  ( i < lineSplit->Length-2 && lineSplit[i+1]->EndsWith("n") && lineSplit[i+2]->EndsWith("f")  ))
			{//ʶ��λ�ö��� �ڲ�����
				lineSplit[i] += " N";
				lineSplit[++i] += " B-L";
				lineSplit[++i] += " I-L";
			}
			//i+1����Խ��
			else if((lineSplit[i]->EndsWith("p")||lineSplit[i]->EndsWith("v"))&&( i<lineSplit->Length-1 && ( lineSplit[i+1]->EndsWith("s") || lineSplit[i+1]->EndsWith("n") ))  )
			{//ʶ��λ�ö��� ���ſ�
				lineSplit[i] += " N";
				lineSplit[++i] += " B-L";
			}
			else
			{
				lineSplit[i] += " N";
			}
		}
		
		return true;
	}
	else
	{
		//cout << "No Match";
		return false;
	}

	//return NERSentence;
}

Boolean ProcessRule4(String^ flags,array<String^> ^lineSplit)
{
	//ʶ���ע��ľ��ӣ�
	String^ NERSentence = gcnew String("");
	//��������ʽƥ
	//���͹��򣺸�/p ��/rr һ��/mq ����/n ��/v ��/p ����/n ��/f ��/wj 
	//					��/v ��/rr ��/v ��/q ƻ��/n ��/v ��/p ����/n ��/f ��/wj 
	String^ pattern1 = "^(p|v)(rr|nr)(vi|v|a)?(m|mq|ng|rz|q)(n|ng|nz)(v|vp)(nf|s)";//û����ʮ�ĵ㡢����ŵ�롢�˵���ʮ���������һ���Ƿֿ���ע����ô���涨��λ�û�ȡʵ��̫����ˡ�
	//������ʽ��
	Regex^ regex = gcnew Regex(pattern1);

	//�����򣬱�עʵ��
	if ( regex->IsMatch(flags) )
	{
		//cout << "Match";
		for (int i = 0; i< lineSplit->Length; i++)
		{
			if (lineSplit[i]->EndsWith("t") && lineSplit[i+1]->EndsWith("m"))
			{// ʶ��ʱ�����
				lineSplit[i] += " B-T";
				lineSplit[++i] += " I-T";
			}
			else if (lineSplit[i]->EndsWith("rr") || lineSplit[i]->EndsWith("nr"))
			{//ʶ�������� ָ�����  �����ð���
				lineSplit[i] += " B-H";
				lineSplit[++i] += " B-S";
			}
			else if (lineSplit[i]->EndsWith("p")  && (lineSplit[i+1]->EndsWith("rr")|| lineSplit[i+1]->EndsWith("nr")) && !lineSplit[i+2]->EndsWith("v") && !lineSplit[i+2]->EndsWith("vi") )
			{//ʶ�������� ָ����� ����һ��ƻ�� �� ��
				lineSplit[i] += " B-S";
				lineSplit[++i] += " B-H";
			}
			else if ( (lineSplit[i]->EndsWith("m")||lineSplit[i]->EndsWith("ng")||lineSplit[i]->EndsWith("rz")) && 
							(lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("ng") || lineSplit[i+1]->EndsWith("nz")) )
			{//ʶ��Ŀ�����1 ����
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( (lineSplit[i]->EndsWith("q")|| lineSplit[i]->EndsWith("mq")) && 
							(lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("ng")) )
			{//ʶ��Ŀ�����1 һ����
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( (lineSplit[i]->EndsWith("p")||lineSplit[i]->EndsWith("v")) &&  ( lineSplit[i+1]->EndsWith("n") && lineSplit[i+2]->EndsWith("f")  ))
			{//ʶ��λ�ö��� �ڲ�����
				lineSplit[i] += " N";
				lineSplit[++i] += " B-L";
				lineSplit[++i] += " I-L";
			}
			else if( (lineSplit[i]->EndsWith("p")||lineSplit[i]->EndsWith("v")) && lineSplit[i+1]->EndsWith("s") )
			{//ʶ��λ�ö��� ���ſ�
				lineSplit[i] += " N";
				lineSplit[++i] += " B-L";
			}
			else
			{
				lineSplit[i] += " N";
			}
		}
		return true;
	}
	else
	{
		return false;
	}

}

Boolean ProcessRule2(String^ flags,array<String^> ^lineSplit)
{
	String^ pattern = gcnew String("");
	//���͹��򣺣���/v ��/p ����������/t ����/m ������/pba ��/n ��/v ��/vf ��/wj  
	//û�з������
	pattern = "^v?(ptm|t|tm)?(pba|p)nd?(v|vi)";
	Regex^ regex = gcnew Regex(pattern);
	if (regex->IsMatch(flags))
	{
		cout << "Rule2 Match";
		for (int i = 0; i < lineSplit->Length; i++)
		{
			if (lineSplit[i]->EndsWith("t"))
			{//ʶ��ʱ�� ����
				lineSplit[i] += " B-T";
				if (lineSplit[i+1]->EndsWith("m"))
				{//ʶ��ʱ�� ����
					lineSplit[++i] += " I-T";
				}
			}
			else if ((lineSplit[i]->EndsWith("pba") || lineSplit[i]->EndsWith("p")) && lineSplit[i+1]->EndsWith("n"))
			{//ʶ�� Ŀ�����
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( i > 1 && lineSplit[i-1]->EndsWith("B-O") && (lineSplit[i]->EndsWith("v") || lineSplit[i]->EndsWith("vi")))
			{
				lineSplit[i] += " B-S";
			}
			else if ( lineSplit[i]->EndsWith("d") && (lineSplit[i+1]->EndsWith("v")||lineSplit[i+1]->EndsWith("vi")))
			{
				lineSplit[i] += " B-S";
				lineSplit[++i] += " I-S";
			}
			else
			{
				lineSplit[i] += " N";
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

Boolean ProcessRule3(String^ flags,array<String^> ^lineSplit)
{
	String^ pattern = gcnew String("");
	//���͹��򣺣���/v ��/p ����������/t ����/m ���� ɹ/v ����/n ��/wj  
	//û�з������ q�����⴦����/q
	pattern = "^v?(ptm|t|tm)?(v|vi)(n|q)";
	Regex^ regex = gcnew Regex(pattern);
	if (regex->IsMatch(flags))
	{
		cout << "Rule2 Match";
		for (int i = 0; i < lineSplit->Length; i++)
		{
			if (lineSplit[i]->EndsWith("t"))
			{//ʶ��ʱ�� ����
				lineSplit[i] += " B-T";
				if (lineSplit[i+1]->EndsWith("m"))
				{//ʶ��ʱ�� ����
					lineSplit[++i] += " I-T";
				}
			}
			else if ((lineSplit[i]->EndsWith("v") || lineSplit[i]->EndsWith("vi")) && ( lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("q")))
			{//ʶ�� Ŀ�����
				lineSplit[i] += " B-S";
				lineSplit[++i] += " B-O";
			}
			else
			{
				lineSplit[i] += " N";
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}