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
	//识别标注后的句子；
	String^ NERSentence = gcnew String("");
	//按正则表达式匹
	//r1:请/v 在/p 上午/t 九点/m 给/p 我/rr 倒/vi 杯/ng 茶/ng 放/v 在/p 餐桌/n 上/f 。/wj 
	String^ pattern1 = "^vptm(p|v)(rr|nr)(vi|v|a)(m|mq|ng|rz)(n|ng|nz)(v|vp)(n|nf|s)";//没处理“十四点、上午九点半、八点四十”的情况，一杯是分开标注，那么按规定的位置获取实体太拟合了。
	//正则表达式类
	Regex^ regex = gcnew Regex(pattern1);

	//按规则，标注实体
	if ( regex->IsMatch(flags) )
	{
		//cout << "Match";
		for (int i = 0; i< lineSplit->Length; i++)
		{
			if (lineSplit[i]->EndsWith("t") && lineSplit[i+1]->EndsWith("m"))
			{// 识别时间对象
				lineSplit[i] += " B-T";
				lineSplit[++i] += " I-T";
			}
			else if (lineSplit[i]->EndsWith("rr") || lineSplit[i]->EndsWith("nr"))
			{//识别服务对象 指令对象
				lineSplit[i] += " B-H";
				lineSplit[++i] += " B-S";
			}
			else if ( (lineSplit[i]->EndsWith("m")||lineSplit[i]->EndsWith("ng")||lineSplit[i]->EndsWith("rz")) && 
							(lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("ng") || lineSplit[i+1]->EndsWith("nz")) )
			{//识别目标对象1 杯茶
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( lineSplit[i]->EndsWith("q") && 
							(lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("ng")) )
			{//识别目标对象1 一杯茶
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( (lineSplit[i]->EndsWith("p")||lineSplit[i]->EndsWith("v")) &&  ( i < lineSplit->Length-2 && lineSplit[i+1]->EndsWith("n") && lineSplit[i+2]->EndsWith("f")  ))
			{//识别位置对象 在餐桌上
				lineSplit[i] += " N";
				lineSplit[++i] += " B-L";
				lineSplit[++i] += " I-L";
			}
			//i+1访问越界
			else if((lineSplit[i]->EndsWith("p")||lineSplit[i]->EndsWith("v"))&&( i<lineSplit->Length-1 && ( lineSplit[i+1]->EndsWith("s") || lineSplit[i+1]->EndsWith("n") ))  )
			{//识别位置对象 在门口
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
	//识别标注后的句子；
	String^ NERSentence = gcnew String("");
	//按正则表达式匹
	//句型规则：给/p 他/rr 一个/mq 橙子/n 放/v 在/p 餐桌/n 上/f 。/wj 
	//					帮/v 我/rr 拿/v 个/q 苹果/n 放/v 在/p 餐桌/n 上/f 。/wj 
	String^ pattern1 = "^(p|v)(rr|nr)(vi|v|a)?(m|mq|ng|rz|q)(n|ng|nz)(v|vp)(nf|s)";//没处理“十四点、上午九点半、八点四十”的情况，一杯是分开标注，那么按规定的位置获取实体太拟合了。
	//正则表达式类
	Regex^ regex = gcnew Regex(pattern1);

	//按规则，标注实体
	if ( regex->IsMatch(flags) )
	{
		//cout << "Match";
		for (int i = 0; i< lineSplit->Length; i++)
		{
			if (lineSplit[i]->EndsWith("t") && lineSplit[i+1]->EndsWith("m"))
			{// 识别时间对象
				lineSplit[i] += " B-T";
				lineSplit[++i] += " I-T";
			}
			else if (lineSplit[i]->EndsWith("rr") || lineSplit[i]->EndsWith("nr"))
			{//识别服务对象 指令对象  给我拿包烟
				lineSplit[i] += " B-H";
				lineSplit[++i] += " B-S";
			}
			else if (lineSplit[i]->EndsWith("p")  && (lineSplit[i+1]->EndsWith("rr")|| lineSplit[i+1]->EndsWith("nr")) && !lineSplit[i+2]->EndsWith("v") && !lineSplit[i+2]->EndsWith("vi") )
			{//识别服务对象 指令对象 给我一个苹果 的 给
				lineSplit[i] += " B-S";
				lineSplit[++i] += " B-H";
			}
			else if ( (lineSplit[i]->EndsWith("m")||lineSplit[i]->EndsWith("ng")||lineSplit[i]->EndsWith("rz")) && 
							(lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("ng") || lineSplit[i+1]->EndsWith("nz")) )
			{//识别目标对象1 杯茶
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( (lineSplit[i]->EndsWith("q")|| lineSplit[i]->EndsWith("mq")) && 
							(lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("ng")) )
			{//识别目标对象1 一杯茶
				lineSplit[i] += " N";
				lineSplit[++i] += " B-O";
			}
			else if ( (lineSplit[i]->EndsWith("p")||lineSplit[i]->EndsWith("v")) &&  ( lineSplit[i+1]->EndsWith("n") && lineSplit[i+2]->EndsWith("f")  ))
			{//识别位置对象 在餐桌上
				lineSplit[i] += " N";
				lineSplit[++i] += " B-L";
				lineSplit[++i] += " I-L";
			}
			else if( (lineSplit[i]->EndsWith("p")||lineSplit[i]->EndsWith("v")) && lineSplit[i+1]->EndsWith("s") )
			{//识别位置对象 在门口
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
	//句型规则：（请/v 在/p ）？（上午/t 六点/m ）？把/pba 米/n 蒸/v 上/vf 。/wj  
	//没有服务对象
	pattern = "^v?(ptm|t|tm)?(pba|p)nd?(v|vi)";
	Regex^ regex = gcnew Regex(pattern);
	if (regex->IsMatch(flags))
	{
		cout << "Rule2 Match";
		for (int i = 0; i < lineSplit->Length; i++)
		{
			if (lineSplit[i]->EndsWith("t"))
			{//识别时间 上午
				lineSplit[i] += " B-T";
				if (lineSplit[i+1]->EndsWith("m"))
				{//识别时间 几点
					lineSplit[++i] += " I-T";
				}
			}
			else if ((lineSplit[i]->EndsWith("pba") || lineSplit[i]->EndsWith("p")) && lineSplit[i+1]->EndsWith("n"))
			{//识别 目标对象
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
	//句型规则：（请/v 在/p ）？（上午/t 六点/m ）？ 晒/v 被子/n 。/wj  
	//没有服务对象 q是特殊处理，米/q
	pattern = "^v?(ptm|t|tm)?(v|vi)(n|q)";
	Regex^ regex = gcnew Regex(pattern);
	if (regex->IsMatch(flags))
	{
		cout << "Rule2 Match";
		for (int i = 0; i < lineSplit->Length; i++)
		{
			if (lineSplit[i]->EndsWith("t"))
			{//识别时间 上午
				lineSplit[i] += " B-T";
				if (lineSplit[i+1]->EndsWith("m"))
				{//识别时间 几点
					lineSplit[++i] += " I-T";
				}
			}
			else if ((lineSplit[i]->EndsWith("v") || lineSplit[i]->EndsWith("vi")) && ( lineSplit[i+1]->EndsWith("n") || lineSplit[i+1]->EndsWith("q")))
			{//识别 目标对象
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