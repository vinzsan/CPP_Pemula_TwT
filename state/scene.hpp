#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <atomic>

#include "../state/assets.hpp"
#include "../state/audio_mixer.hpp"

#define WIDTH 800
#define HEIGHT 600

class GameState : public AssetsState {
private:

    TextScene scene;
    int width,height;

public:

    GameState(){
        InitWindow(WIDTH,HEIGHT,"Simple window");
        SetTargetFPS(60);
        
        this->load_font();
        counter.store(true,std::memory_order_relaxed);
    }

    void run_state(){
        while(!WindowShouldClose() && RuntimeState::counter.load(std::memory_order_relaxed)){
            BeginDrawing();
            ClearBackground(BLACK);
            this->update();
            this->event();
            EndDrawing();
        }
    }

    void update(){
        this->width = GetScreenWidth();
        this->height = GetScreenHeight();
        switch(scene){
            case TextScene::Scene1: {
                auto &text1 = AssetsState::text_scene[Scene1];
                if(this->scene < text1.size()){
                    for(int i = 0;i < text1.size();i++){
                        DrawTextEx(
                            this->font,text1[i].c_str(),
                            Vector2 { 50,static_cast<float>(100 + 20 * i) },
                            20,0.10,
                            this->color_text_scene[Scene1][i]
                        );
                    }
                }
                break;
            }
            case TextScene::Scene2: {
                auto& text1 = AssetsState::text_scene[Scene2];
                for(int i = 0;i < text1.size();i++){
                        DrawTextEx(
                            this->font,text1[i].c_str(),
                            Vector2 { 50,static_cast<float>(100 + 20 * i) },
                            20,0.10,
                            this->color_text_scene[Scene2][i]
                        );
                    }
                break;
            }
            default:
                break;
        }
    }

    void event(){
        // global events
        switch(GetKeyPressed()){
            case KEY_Q:
                RuntimeState::counter.store(false,std::memory_order_relaxed);
                break;
            case KEY_ONE:
                this->scene = TextScene::Scene1;
                for(int i = 0;i < this->color_text_scene[this->scene].size();i++){
                    this->reset_alpha(this->scene, i);
                } // Reset jika saya pindah scene
                break;
            case KEY_TWO:
                this->scene = TextScene::Scene2;
                for(int i = 0;i < this->color_text_scene[this->scene].size();i++){
                    this->reset_alpha(this->scene, i);
                } // Reset jika saya pindah scene
                break;
            case KEY_R:
                this->reset_alpha(this->scene,0); // ------------------------------
                break;
            case KEY_THREE:
                this->text_at_iter++;
                if(this->text_at_iter >= this->color_text_scene[this->scene].size()){
                    this->text_at_iter = 0;
                }
                break;
            case KEY_FOUR:
                if(this->text_at_iter == 0){
                    this->text_at_iter = this->color_text_scene[this->scene].size() - 1;
                } else {
                    this->text_at_iter--;
                }
                break;
            default:
                break;
        }
        
        // event for colors

        if(IsKeyDown(KeyboardKey::KEY_W)){
            this->add_alpha(this->scene,this->text_at_iter,5);
        }
        if(IsKeyDown(KeyboardKey::KEY_E)){
            this->sub_alpha(this->scene,this->text_at_iter,5);
        }
    }

    ~GameState(){
        // 
    }
};