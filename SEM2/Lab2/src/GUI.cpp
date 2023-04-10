#include <string.h>

#include "GUI.h"
#include "Sequence.h"

GUI* init(int dataType, int sequenceType, SequenceLabel* labels)
{
    if (dataType == 0)
    {
        if (sequenceType == 0)
        {
            memcpy(labels->type, ARRAY_SEQUENCE , strlen(ARRAY_SEQUENCE));
            memcpy(labels->elements, "Element", sizeof("Element"));

            return (GUI*) new IntButton ((Allocator<int>::AllocateArraySequence()));
        }
        
        if (sequenceType == 1)
        {
            memcpy(labels->type, LIST_SEQUENCE , strlen(LIST_SEQUENCE));
            memcpy(labels->elements, "Node", sizeof("Node"));

            return (GUI*) new IntButton ((Allocator<int>::AllocateListSequence()));
        }
    }
}

void GUI::showStatusBar(int status, float fps, float memoryUsage, Interface* interface)
{
    ImGui::Text("Status: ");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // white text
    if (status == 0) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // green status
    } else if (status == 1) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow status
    } else {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // red status
    }
    ImGui::SameLine();
    ImGui::Text("%s", status == 0 ? GREEN : (status == 1 ? YELLOW : RED));
    ImGui::PopStyleColor(2);
    ImGui::SameLine();
    ImGui::Text("\tFPS: %.1f\tMemory: %.1f MB\tWorking in Sequence: %s", fps, memoryUsage, interface->role);
}

void IntButton::Append(bool* showWindow, Interface* current)
{
    short shortInt = 0;
    ImGui::InputScalar(PROMT,     ImGuiDataType_S16,    &shortInt, NULL, "%d");

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || !ImGui::IsWindowFocused())
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
        {
            Sequence<int>* result = current->gui->GetSequence();
            result->Append(shortInt);
        }
        *showWindow = false;
    }
}

void IntButton::Prepend(bool* showWindow, Interface* current)
{
    short shortInt = 0;
    ImGui::InputScalar(PROMT,     ImGuiDataType_S16,    &shortInt, NULL, "%d");

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || !ImGui::IsWindowFocused())
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
        {
            Sequence<int>* result = current->gui->GetSequence();
            result->Prepend(shortInt);
        }
        *showWindow = false;
    }
}

void IntButton::InsertAt(bool *showWindow, Interface* current)
{
    static int inputs[4] = { 0, 0, 100, 255 };

    ImGui::InputInt2(PROMT, inputs);

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || (!ImGui::IsWindowFocused()))
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
        {
            Sequence<int>* result = current->gui->GetSequence();
            result->InsertAt(inputs[0], inputs[1]);
        }

        *showWindow = false;

        inputs[0] = 0;
        inputs[1] = 0;
    }
}

void IntButton::GetSubSequence(bool* showWindow, Interface* current)
{
    static int inputs[4] = { 0, 0, 100, 255 };

    ImGui::InputInt2(PROMT, inputs);

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || (!ImGui::IsWindowFocused()))
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
        {
            Sequence<int>* temp = current->gui->GetSequence();
            Sequence<int>* result = temp->GetSubsequence(inputs[0], inputs[1]);

            delete (current->other->gui->GetSequence());
            current->other->gui->SetSequence(result);
        }

        *showWindow = false;

        inputs[0] = 0;
        inputs[1] = 0;
    }
}

void IntButton::Concatenate(bool* showWindow, Interface* current)
{

    const char* items[] = {"Main", "Auxiliary"};

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
            Sequence<int>* first = nullptr;
            if (values[0] == 0)
            {
                first = current->gui->GetSequence();
            }
            else
            {
                first = current->other->gui->GetSequence();
            }

            Sequence<int>* second = nullptr;
            if (values[1] == 0)
            {
                second = current->gui->GetSequence();
            }
            else
            {
                second = current->other->gui->GetSequence();
            }

            Sequence<int>* result = first->Concat(second);

            if (values[2] == 0)
            {
                current->gui->SetSequence(result);
            }
            else
            {
                current->other->gui->SetSequence(result);
            }
        }

        *showWindow = false;

        values[0] = 0;
        values[1] = 0;
        values[2] = 0;
    }
}

void IntButton::Slice(bool* showWindow, Interface* current)
{
    static int inputs[4] = { 0, 0, 100, 255 };
    
    const char* items[] = {"Main", "Auxiliary"};

    ImGui::PushItemWidth(100);

    ImGui::InputInt2("##value12", inputs);
    ImGui::SameLine();
    ImGui::Combo("##value3", &whichSequence, items, IM_ARRAYSIZE(items));

    ImGui::PopItemWidth();

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
        {
            Sequence<int>* temp = nullptr;
            (whichSequence == 0) ? temp = current->gui->GetSequence() : temp = current->other->gui->GetSequence();

            if (whichSequence == 0)
            {
                Sequence<int>* result = temp->Slice(inputs[0], inputs[1], current->other->gui->GetSequence());
                delete temp;
                current->gui->SetSequence(result);
            }
            else
            {
                Sequence<int>* result = temp->Slice(inputs[0], inputs[1], current->gui->GetSequence());
                delete temp;
                current->other->gui->SetSequence(result);
            }
            
        }

        *showWindow = false;

        inputs[0] = 0;
        inputs[1] = 0;
    }
}