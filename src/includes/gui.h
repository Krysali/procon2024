#ifndef GUI_H
#define GUI_H

#include <Siv3D.hpp>
#include <types.h>

using namespace SimpleGUI;

namespace gui {

	enum GUIElement {
		Board,
		Die,
		Direction,
		DieSize,
		DiePosition,
		Action
	};

	bool Button(StringView label, const Vec2& pos, const Optional<double>& width = unspecified, const Optional<double>& height = unspecified, bool enabled = true);

	bool ButtonAt(StringView label, const Vec2& center, const Optional<double>& width = unspecified, const Optional<double>& height = unspecified, bool enabled = true);

	bool CheckBox(bool& checked, StringView label, const Vec2& pos, const Optional<double>& width = unspecified, const Optional<double>& _height = unspecified, bool enabled = true);

	bool CheckBoxAt(bool& checked, StringView label, const Vec2& center, const Optional<double>& width = unspecified, const Optional<double>& _height = unspecified, bool enabled = true);

	bool Slider(StringView label, double& value, double min, double max, const Vec2& pos, double labelWidth = 80.0, double sliderWidth = 120.0, const Optional<double>& height = unspecified, bool enabled = true);

	bool SliderAt(StringView label, double& value, double min, double max, const Vec2& center, double labelWidth = 80.0, double sliderWidth = 120.0, const Optional<double>& height = unspecified, bool enabled = true);

	bool TextBox(TextEditState& text, const Vec2& pos, double width = 200.0, const Optional<double>& height = unspecified, const Optional<size_t>& maxChars = unspecified, bool enabled = true);

	bool TextBoxAt(TextEditState& text, const Vec2& center, double width = 200.0, const Optional<double>& height = unspecified, const Optional<size_t>& maxChars = unspecified, bool enabled = true);

}

class GUI {

public:
	GUI(GameState& _game_state);
	GameState game_state;
	std::string serverUrl, token, revision;
	void Render();

private:
	int desiredWindowHeight, margin;
	double cellSize, _dieSize, buttonSize, unitSize;
	int windowHeight, windowWidth;
	int dieSize, dieIndex, selectedDieType, selectedDirection;
	TextEditState _selectedGeneralDie, _posX, _posY;
	int selectedGeneralDie, posX, posY;
	bool showGoal, solved;
	std::vector<std::pair<Board, Action>> applied_actions;
	std::vector<std::pair<Board, Action>> undone_actions;
	Stopwatch stopwatch;
	Font label;

	RectF boardArea;
	RectF controlArea;
	RectF dieArea;
	RectF directionArea;
	RectF sizeArea;
	RectF positionArea;
	RectF actionArea;
	RectF bottomArea;

	void UpdateValue();
	void Reset();
	void RenderText(gui::GUIElement element);
	void RenderBoard();
	void RenderStopwatch();
	void RenderDieSelection();
	void RenderDirectionSelection();
	void RenderReset();
	void RenderDieSizeSelection();
	void RenderDiePositionSelection();
	void RenderBottom();
	void RenderAction();
};

#endif // GUI_H
