#pragma once
#include "UIScreen.h"
#include <memory>

class HUD : public UIScreen
{
public:
	HUD();

	void Draw() override;
	void Update(float deltaTime) override;

private:
	class FPSObject* m_playerObj;
	std::unique_ptr<Bitmap> m_crosshair;
	std::unique_ptr<Bitmap> m_hitMarker;
	std::unique_ptr<Bitmap> m_damageAlert;
	std::vector<std::unique_ptr<Bitmap>> m_ploxAlerts;
	std::vector<std::pair<Bitmap*,float>> m_pitchMeters;
	std::unique_ptr<class Slider> m_step1;
	std::unique_ptr<class Slider> m_step2;
	std::unique_ptr<Text> m_arm;
	std::unique_ptr<Text> m_armVal;
	std::unique_ptr<Text> m_dmg;
	std::unique_ptr<Text> m_dmgVal;
	std::unique_ptr<Text> m_speed;
	std::unique_ptr<Text> m_speedVal;
	std::unique_ptr<Text> m_alt;
	std::unique_ptr<Text> m_altVal;
	std::unique_ptr<Text> m_jump;
	std::unique_ptr<Text> m_jumpVal;
	std::unique_ptr<Text> m_time;
	std::unique_ptr<Text> m_timeVal;
	std::unique_ptr<Text> m_wave;
	std::unique_ptr<Text> m_waveVal;
	std::unique_ptr<Text> m_guide_jump;
	std::unique_ptr<Text> m_guide_dash;
	std::unique_ptr<Text> m_guide_shoot;
	std::unique_ptr<Text> m_guide_move;
	D2D1_RECT_F m_rect1;
	D2D1_RECT_F m_rect2;
};

//ヒットマーカーの表示時間
#define MARKER_DISPLAY (0.12f)

