#pragma once
#include "Cinnabar/EventBroker.h"
#include "Cinnabar/Module.h"
#include "Events.h"
#include <string>
#include <vector>

class VideoControlModule
	: public Cinnabar::Module<VideoControlModule>,
	Cinnabar::EventBroker::Observer
{
public:
	void init() override;
	void shutdown() override;
	void update(float) override;

	void onEvent(const Cinnabar::EventBroker::Event*) override;
	void onEvent(const VideoPlayRequestEvent&);
	void onEvent(const VideoStopRequestEvent&);

	void open(const std::string& filename, bool loop = false);
	void play();
	void pause();
	void pauseToggle();
	void stop();

private:
	std::string _send(const std::string& data);
	std::string _sendCommand(const std::vector<std::string>& command);

	float _pollTimer = 0.0f;
	bool _playerOpen = false;
};
