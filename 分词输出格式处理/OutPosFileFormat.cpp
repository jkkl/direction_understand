#include<fstream>
#include<iostream>
#include<string>
#include<cliext\vector>
#using <system.dll>
#include "NERrule.h"
#include "Tools.h"
#define inputfile "E:/NLP/corpus/out.txt"
//去掉多余行
#define inputfile2 "E:/NLP/corpus/out2.txt"	//保存随机化的分词指令记录
#define outputfile "E:/NLP/corpus/outFormat.txt"
#define NO_MATCH_FILE "E:/NLP/corpus/NoMatchFile.txt"
#define POS_TRAIN	"E:/NLP/corpus/pos_train.txt"
#define POS_TEST	"E:/NLP/corpus/pos_test.txt"
#define TRAIN_FILE "E:/NLP/corpus/train.txt"
#define TEST_FILE "E:/NLP/corpus/Test.txt"

#define INFILE_WORDTOINT "E:\\myproject\\learncorpus_pos.txt"
#define OUTFILE_WORDTOINT "E:\\myproject\\wordid.txt"
#define OUTFILE_WORDTOINT2 "E:\\myproject\\wordid2.txt"
//定义语料库中用于训练和测试的百分比
#define TRAIN_SCALE 70
#define TEST_SCALE 30

using namespace System;
using namespace System::IO;
using namespace std;
using namespace cliext;
using namespace System::Text::RegularExpressions;
//using namespace System::IO;

//词、词性对 类
ref class segpos 
{
public:
	String^ word;
	String^ flag;
	segpos():word(""),flag(""){};
	segpos(const segpos %a ) //Copy constructor function
	{
		word = a.word;
		flag = a.flag;
	}
};

int  OutFileFormat()
{
	//StreamReader sr = new StreamReader();
	ifstream infile;
	ofstream outfile;

	infile.open("E:/NLP/corpus/out.txt");
	if(!infile){
		std::cerr << "ERROR: infile is NULL";
		return 0;
	}
	outfile.open("E:/NLP/corpus/outFormat.txt");
	if(!outfile){
	std::cerr << "ERROR: outfile is NULL";
		return 0;
	}

	std::string line;
	std::string newline = "";

	while ( !infile.eof() ){
		std::getline(infile,line);
		for (int i = 0; i < strlen(line.c_str()); i++)
		{
			if (line[i] == ' ')
			{
				line[i] = '\n';
			}
			else if (line[i] == '/')
			{
				line[i] = '\t';
			}
			else
			{
				newline+= line[i];
			}
		}
		outfile.write(line.c_str(),strlen(line.c_str()));
		char* _n = "\n";
		outfile.write(_n,strlen(_n));
	}

	infile.close();
	outfile.close();
	return 0;
}


String^ ProcessSentance(String^ sentance)
{
	cliext::vector<segpos^> vsegpos;//将lineSplit里的块再分成segpos
	array<String^> ^lineSplit = gcnew array<String^>(50);//分词标注后的一行按空格分成  词/词性  块
	//char charsToTrim[] = {' '};
	sentance = sentance->TrimEnd();
	lineSplit = sentance->Split(' ');
	array<String^> ^oneseg = gcnew array<String^>(2);
	String^ flags = gcnew String("");//存储该句子的词性串。
	for (int i = 0; i < lineSplit->Length; i++)	//将一个句子分割成 词/词性对 组合。
	{
		oneseg = lineSplit[i]->Split('/');
		if (oneseg->Length == 2)
		{
			segpos ^getSegPos = gcnew segpos();//一个 词/词性 对
			getSegPos->word = oneseg[0];		//存储 词
			getSegPos->flag = oneseg[1];		//存储词性
			flags += oneseg[1];
			vsegpos.push_back(getSegPos);	//组成整个句子的 词/词 性对
		}
	}

	String^ NERSentence = gcnew String("");
	if (ProcessRule(flags,lineSplit))
	{
		//NERSentence += "Rule1:\n";
	}
	else if (ProcessRule2(flags,lineSplit))
	{
		//NERSentence += "Rule2:\n";
	}
	else if (ProcessRule3(flags,lineSplit))
	{
		//NERSentence += "Rule3:\n";
	}
	else if (ProcessRule4(flags,lineSplit))
	{
		//NERSentence += "Rule4:\n";
	}
	else
	{
		NERSentence += "No Match:\n";
	}

	for (int i = 0; i < lineSplit->Length; i++)
	{
		NERSentence += (lineSplit[i]->Replace('/',' ')+"\n");
	}
	return NERSentence;
}


int  ReadWriteFile(String^ infile, String^ outfile )
{
	StreamReader^ sr = gcnew StreamReader(infile);
	StreamWriter^ wr = gcnew StreamWriter(outfile);
	StreamWriter^ wrNoMatch = gcnew StreamWriter(NO_MATCH_FILE);
	String^ line=gcnew String("");	//存储inputfile中的一行文本
	
	while (sr->Peek() >=0)
	{
		line = sr->ReadLine();
		if (line == "")
		{
			//wr->WriteLine("\n");
			continue;
		}
		String^ NERStence = ProcessSentance(line);
		if (NERStence->Contains("No Match"))
		{
			wrNoMatch->WriteLine(NERStence);
		}
		else
		{
			wr->WriteLine(NERStence);
		}
	}
	sr->Close();
	wr->Close();
	wrNoMatch->Close();
	return 0;
}

//获取文件中指令的行数，若首字符大于127则认为改行有效
int GetRecordRows()
{
	StreamReader^ sr = gcnew StreamReader(inputfile);
	StreamWriter^ wr_pos_train = gcnew StreamWriter(POS_TRAIN);
	StreamWriter^ wr_pos_test = gcnew StreamWriter(POS_TEST);
	String^ line=gcnew String("");	//存储inputfile中的一行文本
	vector<String^> vRecord;	//存储指令文件中的所有行记录写成vector<String^>^vRecord在pushback的时候报错

	int iRecordRows = 0;
	while (sr->Peek() >=0)
	{
		line = sr->ReadLine();
		//
		if (line != "" && line[0] > 127)
		{
			iRecordRows += 1;
			vRecord.push_back(line);
			continue;
		}
	}
	//将vRecord中的记录，随机输出
	Random rd;
	int rdNum;
	int scale_train = vRecord.size()*TRAIN_SCALE/100 ;
	while (vRecord.size() > 1)
	{
		rdNum = rd.Next(vRecord.size()-1);
		if (scale_train > 0)
		{
			scale_train--;
			wr_pos_train->WriteLine(vRecord.at(rdNum));
		}
		else
		{
			wr_pos_test->WriteLine(vRecord.at(rdNum));
		}
		vRecord.erase(vRecord.begin()+ rdNum);
	}
	wr_pos_test->WriteLine(vRecord.at(0));
	sr->Close();
	wr_pos_test->Close();
	wr_pos_train->Close();
	return iRecordRows;
}

void main()
{
	OutFileFormat("E:\\NLP\\mywork\\project\\githup\\direction_understand\\learncorpus_pos.txt",
					"E:\\NLP\\mywork\\project\\githup\\direction_understand\\learncorpus_posformat.txt");
	//ReadWriteFile();
	//cout << GetRecordRows();
	//ReadWriteFile("E:/NLP/corpus/pos_train.txt","E:/NLP/corpus/CRF/TestNER_train.txt");
	//ReadWriteFile("E:/NLP/corpus/pos_test.txt","E:/NLP/corpus/CRF/TestNER_test.txt");
	//getchar();

	
	 //getWordDict(INFILE_WORDTOINT,OUTFILE_WORDTOINT);
	 //tagWordToInt(INFILE_WORDTOINT,OUTFILE_WORDTOINT,OUTFILE_WORDTOINT2);


}