#include<fstream>
#include<iostream>
#include<string>
#include<cliext\vector>
#using <system.dll>
#include "NERrule.h"
#include "Tools.h"
#define inputfile "E:/NLP/corpus/out.txt"
//ȥ��������
#define inputfile2 "E:/NLP/corpus/out2.txt"	//����������ķִ�ָ���¼
#define outputfile "E:/NLP/corpus/outFormat.txt"
#define NO_MATCH_FILE "E:/NLP/corpus/NoMatchFile.txt"
#define POS_TRAIN	"E:/NLP/corpus/pos_train.txt"
#define POS_TEST	"E:/NLP/corpus/pos_test.txt"
#define TRAIN_FILE "E:/NLP/corpus/train.txt"
#define TEST_FILE "E:/NLP/corpus/Test.txt"

#define INFILE_WORDTOINT "E:\\myproject\\learncorpus_pos.txt"
#define OUTFILE_WORDTOINT "E:\\myproject\\wordid.txt"
#define OUTFILE_WORDTOINT2 "E:\\myproject\\wordid2.txt"
//�������Ͽ�������ѵ���Ͳ��Եİٷֱ�
#define TRAIN_SCALE 70
#define TEST_SCALE 30

using namespace System;
using namespace System::IO;
using namespace std;
using namespace cliext;
using namespace System::Text::RegularExpressions;
//using namespace System::IO;

//�ʡ����Զ� ��
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
	cliext::vector<segpos^> vsegpos;//��lineSplit��Ŀ��ٷֳ�segpos
	array<String^> ^lineSplit = gcnew array<String^>(50);//�ִʱ�ע���һ�а��ո�ֳ�  ��/����  ��
	//char charsToTrim[] = {' '};
	sentance = sentance->TrimEnd();
	lineSplit = sentance->Split(' ');
	array<String^> ^oneseg = gcnew array<String^>(2);
	String^ flags = gcnew String("");//�洢�þ��ӵĴ��Դ���
	for (int i = 0; i < lineSplit->Length; i++)	//��һ�����ӷָ�� ��/���Զ� ��ϡ�
	{
		oneseg = lineSplit[i]->Split('/');
		if (oneseg->Length == 2)
		{
			segpos ^getSegPos = gcnew segpos();//һ�� ��/���� ��
			getSegPos->word = oneseg[0];		//�洢 ��
			getSegPos->flag = oneseg[1];		//�洢����
			flags += oneseg[1];
			vsegpos.push_back(getSegPos);	//����������ӵ� ��/�� �Զ�
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
	String^ line=gcnew String("");	//�洢inputfile�е�һ���ı�
	
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

//��ȡ�ļ���ָ��������������ַ�����127����Ϊ������Ч
int GetRecordRows()
{
	StreamReader^ sr = gcnew StreamReader(inputfile);
	StreamWriter^ wr_pos_train = gcnew StreamWriter(POS_TRAIN);
	StreamWriter^ wr_pos_test = gcnew StreamWriter(POS_TEST);
	String^ line=gcnew String("");	//�洢inputfile�е�һ���ı�
	vector<String^> vRecord;	//�洢ָ���ļ��е������м�¼д��vector<String^>^vRecord��pushback��ʱ�򱨴�

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
	//��vRecord�еļ�¼��������
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