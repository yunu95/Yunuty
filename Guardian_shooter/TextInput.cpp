#include "TextInput.h"

unordered_set<gs_map::TextInput*> gs_map::TextInput::textInputs;

void gs_map::TextInput::Update()
{
    if (isTyping)
    {
        if (D2DInput::isKeyPushed(KeyCode::A)) typedInput.push_back('a');
        if (D2DInput::isKeyPushed(KeyCode::B)) typedInput.push_back('b');
        if (D2DInput::isKeyPushed(KeyCode::C)) typedInput.push_back('c');
        if (D2DInput::isKeyPushed(KeyCode::D)) typedInput.push_back('d');
        if (D2DInput::isKeyPushed(KeyCode::E)) typedInput.push_back('e');
        if (D2DInput::isKeyPushed(KeyCode::F)) typedInput.push_back('f');
        if (D2DInput::isKeyPushed(KeyCode::G)) typedInput.push_back('g');
        if (D2DInput::isKeyPushed(KeyCode::H)) typedInput.push_back('h');
        if (D2DInput::isKeyPushed(KeyCode::I)) typedInput.push_back('i');
        if (D2DInput::isKeyPushed(KeyCode::J)) typedInput.push_back('j');
        if (D2DInput::isKeyPushed(KeyCode::K)) typedInput.push_back('k');
        if (D2DInput::isKeyPushed(KeyCode::L)) typedInput.push_back('l');
        if (D2DInput::isKeyPushed(KeyCode::M)) typedInput.push_back('m');
        if (D2DInput::isKeyPushed(KeyCode::N)) typedInput.push_back('n');
        if (D2DInput::isKeyPushed(KeyCode::O)) typedInput.push_back('o');
        if (D2DInput::isKeyPushed(KeyCode::P)) typedInput.push_back('p');
        if (D2DInput::isKeyPushed(KeyCode::Q)) typedInput.push_back('q');
        if (D2DInput::isKeyPushed(KeyCode::R)) typedInput.push_back('r');
        if (D2DInput::isKeyPushed(KeyCode::S)) typedInput.push_back('s');
        if (D2DInput::isKeyPushed(KeyCode::T)) typedInput.push_back('t');
        if (D2DInput::isKeyPushed(KeyCode::U)) typedInput.push_back('u');
        if (D2DInput::isKeyPushed(KeyCode::V)) typedInput.push_back('v');
        if (D2DInput::isKeyPushed(KeyCode::W)) typedInput.push_back('w');
        if (D2DInput::isKeyPushed(KeyCode::X)) typedInput.push_back('x');
        if (D2DInput::isKeyPushed(KeyCode::Y)) typedInput.push_back('y');
        if (D2DInput::isKeyPushed(KeyCode::Z)) typedInput.push_back('z');
        if (D2DInput::isKeyPushed(KeyCode::NUM_0)) typedInput.push_back('0');
        if (D2DInput::isKeyPushed(KeyCode::NUM_1)) typedInput.push_back('1');
        if (D2DInput::isKeyPushed(KeyCode::NUM_2)) typedInput.push_back('2');
        if (D2DInput::isKeyPushed(KeyCode::NUM_3)) typedInput.push_back('3');
        if (D2DInput::isKeyPushed(KeyCode::NUM_4)) typedInput.push_back('4');
        if (D2DInput::isKeyPushed(KeyCode::NUM_5)) typedInput.push_back('5');
        if (D2DInput::isKeyPushed(KeyCode::NUM_6)) typedInput.push_back('6');
        if (D2DInput::isKeyPushed(KeyCode::NUM_7)) typedInput.push_back('7');
        if (D2DInput::isKeyPushed(KeyCode::NUM_8)) typedInput.push_back('8');
        if (D2DInput::isKeyPushed(KeyCode::NUM_9)) typedInput.push_back('9');
        if (D2DInput::isKeyPushed(KeyCode::Perido))
            typedInput.push_back('.');
        if (D2DInput::isKeyPushed(KeyCode::Slash))
            typedInput.push_back('/');
        if (D2DInput::isKeyPushed(KeyCode::BackSlash))
            typedInput.push_back('\\');
        if (D2DInput::isKeyPushed(KeyCode::Minus) && (D2DInput::isKeyDown(KeyCode::LShift) || D2DInput::isKeyDown(KeyCode::RShift)))
            typedInput.push_back('_');


        if (D2DInput::isKeyPushed(KeyCode::Backspace) && !typedInput.empty())
            typedInput.pop_back();
    }
}
