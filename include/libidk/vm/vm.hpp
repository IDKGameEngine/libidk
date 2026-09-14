#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stack>

namespace vanevm
{
    #define VVM_OPNAMES \
        X(NONE) \
        X(NOP) \
        \
        X(u32add) \
        X(u32sub) \
        X(u32mul) \
        X(u32div) \
        X(f32add) \
        X(f32sub) \
        X(f32mul) \
        X(f32div) \
        \
        X(u32push) \
        X(u32pushA) \
        X(u32pushX) \
        X(u32pushY) \
        \
        X(u32pop) \
        X(u32popA) \
        X(u32popX) \
        X(u32popY) \
        \
        X(u32stA) \
        X(u32stX) \
        X(u32stY) \
        \
        X(u32writeA) \
        X(u32writeX) \
        X(u32writeY) \
        \
        X(u32print) \
        X(u32printA) \
        X(u32printX) \
        X(u32printY) \
        \
        X(i32print) \
        X(i32printA) \
        X(i32printX) \
        X(i32printY) \
        \
        X(f32print) \
        X(f32printA) \
        X(f32printX) \
        X(f32printY) \
        \
        X(strprintX) \
        \
        X(jmp) \
        X(call) \
        X(ret) \
        X(exit)

    enum OP_: uint8_t
    {
        #define X(Nm) OP_##Nm,
        VVM_OPNAMES
        #undef X
    };

    class VmCtx;
}


class vanevm::VmCtx
{
public:
    VmCtx() {  }
    void execute( uint32_t*, size_t );

private:
    template <typename T>
    struct VmStack: public std::stack<T>
    {
        T pop()
        {
            assert((!std::stack<T>::empty()));
            T x=std::stack<T>::top();
            std::stack<T>::pop();
            return x;
        }
    };

    struct StackFrame
    {
        uint32_t ip;
        uint32_t sp;
        // uint32_t *prev;
    };

    uint32_t IP, SP, AC, AX, AY;
    uint32_t *vstk = new uint32_t[1024];
    VmStack<StackFrame> cstk; // call stack
    
};