#pragma once

#include "ADT.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>


// default promts
#define PROMT "..."
#define INPUT_TEXT "Input is here"

template <typename T>
class GUI
{
private:
    Sequence<T>* sequence;

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

    virtual void Append(bool* showWindow) = 0;

    virtual void Prepend(bool* showWindow) = 0;

    virtual void InsertAt(bool *showWindow) = 0;

    virtual void GetSubSequence(bool* showWindow) = 0;

    virtual void Concatnate(bool* showWindow, int values[]) = 0;

    virtual void ShowTree(const char* label, const char* children) = 0;

public:
    GUI()
        : sequence((Sequence<T>*) new ListSequence<T>()) { Logger::Info("Default constructor of GUI"); }
    

    GUI(Sequence<T>* another)
        : sequence(another) { Logger::Info("Sequence based constructor of GUI"); }

    void showAppendButton(bool* showWindow)
    {
        this->setupWindow();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        Append(showWindow);

        ImGui::End();
    }

    void showPrependButton(bool* showWindow)
    {
        this->setupWindow();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        Prepend(showWindow);

        ImGui::End();
    }

    void showInsertAtButton(bool* showWindow)
    {
        this->setupWindow();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        InsertAt(showWindow);

        ImGui::End();
    }

    void showGetSubsequenceButton(bool* showWindow)
    {
        this->setupWindowToUpwards();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        GetSubSequence(showWindow);

        ImGui::End();
    }

    void showConcatenateButton(bool* showWindow, int inputs[])
    {
        this->setupWindowToUpwards();

        ImGui::Begin(INPUT_TEXT, showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        Concatnate(showWindow, inputs);

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
};

class IntButton : virtual public GUI<int>
{
public:
    IntButton()
        : GUI() { Logger::Info("Default constructor of IntButton"); }
    

    IntButton(Sequence<int>* another)
        : GUI(another) { Logger::Info("Sequence based constructor of IntButton"); }

    virtual void Append(bool* showWindow)
    {
        short shortInt = 0;
        ImGui::InputScalar(PROMT,     ImGuiDataType_S16,    &shortInt, NULL, "%d");

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || !ImGui::IsWindowFocused())
        {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
            {
                print(shortInt, "\n");
            }
            *showWindow = false;
        }
    }

    virtual void Prepend(bool* showWindow)
    {
        short shortInt = 0;
        ImGui::InputScalar(PROMT,     ImGuiDataType_S16,    &shortInt, NULL, "%d");

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || !ImGui::IsWindowFocused())
        {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
            {
                print(shortInt, "\n");
            }
            *showWindow = false;
        }
    }

    virtual void InsertAt(bool *showWindow)
    {
        static int inputs[4] = { 0, 0, 100, 255 };

        ImGui::InputInt2(PROMT, inputs);

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || (!ImGui::IsWindowFocused()))
        {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
            {
                print(inputs[0], " <-> ", inputs[1], "\n");
            }

            *showWindow = false;

            inputs[0] = 0;
            inputs[1] = 0;
        }
    }

    virtual void GetSubSequence(bool* showWindow)
    {
        static int inputs[4] = { 0, 0, 100, 255 };

        ImGui::InputInt2(PROMT, inputs);

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || (!ImGui::IsWindowFocused()))
        {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
            {
                print(inputs[0], " <-> ", inputs[1], "\n");
            }

            *showWindow = false;

            inputs[0] = 0;
            inputs[1] = 0;
        }
    }

    virtual void Concatnate(bool* showWindow, int values[])
    {

        const char* items[] = {"1", "2"};

        ImGui::PushItemWidth(67);

        ImGui::Combo("##value1", &values[0], items, IM_ARRAYSIZE(items));
        ImGui::SameLine();
        ImGui::Combo("##value2", &values[1], items, IM_ARRAYSIZE(items));
        ImGui::SameLine();
        ImGui::Combo("##value3", &values[2], items, IM_ARRAYSIZE(items));

        ImGui::PopItemWidth();

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
        {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
            {
                print(values[0], " <-> ", values[1], " <-> ", values[2], "\n");
            }

            *showWindow = false;

            values[0] = 0;
            values[1] = 0;
            values[2] = 0;
        }
    }

    virtual void ShowTree(const char* label, const char* children)
    {
        if (ImGui::TreeNode(label))
        {
            for (int i = 0; i < 100; i++)
            {
                if (ImGui::TreeNode((void*)(intptr_t)i, "%s %d", children, i))
                {
                    ImGui::Text("blah blah");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("button")) {}
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
};