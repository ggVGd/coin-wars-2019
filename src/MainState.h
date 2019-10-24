#include "Board.h"
#include "Cinnabar/EventBroker.h"
#include "Cinnabar/NanoVGModule.h"
#include "Cinnabar/State.h"
#include "Events.h"
#include "RandomVideo.h"
#include "Scene.h"
#include "SceneRenderer.h"
#include "UI.h"
#include <SDL2/SDL_events.h>
#include <deque>

class MainState
	: public Cinnabar::State,
	Cinnabar::EventBroker::Observer

{
public:
	MainState();
	void enter() override;
	void leave() override;
	void render() override;
	void update(float) override;

	void onEvent(const Cinnabar::EventBroker::Event*) override;
	void onEvent(const CoinInsertEvent&);
	void onEvent(const DepartmentSelectEvent&);
	void onEvent(const DepartmentDeselectEvent&);
	void onEvent(const PuckBucketEvent&);
	void onEvent(const VideoEndEvent&);

	void mousePressed(const SDL_MouseButtonEvent&);
	void mouseReleased(const SDL_MouseButtonEvent&);
	void mouseMoved(const SDL_MouseMotionEvent&);

private:
	std::shared_ptr<Board> _board;

	std::string _department;

	bool _placing = false;
	glm::vec2 _placingPosition;

	std::deque<CoinType> _coinQueue;
	int _coinsDropping = 0;

	bool _showingResult = false;

	UI _ui;
	RandomVideo _randomVideo;
};
