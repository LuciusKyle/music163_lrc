
#if defined _MSC_VER && 1600 < _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include"lrc_parser.h"

#include<regex>
#include<assert.h>

using std::string;
using std::regex;
namespace LRC {

	const regex lyric_line_pattern("\\[[[:digit:]]{2}:[[:digit:]]{2}\\.[[:digit:]]{2}\\]\.*?(?=\\r|\\n|\\[)");
	const regex head_pattern("\\[\.*?\\:\.*?\\](?=\\r|\\n|\\[)");

	LRC_Parser::LRC_Parser(const string &content) {

	}

	bool LRC_Parser::ParseLyrics(const string &lyrics_str) {
		auto pos = lyrics_str.cbegin();
		auto end = lyrics_str.cend();
		std::smatch m;
		for (; std::regex_search(pos, end, m, head_pattern); pos = m.suffix().first)
			lrc_head_.push_back(m.str());
		pos = lyrics_str.cbegin();
		for (; std::regex_search(pos, end, m, lyric_line_pattern); pos = m.suffix().first) {
			const string one_line = m.str();
			lrc_time_line_.push_back({ one_line.cbegin(), one_line.cbegin() + 10 });
			lrc_content_.insert({ {one_line.cbegin(), one_line.cbegin() + 10}, {one_line.cbegin() + 10, one_line.cend()} });
		}
		return true;
	}

	string LRC_Parser::AdjustTime(const string &time_info, const int miliseconds) const {
		const int seconds = miliseconds / 1000;

		int cur_miliseconds = atoi(time_info.c_str() + 7) + miliseconds / 10;
		int carry = cur_miliseconds / 100;
		cur_miliseconds %= 100;
		cur_miliseconds *= (cur_miliseconds < 0 ? -1 : 1);
		int cur_seconds = atoi(time_info.c_str() + 4) + carry;
		carry = cur_seconds / 60;
		cur_seconds %= 60;
		cur_seconds *= (cur_seconds < 0 ? -1 : 1);

		int cur_minute = atoi(time_info.c_str() + 1) + carry;
		assert(std::abs(cur_minute) < 60);
		if (cur_minute < 0)
			cur_minute = 100 - cur_minute;

		string rtn = "[00:00.00]";
		char buffer[4] = { '\0' };
		memcpy(rtn.data() + (10 <= cur_minute ? 1 : 2), _itoa(cur_minute, buffer, 10), (10 <= cur_minute ? 2 : 1));
		memcpy(rtn.data() + (10 <= cur_seconds ? 4 : 5), _itoa(cur_seconds, buffer, 10), (10 <= cur_seconds ? 2 : 1));
		memcpy(rtn.data() + (10 <= cur_miliseconds ? 7 : 8), _itoa(cur_miliseconds, buffer, 10), (10 <= cur_miliseconds ? 2 : 1));

		return rtn;
	}

}

