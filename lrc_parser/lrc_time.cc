
#if defined _MSC_VER && 1600 < _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include"lrc_time.h"
#include<regex>
#include<assert.h>

namespace lrc {

	using std::regex;
	using std::string;

	const regex kLRC_time_format("\\[[[:digit:]]{2}:[[:digit:]]{2}\\.[[:digit:]]{2}\\]");

	LRC_time::LRC_time(const string &lrc_time) :LRC_time() {
		if (!setTime(lrc_time)) {
			assert(false);
			abort();
		}
	}

	bool LRC_time::setTime(const string &lrc_time) {
		if (!std::regex_match(lrc_time, kLRC_time_format)) return false;
		milliseconds_ = LRCTimeToMillisecond(lrc_time);
		return true;
	}

	string LRC_time::getStringTime() const {
		char rtn[12] = { '\0' };
		sprintf(rtn, "[%02d:%02d.%02d]", static_cast<int>(milliseconds_) / 1000 / 60, (static_cast<int>(milliseconds_) / 1000) % 60, (static_cast<int>(milliseconds_) % 1000) / 10);
		return rtn;
	}

	std::string LRC_time::getStringTime(size_t milliseconds) const {
		char rtn[12] = { '\0' };
		sprintf(rtn, "[%02d:%02d.%02d]", static_cast<int>(milliseconds) / 1000 / 60, (static_cast<int>(milliseconds) / 1000) % 60, (static_cast<int>(milliseconds) % 1000) / 10);
		return rtn;
	}

	size_t LRC_time::LRCTimeToMillisecond(const string &lrc_time) const {
		return size_t(atoi(lrc_time.c_str() + 7)) +
			1000 * size_t(atoi(lrc_time.c_str() + 4)) +
			1000 * 60 * size_t(atoi(lrc_time.c_str() + 1));
	}

}

