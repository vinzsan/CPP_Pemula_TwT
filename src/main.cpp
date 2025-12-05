#include <iostream>
#include <memory>
#include "../state/scene.hpp"

int main(){
    auto state = std::make_unique<GameState>();
    state->run_state();
    return 0;
}