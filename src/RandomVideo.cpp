#include "RandomVideo.h"
#include <boost/filesystem.hpp>

RandomVideo::RandomVideo() : _gen(_rd())
{
	namespace fs = boost::filesystem;

	fs::path path("./resources/video/random/");
	for(fs::recursive_directory_iterator it(path), end; it != end; it++)
	{
		const auto rel = fs::relative(it->path(), fs::current_path());
		_videos.push_back(rel.string());
	}

	_dis = std::make_shared<std::uniform_int_distribution<>>(0, _videos.size() - 1);
}
const std::string& RandomVideo::get()
{
	return _videos.at((*_dis)(_gen));
}
