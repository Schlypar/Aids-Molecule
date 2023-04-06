#include <string.h>

#include "GUI.h"

// if (ImGui::BeginMenuBar())
//             {
//                 if (ImGui::BeginMenu("Main"))
//                 {
//                     if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Handle save action */ }
//                     if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Handle open action */ }
//                     if (ImGui::MenuItem("Close")) { /* Handle close action */ }
//                     ImGui::EndMenu();
//                 }
//                 if (ImGui::BeginMenu("Tools"))
//                 {
//                     if (ImGui::MenuItem("Open log")) { /* Handle open log action */ }
//                     if (ImGui::MenuItem("Show stats")) { /* Handle show stats action */ }
//                     ImGui::EndMenu();
//                 }
//                 ImGui::EndMenuBar();
//             }
            
//             ImGui::SetCursorPos({13.f,74.f});
//             ImGui::BeginChild("Methods",{926.f,172.f},true );

//                 ImGui::SetCursorPos({14.f,12.f});
//                     if(ImGui::Button("Append", {232.f,71.f }))
//                     {
//                         appendButtonClicked = true;
//                     }
//                     if (appendButtonClicked)
//                     {
//                         button->showAppendButton(&appendButtonClicked);
//                     }

//                 ImGui::SetCursorPos({254.f,12.f});
//                     if(ImGui::Button("Prepend", {232.f,71.f }))
//                     {
//                         prependButtonClicked = true;
//                     }
//                     ImGui::SetCursorPos({254.f,12.f});
//                     if (prependButtonClicked)
//                     {
//                         button->showPrependButton(&prependButtonClicked);
//                     }


//                 ImGui::SetCursorPos({494.f,12.f});
//                     if(ImGui::Button("Insert At", {232.f,71.f }))
//                     {
//                         insertAtButtonClicked = true;
//                     }
//                     if (insertAtButtonClicked)
//                     {
//                         button->showInsertAtButton(&insertAtButtonClicked);
//                     }

//             ImGui::EndChild();