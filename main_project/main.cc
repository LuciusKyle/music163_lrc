
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
using std::regex;

#define SOMEWHAT
class SOMEWHAT SomeClass {
public:
	int a;
	double b;
};

int main(int argc, char const *argv[]) {

	size_t test = 10;
	int test2 = -2;
	size_t test3 = test + test2;

	char test4[12];
	_itoa(test2, test4, 10);

	const size_t milliseconds_ = 3600;
	//[00:00.00]
	const int minutes = milliseconds_ / 1000 / 60;
	const int seconds = (milliseconds_ / 1000) % 60;
	const int lrc_milliseconds = (milliseconds_ % 1000) / 10;
	sprintf(test4, "[%02d:%02d.%02d]", minutes, seconds, lrc_milliseconds);

	const regex lyric_line_pattern("\\[.*?\\].*?(?=\\r|\\n|\\[)");
	const regex lyric_time_tag_pattern("\\[[[:digit:]]{2}:[[:digit:]]{2}\\.[[:digit:]]{2}\\]");
	const regex head_pattern("\\[\.*?\\:\.*?\\](?=\\r|\\n|\\[)");
	
	std::smatch match;
	string test5 = "[00:12.00]\n";
	if (std::regex_search(test5, match, lyric_line_pattern)) {
		const string &test6 = match.str();
		const size_t pos = test6.find(']') + 1;
		string test7(test6.cbegin(), test6.cbegin() + pos);
		string test8(test6.cbegin() + pos, test6.cend());
		int a = 0;
	}

	//if (argc < 2) return 1;
	//FILE *fp = fopen(argv[1], "rb");


	FILE *fp = fopen("D:/KuGou/Lyric/abc.lrc", "rb");
	fseek(fp, 0L, SEEK_END);
	const size_t file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char *file_content = new char[file_size];
	fread(file_content, 1, file_size, fp);
	fclose(fp);

	std::string str(file_content, file_size);
	//LRC::LRC_Parser parser(str);

	//str.clear();
	//for (auto iter = parser.lrc_head_.cbegin(); iter != parser.lrc_head_.cend(); ++iter) {
	//	str.append(*iter + "\n\n");
	//}
	//for (auto iter = parser.lrc_time_line_.cbegin(); iter != parser.lrc_time_line_.cend(); ++iter) {
	//	str.append(*iter + "ja-jp: " + parser.lrc_content_.at(*iter) + "\n\n" + parser.AdjustTime(*iter, 400) + "zh-cn: \n\n");
	//}
	LRC::LRC_Parser parser(str);
	str = parser.OutputLyric();
	std::ofstream file("out.file");
	file << str;

	return 0;
}
