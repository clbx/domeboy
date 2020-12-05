#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

#include "gameboy.h"

void memoryEditor(Memory memory);

int main(){
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char *glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);


    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("domeboy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    bool err = glewInit() != 0;
    if (err){
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glEnable(GL_DEBUG_OUTPUT);

    bool done = false;
    


    //Create GB Out Texture. Thanks to @ThePixelCoder for helping me with this. 
    GLuint gbtex = 0;
    uint32_t gbdata[160 * 144];
    glGenTextures(1,&gbtex);
    glBindTexture(GL_TEXTURE_2D,gbtex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 160, 144, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, gbdata);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    Gameboy gameboy = Gameboy();

    while (!done){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        /** VIDEO OUT **/
        glBindTexture(GL_TEXTURE_2D,gbtex);
        glTexSubImage2D(GL_TEXTURE_2D,0,0,0,160,144,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,gbdata);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        float my_tex_w = (float)160;
        float my_tex_h = (float)144;


        ImVec2 uv_min = ImVec2(0.0f,0.0f);
        ImVec2 uv_max = ImVec2(1.0f,1.0f);
        ImVec4 tint_col = ImVec4(1.0f,1.0f,1.0f,1.0f);
        ImVec4 border_col = ImVec4(1.0f,1.0f,1.0f,1.0f);

        ImGui::SetNextWindowPos(ImVec2(630, 30), ImGuiCond_Once);
        ImGui::Begin("Gameboy");
        ImGui::Image((void*)(intptr_t)(gbtex),ImVec2(my_tex_w,my_tex_h), uv_min, uv_max, tint_col, border_col);
        ImGui::End();
        /** VIDEO OUT END **/


        memoryEditor(gameboy.memory);



        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
   
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

    }




    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void memoryEditor(Memory memory){
    static MemoryEditor ROMBank0_Edit;
    static MemoryEditor ROMBank1_Edit;
    static MemoryEditor VRAM_Edit;
    static MemoryEditor ExternRAM_Edit;
    static MemoryEditor WorkRAM0_Edit;
    static MemoryEditor WorkRAM1_Edit;
    static MemoryEditor EchoRAM_edit;
    static MemoryEditor OAMRAM_edit;
    static MemoryEditor HRAM_edit;
    static MemoryEditor IE_Edit;

    ROMBank1_Edit.DrawWindow("asdf",memory.ROMBank1.data(), sizeof(uint8_t)*memory.ROMBank1.size());

    /*
    ImGui::SetNextWindowSize(ImVec2(530, 280), ImGuiCond_Once);
    ImGui::Begin("Memory Edit");
    if(ImGui::BeginTabBar("Memory Editors", 0)){

        if(ImGui::BeginTabItem("ROMBank0")){
            ImGui::Text("Poo poo");
            ROMBank0_Edit.DrawContents(memory.ROMBank0.data(), sizeof(uint8_t)*memory.ROMBank0.size());
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Penis")){
            ImGui::Text("Weiner");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
    */

    
}