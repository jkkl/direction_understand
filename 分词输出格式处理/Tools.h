#ifndef _TOOLS_H
#define _TOOLS_H

//int FeatureToInt(String^ infile, String^ outfile);
void getWordDict(const char* infile,const char* outfile);
/* 
**	语料文本数值化
**	infile  输入文件
**	indict	词典
**  out 输出文件
*/
void tagWordToInt(const char* fileDir,const char* dictDir, const char* outDir);

#endif