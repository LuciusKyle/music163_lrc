
#ifndef _LRC_LRC_TIME_H_
#define _LRC_LRC_TIME_H_

#include<string>

namespace LRC {
	class LRC_time {
	public:
		LRC_time() :milliseconds_(0) {}
		LRC_time(const std::string &);
		LRC_time(const size_t milliseconds) { milliseconds_ = milliseconds; }
		bool setTime(const std::string &);
		void setTime(const size_t milliseconds) { milliseconds_ = milliseconds; }

		std::string getStringTime() const;
		std::string getStringTime(size_t) const;
		size_t getMilliSeconds() const { return milliseconds_; }

	private:
		size_t LRCTimeToMillisecond(const std::string &) const;

		size_t milliseconds_;
	};
}

#endif // !_LRC_LRC_TIME_H_
