
#include"lrc_time.h"
#include<regex>
#include<assert.h>

namespace LRC {

	using std::regex;
	using std::string;

	const regex kLRC_time_format("\\[[[:digit:]]{2}:[[:digit:]]{2}\\.[[:digit:]]{2}\\]");

	LRC_time::LRC_time(const string &lrc_time) :milliseconds_(0) {

	}

	bool LRC_time::setTime(const string &lrc_time) {
		if (!std::regex_match(lrc_time, kLRC_time_format)) return false;
		milliseconds_ = LRCTimeToMillisecond(lrc_time);
		return true;
	}

	string LRC_time::getStringTime() const {
		char rtn[12]{ '\0' };

		return rtn;
	}

	size_t LRC_time::LRCTimeToMillisecond(const std::string &lrc_time) const {
		return size_t(atoi(lrc_time.c_str() + 7)) +
			1000 * size_t(atoi(lrc_time.c_str() + 4)) +
			1000 * 60 * size_t(atoi(lrc_time.c_str() + 1));
	}

}

