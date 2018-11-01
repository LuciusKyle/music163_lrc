
#if defined _MSC_VER && 1600 < _MSC_VER

#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING

#endif

#include<regex>
#include<fstream>
#include<iostream>
#include<boost/asio.hpp>
#include<boost/beast.hpp>
#include"../lrc_parser/lrc_parser.h"
#include"../music163_web_parser/music163_web_parser.h"

#ifdef _MSC_VER

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/lrc_parser.lib")
#pragma comment(lib, "../x64/Debug/music163_web_parser.lib")
#else // _DEBUG
#pragma comment(lib, "../x64/Release/lrc_parser.lib")
#pragma comment(lib, "../x64/Release/music163_web_parser.lib")
#endif // _DEBUG
#else // _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../Debug/lrc_parser.lib")
#pragma comment(lib, "../Debug/music163_web_parser.lib")
#else
#pragma comment(lib, "../Release/lrc_parser.lib")
#pragma comment(lib, "../Release/music163_web_parser.lib")
#endif // _DEBUG
#endif // _WIN64

#endif // _MSC_VER

using std::string;
using std::regex;
using boost::asio::ip::tcp;

int main(int argc, char const *argv[]) {
	const int version = 11;
	const string host = "music.163.com";
	const string port = "80";
	const string target = "/api/song/lyric?id=186145&lv=1&kv=1&tv=-1";

	boost::asio::io_context ioc;
	tcp::resolver resolver{ ioc };
	tcp::socket socket{ ioc };

	auto const results = resolver.resolve(host, port);
	boost::asio::connect(socket, results.begin(), results.end());

	boost::beast::http::request<boost::beast::http::string_body> req{ boost::beast::http::verb::get, target, version };
	req.set(boost::beast::http::field::host, host);
	req.set(boost::beast::http::field::user_agent, "Mozilla/5.0 (X11; Linux x86_64; rv:65.0) Gecko/20100101 Firefox/65.0");
	boost::beast::http::write(socket, req);

	boost::beast::flat_buffer buffer;

	boost::beast::http::response<boost::beast::http::string_body> res;

	boost::beast::http::read(socket, buffer, res);

	lrc::Music163WebParser web_parser;
	std::ofstream file("out.file");
	file << web_parser.Extract_LRC_FromJson(res.body());


	boost::system::error_code ec;
	socket.shutdown(tcp::socket::shutdown_both, ec);

	return 0;
}

