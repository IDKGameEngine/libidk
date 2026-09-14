// #pragma once

// #include "libidk/Types.hpp"


// namespace idk
// {
//     class Command
//     {
//     public:
//         virtual ~Command() = default;
//         virtual void execute() = 0;
//     };


//     class JumpCommand : public Command
//     {
//     public:
//         virtual void execute() override {} // { jump(); }
//     };


//     class FireCommand : public Command
//     {
//     public:
//         virtual void execute() override {} // { fireGun(); }
//     };


//     class InputHandler
//     {
//     public:
//         static constexpr uint8_t BUTTON_X = 0;
//         static constexpr uint8_t BUTTON_Y = 0;
//         static constexpr uint8_t BUTTON_A = 0;
//         static constexpr uint8_t BUTTON_B = 0;

//         bool isPressed(uint8_t) { return true; }

//         Command *handleInput()
//         {
//             if (isPressed(BUTTON_X)) return mButtonX;
//             if (isPressed(BUTTON_Y)) return mButtonY;
//             if (isPressed(BUTTON_A)) return mButtonA;
//             if (isPressed(BUTTON_B)) return mButtonB;
//             return mDefault;
//         }

//         // Methods to bind commands...

//     private:
//         Command* mButtonX;
//         Command* mButtonY;
//         Command* mButtonA;
//         Command* mButtonB;
//         Command* mDefault;
//     };


//     void fsd9f0(InputHandler &h)
//     {
//         Command *cmd = h.handleInput();
//         cmd->execute();
//     }

// }
