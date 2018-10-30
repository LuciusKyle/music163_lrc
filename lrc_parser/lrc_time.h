
#ifndef _LRC_LRC_TIME_H_
#define _LRC_LRC_TIME_H_

#include<string>

namespace LRC {
	class LRC_time {
	public:
		LRC_time(const std::string &);
		bool setTime(const std::string &);

		std::string getStringTime() const;

	private:
		size_t LRCTimeToMillisecond(const std::string &) const;

		size_t milliseconds_;
	};
}

#endif // !_LRC_LRC_TIME_H_
