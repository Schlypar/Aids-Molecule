#include "Logger.h"
#include "GUI.h"
#include "Tuple.h"
#include "imgui.h"

#include <cctype>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>

LogPriority Logger::priority = TracePriority;

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static int FramesToWearOffStatus = 0;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    static int selected = -1;
    static int firstSelect = -1;
    static int secondSelect = -1;

    float fps = 0.0f;
    float memoryUsage = 0.0f;
    int status = 1; // 0 = green, 1 = yellow, 2 = red

    bool selectedType = false;
    bool selectedSequenceFirst = false;
    bool selectedSequenceSecond = false;

    bool appendButtonClicked = false;
    bool prependButtonClicked = false;
    bool insertAtButtonClicked = false;
    bool getSubsequenceButtonClicked = false;
    bool concatButtonClicked = false;
    bool sliceButtonClicked = false;

    bool matrixWindow = true;

    char buffer[128];
    const char* types[] = { INT, FLOAT };

    const char* sequenceTypes[] = { ARRAY_SEQUENCE, LIST_SEQUENCE };

    int inputs[] = { 0,0,0 };


    Interface* secondInterface = new Interface;
    SequenceLabel secondLabel;
    GUI* secondGUI = init(0, 0, &secondLabel);
    secondInterface->gui = secondGUI;
    
    
    Interface* firstInterface = new Interface;
    SequenceLabel firstLabel;
    GUI* GUI = init(0, 0, &firstLabel);
    firstInterface->gui = GUI;

    secondInterface->other = firstInterface;
    firstInterface->other = secondInterface;

    Interface* interface = firstInterface;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::SetNextWindowSize({962.f,600.f});

            ImGui::Begin("Main menu", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

            ImGui::SetCursorPos({10.f,260.f}); // init phase

                ImGui::BeginChild("TypeSelection",{931.f,86.f}, true);

                if (!selectedType)
                {
                
                    ImGui::SetCursorPos({10.f,14.f});
                    if (ImGui::TreeNodeEx("Selection State: Type", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        
                        for (int n = 0; n < NUMBER_OF_TYPES; n++)
                        {
                            if (ImGui::Selectable( types[n], selected == n))
                            {
                                selected = n;
                                selectedType = true;
                            }

                        }
                        ImGui::TreePop();
                    }
                }
                else if (!selectedSequenceFirst)
                {
                    ImGui::SetCursorPos({10.f,14.f});
                    if (ImGui::TreeNodeEx("Selection State: First Sequence", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        for (int n = 0; n < NUMBER_OF_SEQUENCES; n++)
                        {
                            if (ImGui::Selectable(sequenceTypes[n], firstSelect == n))
                            {
                                firstSelect = n;
                                selectedSequenceFirst = true;
                            }
                        }
                        ImGui::TreePop();
                    }
                }
                else if (!selectedSequenceSecond)
                {
                    ImGui::SetCursorPos({10.f,14.f});
                    if (ImGui::TreeNodeEx("Selection State: Second Sequence", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        for (int n = 0; n < NUMBER_OF_SEQUENCES; n++)
                        {
                            if (ImGui::Selectable(sequenceTypes[n], secondSelect == n))
                            {
                                secondSelect = n;
                                selectedSequenceSecond = true;

                                delete GUI;
                                delete secondGUI;

                                GUI = init(selected, firstSelect, &firstLabel);
                                secondGUI = init(selected, secondSelect, &secondLabel);

                                firstInterface->gui = GUI;
                                firstInterface->labels = firstLabel;
                                memcpy(firstInterface->role, "Main", 5);

                                secondInterface->gui = secondGUI;
                                secondInterface->labels = secondLabel;
                                memcpy(secondInterface->role, "Auxiliary", strlen("Auxiliary"));
                            }
                        }
                        
                        ImGui::TreePop();
                    }
                }
               
                ImGui::EndChild();

            // Console::Debug("Another test");
            fps = ImGui::GetIO().Framerate;
            memoryUsage = ImGui::GetIO().MetricsActiveAllocations;

            ImGui::SetCursorPos({10.f,43.f});
            ImGui::BeginChild("StatusBarFrame",{931.f,25.f},true );

                ImGui::SetCursorPos({11.f,4.f});
                    interface->gui->showStatusBar(status, fps, memoryUsage, interface);
            
            ImGui::EndChild();


            ImGui::BeginDisabled(!selectedType || !selectedSequenceFirst || !selectedSequenceSecond);

            ImGui::SetCursorPos({10.f,74.f});
            ImGui::BeginChild("MethodsFrame",{931.f,172.f},true);

                ImGui::SetCursorPos({14.f,12.f});
                    if(ImGui::Button("Append", {232.f,71.f }))
                    {
                        appendButtonClicked = true;
                    }
                    if (appendButtonClicked)
                    {
                        try
                        {
                            interface->gui->showAppendButton(&appendButtonClicked, interface);
                        }
                        catch(Exception e)
                        {
                            status = 2;
                            FramesToWearOffStatus = RED_FRAMES;
                        }
                    }

                ImGui::SetCursorPos({254.f,12.f});
                    if(ImGui::Button("Prepend", {232.f,71.f }))
                    {
                        prependButtonClicked = true;
                    }
                    ImGui::SetCursorPos({254.f,12.f});
                    if (prependButtonClicked)
                    {
                        try
                        {
                            interface->gui->showPrependButton(&prependButtonClicked, interface);
                        }
                        catch(Exception e)
                        {
                            status = 2;
                            FramesToWearOffStatus = RED_FRAMES;
                        }
                    }


                ImGui::SetCursorPos({494.f,12.f});
                    if(ImGui::Button("Insert At", {232.f,71.f }))
                    {
                        insertAtButtonClicked = true;
                    }
                    if (insertAtButtonClicked)
                    {
                        try
                        {
                            interface->gui->showInsertAtButton(&insertAtButtonClicked, interface);
                        }
                        catch(Exception e)
                        {
                            status = 2;
                            FramesToWearOffStatus = RED_FRAMES;
                        }
                    }
                ImGui::SetCursorPos({734.f,12.f});
                    if(ImGui::Button("Map", {171.f,71.f }))
                    {
                        
                    }


                ImGui::SetCursorPos({14.f,156.f});
                    if(ImGui::Button("Get Subsequence", {232.f,71.f }))
                    {
                        getSubsequenceButtonClicked = true;
                    }
                    if (getSubsequenceButtonClicked)
                    {
                        try
                        {
                            interface->gui->showGetSubsequenceButton(&getSubsequenceButtonClicked, interface);
                        }
                        catch(Exception e)
                        {
                            status = 2;
                            FramesToWearOffStatus = RED_FRAMES;
                        }
                    }
                
                ImGui::SetCursorPos({254.f,156.f});
                    if(ImGui::Button("Concatenate", {232.f,71.f }))
                    {
                        concatButtonClicked = true;
                    }
                    if (concatButtonClicked)
                    {
                        try
                        {
                            interface->gui->showConcatenateButton(&concatButtonClicked, interface);
                        }
                        catch(Exception e)
                        {
                            status = 2;
                            FramesToWearOffStatus = RED_FRAMES;
                        }
                    }
                
                ImGui::SetCursorPos({494.f,156.f});
                    if(ImGui::Button("Slice", {232.f,71.f }))
                    {
                        sliceButtonClicked = true;
                    }
                    if (sliceButtonClicked)
                    {
                        try
                        {
                            interface->gui->showSliceButton(&sliceButtonClicked, interface);
                        }
                        catch(Exception e)
                        {
                            status = 2;
                            FramesToWearOffStatus = RED_FRAMES;
                        }
                    }

                    ImGui::SetCursorPos({734.f,156.f});
                    if(ImGui::Button("Where", {171.f,71.f }))
                    {

                    }
            
            ImGui::EndChild();

            const float treeWindowWidth = 450;

            ImGui::SetCursorPos({10.f,360.f});
            ImGui::BeginChild("OutputFrame",{931.f,238.f},true );

                // first debug-tree
                firstInterface->gui->ShowData({5.f,24.f}, treeWindowWidth,
                 0, "Main", firstInterface->labels.type, firstInterface->labels.elements); 

                // second debug-tree
                secondInterface->gui->ShowData({5.f,24.f}, treeWindowWidth,
                 10 + treeWindowWidth, "Auxiliary", secondInterface->labels.type, secondInterface->labels.elements);

            ImGui::EndChild();

            ImGui::EndDisabled();

            if (selectedSequenceSecond)
            {
                if (FramesToWearOffStatus > 0)
                    FramesToWearOffStatus--;
                
                if (FramesToWearOffStatus == 0)
                    status = 0;
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    delete firstInterface;
    delete secondInterface;

    return 0;
}