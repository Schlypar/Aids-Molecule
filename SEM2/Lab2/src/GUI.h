#pragma once

#include <any>

#include "ADT.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Sequence.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <utility>

static int whichSequence = 0;

static int values[] = { 0,0,0 };

// default promts
#define PROMT "..."
#define INPUT_TEXT "Input is here"

#define GREEN "I'm fine"
#define YELLOW "Initialising"
#define RED "Error detected"

#define RED_FRAMES 300

#define INT "int"
#define FLOAT "float (not working yet)"
#define NUMBER_OF_TYPES 2


#define ARRAY_SEQUENCE "Array Sequence"
#define LIST_SEQUENCE "List Sequence"
#define NUMBER_OF_SEQUENCES 2

typedef class Interface Interface;
struct SequenceLabel
{
    char type[100] = {"Sequence"};
    char elements[100] = {"Element"};
};
class GUI
{
private:
    void setupWindow()
    {
        ImVec2 prev_item_pos = ImGui::GetItemRectMin();

        ImVec2 window_pos = ImVec2(prev_item_pos.x, prev_item_pos.y + ImGui::GetItemRectSize().y + 10); // position window below previous element
        ImVec2 window_size = ImVec2(232, 56); // set size of input text box window

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);
    }

    void setupWindowToUpwards()
    {
        ImVec2 prev_item_pos = ImGui::GetItemRectMin();

        ImVec2 window_pos = ImVec2(prev_item_pos.x, prev_item_pos.y - ImGui::GetItemRectSize().y + 6); // position window over previous element
        ImVec2 window_size = ImVec2(232, 56); // set size of input text box window

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);
    }

    void setupWindowWithOffset(int width, int offset)
    {
        ImVec2 prev_item_pos = ImGui::GetItemRectMin();

        ImVec2 window_pos = ImVec2(prev_item_pos.x + 10 + offset, prev_item_pos.y + ImGui::GetItemRectSize().y + 10); // position window below previous element
        ImVec2 window_size = ImVec2(width, 218); // set size of input text box window

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);
    }

    virtual void Append(bool* showWindow, Interface* current) {}

    virtual void Prepend(bool* showWindow, Interface* current) {}

    virtual void InsertAt(bool *showWindow, Interface* current) {}

    virtual void GetSubSequence(bool* showWindow, Interface* current) {}

    virtual void Concatenate(bool* showWindow, Interface* other) {}

    // virtual Sequence<float>* Concatenate(bool* showWindow) {}



    virtual void ShowTree(const char* label, const char* children) {}

    virtual void Slice(bool* showWindow, Interface* other) {}

public:
    virtual ~GUI() {}

    virtual Size GetLength() const  = 0;

    // virtual Sequence<int>* GetSequence() const = 0;
    
    void showAppendButton(bool* showWindow, Interface* current)
    {
        this->setupWindow();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        Append(showWindow, current);

        ImGui::End();
    }

    void showPrependButton(bool* showWindow, Interface* current)
    {
        this->setupWindow();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        Prepend(showWindow, current);

        ImGui::End();
    }

    void showInsertAtButton(bool* showWindow, Interface* current)
    {
        this->setupWindow();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        InsertAt(showWindow, current);

        ImGui::End();
    }

    void showGetSubsequenceButton(bool* showWindow, Interface* current)
    {
        this->setupWindowToUpwards();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        GetSubSequence(showWindow, current);

        ImGui::End();
    }

    void showConcatenateButton(bool* showWindow, Interface* other)
    {
        this->setupWindowToUpwards();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        Concatenate(showWindow, other);

        ImGui::End();
    }

    void showSliceButton(bool* showWindow, Interface* current)
    {
        this->setupWindowToUpwards();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        Slice(showWindow, current);

        ImGui::End();
    }

    void ShowData(ImVec2 pos, int width, int offset, const char* windowName, const char* label, const char* children)
    {
        this->setupWindowWithOffset(width, offset);

        ImGui::Begin(windowName, NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        ImGui::SetCursorPos(pos);

            ShowTree(label, children);
        
        ImGui::End();
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

    Size GetLength() const override { return sequence->GetLength(); }

    void Append(bool* showWindow, Interface* current) override;

    void Prepend(bool* showWindow, Interface* current) override;

    void InsertAt(bool *showWindow, Interface* current) override;

    void GetSubSequence(bool* showWindow, Interface* current) override;

    void Concatenate(bool* showWindow, Interface* other) override;

    void Slice(bool* showWindow, Interface* current) override;

    void ShowTree(const char* label, const char* children) override;

    Sequence<int>* GetSequenceInt() const 
    { 
        return sequence; 
    }

    std::any GetSequence() override
    {
        return std::any(GetSequenceInt());
    }

    void SetSequenceInt(Sequence<int>* newSequence) 
    { 
        sequence = newSequence; 
    }

    void SetSequence(std::any other) override
    {
        // delete sequence;

        sequence = std::any_cast<Sequence<int>*>(other);
    }
};

GUI* init(int dataType, int sequenceType, SequenceLabel* labels);

struct Interface
{
public:
    GUI* gui;
    SequenceLabel labels;
    char role[100] = {"None"}; 
    Interface* other;

    ~Interface()
    {
        delete gui;
    }
};
