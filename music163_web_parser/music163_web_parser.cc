
#include "music163_web_parser.h"
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>

namespace lrc {
	using std::string;

	string Music163WebParser::Extract_LRC_FromJson(const string &json_str) {
		json_doc_.Parse(json_str.c_str());
		const auto &lrc = json_doc_["lrc"];
		const auto &lrc_str = lrc["lyric"];
		return lrc_str.GetString();

	}

}
