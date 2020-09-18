#include "ui/ui.h"

#include <base/components/basic.h>

#include "dear-imgui/ImSequencer.h"
#include "dear-imgui/ImCurveEdit.h"
#include "dear-imgui/imgui_internal.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"

#include <algorithm>
#include <vector>

namespace
{

//
//
// ImSequencer interface
//
//
	static const char* SequencerItemTypeNames[] = { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };


	struct RampEdit : public ImCurveEdit::Delegate
	{
		RampEdit()
		{
			mPts[0][0] = ImVec2(-10.f, 0);
			mPts[0][1] = ImVec2(20.f, 0.6f);
			mPts[0][2] = ImVec2(25.f, 0.2f);
			mPts[0][3] = ImVec2(70.f, 0.4f);
			mPts[0][4] = ImVec2(120.f, 1.f);
			mPointCount[0] = 5;

			mPts[1][0] = ImVec2(-50.f, 0.2f);
			mPts[1][1] = ImVec2(33.f, 0.7f);
			mPts[1][2] = ImVec2(80.f, 0.2f);
			mPts[1][3] = ImVec2(82.f, 0.8f);
			mPointCount[1] = 4;


			mPts[2][0] = ImVec2(40.f, 0);
			mPts[2][1] = ImVec2(60.f, 0.1f);
			mPts[2][2] = ImVec2(90.f, 0.82f);
			mPts[2][3] = ImVec2(150.f, 0.24f);
			mPts[2][4] = ImVec2(200.f, 0.34f);
			mPts[2][5] = ImVec2(250.f, 0.12f);
			mPointCount[2] = 6;
			mbVisible[0] = mbVisible[1] = mbVisible[2] = true;
			mMax = ImVec2(1.f, 1.f);
			mMin = ImVec2(0.f, 0.f);
		}
		size_t GetCurveCount()
		{
			return 3;
		}

		bool IsVisible(size_t curveIndex)
		{
			return mbVisible[curveIndex];
		}
		size_t GetPointCount(size_t curveIndex)
		{
			return mPointCount[curveIndex];
		}

		uint32_t GetCurveColor(size_t curveIndex)
		{
			uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
			return cols[curveIndex];
		}
		ImVec2* GetPoints(size_t curveIndex)
		{
			return mPts[curveIndex];
		}
		virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const { return ImCurveEdit::CurveSmooth; }
		virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
		{
			mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
			SortValues(curveIndex);
			for (size_t i = 0; i < GetPointCount(curveIndex); i++)
			{
				if (mPts[curveIndex][i].x == value.x)
					return (int)i;
			}
			return pointIndex;
		}
		virtual void AddPoint(size_t curveIndex, ImVec2 value)
		{
			if (mPointCount[curveIndex] >= 8)
				return;
			mPts[curveIndex][mPointCount[curveIndex]++] = value;
			SortValues(curveIndex);
		}
		virtual ImVec2& GetMax() { return mMax; }
		virtual ImVec2& GetMin() { return mMin; }
		virtual unsigned int GetBackgroundColor() { return 0; }
		ImVec2 mPts[3][8];
		size_t mPointCount[3];
		bool mbVisible[3];
		ImVec2 mMin;
		ImVec2 mMax;
	private:
		void SortValues(size_t curveIndex)
		{
			auto b = std::begin(mPts[curveIndex]);
			auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
			std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });

		}
	};

	struct MySequence : public ImSequencer::SequenceInterface
	{
		// interface with sequencer

		virtual int GetFrameMin() const {
			return mFrameMin;
		}
		virtual int GetFrameMax() const {
			return mFrameMax;
		}
		virtual int GetItemCount() const { return (int)myItems.size(); }

		virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
		virtual const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
		virtual const char* GetItemLabel(int index) const
		{
			static char tmps[512];
			sprintf_s(tmps, "[%02d] %s", index, SequencerItemTypeNames[myItems[index].mType]);
			return tmps;
		}

		virtual void Get(int index, int** start, int** end, int* type, unsigned int* color)
		{
			MySequenceItem& item = myItems[index];
			if (color)
				*color = 0xFFAA8080; // same color for everyone, return color based on type
			if (start)
				*start = &item.mFrameStart;
			if (end)
				*end = &item.mFrameEnd;
			if (type)
				*type = item.mType;
		}
		virtual void Add(int type) { myItems.push_back(MySequenceItem{ type, 0, 10, false }); };
		virtual void Del(int index) { myItems.erase(myItems.begin() + index); }
		virtual void Duplicate(int index) { myItems.push_back(myItems[index]); }

		virtual size_t GetCustomHeight(int index) { return myItems[index].mExpanded ? 300 : 0; }

		// my datas
		MySequence() : mFrameMin(0), mFrameMax(0) {}
		int mFrameMin, mFrameMax;
		struct MySequenceItem
		{
			int mType;
			int mFrameStart, mFrameEnd;
			bool mExpanded;
		};
		std::vector<MySequenceItem> myItems;
		RampEdit rampEdit;

		virtual void DoubleClick(int index) {
			if (myItems[index].mExpanded)
			{
				myItems[index].mExpanded = false;
				return;
			}
			for (auto& item : myItems)
				item.mExpanded = false;
			myItems[index].mExpanded = !myItems[index].mExpanded;
		}

		virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
		{
			static const char* labels[] = { "Translation", "Rotation" , "Scale" };

			rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
			rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
			draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
			for (int i = 0; i < 3; i++)
			{
				ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
				ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
				draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
				if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
					rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
			}
			draw_list->PopClipRect();

			ImGui::SetCursorScreenPos(rc.Min);
			ImCurveEdit::Edit(rampEdit, {rc.Max.x - rc.Min.x, rc.Max.y - rc.Min.y}, 137 + index, &clippingRect);
		}

		virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
		{
			rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
			rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
			draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < rampEdit.mPointCount[i]; j++)
				{
					float p = rampEdit.mPts[i][j].x;
					if (p < myItems[index].mFrameStart || p > myItems[index].mFrameEnd)
						continue;
					float r = (p - mFrameMin) / float(mFrameMax - mFrameMin);
					float x = ImLerp(rc.Min.x, rc.Max.x, r);
					draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
				}
			}
			draw_list->PopClipRect();
		}
	};


	void settings(entt::registry &reg)
	{
		ImGui::Begin("Settings");

		auto stats = bgfx::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats->numDraw);


		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		
		reg.view<component::Transform>().each([&](glm::mat4 &transform){
			auto transform_ptr = glm::value_ptr(transform);
			ImGuizmo::DecomposeMatrixToComponents(transform_ptr, matrixTranslation, matrixRotation, matrixScale);
			ImGui::InputFloat3("Tr", matrixTranslation);
			ImGui::InputFloat3("Rt", matrixRotation);
			ImGui::InputFloat3("Sc", matrixScale);
			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, transform_ptr);
			});

		ImGui::End();
	}

	void viewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		//m_ViewportFocused = ImGui::IsWindowFocused();
		//m_ViewportHovered = ImGui::IsWindowHovered();
		//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		//m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		//uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		//ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}
	
	static MySequence mySequence;
	void sequencer()
	{
		ImGui::Begin("Sequencer");

		static int selectedEntry = -1;
		static int firstFrame = 0;
		static bool expanded = true;
		static int currentFrame = 100;
		//ImGui::SetNextWindowPos(ImVec2(10, 350));

		//ImGui::SetNextWindowSize(ImVec2(940, 480));
		ImGui::Begin("Sequencer");

		ImGui::PushItemWidth(130);
		ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
		ImGui::SameLine();
		ImGui::InputInt("Frame ", &currentFrame);
		ImGui::SameLine();
		ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
		ImGui::PopItemWidth();
		Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
		// add a UI to edit that particular item
		if (selectedEntry != -1)
		{
			const MySequence::MySequenceItem& item = mySequence.myItems[selectedEntry];
			ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.mType]);
			// switch (type) ....
		}

		ImGui::End();

		ImGui::End();
	}

	void dockSpace()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", NULL, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		
		ImGui::End();
	}
}

namespace ui
{
	void init()
	{
		imguiCreate();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		io.IniFilename = "editorconfig.ini";

		auto style = &ImGui::GetStyle();
		style->WindowRounding = 5.3f;
		style->GrabRounding = style->FrameRounding = 2.3f;
		style->ScrollbarRounding = 5.0f;
		style->FrameBorderSize = 1.0f;
		style->ItemSpacing.y = 6.5f;

		style->Colors[ImGuiCol_Text] = { 0.73333335f, 0.73333335f, 0.73333335f, 1.00f };
		style->Colors[ImGuiCol_TextDisabled] = { 0.34509805f, 0.34509805f, 0.34509805f, 1.00f };
		style->Colors[ImGuiCol_WindowBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.94f };
		style->Colors[ImGuiCol_ChildBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.00f };
		style->Colors[ImGuiCol_PopupBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.94f };
		style->Colors[ImGuiCol_Border] = { 0.33333334f, 0.33333334f, 0.33333334f, 0.50f };
		style->Colors[ImGuiCol_BorderShadow] = { 0.15686275f, 0.15686275f, 0.15686275f, 0.00f };
		style->Colors[ImGuiCol_FrameBg] = { 0.16862746f, 0.16862746f, 0.16862746f, 0.54f };
		style->Colors[ImGuiCol_FrameBgHovered] = { 0.453125f, 0.67578125f, 0.99609375f, 0.67f };
		style->Colors[ImGuiCol_FrameBgActive] = { 0.47058827f, 0.47058827f, 0.47058827f, 0.67f };
		style->Colors[ImGuiCol_TitleBg] = { 0.04f, 0.04f, 0.04f, 1.00f };
		style->Colors[ImGuiCol_TitleBgCollapsed] = { 0.16f, 0.29f, 0.48f, 1.00f };
		style->Colors[ImGuiCol_TitleBgActive] = { 0.00f, 0.00f, 0.00f, 0.51f };
		style->Colors[ImGuiCol_MenuBarBg] = { 0.27058825f, 0.28627452f, 0.2901961f, 0.80f };
		style->Colors[ImGuiCol_ScrollbarBg] = { 0.27058825f, 0.28627452f, 0.2901961f, 0.60f };
		style->Colors[ImGuiCol_ScrollbarGrab] = { 0.21960786f, 0.30980393f, 0.41960788f, 0.51f };
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.21960786f, 0.30980393f, 0.41960788f, 1.00f };
		style->Colors[ImGuiCol_ScrollbarGrabActive] = { 0.13725491f, 0.19215688f, 0.2627451f, 0.91f };
		// style->Colors[ImGuiCol_ComboBg]               = {0.1f, 0.1f, 0.1f, 0.99f};
		style->Colors[ImGuiCol_CheckMark] = { 0.90f, 0.90f, 0.90f, 0.83f };
		style->Colors[ImGuiCol_SliderGrab] = { 0.70f, 0.70f, 0.70f, 0.62f };
		style->Colors[ImGuiCol_SliderGrabActive] = { 0.30f, 0.30f, 0.30f, 0.84f };
		style->Colors[ImGuiCol_Button] = { 0.33333334f, 0.3529412f, 0.36078432f, 0.49f };
		style->Colors[ImGuiCol_ButtonHovered] = { 0.21960786f, 0.30980393f, 0.41960788f, 1.00f };
		style->Colors[ImGuiCol_ButtonActive] = { 0.13725491f, 0.19215688f, 0.2627451f, 1.00f };
		style->Colors[ImGuiCol_Header] = { 0.33333334f, 0.3529412f, 0.36078432f, 0.53f };
		style->Colors[ImGuiCol_HeaderHovered] = { 0.453125f, 0.67578125f, 0.99609375f, 0.67f };
		style->Colors[ImGuiCol_HeaderActive] = { 0.47058827f, 0.47058827f, 0.47058827f, 0.67f };
		style->Colors[ImGuiCol_Separator] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
		style->Colors[ImGuiCol_SeparatorHovered] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
		style->Colors[ImGuiCol_SeparatorActive] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
		style->Colors[ImGuiCol_ResizeGrip] = { 1.00f, 1.00f, 1.00f, 0.85f };
		style->Colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.00f, 1.00f, 0.60f };
		style->Colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.00f, 0.90f };
		style->Colors[ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.00f };
		style->Colors[ImGuiCol_PlotLinesHovered] = { 1.00f, 0.43f, 0.35f, 1.00f };
		style->Colors[ImGuiCol_PlotHistogram] = { 0.90f, 0.70f, 0.00f, 1.00f };
		style->Colors[ImGuiCol_PlotHistogramHovered] = { 1.00f, 0.60f, 0.00f, 1.00f };
		style->Colors[ImGuiCol_TextSelectedBg] = { 0.18431373f, 0.39607847f, 0.79215693f, 0.90f };

		// init sequencer

		mySequence.mFrameMin = -100;
		mySequence.mFrameMax = 1000;
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 0, 10, 30, false });
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 1, 20, 30, true });
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 3, 12, 60, false });
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 2, 61, 90, false });
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 4, 90, 99, false });
	}

	void render(entt::registry &reg, const uint32_t& width, const uint32_t& height, const entry::MouseState& mouse, int ch)
	{
		imguiBeginFrame(mouse.m_mx
			, mouse.m_my
			, (mouse.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
			| (mouse.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
			| (mouse.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
			, mouse.m_mz
			, uint16_t(width)
			, uint16_t(height)
			, ch
		);

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		dockSpace();
		settings(reg);
		viewport();
		sequencer();

		imguiEndFrame();

	}
}
