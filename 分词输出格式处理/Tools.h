#ifndef _TOOLS_H
#define _TOOLS_H

//int FeatureToInt(String^ infile, String^ outfile);
void getWordDict(const char* infile,const char* outfile);
/* 
**	�����ı���ֵ��
**	infile  �����ļ�
**	indict	�ʵ�
**  out ����ļ�
*/
void tagWordToInt(const char* fileDir,const char* dictDir, const char* outDir);

#endif