
#if defined _MSC_VER && 1600 < _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<regex>
#include<fstream>
#include<iostream>
#include"../lrc_parser/lrc_parser.h"

#ifdef _MSC_VER

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/lrc_parser.lib")
#else // _DEBUG
#pragma comment(lib, "../x64/Release/lrc_parser.lib")
#endif // _DEBUG
#else // _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../Debug/lrc_parser.lib")
#else
#pragma comment(lib, "../Release/lrc_parser.lib")
#endif // _DEBUG
#endif // _WIN64

#endif // _MSC_VER

using std::string;

int main(int argc, char const *argv[]) {

	size_t test = 10;
	int test2 = -2;
	size_t test3 = test + test2;

	if (argc < 2) return 1;
	FILE *fp = fopen(argv[1], "rb");
	fseek(fp, 0L, SEEK_END);
	const size_t file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char *file_content = new char[file_size];
	fread(file_content, 1, file_size, fp);
	fclose(fp);

	std::string str(file_content, file_size);
	LRC::LRC_Parser parser(str);

	str.clear();
	for (auto iter = parser.lrc_head_.cbegin(); iter != parser.lrc_head_.cend(); ++iter) {
		str.append(*iter + "\n\n");
	}
	//for (auto iter = parser.lrc_time_line_.cbegin(); iter != parser.lrc_time_line_.cend(); ++iter) {
	//	str.append(*iter + "ja-jp: " + parser.lrc_content_.at(*iter) + "\n\n" + parser.AdjustTime(*iter, 400) + "zh-cn: \n\n");
	//}

	std::ofstream file(argv[2]);
	file << str;

	return 0;
}
