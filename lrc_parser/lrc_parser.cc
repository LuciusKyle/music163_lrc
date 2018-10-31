
#if defined _MSC_VER && 1600 < _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include"lrc_parser.h"

#include<regex>
#include<assert.h>

#include"lrc_time.h"

using std::string;
using std::regex;
namespace LRC {

	const regex lyric_line_pattern("\\[.*?\\].*?(?=\\r|\\n|\\[)");
	const regex lyric_time_tag_pattern("\\[[[:digit:]]{2}:[[:digit:]]{2}\\.[[:digit:]]{2}\\]");

	LRC_Parser::LRC_Parser(const string &content) {

	}

	bool LRC_Parser::ParseLyrics(const string &lyrics_str) {
		auto pos = lyrics_str.cbegin();
		auto end = lyrics_str.cend();
		std::smatch m;
		LRC_time lrc_time;
		for (; std::regex_search(pos, end, m, lyric_line_pattern); pos = m.suffix().first) {
			const string &lrc_line = m.str();
			const size_t pos = lrc_line.find(']') + 1;
			const string preceding_str(lrc_line.cbegin(), lrc_line.cbegin() + pos);
			int tag = getIDTags(preceding_str);
			if (tag < 0) return false;
			if (tag == ID_Tags::time_info) {
				lrc_time.setTime(preceding_str);
				if (!lrc_content_.insert({ lrc_time.getMilliSeconds(), { lrc_line.cbegin() + pos, lrc_line.cend() } }).second) {
					paralleled_lrc_content_.insert({ lrc_time.getMilliSeconds(), { lrc_line.cbegin() + pos, lrc_line.cend() } }); // It's not necessary to deal with three paralleled line.
				}
				continue;
			}			
			tags_[tag] = preceding_str;
		}
		return true;
	}

	void LRC_Parser::resetParser() {
		lrc_content_.clear();
		paralleled_lrc_content_.clear();
		for (auto &str : tags_) str.clear();
	}

	string LRC_Parser::OutputLyric(const double paralleled_line_adjustment, const bool merge_duplicate_lyrics) const {
		auto lrc_content_copy = lrc_content_;
		for (auto iter = paralleled_lrc_content_.cbegin(); iter != paralleled_lrc_content_.cend(); ++iter) {
			auto iter2 = lrc_content_copy.find(iter->first);
			assert(iter2 != lrc_content_copy.end());
			const size_t cur_line_time = iter2->first;
			++iter2;
			const size_t next_line_time = iter2 == lrc_content_copy.end() ? cur_line_time + 1000 : iter2->first;
			auto insertion_result = lrc_content_copy.insert({ cur_line_time + static_cast<size_t>((next_line_time - cur_line_time)*paralleled_line_adjustment), iter->second });
			assert(insertion_result.second);
		}
		string rtn;
		for (size_t i = 0; i < tags_.size(); ++i)
			if (i != ID_Tags::by)
				rtn.append(tags_[i] + "\n");
			else
				rtn.append("[by:Lucius Kyle]");

		LRC_time lrc_time;
		for (auto iter = lrc_content_copy.cbegin(); iter != lrc_content_copy.cend(); ++iter) {
			lrc_time.setTime(iter->first);
			rtn.append(lrc_time.getStringTime() + iter->second + '\n');
		}
		return rtn;
	}

	LRC_Parser::ID_Tags LRC_Parser::getIDTags(const std::string &tags_str) {
		if (tags_str.npos != tags_str.find("[ar:"))
			return ID_Tags::ar;
		if (tags_str.npos != tags_str.find("[al:"))
			return ID_Tags::al;
		if (tags_str.npos != tags_str.find("[ti:"))
			return ID_Tags::ti;
		if (tags_str.npos != tags_str.find("[au:"))
			return ID_Tags::au;
		if (tags_str.npos != tags_str.find("[length:"))
			return ID_Tags::length;
		if (tags_str.npos != tags_str.find("[by:"))
			return ID_Tags::by;
		if (tags_str.npos != tags_str.find("[offset:"))
			return ID_Tags::offset;
		if (tags_str.npos != tags_str.find("[re:"))
			return ID_Tags::re;
		if (tags_str.npos != tags_str.find("[ve:"))
			return ID_Tags::ve;
		if (std::regex_match(tags_str, lyric_time_tag_pattern))
			return ID_Tags::time_info;
		return error_tag;
	}


}

