#include <stdafx.h>
#include <gui.h>
#include <ops.h>
#include <procon_api.h>

using namespace s3d;
using namespace SimpleGUI;

namespace gui {

	constexpr int32 FontYOffset = -1;
	constexpr double RoundSize = 4.8;
	constexpr double unitSize = 36;
	constexpr ColorF FrameColor{ 0.33 };

	constexpr ColorF BackgroundColor{ 0.1 };
	constexpr ColorF DisabledBackgroundColor{ 0.25 };
	constexpr ColorF ActiveTextColor{ 0.9 };
	constexpr ColorF DisabledTextColor{ 0.67 };
	constexpr ColorF ButtonMouseOverColor{ 0, 0.05, 0.08 };
	
	constexpr ColorF SliderBaseColor{ 0.33 };
	constexpr ColorF SliderFillColor{ 0, 0.1, 0.2 };
	constexpr ColorF SliderDisabledBaseColor{ 0.75 };
	constexpr ColorF SliderDisabledFillColor{ 0.6 };
	constexpr double SliderBarRoundSize = 2.0;
	constexpr double SliderMinLength = 40.0;

	constexpr ColorF CheckBoxFillColor{ 0, 0.1, 0.2 };
	constexpr ColorF CheckBoxHighlightedFillColor{ 0, 0.05, 0.1 };
	constexpr ColorF CheckBoxBaseColor{ 0.67 };
	constexpr ColorF CheckBoxDisabledBaseColor{ SliderDisabledBaseColor };
	constexpr ColorF CheckBoxDisabledFillColor{ SliderDisabledFillColor };
	constexpr ColorF CheckBoxMouseOverColor{ 0.5 };

	constexpr int32 CheckBoxSize = 24;
	constexpr int32 CheckBoxPadding = 8;

	constexpr double MinTextBoxWidth = 40.0;
	constexpr int32 TextBoxHeight = 36;
	constexpr ColorF TextAreaEditingTextBackgroundColor{ 0.2 };

	inline constexpr ColorF GetTextColor(bool enabled) noexcept
	{
		return (enabled ? ActiveTextColor : DisabledTextColor);
	}

	static void DrawCheck(const Vec2& center)
	{
		Quad{ 2.5, 13.5, 6, 11, 9.5, 14.5, 9.5, 20.5 }.movedBy(-12, -12).movedBy(center).draw();
		Quad{ 9.5, 20.5, 9.5, 14.5, 17.75, 3.75, 20.5, 6.5 }.movedBy(-12, -12).movedBy(center).draw();
	}

	bool Button(const StringView label, const Vec2& pos, const Optional<double>& _width, const Optional<double>& _height, const bool enabled)
	{
		const Vec2 center = ButtonRegion(label, pos, _width).center();
		return gui::ButtonAt(label, center, _width, _height, enabled);
	}

	bool ButtonAt(const StringView label, const Vec2& center, const Optional<double>& _width, const Optional<double>& _height, const bool enabled)
	{
		const Font& font = GetFont();
		const auto dtext = font(label);

		const double labelWidth = Math::Ceil(dtext.region().w);
		const double width = _width.value_or_eval([&]() { return (labelWidth + 40); });
		const double height = _height.value_or_eval([&]() { return (unitSize); });

		const RectF rect{ Arg::center = center, width, height };
		const Vec2 labelPos{ (rect.x + (width - labelWidth) / 2.0), (center.y - font.height() / 2.0 + FontYOffset) };

		const bool mouseOver = (enabled && rect.mouseOver());
		const bool pushed = (mouseOver && Cursor::OnClientRect() && MouseL.down());
		const s3d::RoundRect rrect = rect.rounded(RoundSize);

		if (enabled)
		{
			rrect.draw(mouseOver ? ButtonMouseOverColor : BackgroundColor);
		}
		else
		{
			rrect.draw(DisabledBackgroundColor);
		}

		if (not pushed)
		{
			rrect.drawFrame(1, 0, FrameColor);
		}

		dtext.draw(labelPos, GetTextColor(enabled));

		if (mouseOver)
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		return pushed;
	}

	bool CheckBox(bool& checked, const StringView label, const Vec2& pos, const Optional<double>& _width, const Optional<double>& _height, const bool enabled)
	{
		const Vec2 center = CheckBoxRegion(label, pos, _width).center();
		return CheckBoxAt(checked, label, center, _width, _height, enabled);
	}

	bool CheckBoxAt(bool& checked, const StringView label, const Vec2& center, const Optional<double>& _width, const Optional<double>& _height, const bool enabled)
	{
		const Font& font = GetFont();
		const DrawableText dtext = font(label);

		const double width = _width.value_or_eval([&]() { return Math::Ceil(CheckBoxPadding * 3 + CheckBoxSize + dtext.region().w); });
		const RectF region{ Arg::center = center, width, _height.value_or_eval([&]() { return unitSize; }) };
		const RectF checkBox{ Arg::leftCenter(region.x + 8, center.y), CheckBoxSize };
		const s3d::RoundRect roundCheckBox = checkBox.rounded(3.2);
		const Vec2 labelPos{ (region.x + CheckBoxPadding * 2 + CheckBoxSize), (center.y - font.height() / 2.0 + FontYOffset) };
		const bool mouseOver = (enabled && checkBox.mouseOver());

		const s3d::RoundRect rregion = region.rounded(RoundSize);
		rregion.draw(enabled ? BackgroundColor : DisabledBackgroundColor);

		if (checked)
		{
			if (enabled)
			{
				roundCheckBox.draw(mouseOver ? CheckBoxHighlightedFillColor : CheckBoxFillColor);
			}
			else
			{
				roundCheckBox.draw(CheckBoxDisabledFillColor);
			}

			DrawCheck(checkBox.center());
		}
		else
		{
			const s3d::RoundRect innerRoundRect = checkBox.stretched(-1.25).rounded(3.2);

			if (enabled)
			{
				roundCheckBox.draw(CheckBoxBaseColor);
				innerRoundRect.draw(mouseOver ? CheckBoxMouseOverColor : ColorF{ 0.95 });
			}
			else
			{
				roundCheckBox.draw(CheckBoxDisabledBaseColor);
				innerRoundRect.draw(ColorF{ 0.8 });
			}
		}

		dtext.draw(labelPos, GetTextColor(enabled));

		if (enabled && Cursor::OnClientRect() && checkBox.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		const bool previousValue = checked;

		if (enabled && Cursor::OnClientRect() && checkBox.leftClicked())
		{
			checked = !checked;
		}

		return (checked != previousValue);
	}

	bool Slider(const StringView label, double& value, const double min, const double max, const Vec2& pos, const double labelWidth, const double sliderWidth, const Optional<double>& height, const bool enabled)
	{
		const Vec2 center = SliderRegion(pos, labelWidth, sliderWidth).center();
		return SliderAt(label, value, min, max, center, labelWidth, sliderWidth, height, enabled);
	}

	bool SliderAt(const StringView label, double& value, double min, double max, const Vec2& center, const double _labelWidth, const double _sliderWidth, const Optional<double>& height, const bool enabled)
	{
		const Font& font = GetFont();

		const double labelWidth = Max(_labelWidth, 0.0);
		const double sliderWidth = Max(_sliderWidth, SliderMinLength);
		const double width = (labelWidth + sliderWidth);
		const RectF region{ Arg::center = center, width, height.value_or_eval([&]() { return unitSize; }) };
		const Vec2 labelPos{ (region.x + 8), (center.y - font.height() / 2.0 + FontYOffset) };

		const double sliderRegionX0 = (region.x + labelWidth + 8);
		const double sliderRegionX1 = (region.x + region.w - 8);
		const double sliderRegionW = (sliderRegionX1 - sliderRegionX0);

		const double actualSliderRegionX0 = (sliderRegionX0 + 8);
		const double actualSliderRegionX1 = (sliderRegionX1 - 8);
		const double actualSliderRegionW = (actualSliderRegionX1 - actualSliderRegionX0);

		const RectF sliderRect{ Arg::leftCenter(sliderRegionX0, center.y), sliderRegionW, 6 };
		const s3d::RoundRect baseRoundRect = sliderRect.rounded(SliderBarRoundSize);

		const double previousValue = value;
		const auto [min_, max_] = std::minmax(min, max);
		value = Clamp(value, min_, max_);

		const double fill = ((value - min) / (max - min));
		const RectF fillRect{ sliderRect.pos, sliderRect.w * fill, sliderRect.h };
		const s3d::RoundRect fillRoundRect = fillRect.rounded(SliderBarRoundSize);

		const RectF smallRect{ Arg::center(actualSliderRegionX0 + actualSliderRegionW * fill, center.y), 16, 24 };
		const s3d::RoundRect smallRoundRect = smallRect.rounded(4.2);
		const bool mouseOver = (enabled && smallRect.mouseOver());

		const s3d::RoundRect rregion = region.rounded(RoundSize);
		rregion.draw(enabled ? BackgroundColor : DisabledBackgroundColor);

		if (enabled)
		{
			baseRoundRect.draw(SliderBaseColor);
			fillRoundRect.draw(SliderFillColor);
			smallRoundRect
				.draw(mouseOver ? ButtonMouseOverColor : BackgroundColor)
				.drawFrame(1, ColorF{ 0.33 });
		}
		else
		{
			baseRoundRect.draw(SliderDisabledBaseColor);
			fillRoundRect.draw(SliderDisabledFillColor);
			smallRoundRect
				.draw(DisabledBackgroundColor)
				.drawFrame(1, ColorF{ 0.1 });
		}

		font(label).draw(labelPos, GetTextColor(enabled));

		const RectF sliderRectExtended = sliderRect.stretched(4, 12);

		if (enabled && Cursor::OnClientRect() && (sliderRectExtended.mouseOver() || smallRect.mouseOver()))
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (enabled && Cursor::OnClientRect() && sliderRectExtended.leftPressed())
		{
			const double pos = (Cursor::PosF().x - actualSliderRegionX0);
			const double posN = Math::Saturate(pos / actualSliderRegionW);
			value = (min + posN * (max - min));
		}

		return (value != previousValue);
	}

	bool TextBox(TextEditState& text, const Vec2& pos, double width, const Optional<double>& height, const Optional<size_t>& maxChars, const bool enabled)
	{
		width = Max(width, MinTextBoxWidth);

		return TextBoxAt(text, pos + Vec2{ width * 0.5, 18.0 }, width, height, maxChars, enabled);
	}

	bool TextBoxAt(TextEditState& text, const Vec2& center, const double _width, const Optional<double>& _height, const Optional<size_t>& maxChars, const bool enabled)
	{
		text.cursorPos = Min(text.cursorPos, text.text.size());
		text.tabKey = false;
		text.enterKey = false;

		const Font& font = SimpleGUI::GetFont();
		const int32 fontHeight = font.height();

		const String previousText = text.text;
		const String editingText = ((text.active && enabled) ? TextInput::GetEditingText() : U"");

		// テキストを更新する
		{
			if (text.active && enabled)
			{
				// text.text を更新する
				text.cursorPos = TextInput::UpdateText(text.text, text.cursorPos, TextInputMode::AllowBackSpaceDelete);

# if not SIV3D_PLATFORM(WEB)

				// ショートカットキーによるペースト
				if ((not editingText) &&
# if SIV3D_PLATFORM(MACOS)
					((KeyCommand + KeyV).down() || (KeyControl + KeyV).down())
# else
					(KeyControl + KeyV).down()
# endif
					)
				{
					if (String paste; Clipboard::GetText(paste))
					{
						paste.remove_if([](char32 ch) { return (ch < 0x20) || (ch == U'\x7F'); });
						text.text.insert(text.cursorPos, paste);
						text.cursorPos += paste.size();
					}
				}

# endif
			}

			// 最大字数を超えていたら削る
			if (maxChars && (*maxChars < text.text.size()))
			{
				text.text.resize(*maxChars);
				text.cursorPos = Min(text.cursorPos, *maxChars);
			}

			// 文字列に変更があったかを調べる
			text.textChanged = (text.text != previousText);

			// 文字列に変更があれば
			if (text.textChanged)
			{
				// カーソル点滅をリセットする
				text.cursorStopwatch.restart();
			}
		}

		// テキストボックス
		const double width = Max(_width, MinTextBoxWidth);
		const RectF region{ Arg::center = center, Max(width, MinTextBoxWidth), _height.value_or_eval([&]() { return TextBoxHeight; }) };

		const s3d::RoundRect rregion = region.rounded(RoundSize);

		// マウスカーソルを IBeam にする
		if (enabled && Cursor::OnClientRect() && region.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::IBeam);
		}

		// 入力カーソルのアクティブ / 非アクティブを切り替える
		if (MouseL.down() && (TextInput::GetEditingText().isEmpty()))
		{
			if (enabled && Cursor::OnClientRect() && region.mouseOver())
			{
				text.active = true;
				text.resetStopwatches();

				// カーソルの位置を計算する
				{
					const double posX = (Cursor::PosF().x - (region.x + 8));
					size_t index = 0;
					double pos = 0.0;

					for (const auto& advance : font(text.text).getXAdvances())
					{
						if (posX <= (pos + (advance / 2)))
						{
							break;
						}

						pos += advance;
						++index;
					}

					text.cursorPos = index;
				}
			}
			else
			{
				text.active = false;
			}
		}

		// テキストカーソルを更新する
		if (text.text)
		{
			if (text.active && enabled && (not editingText))
			{
				// キーでテキストカーソルを移動させる
				// 一定時間押下すると、テキストカーソルが高速に移動

				// テキストカーソルを先頭へ移動させる
				if ((KeyControl + KeyHome).down()) // [ctrl] + [home]: 全体の先頭へ
				{
					text.cursorPos = 0;
					text.cursorStopwatch.restart();
				}
				else if (
# if SIV3D_PLATFORM(MACOS)
					((KeyControl + KeyA).down() || KeyHome.down())
# else
					KeyHome.down()
# endif
					) // [home]: 行頭へ
				{
					text.cursorPos = 0;
					text.cursorStopwatch.restart();
				}

				// テキストカーソルを末尾へ移動させる
				if ((KeyControl + KeyEnd).down()) // [ctrl] + [end]: 全体の末尾へ
				{
					text.cursorPos = text.text.size();
					text.cursorStopwatch.restart();
				}
				else if (
# if SIV3D_PLATFORM(MACOS)
					((KeyControl + KeyE).down() || KeyEnd.down())
# else
					KeyEnd.down()
# endif
					) // [end]: 行末へ
				{
					text.cursorPos = text.text.size();
					text.cursorStopwatch.restart();
				}

				// [←] キー
				if ((0 < text.cursorPos)
					&& (KeyLeft.down() || ((SecondsF{ 0.33 } < KeyLeft.pressedDuration()) && (SecondsF{ 0.06 } < text.leftPressStopwatch))))
				{
					--text.cursorPos;
					text.leftPressStopwatch.restart();
				}

				// [→] キー
				if ((text.cursorPos < text.text.size())
					&& (KeyRight.down() || ((SecondsF{ 0.33 } < KeyRight.pressedDuration()) && (SecondsF{ 0.06 } < text.rightPressStopwatch))))
				{
					++text.cursorPos;
					text.rightPressStopwatch.restart();
				}
			}
		}

		if (text.active && enabled && (not editingText))
		{
			// [tab][enter] キーで入力カーソルを非アクティブに
			{
				const String raw = TextInput::GetRawInput();
				text.tabKey = raw.contains(U'\t');
				text.enterKey = raw.contains(U'\r');

				if (text.tabKey || text.enterKey)
				{
					text.active = false;
				}
			}
		}

		// 描画
		{
			const Vec2 textPos{ (region.x + 8), (center.y - font.height() / 2 + FontYOffset - 0.5) };

			if (enabled)
			{
				if (text.active)
				{
					rregion
						.draw(ColorF{ 0.15, 0.15, 0.18 })
						.drawFrame(2.5, 0.0, ColorF{ 0.6 });
				}
				else
				{
					rregion
						.draw(BackgroundColor)
						.drawFrame(2.0, 0.0, ColorF{ 0.4 });
				}
			}
			else
			{
				rregion
					.draw(DisabledBackgroundColor)
					.drawFrame(2.0, 0.0, ColorF{ 0.33 });
			}

			{
				const ColorF textColor = GetTextColor(enabled);
				const auto& pixelShader = Font::GetPixelShader(font.method());

				double cursorPosX = textPos.x;
				Vec2 editingTextPos = textPos;

				// テキストの描画
				{
					const ScopedCustomShader2D shader{ pixelShader };
					Vec2 penPos = textPos;
					const Array<Glyph> glyphs = font.getGlyphs(text.text);

					for (auto&& [index, glyph] : Indexed(glyphs))
					{
						const double xAdvance = glyph.xAdvance;
						const Vec2 glyphPos = (penPos + glyph.getOffset());

						glyph.texture.draw(glyphPos, textColor);
						penPos.x += xAdvance;

						// テキストカーソルの位置の計算を計算する
						if (text.active && (text.cursorPos == (index + 1)))
						{
							cursorPosX = penPos.x;
							editingTextPos = penPos;
						}
					}
				}

				if (editingText)
				{
					// 変換テキストとその領域の取得
					const Array<Glyph> editingGlyphs = font.getGlyphs(editingText);
					Array<Vec2> editingGlyphPositions(editingGlyphs.size());
					{
						Vec2 penPos = editingTextPos;

						for (size_t i = 0; i < editingGlyphs.size(); ++i)
						{
							const auto& glyph = editingGlyphs[i];
							editingGlyphPositions[i] = (penPos + glyph.getOffset());
							penPos.x += glyph.xAdvance;
						}
					}

					// 変換テキスト背景の描画
					if (editingGlyphs)
					{
						const auto& firstGlyph = editingGlyphs.front();
						const auto& lastGlyph = editingGlyphs.back();
						const Vec2 pos = (editingGlyphPositions.front() - firstGlyph.getOffset());
						const double w = ((editingGlyphPositions.back().x - lastGlyph.getOffset().x + lastGlyph.xAdvance) - pos.x);
						RectF{ pos, w, fontHeight }.draw(TextAreaEditingTextBackgroundColor);

						// 変換テキストの選択範囲の描画
						{
# if SIV3D_PLATFORM(WINDOWS)

							const std::pair<int32, int32> editingTarget = Platform::Windows::TextInput::GetCursorIndex();

# elif SIV3D_PLATFORM(WEB)

							const std::pair<int32, int32> editingTarget = Platform::Web::TextInput::GetCandicateCursorIndex();

# else

							const std::pair<int32, int32> editingTarget{ -1, 0 };

# endif

							if (editingTarget.second && ((editingTarget.first + editingTarget.second) <= editingGlyphPositions.size()))
							{
								const int32 firstIndex = editingTarget.first;
								const int32 lastIndex = (editingTarget.first + editingTarget.second - 1);
								const double x0 = editingGlyphPositions[firstIndex].x;
								const double x1 = editingGlyphPositions[lastIndex].x + editingGlyphs[lastIndex].xAdvance;
								RectF{ x0, (pos.y + fontHeight - 2), (x1 - x0), 2 }.draw(ActiveTextColor);
							}
						}
					}

					// 変換テキストの描画
					{
						const ScopedCustomShader2D shader{ pixelShader };

						for (size_t i = 0; i < editingGlyphs.size(); ++i)
						{
							const auto& glyph = editingGlyphs[i];
							glyph.texture.draw(editingGlyphPositions[i], textColor);
						}
					}
				}

				// テキスト入力カーソルの描画
				if (text.active && enabled)
				{
					const bool showCursor = (text.cursorStopwatch.ms() % 1200 < 600)
						|| (text.leftPressStopwatch.isRunning() && (text.leftPressStopwatch < SecondsF{ 0.5 }))
						|| (text.rightPressStopwatch.isRunning() && (text.rightPressStopwatch < SecondsF{ 0.5 }));

					if (showCursor)
					{
						const RectF cursor(Arg::leftCenter(Vec2{ cursorPosX, region.center().y }.asPoint()), 1, 26);
						cursor.draw(Palette::Black);
					}
				}
			}
		}

# if SIV3D_PLATFORM(WEB)

		Platform::Web::TextInput::SetFocusToTextInput(text.active);

		if (text.active && not editingText)
		{
			if (text.lastCursorPos != text.cursorPos)
			{
				Platform::Web::TextInput::SyncronizeText(text.text);
				Platform::Web::TextInput::SetCursorIndex(text.cursorPos);
			}
			else if (auto currentCursorPos = Platform::Web::TextInput::GetCursorIndex(); text.lastCursorPos != currentCursorPos)
			{
				text.cursorPos = currentCursorPos;
				text.cursorStopwatch.restart();
			}

			text.lastCursorPos = text.cursorPos;
		}

# endif

		return text.textChanged;
	}
};

GUI::GUI(GameState& _game_state) {
	game_state = _game_state;
	desiredWindowHeight = 800;
	margin = desiredWindowHeight / 10;
	cellSize = (desiredWindowHeight - margin * 5 / 4) / game_state.board.height;
	windowHeight = static_cast<int>(game_state.board.height * cellSize + margin);
	windowWidth = static_cast<int>(game_state.board.width * cellSize + windowHeight * 2 / 3);
	buttonSize = (windowWidth - cellSize * game_state.board.width - (margin * 3) / 4) / 5;
	unitSize = windowHeight / 20;
	label = { FontMethod::MSDF, margin / 4 };

	boardArea = { margin / 4, margin, cellSize * game_state.board.width, cellSize * game_state.board.height };
	controlArea = { boardArea.tr().x + margin / 4, margin / 4, windowWidth - boardArea.w - (margin * 3) / 4, windowHeight - margin / 4 };
	dieArea = { controlArea.x, controlArea.y + margin / 1.5, controlArea.w, buttonSize };
	directionArea = { dieArea.bl().movedBy(0, margin), buttonSize * 2.5, buttonSize * 2.5 };
	sizeArea = { directionArea.tr().movedBy(margin / 4, 0), controlArea.w - directionArea.w - margin / 4, unitSize };
	positionArea = { sizeArea.bl().movedBy(0, margin), sizeArea.w, unitSize * 2 + margin / 8 };
	actionArea = { controlArea.x + controlArea.w - buttonSize * 2,directionArea.y + directionArea.h + margin, buttonSize * 2, buttonSize };
	bottomArea = { controlArea.br().movedBy(buttonSize * -2, unitSize * -2 - margin / 4), { buttonSize * 2, unitSize * 2 + margin / 4 } };

	Window::Resize(windowWidth, windowHeight + margin / 4);

	_dieSize = 1;
	selectedDieType = -1;
	_selectedGeneralDie.text = U"25";
	selectedDirection = -1;
	_posX.text = U"0";
	_posY.text = U"0";
	showGoal = false;
	UpdateValue();

	stopwatch.start();
}

void GUI::UpdateValue() {
	posX = ParseOr<int>(_posX.text, 0);
	posY = ParseOr<int>(_posY.text, 0);
	dieSize = static_cast<int>(_dieSize);
	if (selectedDieType != -1) {
		dieIndex = (selectedDieType != 0) ? (dieSize - 1) * 3 - (3 - selectedDieType) : selectedGeneralDie;
		if (dieIndex == -2) dieIndex = 0;
	}
	else {
		dieIndex = -1;
	}
	selectedGeneralDie = ParseOr<int>(_selectedGeneralDie.text, 25);
	if (selectedGeneralDie < 25) selectedGeneralDie = 25;
}

void GUI::Reset() {
	_dieSize = 1;
	selectedDieType = -1;
	_selectedGeneralDie.text = U"25";
	selectedDirection = -1;
	_posX.text = U"0";
	_posY.text = U"0";
	showGoal = false;
	UpdateValue();
}

void GUI::Render() {
	RenderBoard();
	RenderStopwatch();
	RenderDieSelection();
	RenderDirectionSelection();
	RenderReset();
	RenderDieSizeSelection();
	RenderDiePositionSelection();
	RenderAction();
	RenderBottom();
}

void GUI::RenderText(gui::GUIElement element) {
	const ColorF textColor = ColorF{ 0.9 };
	switch (element) {
	case gui::Board:
		gui::CheckBox(showGoal, U"Show Goal",
			          Vec2{ boardArea.tr().x - 160, boardArea.y - margin / 1.5 },
			          160, unitSize, !solved);
		break;
	case gui::Die:
		label(U"Select Die").drawAt(margin / 3, dieArea.topCenter().movedBy(0, margin / -2), textColor);
		break;
	case gui::Direction:
		label(U"Select Direction").drawAt(margin / 3, directionArea.topCenter().movedBy(0, margin / -2), textColor);
		break;
	case gui::DieSize:
		if (selectedDieType != 0)
			label(U"Die Size").drawAt(margin / 3, sizeArea.topCenter().movedBy(0, margin / -2), textColor);
		else
			label(U"Die Index").drawAt(margin / 3, sizeArea.topCenter().movedBy(0, margin / -2), textColor);
		break;
	case gui::DiePosition:
		label(U"Die Position").drawAt(margin / 3, positionArea.topCenter().movedBy(0, margin / -2), textColor);
		label(U"X:").drawAt(margin / 4, positionArea.tl().movedBy(margin / 2, unitSize / 2), textColor);
		label(U"Y:").drawAt(margin / 4, positionArea.bl().movedBy(margin / 2, unitSize / -2), textColor);
		break;
	default:
		break;
	}
}

void GUI::RenderBoard() {
	int startX = static_cast<int>(boardArea.x + (boardArea.w - game_state.board.width * cellSize) / 2);
	int startY = static_cast<int>(boardArea.y + (boardArea.h - game_state.board.height * cellSize) / 2);

	RenderText(gui::Board);

	if (!showGoal) {
		for (int y = 0; y < game_state.board.height; ++y) {
			for (int x = 0; x < game_state.board.width; ++x) {
				const RectF rect(startX + x * cellSize, startY + y * cellSize, cellSize, cellSize);
				const int32 value = game_state.board.pieces[y][x];

				bool cellIsHovered = rect.mouseOver();
				if (selectedDieType != -1 && cellIsHovered && MouseL.down()) {
					_posX.text = U"{}"_fmt(x);
					_posY.text = U"{}"_fmt(y);
					UpdateValue();
				}
				if (dieIndex == -1 ||
					x - posX < 0 || x - posX >= game_state.dies[dieIndex].width || 
					y - posY < 0 || y - posY >= game_state.dies[dieIndex].height || 
					!game_state.dies[dieIndex].cells[y - posY][x - posX]) {

					double brightness = (cellIsHovered && selectedDieType != -1) ? 0.9 : 0.6;

					rect.stretched(-1).draw(HSV{ 240 - value * 80, 0.3, brightness });
				}
				else {
					double alpha = EaseInOutSine(Periodic::Sine0_1(2s));
					rect.stretched(-1).draw(HSV{ 240 - value * 80, 0.3, 0.6, alpha });
				}
			}
		}
	}
	else {
		for (int y = 0; y < game_state.goal_state.height; ++y) {
			for (int x = 0; x < game_state.goal_state.width; ++x) {
				const RectF rect(startX + x * cellSize, startY + y * cellSize, cellSize, cellSize);
				const int32 value = game_state.goal_state.pieces[y][x];
				rect.stretched(-1).draw(HSV{ 240 - value * 80, 0.3, 0.6 });
			}
		}
	}
}


void GUI::RenderDieSelection() {
	RenderText(gui::Die);
	const String dieLabels[] = { U"General", U"III", U"II", U"I" };
	bool disabled = showGoal || solved;
	for (int i = 0; i < 4; ++i) {
		if (gui::Button(dieLabels[i],
			{ dieArea.x + buttonSize * (3 - i), dieArea.y + buttonSize / 4 },
			(i == 0) ? buttonSize * 2 : buttonSize, buttonSize,
			selectedDieType != ((i == 0) ? 0 : 4 - i) && !disabled)) {
			selectedDieType = (i == 0) ? i : 4 - i;
		}
	}
}

void GUI::RenderDirectionSelection() {
	bool disabled = showGoal || solved;
	RenderText(gui::Direction);
	if (gui::Button(U"\U000F0737", // Up
		directionArea.topCenter().movedBy(buttonSize / -4, 0),
		buttonSize / 2, buttonSize / 2, selectedDirection != 0 && !disabled)) {
		selectedDirection = 0;
	}
	if (gui::Button(U"\U000F072E", // Down
		directionArea.bottomCenter().movedBy(buttonSize / -4, buttonSize / -2),
		buttonSize / 2, buttonSize / 2, selectedDirection != 1 && !disabled)) {
		selectedDirection = 1;
	}
	if (gui::Button(U"\U000F0731", // Left
		directionArea.leftCenter().movedBy(0, buttonSize / -4), buttonSize / 2,
		buttonSize / 2, selectedDirection != 2 && !disabled)) {
		selectedDirection = 2;
	}
	if (gui::Button(U"\U000F0734", // Right
		directionArea.rightCenter().movedBy(buttonSize / -2, buttonSize / -4),
		buttonSize / 2, buttonSize / 2, selectedDirection != 3 && !disabled)) {
		selectedDirection = 3;
	}
}

void GUI::RenderReset() {
	if (gui::Button(U"\U000F099B", directionArea.center().movedBy(buttonSize / -2, buttonSize / -4), buttonSize, buttonSize, !showGoal && !solved)) {
		Reset();
	}
}

void GUI::RenderDieSizeSelection() {
	RenderText(gui::DieSize);
	if (selectedDieType == 0) {
		label(U"Die Index").drawAt(margin / 3, sizeArea.topCenter().movedBy(0, margin / -2), ColorF{ 0.9 });
		if (gui::TextBox(_selectedGeneralDie, sizeArea.topCenter().movedBy(margin / -2, 0), margin, unitSize, 3, selectedDieType != -1 && !showGoal && !solved) && selectedGeneralDie > game_state.dies.size() - 1) {
			_selectedGeneralDie.text = U"{}"_fmt(game_state.dies.size() - 1);
		}
	}
	else {
		label(U"Die Size").drawAt(margin / 3, sizeArea.topCenter().movedBy(0, margin / -2), ColorF{ 0.9 });
		if (gui::Slider(U"{}"_fmt(pow(2, _dieSize - 1)), _dieSize, (selectedDieType > 1 ? 2 : 1), 9, sizeArea.tl().movedBy(unitSize, 0), margin / 4, sizeArea.w - margin / 4 - unitSize * 2, unitSize, selectedDieType != -1 && !showGoal && !solved)) {
			_dieSize = int(_dieSize);
		}
		if (gui::Button(U"\U000F0374", sizeArea.tl(), unitSize, unitSize, selectedDieType != -1 && !showGoal && !solved)) {
			_dieSize--;
		}
		if (gui::Button(U"\U000F0415", sizeArea.tr().movedBy(unitSize * -1, 0), unitSize, unitSize, selectedDieType != -1 && !showGoal && !solved)) {
			_dieSize++;
		}
	}
	UpdateValue();
}

void GUI::RenderDiePositionSelection() {
	RenderText(gui::DiePosition);
	// X Position Controls
	if (gui::Button(U"\U000F0141", positionArea.tr().movedBy(margin / -1.5 + unitSize * -2, 0), unitSize, unitSize, selectedDieType != -1 && !showGoal && !solved)) {
		_posX.text = U"{}"_fmt(ParseOr<int>(_posX.text, posX) - 1);
	}
	if (gui::Button(U"\U000F0142", positionArea.tr().movedBy(unitSize * -1, 0), unitSize, unitSize, selectedDieType != -1 && !showGoal && !solved)) {
		_posX.text = U"{}"_fmt(ParseOr<int>(_posX.text, posX) + 1);
	}
	if (gui::TextBox(_posX, positionArea.tr().movedBy(unitSize * -1 + margin / -1.5, 0), margin / 1.5, unitSize, 3, selectedDieType != -1 && !showGoal && !solved)) {}

	// Y Position Controls
	if (gui::Button(U"\U000F0143", positionArea.br().movedBy(margin / -1.5 + unitSize * -2, -unitSize), unitSize, unitSize, selectedDieType != -1 && !showGoal && !solved)) {
		_posY.text = U"{}"_fmt(ParseOr<int>(_posY.text, posY) - 1);
	}
	if (gui::Button(U"\U000F0140", positionArea.br().movedBy(unitSize * -1, -unitSize), unitSize, unitSize, selectedDieType != -1 && !showGoal && !solved)) {
		_posY.text = U"{}"_fmt(ParseOr<int>(_posY.text, posY) + 1);
	}
	if (gui::TextBox(_posY, positionArea.br().movedBy(unitSize * -1 + margin / -1.5, -unitSize), margin / 1.5, unitSize, 3, selectedDieType != -1 && !showGoal && !solved)) {}

	UpdateValue();
}

void GUI::RenderBottom() {
	if (gui::Button(U"\U000F0552 Submit", bottomArea.tl(), buttonSize * 2, unitSize, game_state.num_moves > 0)) {
		try {
			if (!solved) {
				if (System::MessageBoxYesNo(U"Warning", U"Current state not same with the goal, continue?", MessageBoxStyle::Warning) == MessageBoxResult::Yes) {
					revision = PostRequest(serverUrl + "/answer", token, OutputJson(game_state));
				}
			}
			else revision = PostRequest(serverUrl + "/answer", token, OutputJson(game_state));
			System::MessageBoxOK(U"", U"Submit successful", MessageBoxStyle::Info);
		}
		catch (std::runtime_error) {
			System::MessageBoxOK(U"", U"Submit failed", MessageBoxStyle::Error);
		}
	}
	if (gui::Button(U"\U000F0343 Exit", bottomArea.tl().movedBy(0, unitSize + margin / 4), buttonSize * 2, unitSize)) {
		System::Exit();
	}
}

void GUI::RenderAction() {
	if (gui::Button(U"Apply", directionArea.bl().movedBy(0, margin), directionArea.w, buttonSize, selectedDieType != -1 && selectedDirection != -1 && !showGoal && !solved)) {
		applied_actions.push_back({ game_state.board, {dieIndex, posX, posY, selectedDirection} });
		apply_die(game_state, { dieIndex, posX, posY, selectedDirection });
		undone_actions.clear();

		if (game_state.board.pieces == game_state.goal_state.pieces) {
			solved = true;
			stopwatch.pause();
			Reset();
			System::MessageBoxOK(U"Solved", U"Congratulations!");
		}
	}
	if (gui::Button(U"\U000F054C", actionArea.tl(), buttonSize, buttonSize, !applied_actions.empty() && !showGoal && !solved)) {
		game_state.board = applied_actions.back().first;
		undone_actions.push_back({ applied_actions.back().first, applied_actions.back().second });
		applied_actions.pop_back();
		game_state.num_moves--;
		game_state.moves.pop_back();

	}
	if (gui::Button(U"\U000F044E", actionArea.tl().movedBy(buttonSize, 0), buttonSize, buttonSize, !undone_actions.empty() && !showGoal && !solved)) {
		game_state.board = undone_actions.back().first;
		game_state.moves.push_back(undone_actions.back().second);
		applied_actions.clear();
		undone_actions.pop_back();
	}
}

void GUI::RenderStopwatch() {
	int minutes = static_cast<int>(stopwatch.sF()) / 60;
	int seconds = static_cast<int>(stopwatch.sF()) % 60;
	label(U"Elapsed Time: {}:{:02}"_fmt(minutes, seconds)).drawAt(margin / 3, boardArea.topCenter().movedBy(0, margin / -3), ColorF{ 0.9 });

}
