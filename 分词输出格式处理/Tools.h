#ifndef _TOOLS_H
#define _TOOLS_H

//int FeatureToInt(String^ infile, String^ outfile);
void getWordDict(const char* infile,const char* outfile);
/* 
**	语料文本数值化
**	infile  输入文件为实体识别后的文件每行一个词
**	indict	词典
**  out 输出文件
*/
void tagWordToInt(const char* fileDir,const char* dictDir, const char* outDir);

/* 
**	控制输出格式:将空格替换为换行 将/替换为\t
**	infile  分词标注后的文件：一句一行
**	outfile	一词一行
*/
int  OutFileFormat(const char* infile,const char* outfile);

#endif