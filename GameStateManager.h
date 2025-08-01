#pragma once
class GameStateManager
{
	public:
		static GameStateManager* getInstance();
		static void destroy();

		enum GameState {Pause = 0, Play = 1, FrameStep = 2};
		GameState getGameState() { return gameState; }
		void setGameState(GameState state) { gameState = state; }
	
	private:
		GameStateManager();
		~GameStateManager();
		GameStateManager(const GameStateManager&) {}
		GameStateManager& operator=(const GameStateManager&) {}

		static GameStateManager* sharedInstance;
		GameState gameState = GameState::Pause;
};

