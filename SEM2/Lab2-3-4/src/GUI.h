#pragma once

#include <any>

#include "ADT.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Sequence.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cstdlib>
#include <imgui.h>
#include <utility>


#include <GLFW/glfw3.h> // Will drag system OpenGL headers

static int whichSequence = 0;

static int values[] = { 0, 0, 0 };

// default promts
#define PROMT "..."
#define INPUT_TEXT "Input is here"

#define GREEN "I'm fine"
#define YELLOW "Initialising"
#define RED "Error detected"

#define RED_FRAMES 300

#define INT "int"
#define FLOAT "float"
#define NUMBER_OF_TYPES 2

#define ARRAY_SEQUENCE "Array Sequence"
#define LIST_SEQUENCE "List Sequence"
#define NUMBER_OF_SEQUENCES 2

#define UNIT 25

typedef class Interface Interface;

struct SequenceLabel
{
	char type[100] = { "Sequence" };
	char elements[100] = { "Element" };
};

class GUI
{
public:
	void setupWindow()
	{
		ImVec2 prev_item_pos = ImGui::GetItemRectMin();

		ImVec2 window_pos = ImVec2(prev_item_pos.x,
			prev_item_pos.y + ImGui::GetItemRectSize().y + 10); // position window below previous element
		ImVec2 window_size = ImVec2(232, 56);			    // set size of input text box window

		ImGui::SetNextWindowSize(window_size);
		ImGui::SetNextWindowPos(window_pos);
	}

	void setupWindowToUpwards()
	{
		ImVec2 prev_item_pos = ImGui::GetItemRectMin();

		ImVec2 window_pos = ImVec2(prev_item_pos.x,
			prev_item_pos.y - ImGui::GetItemRectSize().y + 6); // position window over previous element
		ImVec2 window_size = ImVec2(232, 56);			   // set size of input text box window

		ImGui::SetNextWindowSize(window_size);
		ImGui::SetNextWindowPos(window_pos);
	}

	void setupWindowWithOffset(int width, int offset)
	{
		ImVec2 prev_item_pos = ImGui::GetItemRectMin();

		ImVec2 window_pos = ImVec2(prev_item_pos.x + 10 + offset,
			prev_item_pos.y + ImGui::GetItemRectSize().y + 10); // position window below previous element
		ImVec2 window_size = ImVec2(width, 218);		    // set size of input text box window

		ImGui::SetNextWindowSize(window_size);
		ImGui::SetNextWindowPos(window_pos);
	}

	virtual void Append(bool* showWindow, Interface* current)
	{
	}

	virtual void Prepend(bool* showWindow, Interface* current)
	{
	}

	virtual void InsertAt(bool* showWindow, Interface* current)
	{
	}

	virtual void GetSubSequence(bool* showWindow, Interface* current)
	{
	}

	virtual void Concatenate(bool* showWindow, Interface* other)
	{
	}

	virtual void ShowTree(const char* label, const char* children)
	{
	}

	virtual void Slice(bool* showWindow, Interface* other)
	{
	}

public:
	virtual ~GUI()
	{
	}

	virtual Size GetLength() const = 0;

	// virtual Sequence<int>* GetSequence() const = 0;

	void showAppendButton(bool* showWindow, Interface* current)
	{
		this->setupWindow();

		ImGui::Begin("Append Window", showWindow,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

		Append(showWindow, current);

		ImGui::End();
	}

	void showPrependButton(bool* showWindow, Interface* current)
	{
		this->setupWindow();

		ImGui::Begin("Prepend Window", showWindow,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

		Prepend(showWindow, current);

		ImGui::End();
	}

	void showInsertAtButton(bool* showWindow, Interface* current)
	{
		this->setupWindow();

		ImGui::Begin("\tIndex\t\t\tvalue", showWindow,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

		InsertAt(showWindow, current);

		ImGui::End();
	}

	void showGetSubsequenceButton(bool* showWindow, Interface* current)
	{
		this->setupWindowToUpwards();

		ImGui::Begin("\tFrom\t\tTo", showWindow,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

		GetSubSequence(showWindow, current);

		ImGui::End();
	}

	void showConcatenateButton(bool* showWindow, Interface* other)
	{
		this->setupWindowToUpwards();

		ImGui::Begin("First\tSecond\tDestination", showWindow,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

		Concatenate(showWindow, other);

		ImGui::End();
	}

	void showSliceButton(bool* showWindow, Interface* current)
	{
		this->setupWindowToUpwards();

		ImGui::Begin("Index\tSize\tFrom which", showWindow,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

		Slice(showWindow, current);

		ImGui::End();
	}

	void ShowData(ImVec2 pos, int width, int offset, const char* windowName, const char* label, const char* children)
	{
		this->setupWindowWithOffset(width, offset);

		ImGui::Begin(windowName, NULL,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

		ImGui::SetCursorPos(pos);

		ShowTree(label, children);

		ImGui::End();
	}

	virtual void ShowGrid(ImDrawList* draw_list, ImVec2 canvas_pos, ImVec2 canvas_size, float grid_size, ImU32 grid_color)
	{
		// Calculate number of grid lines to draw
		int num_lines_x = static_cast<int>(std::ceil(canvas_size.x / grid_size));
		int num_lines_y = static_cast<int>(std::ceil(canvas_size.y / grid_size));

		// Draw vertical grid lines
		for (int i = 0; i < num_lines_x; ++i)
		{
			float x = canvas_pos.x + i * grid_size;
			draw_list->AddLine(ImVec2(x, canvas_pos.y), ImVec2(x, canvas_pos.y + canvas_size.y), grid_color);
		}

		// Draw horizontal grid lines
		for (int i = 0; i < num_lines_y; ++i)
		{
			float y = canvas_pos.y + i * grid_size;
			draw_list->AddLine(ImVec2(canvas_pos.x, y), ImVec2(canvas_pos.x + canvas_size.x, y), grid_color);
		}
	}

	void DrawGrid(ImDrawList* draw_list, ImVec2 O, ImVec2 i, ImVec2 j, ImU32 color, float size)
	{

		int numOfLinesX = 20000;
		int numOfLinesY = 20000;

		i.y = -i.y;
		j.y = -j.y;

		if (i.x == 0)
			std::swap(i.x, i.y);

		if (j.x == 0)
			std::swap(j.x, j.y);

		float tanOfX = i.y / i.x;

		float cotOfY = j.x / j.y;

		for (Index first = 0; first < numOfLinesX; first += UNIT)
		{
			float x = O.x + first * i.x;

			float firstPointX = x - size;
			float firstPointY = O.y + tanOfX * (x - size);

			float secondPointX = x + size;
			float secondPointY = O.y + tanOfX * (x + size);

			draw_list->AddLine(ImVec2(firstPointX + first, firstPointY), ImVec2(secondPointX + first, secondPointY),
				color);
		}

		for (Index second = 0; second < numOfLinesY; second += UNIT)
		{
			float y = O.y + second * j.y;

			float firstPointX = O.x + cotOfY * (y - size);
			float firstPointY = y - size;

			float secondPointX = O.x + cotOfY * (y + size);
			float secondPointY = y + size;

			draw_list->AddLine(ImVec2(firstPointX + second, firstPointY), ImVec2(secondPointX + second, secondPointY),
				color);
		}
	}

	void showStatusBar(int status, float fps, float memoryUsage, Interface* interface);

	virtual std::any GetSequence() = 0;

	virtual void SetSequence(std::any other) = 0;
};

template <typename T>
class Button : public GUI
{
private:
	Sequence<T>* sequence;

public:
	Button()
	    : sequence((Sequence<T>*) new ListSequence<T>)
	{
		Logger::Info("Default constructor of IntButton");
	}

	Button(Sequence<T>* another)
	    : sequence(another)
	{
		Logger::Info("Sequence based constructor of IntButton");
	}

	virtual ~Button()
	{
		delete sequence;
	}

	Size GetLength() const override
	{
		return sequence->GetLength();
	}

	void Append(bool* showWindow, Interface* current) override;

	void Prepend(bool* showWindow, Interface* current) override;

	void InsertAt(bool* showWindow, Interface* current) override;

	void GetSubSequence(bool* showWindow, Interface* current) override;

	void Concatenate(bool* showWindow, Interface* other) override;

	void Slice(bool* showWindow, Interface* current) override;

	void ShowTree(const char* label, const char* children) override;

	std::any GetSequence() override
	{
		return std::any(sequence);
	}

	void SetSequence(std::any other) override
	{
		sequence = std::any_cast<Sequence<T>*>(other);
	}
};

GUI* init(int dataType, int sequenceType, SequenceLabel* labels);

struct Interface
{
public:
	GUI* gui;
	SequenceLabel labels;
	char role[100] = { "None" };
	Interface* other;

	~Interface()
	{
		delete gui;
	}
};