
#ifndef _LRC_LRC_PARSER_H_
#define _LRC_LRC_PARSER_H_

#include<string>
#include<list>
#include<unordered_map>

namespace LRC {
#if defined _MSC_VER
#ifdef LRCPARSER_EXPORTS
#define LRCPARSER_API __declspec(dllexport)
#else
#define LRCPARSER_API __declspec(dllimport)
#endif // LRCPARSER_EXPORTS
#elif defined __GNUC__
	#define LRCPARSER_API
#endif

	class LRCPARSER_API LRC_Parser {
	public:
		LRC_Parser() {}
		LRC_Parser(const std::string &);
		
		bool ParseLyrics(const std::string &);
	private:
		std::string AdjustTime(const std::string &, const int miliseconds) const;
		
	private:
		enum ID_Tags {
			ar,			// Lyrics artist
			al,			// Album where the song is from
			ti,			// Lyrics(song) title
			au,			// Creator of the Songtext
			length,		// How long the song is
			by,			// Creator of the LRC file
			offset,		// +/-Overall timestamp adjustment in milliseconds, +shifts time up, -shifts down
			re,			// The player or editor that created the LRC file
			ve,			// version of program
			time_info	// [mm:ss.xx] lrc time
		};
	public:
		std::list<std::string> lrc_head_;
		std::list<std::string> lrc_time_line_;
		std::unordered_map<std::string, std::string> lrc_content_;

	};
}

#endif // !_LRC_LRC_PARSER_H_

//LRC format:
//[00:12.00]Line 1 lyrics
//[00:17.20]Line 2 lyrics
//[00:21.10]Line 3 lyrics
//...
//[mm:ss.xx]last lyrics line

//tags
//[ar:Lyrics artist]
//[al:Album where the song is from]
//[ti:Lyrics(song) title]
//[au:Creator of the Songtext]
//[length:How long the song is]
//[by:Creator of the LRC file]
//[offset:+/ -Overall timestamp adjustment in milliseconds, +shifts time up, -shifts down]
//[re:The player or editor that created the LRC file]
//[ve:version of program]

//Example with ID tags
//[ar:Chubby Checker oppure  Beatles, The]
//[al:Hits Of The 60's - Vol. 2 �C Oldies]
//[ti:Let's Twist Again]
//[au:Written by Kal Mann / Dave Appell, 1961]
//[length:2 : 23]
//
//[00:12.00]Naku Penda Piya - Naku Taka Piya - Mpenziwe
//[00:15.30]Some more lyrics ...
//...