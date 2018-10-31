
#if defined _MSC_VER && 1600 < _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include"time_tagged_string.h"

#include<string.h>
#include<assert.h>

TimeTagedString::TimeTagedString(int ID, size_t millisecond, const char *content, int content_length)
	:ID_tag_(ID),
	millisecond_(millisecond),
	content_(0 < content_length ? new char[content_length] : nullptr),
	cur_capacity(0 < content_length ? content_length : 0),
	next_(nullptr)
{
	if (content != nullptr) {
		const size_t len = strlen(content);
		assert(len <= content_length);

		if (cur_capacity < len + 1) {
			content_ = new char[len + 1];
			cur_capacity = len + 1;
		}
		strcpy(content_, content);
	}
}

TimeTagedString::TimeTagedString(const TimeTagedString &r) :TimeTagedString(r.get_ID(), r.getMillisecond(), r.getContent()) {
	this->next_ = r.Next();
}

TimeTagedString::~TimeTagedString() {
	if (content_ != nullptr)
		delete content_;
}

void TimeTagedString::set_Variables(int ID, size_t millisecond, const char *content, int content_length) {
	ID_tag_ = ID;
	millisecond_ = millisecond;
	if (content != nullptr) {
		size_t len(0 < content_length ? content_length : strlen(content));
		if (cur_capacity < len + 1) {
			delete content_;
			content_ = new char[len + 1];
		}
		strcpy(content_, content);
	}
}

char *TimeTagedString::getContent(char *output) const {
	if (output == nullptr || content_ == nullptr)
		return content_;
	else {
		strcpy(output, content_);
		return output;
	}
}
bool TimeTagedString::operator<(const TimeTagedString &r) const {
	if (this->ID_tag_ < r.get_ID())
		return true;
	else if (r.get_ID() < this->ID_tag_)
		return false;
	else
		if (this->millisecond_ < r.getMillisecond())
			return true;
		else
			return false;
}
