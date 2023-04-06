#include "Logger.h"
#include "GUI.h"

#include <cctype>
#include <stdlib.h>
#include <string.h>

LogPriority Logger::priority = TracePriority;

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

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
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



    bool appendButtonClicked = false;
    bool prependButtonClicked = false;
    bool insertAtButtonClicked = false;
    bool getSubsequenceButtonClicked = false;
    bool concatButtonClicked = false;
    bool sliceButtonClicked = false;

    char buffer[128];

    int inputs[] = { 0,0,0 };

    GUI<int>* button = new IntButton();
    // GUI* gui = new GUI;

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

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Main"))
                {
                    if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Handle save action */ }
                    if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Handle open action */ }
                    if (ImGui::MenuItem("Close")) { /* Handle close action */ }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Tools"))
                {
                    if (ImGui::MenuItem("Open log")) { /* Handle open log action */ }
                    if (ImGui::MenuItem("Show stats")) { /* Handle show stats action */ }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::SetCursorPos({10.f,43.f});
            ImGui::BeginChild("StatusBarFrame",{931.f,25.f},true );

            ImGui::SetCursorPos({11.f,4.f});
                ImGui::PushItemWidth(63.000000);
                ImGui::Text("| Status:");
                ImGui::PopItemWidth( );
            
            ImGui::EndChild();


            ImGui::SetCursorPos({10.f,74.f});
            ImGui::BeginChild("MethodsFrame",{931.f,172.f},true);

                ImGui::SetCursorPos({14.f,12.f});
                    if(ImGui::Button("Append", {232.f,71.f }))
                    {
                        appendButtonClicked = true;
                    }
                    if (appendButtonClicked)
                    {
                        button->showAppendButton(&appendButtonClicked);
                    }

                ImGui::SetCursorPos({254.f,12.f});
                    if(ImGui::Button("Prepend", {232.f,71.f }))
                    {
                        prependButtonClicked = true;
                    }
                    ImGui::SetCursorPos({254.f,12.f});
                    if (prependButtonClicked)
                    {
                        button->showPrependButton(&prependButtonClicked);
                    }


                ImGui::SetCursorPos({494.f,12.f});
                    if(ImGui::Button("Insert At", {232.f,71.f }))
                    {
                        insertAtButtonClicked = true;
                    }
                    if (insertAtButtonClicked)
                    {
                        button->showInsertAtButton(&insertAtButtonClicked);
                    }
                
                ImGui::SetCursorPos({14.f,156.f});
                    if(ImGui::Button("Get Subsequence", {232.f,71.f }))
                    {
                        getSubsequenceButtonClicked = true;
                    }
                    if (getSubsequenceButtonClicked)
                    {
                       button->showGetSubsequenceButton(&getSubsequenceButtonClicked);
                    }
                
                ImGui::SetCursorPos({254.f,156.f});
                    if(ImGui::Button("Concatenate", {232.f,71.f }))
                    {
                        concatButtonClicked = true;
                    }
                    if (concatButtonClicked)
                    {
                       button->showConcatenateButton(&concatButtonClicked, inputs);
                    }
                
                ImGui::SetCursorPos({494.f,156.f});
                    if(ImGui::Button("Slice", {232.f,71.f }))
                    {
                        concatButtonClicked = true;
                    }
                    if (concatButtonClicked)
                    {
                       
                    }
            
            ImGui::EndChild();

            const float treeWindowWidth = 450;

            ImGui::SetCursorPos({10.f,260.f});
            ImGui::BeginChild("OutputFrame",{931.f,238.f},true );

                // first debug-tree
                button->ShowData({5.f,24.f}, treeWindowWidth, 0, "First", "Sequence", "Child"); 

                // second debug-tree
                button->ShowData({5.f,24.f}, treeWindowWidth, 10 + treeWindowWidth, "Second", "Omegalul Sequence", "lul");

            ImGui::EndChild();

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

    delete button;

    return 0;
}