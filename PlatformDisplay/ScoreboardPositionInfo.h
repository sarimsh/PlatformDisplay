#pragma once

#include "bakkesmod/wrappers/wrapperstructs.h"

#define SCOREBOARD_LEFT 537
#define BLUE_BOTTOM 77
#define ORANGE_TOP 32
#define BANNER_DISTANCE 57
#define IMAGE_WIDTH 150
#define IMAGE_HEIGHT 100
#define CENTER_X 960
#define CENTER_Y 540
#define SCOREBOARD_HEIGHT 548
#define SCOREBOARD_WIDTH 1033
#define IMBALANCE_SHIFT 32
#define MUTATOR_SIZE 478
#define SKIP_TICK_SHIFT 67

// Scoreboard Position Info: 
struct SbPosInfo {
	Vector2F blueLeaderPos;  // position of the profile box for the top blue player
	Vector2F orangeLeaderPos;
	float playerSeparation;  // distance between profile pics on each team
	float profileScale;      // amount to scale each platform icon / profile pic
};

SbPosInfo getSbPosInfo(Vector2F canvasSize, float uiScale, bool mutators, int numBlues, int numOranges) {
	//-----Black Magic, thanks BenTheDan------------
	float scale;
	if (canvasSize.X / canvasSize.Y > 1.5f) {
		scale = 0.507f * canvasSize.Y / SCOREBOARD_HEIGHT;
	}
	else {
		scale = 0.615f * canvasSize.X / SCOREBOARD_WIDTH;
	}

	Vector2F center = Vector2F{ canvasSize.X / 2, canvasSize.Y / 2 };
	float mutators_center = canvasSize.X - 1005.0f * scale * uiScale;
	if (mutators && mutators_center < center.X) {
		center.X = mutators_center;
	}
	int team_difference = numBlues - numOranges;
	center.Y += IMBALANCE_SHIFT * (team_difference - ((numBlues == 0) != (numOranges == 0)) * (team_difference >= 0 ? 1 : -1)) * scale * uiScale;

	SbPosInfo output;
	output.profileScale = 0.48f;
	float tier_X = -SCOREBOARD_LEFT - (IMAGE_WIDTH - 100.0f) * output.profileScale + 30.5f;
	tier_X = center.X + tier_X * scale * uiScale;
	float tier_Y_blue = -BLUE_BOTTOM + (6 * (4 - numBlues)) - BANNER_DISTANCE * numBlues + 9.0f;
	tier_Y_blue = center.Y + tier_Y_blue * scale * uiScale;
	float tier_Y_orange = ORANGE_TOP;
	tier_Y_orange = center.Y + tier_Y_orange * scale * uiScale;
	output.blueLeaderPos = { tier_X, tier_Y_blue };
	output.orangeLeaderPos = { tier_X, tier_Y_orange };
	output.playerSeparation = BANNER_DISTANCE * scale * uiScale;
	output.profileScale *= scale * uiScale;

	// based on 100x100 img
	//------End Black Magic---------
	return output;
}
