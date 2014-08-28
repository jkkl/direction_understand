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

//按正则表达式匹
//r1:请/v 在/p 上午/t 九点/m 给/p 我/rr 倒/vi 杯/ng 茶/ng 放/v 在/p 餐桌/n 上/f 。/wj 
Boolean ProcessRule(String^ flags,array<String^> ^lineSplit);

//句型规则：给/p 他/rr 一个/mq 橙子/n 放/v 在/p 餐桌/n 上/f 。/wj 
//					帮/v 我/rr 拿/v 个/q 苹果/n 放/v 在/p 餐桌/n 上/f 。/wj 
Boolean ProcessRule4(String^ flags,array<String^> ^lineSplit);

//句型规则：（请/v 在/p ）？（上午/t 六点/m ）？把/pba 米/n 蒸/v 上/vf 。/wj  
//没有服务对象
Boolean ProcessRule2(String^ flags,array<String^> ^lineSplit);

//句型规则：（请/v 在/p ）？（上午/t 六点/m ）？ 晒/v 被子/n 。/wj  
//没有服务对象 q是特殊处理，米/q
Boolean ProcessRule3(String^ flags,array<String^> ^lineSplit);