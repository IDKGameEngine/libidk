#pragma once

#ifdef __GNUC__
    #define idk_sin __builtin_sin
    #define idk_sinh __builtin_sinh
    #define idk_asin __builtin_asin
    #define idk_cos __builtin_cos
    #define idk_cosh __builtin_cosh
    #define idk_acos __builtin_acos
    #define idk_tan __builtin_tan
    #define idk_tanh __builtin_tanh
    #define idk_atan __builtin_atan
    #define idk_atan2 __builtin_atan2
    #define idk_abs __builtin_abs
    #define idk_fabs __builtin_fabs
    #define idk_labs __builtin_labs
    #define idk_ceil __builtin_ceil
    #define idk_floor __builtin_floor
    #define idk_exp __builtin_exp
    #define idk_ldexp __builtin_ldexp
    #define idk_pow __builtin_pow
    #define idk_sqrt __builtin_sqrt
    #define idk_log __builtin_log
    #define idk_log10 __builtin_log10
    #define idk_fmod __builtin_fmod
    #define idk_modf __builtin_modf
#else
    #include <cmath>
    #define idk_sin sin
    #define idk_sinh sinh
    #define idk_asin asin
    #define idk_cos cos
    #define idk_cosh cosh
    #define idk_acos acos
    #define idk_tan tan
    #define idk_tanh tanh
    #define idk_atan atan
    #define idk_atan2 atan2
    #define idk_abs abs
    #define idk_fabs fabs
    #define idk_labs labs
    #define idk_ceil ceil
    #define idk_floor floor
    #define idk_exp exp
    #define idk_ldexp ldexp
    #define idk_pow pow
    #define idk_sqrt sqrt
    #define idk_log log
    #define idk_log10 log10
    #define idk_fmod fmod
    #define idk_modf modf
#endif
