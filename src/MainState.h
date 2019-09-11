#include "Board.h"
#include "Cinnabar/EventBroker.h"
#include "Cinnabar/NanoVGModule.h"
#include "Cinnabar/State.h"
#include "Events.h"
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
	void onEvent(const PuckBucketEvent&);
	void onEvent(const VideoEndEvent&);

	void mousePressed(const SDL_MouseButtonEvent&);
	void mouseReleased(const SDL_MouseButtonEvent&);
	void mouseMoved(const SDL_MouseMotionEvent&);

private:
	Board* _board;

	bool _placing = false;
	glm::vec2 _placingPosition;

	std::deque<CoinType> _coinQueue;

	bool _showingResult = false;

	UI _ui;
};
