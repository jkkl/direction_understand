#ifndef _TOOLS_H
#define _TOOLS_H

//int FeatureToInt(String^ infile, String^ outfile);
void getWordDict(const char* infile,const char* outfile);
/* 
**	�����ı���ֵ��
**	infile  �����ļ�Ϊʵ��ʶ�����ļ�ÿ��һ����
**	indict	�ʵ�
**  out ����ļ�
*/
void tagWordToInt(const char* fileDir,const char* dictDir, const char* outDir);

/* 
**	���������ʽ:���ո��滻Ϊ���� ��/�滻Ϊ\t
**	infile  �ִʱ�ע����ļ���һ��һ��
**	outfile	һ��һ��
*/
int  OutFileFormat(const char* infile,const char* outfile);

#endif