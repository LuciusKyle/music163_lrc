
#if defined _MSC_VER && 1600 < _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include"lrc_parser.h"

#include<assert.h>
#include<regex>

#include"lrc_time.h"

namespace LRC {
	using std::string;
	using std::regex;

	const regex lyric_line_pattern("\\[.*?\\].*?(?=\\r|\\n|\\[)");
	const regex lyric_time_tag_pattern("\\[[[:digit:]]{2}:[[:digit:]]{2}\\.[[:digit:]]{2}\\]");

	LRC_Parser::LRC_Parser(const string &content) {
		if (!ParseLyrics(content)) {
			abort();
		}
	}

	bool LRC_Parser::ParseLyrics(const string &lyrics_str) {
		auto pos = lyrics_str.cbegin();
		const auto end = lyrics_str.cend();
		std::smatch m;
		LRC_time lrc_time;
		
		bool did_lrc_creator_show_up = false;
		for (; std::regex_search(pos, end, m, lyric_line_pattern); pos = m.suffix().first) {
			const string &lrc_line = m.str();
			const size_t pos = lrc_line.find(']') + 1;
			const string preceding_str(lrc_line.cbegin(), lrc_line.cbegin() + pos);
			int tag = getIDTags(preceding_str);
			if (tag < 0) return false;
			if (tag == ID_Tags::time_info) {
				lrc_time.setTime(preceding_str);
				const string lyric(lrc_line.cbegin() + pos, lrc_line.cend());
				if (!lrc_content_.emplace(tag, lrc_time.getMilliSeconds(), lyric.c_str(), static_cast<int>(lyric.size())).second) {
					paralleled_lrc_content_.emplace(tag, lrc_time.getMilliSeconds(), lyric.c_str(), static_cast<int>(lyric.size())); // It's not necessary to deal with three paralleled line.
				}
				continue;
			}
			lrc_content_.emplace(tag, 0, preceding_str.c_str(), static_cast<int>(preceding_str.size()));
			if (tag == ID_Tags::by) did_lrc_creator_show_up = true;
		}
		if (!did_lrc_creator_show_up)
			lrc_content_.emplace(ID_Tags::by, 0, "[by:Lucius Kyle(LuciusKyle@outlook.com)");
		return true;
	}

	void LRC_Parser::resetParser() {
		lrc_content_.clear();
		paralleled_lrc_content_.clear();
	}

	string LRC_Parser::OutputLyric(const double paralleled_line_adjustment, const bool merge_duplicate_lyrics) const {
		TimeTagedString head;

		TimeTagedString *ptr = &head;
		for (auto iter = lrc_content_.cbegin(); iter != lrc_content_.cend(); ++iter) {
			ptr->addNext(new TimeTagedString(iter->get_ID(), iter->getMillisecond(), iter->getContent()));
			ptr = ptr->Next();
		}

		ptr = &head;
		for (auto iter = paralleled_lrc_content_.cbegin(); iter != paralleled_lrc_content_.cend(); ++iter) {
			while (ptr != nullptr) {
				if (ptr->getMillisecond() == iter->getMillisecond()) {
					const size_t cur_millisecond = ptr->getMillisecond();
					TimeTagedString *next_ptr = ptr->Next();
					ptr->addNext(new TimeTagedString(iter->get_ID(), cur_millisecond + (ptr->Next() == nullptr ? cur_millisecond + 1000 : ptr->Next()->getMillisecond() - cur_millisecond) * paralleled_line_adjustment, iter->getContent()));
					ptr->Next()->addNext(next_ptr);
					break;
				}
				ptr = ptr->Next();
			}
		}
		string rtn;
		ptr = head.Next();
		LRC_time lrc_time;
		while (ptr != nullptr) {
			if (ptr->get_ID() < ID_Tags::time_info)
				if (ptr->get_ID() == ID_Tags::by)
					rtn.append("[by:Lucius Kyle(LuciusKyle@outlook.com)]\n"); // hahaha, Happy!
				else {
					rtn.append(ptr->getContent());
					rtn.append("\n");
				}
			else
				rtn.append(lrc_time.getStringTime(ptr->getMillisecond()) + ptr->getContent() + "\n");

			auto cur_ptr = ptr;
			ptr = ptr->Next();
			delete cur_ptr;
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

