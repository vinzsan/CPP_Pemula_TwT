#include <raylib.h>
#include <raymath.h>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <atomic>

enum TextScene{
    Scene1,
    Scene2,
};

class RuntimeState{
protected:

    std::atomic<bool> counter;
    int text_at_iter;

public:

    
};

class AssetsState : public RuntimeState{
protected:

    std::unordered_map<TextScene,std::vector<std::string>> text_scene;
    std::unordered_map<TextScene,std::vector<Color>> color_text_scene;
    Font font;

public:

    AssetsState(){
        std::cout << "INFO : Assets called" << std::endl;

        // init langsung

        //std::vector<std::string> buffer_text_str;

        const char *str[] = {
            "Hello?.. (signature?)",
            "Who are you?",
            "Dont tell me you re traveler?"
        };

        const char *str2[] = {
            "What is that?",
            "i dont thinks so that was working?,isnt??..",
            "So i dont use that things actually."
        };

        for(auto &i : str){
            text_scene[TextScene::Scene1].push_back(i);
            color_text_scene[TextScene::Scene1].push_back(Color{255,255,255,(unsigned char)255});
        }

        for(auto &i : str2){
            text_scene[TextScene::Scene2].push_back(i);
            color_text_scene[TextScene::Scene2].push_back(Color { 255,255,255,(unsigned char)255});
        }
    }
    
    void add_alpha(TextScene scene,int text_at,int add_size){
        auto &size = this->color_text_scene[scene][text_at].a;
        if((int)size >= 255) {
            size = 255;
        } else {
            size += add_size;
        }
    }

    void sub_alpha(TextScene scene,int text_at,int sub_size){
        auto &size = this->color_text_scene[scene][text_at].a;
        if((int)size > sub_size){
            size -= sub_size;
        }
    }

    void reset_alpha(TextScene scene,int text_at){
        auto &text = this->color_text_scene[scene][text_at].a;
        text = 255;
    }

    void load_font(){
        font = LoadFont("./assets/JetBrainsMonoNerdFont-Regular.ttf");
        if(font.texture.id == 0){
            std::cerr << "ERROR : failed to open file" << std::endl;
            throw std::runtime_error("ERROR : failed to load font");
        }
        SetTextureFilter(font.texture,TEXTURE_FILTER_BILINEAR);
    }

    ~AssetsState(){
        if(font.texture.id) UnloadFont(this->font);
    }
};