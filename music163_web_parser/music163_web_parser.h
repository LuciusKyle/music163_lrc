
#ifndef _LRC_MUSIC163_WEB_PARSER_H_
#define _LRC_MUSIC163_WEB_PARSER_H_

#include<string>
#include<rapidjson/document.h>

namespace lrc {

#if defined _MSC_VER
#ifdef MUSIC163WEBPARSER_EXPORTS
#define MUSIC163WEBPARSER_API __declspec(dllexport)
#else
#define MUSIC163WEBPARSER_API __declspec(dllimport)
#endif // MUSIC163WEBPARSER_EXPORTS
#elif defined __GNUC__
#define MUSIC163WEBPARSER_API
#endif

	class MUSIC163WEBPARSER_API Music163WebParser {
	public:
		std::string Extract_LRC_FromJson(const std::string &);
	private:
		rapidjson::Document json_doc_;
	};

}

#endif // !_LRC_MUSIC163_WEB_PARSER_H_

