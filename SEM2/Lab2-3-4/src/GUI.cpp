#include <any>
#include <string.h>

#include "GUI.h"
#include "Sequence.h"
#include "imgui.h"

GUI* init(int dataType, int sequenceType, SequenceLabel* labels)
{
	if (dataType == 0)
	{
		if (sequenceType == 0)
		{
			memcpy(labels->type, ARRAY_SEQUENCE, strlen(ARRAY_SEQUENCE));
			memcpy(labels->elements, "Element", sizeof("Element"));

			return (GUI*) new Button<int>((Allocator<int>::AllocateArraySequence()));
		}

		if (sequenceType == 1)
		{
			memcpy(labels->type, LIST_SEQUENCE, strlen(LIST_SEQUENCE));
			memcpy(labels->elements, "Node", sizeof("Node"));

			return (GUI*) new Button<int>((Allocator<int>::AllocateListSequence()));
		}
	}
	if (dataType == 1)
	{
		if (sequenceType == 0)
		{
			memcpy(labels->type, ARRAY_SEQUENCE, strlen(ARRAY_SEQUENCE));
			memcpy(labels->elements, "Element", sizeof("Element"));

			return (GUI*) new Button<float>((Allocator<float>::AllocateArraySequence()));
		}

		if (sequenceType == 1)
		{
			memcpy(labels->type, LIST_SEQUENCE, strlen(LIST_SEQUENCE));
			memcpy(labels->elements, "Node", sizeof("Node"));

			return (GUI*) new Button<float>((Allocator<float>::AllocateListSequence()));
		}
	}
}

void GUI::showStatusBar(int status, float fps, float memoryUsage, Interface* interface)
{
	ImGui::Text("Status: ");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // white text
	if (status == 0)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // green status else if (status == 1)
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));	      // yellow status else
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));	      // red status
	ImGui::SameLine();
	ImGui::Text("%s", status == 0 ? GREEN : (status == 1 ? YELLOW : RED));
	ImGui::PopStyleColor(2);
	ImGui::SameLine();
	ImGui::Text("\tFPS: %.1f\tMemory: %.1f MB\tWorking in Sequence: %s", fps, memoryUsage, interface->role);
}

template <>
void Button<int>::Append(bool* showWindow, Interface* current)
{
	short shortInt = 0;
	ImGui::InputScalar(PROMT, ImGuiDataType_S16, &shortInt, NULL, "%d");

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || !ImGui::IsWindowFocused())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			Sequence<int>* result = std::any_cast<Sequence<int>*>(current->gui->GetSequence());
			result->Append(shortInt);
		}
		*showWindow = false;
	}
}

template <>
void Button<float>::Append(bool* showWindow, Interface* current)
{
	float floatNum = 0;
	ImGui::InputScalar(PROMT, ImGuiDataType_Float, &floatNum, NULL, "%f");

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || !ImGui::IsWindowFocused())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			Sequence<float>* result = std::any_cast<Sequence<float>*>(current->gui->GetSequence());
			result->Append(floatNum);
		}
		*showWindow = false;
	}
}

template <>
void Button<int>::Prepend(bool* showWindow, Interface* current)
{
	short shortInt = 0;
	ImGui::InputScalar(PROMT, ImGuiDataType_S16, &shortInt, NULL, "%d");

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || !ImGui::IsWindowFocused())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			Sequence<int>* result = std::any_cast<Sequence<int>*>(current->gui->GetSequence());
			result->Prepend(shortInt);
		}
		*showWindow = false;
	}
}

template <>
void Button<float>::Prepend(bool* showWindow, Interface* current)
{
	float floatNum = 0;
	ImGui::InputScalar(PROMT, ImGuiDataType_Float, &floatNum, NULL, "%f");

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || !ImGui::IsWindowFocused())
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			Sequence<float>* result = std::any_cast<Sequence<float>*>(current->gui->GetSequence());
			result->Prepend(floatNum);
		}
		*showWindow = false;
	}
}

template <>
void Button<int>::InsertAt(bool* showWindow, Interface* current)
{
	static int inputs[4] = { 0, 0, 100, 255 };

	ImGui::InputInt2(PROMT, inputs);

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || (!ImGui::IsWindowFocused()))
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			Sequence<int>* result = std::any_cast<Sequence<int>*>(current->gui->GetSequence());
			result->InsertAt(inputs[0], inputs[1]);
		}

		*showWindow = false;

		inputs[0] = 0;
		inputs[1] = 0;
	}
}

template <>
void Button<float>::InsertAt(bool* showWindow, Interface* current)
{
	const int itemWidth = 97;

	int shortInt = 0;

	float floatNum = 0.0f;

	ImGui::PushItemWidth(itemWidth);
	ImGui::InputScalar("|", ImGuiDataType_S16, &shortInt, NULL, "%d");
	ImGui::PopItemWidth();

	ImGui::SameLine();

	ImGui::PushItemWidth(itemWidth);
	ImGui::InputScalar("", ImGuiDataType_Float, &floatNum, NULL, "%f");
	ImGui::PopItemWidth();

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || (!ImGui::IsWindowFocused()))
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			Sequence<float>* result = std::any_cast<Sequence<float>*>(current->gui->GetSequence());
			result->InsertAt(shortInt, floatNum);
		}

		*showWindow = false;

		shortInt = 0;
		floatNum = 0;
	}
}

template <typename T>
void Button<T>::GetSubSequence(bool* showWindow, Interface* current)
{
	static int inputs[4] = { 0, 0, 100, 255 };

	ImGui::InputInt2(PROMT, inputs);

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || (!ImGui::IsWindowFocused()))
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			Sequence<T>* temp = std::any_cast<Sequence<T>*>(current->gui->GetSequence());
			Sequence<T>* result = temp->GetSubsequence(inputs[0], inputs[1]);

			delete (std::any_cast<Sequence<int>*>(current->other->gui->GetSequence()));
			current->other->gui->SetSequence(result);
		}

		*showWindow = false;

		inputs[0] = 0;
		inputs[1] = 0;
	}
}

template <typename T>
void Button<T>::Concatenate(bool* showWindow, Interface* current)
{

	const char* items[] = { "Main", "Auxiliary" };

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
			Sequence<T>* first = nullptr;
			if (values[0] == 0)
				first = std::any_cast<Sequence<T>*>(current->gui->GetSequence());
			else
				first = std::any_cast<Sequence<T>*>(current->other->gui->GetSequence());

			Sequence<T>* second = nullptr;
			if (values[1] == 0)
				second = std::any_cast<Sequence<T>*>(current->gui->GetSequence());
			else
				second = std::any_cast<Sequence<T>*>(current->other->gui->GetSequence());

			Sequence<T>* result = first->Concat(second);

			if (values[2] == 0)
			{
				delete (std::any_cast<Sequence<T>*>(current->gui->GetSequence()));
				current->gui->SetSequence(result);
			}
			else
			{
				delete (std::any_cast<Sequence<T>*>(current->other->gui->GetSequence()));
				current->other->gui->SetSequence(result);
			}
		}

		*showWindow = false;

		values[0] = 0;
		values[1] = 0;
		values[2] = 0;
	}
}

template <typename T>
void Button<T>::Slice(bool* showWindow, Interface* current)
{
	static int inputs[4] = { 0, 0, 100, 255 };

	const char* items[] = { "Main", "Auxiliary" };

	ImGui::PushItemWidth(100);

	ImGui::InputInt2("##value12", inputs);
	ImGui::SameLine();
	ImGui::Combo("##value3", &whichSequence, items, IM_ARRAYSIZE(items));

	ImGui::PopItemWidth();

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			Sequence<T>* temp = nullptr;
			(whichSequence == 0) ? temp = std::any_cast<Sequence<T>*>(current->gui->GetSequence())
					     : temp = std::any_cast<Sequence<T>*>(current->other->gui->GetSequence());

			if (whichSequence == 0)
			{
				Sequence<T>* result = temp->Slice(inputs[0], inputs[1],
					std::any_cast<Sequence<T>*>(current->other->gui->GetSequence()));
				delete temp;
				current->gui->SetSequence(result);
			}
			else
			{
				Sequence<T>* result = temp->Slice(inputs[0], inputs[1],
					std::any_cast<Sequence<T>*>(current->gui->GetSequence()));
				delete temp;
				current->other->gui->SetSequence(result);
			}
		}

		*showWindow = false;

		inputs[0] = 0;
		inputs[1] = 0;
	}
}

template <>
void Button<int>::ShowTree(const char* label, const char* children)
{
	if (ImGui::TreeNode(label))
	{
		for (Index i = 0; i < sequence->GetLength(); i++)
		{
			if (ImGui::TreeNode((void*) (intptr_t) i, "%s %zu", children, i))
			{
				ImGui::Text("Address: %p", &sequence->Get(i));
				ImGui::Text("Value: %d", sequence->Get(i));
				if (ImGui::SmallButton("Delete"))
					sequence->Remove(i);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

template <>
void Button<float>::ShowTree(const char* label, const char* children)
{
	if (ImGui::TreeNode(label))
	{
		for (Index i = 0; i < sequence->GetLength(); i++)
		{
			if (ImGui::TreeNode((void*) (intptr_t) i, "%s %zu", children, i))
			{
				ImGui::Text("Address: %p", &sequence->Get(i));
				ImGui::Text("Value: %f", sequence->Get(i));
				if (ImGui::SmallButton("Delete"))
					sequence->Remove(i);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}