#include "vm.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>

template <typename OutType, typename InType>
OutType vvmcast( const InType &in )
{
    union UType { InType i; OutType o; };
    return (UType{in}).o;
}

constexpr uint32_t vvmalignd( uint32_t x, uint32_t a )
{ return x & ~(a-1); }

constexpr uint32_t vvmalignu( uint32_t x, uint32_t a )
{ return (x + a-1) & ~(a-1); }

void vanevm::VmCtx::execute( uint32_t *prg, size_t prgsize )
{
    IP=0, SP=0, AC=0, AX=0, AY=0;

    // char msgstr[] = "\nMichael\n\0\0";
    // auto *msgbuf = (uint32_t*)msgstr;

    // uint32_t prg[] = {
    //     OP_u32push,     vvmcast<uint32_t>(1.0f/3.0f),
    //     OP_u32push,     vvmcast<uint32_t>(5.105f),
    //     OP_f32mul,
    //     OP_u32popX,
    //     OP_f32printX,
    //     OP_u32stX,      (uint32_t)512,
    //     OP_u32writeX,   msgbuf[0],      // mem[AX++] = \nMic
    //     OP_u32writeX,   msgbuf[1],      // mem[AX++] = hael
    //     OP_u32writeX,   msgbuf[2],      // mem[AX++] = \n\0\0\0
    //     OP_strprintX,   (uint32_t)512,
    //     OP_EXIT
    // };
    // FILE *fh = fopen("rom.vvm", "w");
    // fwrite(prg, 1, sizeof(prg), fh);
    // fclose(fh);

    uint32_t *mem = new uint32_t[1024];
    std::memset(mem, 0, 1024*sizeof(uint32_t));
    std::memcpy(mem, prg, prgsize);

    void *jtab[] = {
        #define X(Nm) &&jtab_##Nm,
        VVM_OPNAMES
        #undef X
    };
    #define DISPATCH() goto *jtab[mem[IP++]]
    #define FETCH() mem[IP++]

    #define FCST(Idx) vvmcast<float, uint32_t>(vstk[Idx])
    #define UCST(Val) vvmcast<uint32_t, float>(Val)
    #define BINARY_OP(Op)  { SP--; vstk[SP] = vstk[SP-1] Op vstk[SP-0]; }
    #define BINARY_FOP(Op) { SP--; vstk[SP] = UCST(FCST(SP-1) Op FCST(SP-0)); }

    DISPATCH();

jtab_NONE: printf("Invalid opcode (%u)\n", mem[IP-1]); return;
jtab_NOP:                   DISPATCH();
jtab_u32add: BINARY_OP(+);  DISPATCH();
jtab_u32sub: BINARY_OP(-);  DISPATCH();
jtab_u32mul: BINARY_OP(*);  DISPATCH();
jtab_u32div: BINARY_OP(/);  DISPATCH();
jtab_f32add: BINARY_FOP(+); DISPATCH();
jtab_f32sub: BINARY_FOP(-); DISPATCH();
jtab_f32mul: BINARY_FOP(*); DISPATCH();
jtab_f32div: BINARY_FOP(/); DISPATCH();

#define OPDEF_NAXY(Nm) \
    jtab_##Nm:    {FnDef(mem[IP++])} DISPATCH(); \
    jtab_##Nm##A: {FnDef(AC)} DISPATCH(); \
    jtab_##Nm##X: {FnDef(AX)} DISPATCH(); \
    jtab_##Nm##Y: {FnDef(AY)} DISPATCH();

#define OPDEF_AXY(Nm) \
    jtab_##Nm##A: {FnDef(AC)} DISPATCH(); \
    jtab_##Nm##X: {FnDef(AX)} DISPATCH(); \
    jtab_##Nm##Y: {FnDef(AY)} DISPATCH();

// PUSH
#define FnDef(Rg) vstk[SP++] = Rg;
OPDEF_NAXY(u32push);
#undef FnDef

// POP
#define FnDef(Rg) Rg = vstk[SP++];
OPDEF_NAXY(u32pop);
#undef FnDef

// ST{A,X,Y}
#define FnDef(Rg) Rg = FETCH();
OPDEF_AXY(u32st);
#undef FnDef

// WT{A,X,Y}
#define FnDef(Rg) mem[Rg++] = FETCH();
OPDEF_AXY(u32write);
#undef FnDef

#define FnDef(Rg) printf("%u", vvmcast<uint32_t>(Rg));
OPDEF_NAXY(u32print);
#undef FnDef

#define FnDef(Rg) printf("%d", vvmcast<int32_t>(Rg));
OPDEF_NAXY(i32print);
#undef FnDef

#define FnDef(Rg) printf("%f", vvmcast<float>(Rg));
OPDEF_NAXY(f32print);
#undef FnDef

// print string starting at mem[dword]
jtab_strprintX: printf("%s", (const char*)(mem + FETCH())); DISPATCH();


jtab_jmp:
    IP = mem[IP];
    DISPATCH();

jtab_call:
    cstk.push({IP+1, SP});
    IP = mem[IP];
    SP = vvmalignu(SP, 2);
    DISPATCH();

jtab_ret:
    auto [ip, sp] = cstk.pop();
    IP = ip;
    SP = sp;
    DISPATCH();

jtab_exit:
    return;

}

