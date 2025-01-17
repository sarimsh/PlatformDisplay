#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
#include <set>
#include <map>
#include <string>
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class PlatformDisplay :	public BakkesMod::Plugin::BakkesModPlugin,
	public BakkesMod::Plugin::PluginSettingsWindow,
	public BakkesMod::Plugin::PluginWindow {
public:
	struct Pri {
		UniqueIDWrapper uid;
		int score{};
		unsigned char team{};
		bool isBot{};
		std::string name;
		OnlinePlatform platform;

		Pri() {}
		Pri(PriWrapper p) {
			if (!p) { return; }
			uid = p.GetUniqueIdWrapper();
			score = p.GetMatchScore();
			team = p.GetTeamNum2();
			isBot = p.GetbBot();
			name = p.GetPlayerName().ToString();
			platform = p.GetPlatform();
		}
	};
private:
	/**
	 * Stores data derived from each scoreboard sort cycle (happens once every second).
	 */
	struct ComputedScoreboardInfo {
		std::vector<Pri>sortedPlayers;
		int bluePlayerCount{};
		int orangePlayerCount{};
	};

	virtual void onLoad();
	/**
	 * Pre-compute scoreboard info after the sorting algorithm finishes. The hook
	 * "TAGame.PRI_TA.GetScoreboardStats" runs at least once immediately after Rocket League sorts
	 * the scoreboard. This happens every second, so computing the info like this saves us some
	 * performance.
	 */
	void ComputeScoreboardInfo();
	void RecordScoreboardComparison(ActorWrapper gameEvent, void* params, std::string eventName);
	void RenderPlatformLogos(CanvasWrapper canvas);
	void RenderDebugInfo(CanvasWrapper canvas);
	void RenderDebugPri(CanvasWrapper canvas, const PlatformDisplay::Pri& pri, Vector2F iconPos);
	void SetTeamColors(bool keepOrder = false);
	std::set<std::string> disconnectedPris;
	void RenderSettings() override;
	std::string GetPluginName() override;
	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;

	// Members for scoreboard tracking logic.
	std::vector<std::pair<Pri, Pri>> comparisons;
	/**
	 * teamHistory records the last team that a player (represented by the 
	 * string combining their name and uid) was seen on. This is necessary, 
	 * because during ScoreboardSort any disconnected players will have a team 
	 * number different from Blue or Orange, but we still need to know which team
	 * they show up on in the scoreboard display.
	 */
	std::unordered_map<std::string, int> teamHistory;
	ComputedScoreboardInfo computedInfo{};  // Derived from comparisons and teamHistory.
	bool accumulateComparisons{};

	// Members for scoreboard rendering.
	bool scoreBoardOpen{};
	LinearColor teamColors[2]{ {255, 255, 255, 255}, {255, 255, 255, 255} };
	const static int LOGO_COUNT = 6;
	std::shared_ptr<ImageWrapper> logos[LOGO_COUNT];
	std::shared_ptr<ImageWrapper> noTintLogos[LOGO_COUNT];

	// Members for menus.
	bool windowOpen{};
	std::string menuTitle{ "PlatformDisplay" };
};

