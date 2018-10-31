
#ifndef _TIME_TAGGED_STIRNG_H_
#define _TIME_TAGGED_STIRNG_H_

class TimeTagedString {
public:
	TimeTagedString() :TimeTagedString(-1, 0, nullptr) {}
	TimeTagedString(int ID, size_t millisecond, const char *content, int content_length = -1);
	TimeTagedString(const TimeTagedString &);
	TimeTagedString(const TimeTagedString &&) = delete;
	TimeTagedString &operator=(const TimeTagedString &) = delete;
	~TimeTagedString();

	void set_Variables(int ID, size_t millisecond, const char *content, int content_length = -1);
	void addNext(TimeTagedString *next_ptr) { next_ = next_ptr; }
	int get_ID()const { return ID_tag_; }
	size_t getMillisecond() const { return millisecond_; }
	char *getContent(char * = nullptr) const;
	TimeTagedString *Next() const { return next_; }

	bool operator<(const TimeTagedString &) const;

private:
	int ID_tag_;
	size_t millisecond_;
	char *content_;
	size_t cur_capacity;
	TimeTagedString *next_;
};


#endif // !_TIME_TAGGED_STIRNG_H_

