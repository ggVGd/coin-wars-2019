#include "VideoControlModule.h"
#include "Cinnabar/Core.h"
#include <boost/algorithm/string.hpp>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

void VideoControlModule::init()
{
	core()->eventBroker().addObserver(this);
	subscribe<VideoPlayRequestEvent>();
	subscribe<VideoStopRequestEvent>();
}
void VideoControlModule::shutdown()
{
}
void VideoControlModule::update(float elapsed)
{
	if(!_playerOpen)
		return;

	_pollTimer += elapsed;
	if(_pollTimer > 1.0f)
	{
		_pollTimer = 0.0f;
		std::string status = _sendCommand({ "client_name" });
		if(status.empty())
		{
			_playerOpen = false;
			core()->eventBroker().emit(VideoEndEvent());
		}
	}
}
void VideoControlModule::onEvent(const Cinnabar::EventBroker::Event* event)
{
	if(event->is<VideoPlayRequestEvent>())
	{
		onEvent(event->as<VideoPlayRequestEvent>());
	}
	else if(event->is<VideoStopRequestEvent>())
	{
		onEvent(event->as<VideoStopRequestEvent>());
	}
}
void VideoControlModule::onEvent(const VideoPlayRequestEvent& event)
{
	open(event.filename, event.loop);
}
void VideoControlModule::onEvent(const VideoStopRequestEvent& event)
{
}
void VideoControlModule::open(const std::string& filename, bool loop)
{
	pid_t pid = fork();
	if(pid < 0)
		throw std::runtime_error("Failed to fork()");

	if(pid == 0)
	{
		using namespace std::string_literals;

		std::vector<std::string> args = {
			"/usr/bin/mpv",
			filename.c_str(),
			"--ao=null",
			"--fullscreen",
			"--input-ipc-server=/tmp/mpv.socket",
			"--keep-open=no",
			"--loop="s + (loop ? "inf"s : "no"s),
			"--no-input-default-bindings",
			"--no-osc",
			"--no-osd-bar",
		};

		int fd = ::open("/dev/null", O_WRONLY);
		dup2(fd, 1);
		dup2(fd, 2);
		close(fd);

		const char** argv = new const char*[args.size() + 1];
		for(std::size_t i = 0; i < args.size(); i++)
		{
			argv[i] = args[i].c_str();
		}
		argv[args.size()] = nullptr;

		execv("/usr/bin/mpv", (char**)argv);
		exit(0);

		delete[] argv;
	}
	else
	{
		_playerOpen = true;
		_pollTimer = -1.0f;
	}
}
void VideoControlModule::play()
{
	_sendCommand({ "set", "pause", "no" });
}
void VideoControlModule::pause()
{
	_sendCommand({ "set", "pause", "yes" });
}
void VideoControlModule::pauseToggle()
{
	_sendCommand({ "cycle", "pause" });
}
void VideoControlModule::stop()
{
	_sendCommand({ "stop" });
}
std::string VideoControlModule::_send(const std::string& data)
{
	std::string tmp = data;
	boost::replace_all(tmp, "'", "\\'");

	std::string result;
	char buffer[64];
	memset(buffer, 0, sizeof(buffer));
	auto pipe = popen(("echo '" + tmp + "' | socat - /tmp/mpv.socket 2> /dev/null").c_str(), "r");
	while(fgets(buffer, sizeof(buffer), pipe) != nullptr)
	{
		result += buffer;
	}
	pclose(pipe);

	return result;
}
std::string VideoControlModule::_sendCommand(const std::vector<std::string>& command)
{
	std::string data = "{\"command\":[";
	for(const auto& part : command)
		data += "\"" + part + "\",";
	data.pop_back();
	data += "]}";
	return _send(data);
}
