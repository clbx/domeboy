#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "imgui_memory_editor/imgui_memory_editor.h"
#include "imguifs/imguifilesystem.h"
#include "imgui_logger.h"

#include <string>
#include <fstream>
#include <sstream>

#include "gameboy.h"

static Logger debugLog;

void debugWindow();
void memoryWindow(Gameboy *gameboy);
void statusWindow(Gameboy *gameboy);
void topMenu(Memory *memory);
void videoWindow(Gameboy *gameboy,GLuint gbtex);
void controlWindow(Gameboy *gameboy);


uint32_t gbdata[160 * 144] = {};

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
    glGenTextures(1,&gbtex);
    glBindTexture(GL_TEXTURE_2D,gbtex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 160, 144, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, gbdata);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    Gameboy gameboy = Gameboy(&debugLog);

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


        /** GUI **/
        ImGui::ShowDemoWindow();
        controlWindow(&gameboy);
        videoWindow(&gameboy,gbtex);
        topMenu(&gameboy.memory);
        memoryWindow(&gameboy);
        statusWindow(&gameboy);
        debugWindow();
        /** END GUI **/

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


void controlWindow(Gameboy *gameboy){
    ImGui::Begin("Control");
    if(gameboy->running){
        if(ImGui::Button("Stop")){
            gameboy->running = false;
        }
    }
    else{
        if(ImGui::Button("Start")){
            gameboy->Run();
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Step")){
        gameboy->Step();
    }

    ImGui::End();
}

void debugWindow(){
    debugLog.Draw("Debug Log");
}

void videoWindow(Gameboy *gameboy,GLuint gbtex){
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
}

void statusWindow(Gameboy *gameboy){
    ImGui::Begin("Processor Status");
    ImGui::Text("A: %04X",gameboy->cpu.a);
    ImGui::Text("F: %04X",gameboy->cpu.f);
    ImGui::Text("");
    ImGui::Text("B: %04X",gameboy->cpu.b);
    ImGui::Text("C: %04X",gameboy->cpu.c);
    ImGui::Text("");
    ImGui::Text("D: %04X",gameboy->cpu.d);
    ImGui::Text("E: %04X",gameboy->cpu.e);
    ImGui::Text("");
    ImGui::Text("H: %04X",gameboy->cpu.h);
    ImGui::Text("L: %04X",gameboy->cpu.l);
    ImGui::End();
}

void memoryWindow(Gameboy *gameboy){
    ImGui::SetNextWindowSize(ImVec2(530, 280), ImGuiCond_Once);
    static MemoryEditor ROMBank0_Edit;
    static MemoryEditor ROMBank1_Edit;
    static MemoryEditor VRAM_Edit;
    static MemoryEditor ExternRAM_Edit;
    static MemoryEditor WorkRAM0_Edit;
    static MemoryEditor WorkRAM1_Edit;
    static MemoryEditor EchoRAM_Edit;
    static MemoryEditor OAMRAM_Edit;
    static MemoryEditor HRAM_Edit;
    ImGui::Begin("Memory Edit");
    if(ImGui::BeginTabBar("Memory Editors", 0)){

        if(ImGui::BeginTabItem("ROM Bank 0")){
            ROMBank0_Edit.DrawContents(gameboy->memory.ROMBank0, sizeof(gameboy->memory.ROMBank0));
            ImGui::EndTabItem();
        }
        /*
        if(ImGui::BeginTabItem("ROM Bank 1")){
            ROMBank1_Edit.DrawContents(memory.ROMBank1.data(), sizeof(uint8_t)*memory.ROMBank1.size());
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("VRAM")){
            VRAM_Edit.DrawContents(memory.VRAM.data(), sizeof(uint8_t)*memory.VRAM.size());
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("External RAM")){
            ExternRAM_Edit.DrawContents(memory.ExternRAM.data(), sizeof(uint8_t)*memory.ExternRAM.size());
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Work RAM 0")){
            WorkRAM0_Edit.DrawContents(memory.WorkRAM0.data(), sizeof(uint8_t)*memory.WorkRAM0.size());
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Work RAM 1")){
            WorkRAM1_Edit.DrawContents(memory.WorkRAM1.data(), sizeof(uint8_t)*memory.WorkRAM1.size());
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Echo RAM")){
            EchoRAM_Edit.DrawContents(memory.EchoRAM.data(), sizeof(uint8_t)*memory.EchoRAM.size());
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("OAM RAM")){
            OAMRAM_Edit.DrawContents(memory.OAMRAM.data(), sizeof(uint8_t)*memory.OAMRAM.size());
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("High RAM")){
            HRAM_Edit.DrawContents(memory.HRAM.data(), sizeof(uint8_t)*memory.HRAM.size());
            ImGui::EndTabItem();
        }
        */
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void topMenu(Memory *memory){
    std::string menu_action = "";
        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("File")){
                if (ImGui::MenuItem("Load ROM")){
                    menu_action="loadrom";
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if(menu_action == "loadrom"){
            ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
            ImGui::OpenPopup("LoadROM");
        }
        // CHANGE TO PFD //
        if (ImGui::BeginPopupModal("LoadROM", NULL)){
            ImGui::Text("Choose File: ");
            static char loadBinFilepath[128] = "";
            ImGui::PushItemWidth(400);
            ImGui::InputText("", loadBinFilepath, IM_ARRAYSIZE(loadBinFilepath));
            ImGui::SameLine();
            const bool browseBinButtonPressed = ImGui::Button("...");
            static ImGuiFs::Dialog loadbinFsInstance;
            loadbinFsInstance.chooseFileDialog(browseBinButtonPressed,"./");
            strcpy(loadBinFilepath,loadbinFsInstance.getChosenPath());
            if (strlen(loadBinFilepath)>0) {
            }
           if (ImGui::Button("Open")){
                std::ifstream File;
                File.open(loadBinFilepath);
                File.read((char *)memory->ROMBank0, 0x4000);
                File.close();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
}