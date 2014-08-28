#include <iostream>
#include <string>
#include <map>
#include <hash_map>
#include <time.h>.
#include <fstream>
#include <queue>
#include <strstream>
#include <sstream>
#include "Tools.h"
using namespace System::IO;
using namespace std;
using namespace System;
using namespace System::Runtime::InteropServices;

//基本数据类型转换
template <class T>
void convertFromString(T &value,const string &s)
{
	stringstream ss(s);
	ss >> value;
}

template <class T>
void convertFromInt(T &value,const int &i)
{
	stringstream ss;
	ss << i;
	ss >> value;
}

void split(const std::string& src, const std::string& separator, std::vector<std::string>& dest)
{
    std::string str = src;
    std::string substring;
    std::string::size_type start = 0, index;

    do
    {
        index = str.find_first_of(separator,start);
        if (index != std::string::npos)
        {    
            substring = str.substr(start,index-start);
            dest.push_back(substring);
            start = str.find_first_not_of(separator,index);
            if (start == std::string::npos) return;
        }
    }while(index != std::string::npos);
    
    //the last token
    substring = str.substr(start);
    dest.push_back(substring);
}
/* 
**	语料文本数值化
**	infile  输入文件
**	indict	词典
**  out 输出文件
*/
void tagWordToInt(const char* fileDir,const char* dictDir, const char* outDir)  
{  
	std::ofstream outfile;
	std::ofstream outfile2;
	std::ifstream infile;
	std::fstream iofile;
	infile.open(fileDir);
	iofile.open(dictDir);
	outfile.open(outDir);

	if (!infile)
	{
		cout << "Can't opent file \n";
		return;
	}
	hash_map<string,int> wordcount;
	hash_map<string,int> wordint;
	hash_map<string,int>::iterator wcIter;
	hash_map<string,int>::iterator hmIter;
	
	string line, outline = "";
	//const char* outline;
	vector<string> tokens;
	vector<string> vline;
	vector<int> vint;
	int lastId = 0;
	stringstream ss;
	//现将词典的词条：id 全部存入hashmap
	while(std::getline(iofile,line)){
		split(line," ",vline);
		if(vline.size()>1){
			wordint.insert(pair<string,int>(vline[0],lastId = atoi(vline[1].c_str())));
		}
		vline.clear();
	}
	iofile.close();
	outfile2.open(dictDir,ios_base::app);
	//vline.clear();
	//获取语料文件的 词块
	//string outline = "";
	while (std::getline(infile,line)){
		split(line," ",vline);
		if (vline.size() > 2)
		{
			for (int i = 0; i < vline.size(); i++)
			{//从HashMap中找词块的id
				if((hmIter = wordint.find(vline[i])) != wordint.end()){//词典中有该词
					vint.push_back(hmIter->second);//找到 则将词块和id拼接 词块:id写入输出文件
				}else{//词典中没有该词
					wordint.insert(pair<string,int>(vline[i],++lastId));//否则 该词块为新词，将其写入词典后，再拼接
					string temp;
					convertFromInt(temp,lastId);
					string newdict = vline[i] + " "+temp +"\n"; 
					outfile2.write(newdict.c_str(),strlen(newdict.c_str()));
					vint.push_back(lastId);
				}
				string temp;
				convertFromInt(temp,vint[i]);
				outline += vline[i]+":"+temp+" " ;
				
			}
		}
		vline.clear();
		vint.clear();
		if(outline.length() < 1) 
			continue;
		outline.replace(outline.length()-1,1,"\n");
		outfile.write(outline.c_str(),strlen(outline.c_str()));
		outline = "";

	}
	
	infile.close();
	outfile.close();
	iofile.close();
}

/*统计文件中所有以空格隔开的词条，编号后输出到输出文件
**	in 输入文件
**  out 输出文件
*/
void getWordDict(const char* in, const char* out)  
{  
	std::ofstream outfile;
	std::ifstream infile;
	infile.open(in);
	outfile.open(out);
	if (!infile)
	{
		cout << "Can't opent file \n";
		return;
	}
	hash_map<string,int> wordcount;
	hash_map<string,int>::iterator wcIter;
	unsigned char temp[2]="";	//判断字符的高位是否再gbk编码范围内
	string line, outline;
	//const char* outline;
	vector<string> tokens;
	stringstream ss;
  
	int id = 1;
	while (std::getline(infile,line))
	{
		split(line," ",tokens);
		for (int i = 0; i < tokens.size(); i++)
		{
			if (wordcount.find(tokens[i]) == wordcount.end())
			{
				wordcount.insert(pair<string,int>(tokens[i],id++));
				wcIter = wordcount.find(tokens[i]);
				if (wcIter != wordcount.end())
				{
					outline.append(wcIter->first);
					outline.append(" ");
					ss << wcIter->second;
					string temp = ss.str();
					outline.append(temp);
					outline.append("\n");
					outfile.write(outline.c_str(),strlen(outline.c_str()));
					outline="";
					ss.str("");
				}
			}
		}
		tokens.clear();
	}
	infile.close();
	outfile.close();
}

//将 String 转为 wstring
void MarshalString ( String ^ s, string& os ) {
   using namespace Runtime::InteropServices;
   const char* chars = 
      (const char*)(Marshal::StringToHGlobalUni(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}

//去掉词条前后的空格
void trimString(std::string & str )
{
    int s = str.find_first_not_of(" ");
    int e = str.find_last_not_of(" ");
    str = str.substr(s,e-s+1);
    return;
}

//把词语、词性等映射数字
int FeatureToInt(String^ infile, String^ outfile)
{

	StreamReader^ rd = gcnew StreamReader(infile);
	StreamWriter^ wt = gcnew StreamWriter(outfile);
	String^ line;

	map<string,int> mStrInt;
	array<String^>^ split;
	int iwordid = 0;
	string temp;
	while (rd->Peek() >= 0)
	{
		line = rd->ReadLine();
		line->Trim();
		if (line == "")
		{
			continue ;
		}
		split = line->Split(' ');
		for (int i = 0; i < split->Length; i++)
		{
			//MarshalString(split[i],temp);
			 
			temp = (const char*)(Marshal::StringToHGlobalUni(split[i])).ToPointer();
			if( mStrInt.find(temp) == mStrInt.end() )
			{
				mStrInt.insert(pair<string,int>(temp,iwordid));
				iwordid++;
			}else{
				continue;
			}
		}
	}

	map<string,int>::iterator miter;
	char a[10];
	string tempint;
	string word;
	for (miter = mStrInt.begin(); miter != mStrInt.end();miter++)
	{
		//_itoa(miter->second,a,10);
		
		tempint = a;
		word = miter->first;
		String^ Stemp = gcnew String(word.c_str());
		wt->WriteLine(Stemp);
	}

	return 0;
}



