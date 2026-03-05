#pragma once

/*
    https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
*/

namespace ANSI
{
    // Regular text
    namespace Reg
    {
        static constexpr auto BLK = "\e[0;30m";
        static constexpr auto RED = "\e[0;31m";
        static constexpr auto GRN = "\e[0;32m";
        static constexpr auto YEL = "\e[0;33m";
        static constexpr auto BLU = "\e[0;34m";
        static constexpr auto MAG = "\e[0;35m";
        static constexpr auto CYN = "\e[0;36m";
        static constexpr auto WHT = "\e[0;37m";
    }

    // Regular bold text
    namespace Bld
    {
        static constexpr auto BLK = "\e[1;30m";
        static constexpr auto RED = "\e[1;31m";
        static constexpr auto GRN = "\e[1;32m";
        static constexpr auto YEL = "\e[1;33m";
        static constexpr auto BLU = "\e[1;34m";
        static constexpr auto MAG = "\e[1;35m";
        static constexpr auto CYN = "\e[1;36m";
        static constexpr auto WHT = "\e[1;37m";
    }

    // Regular underline text
    namespace Und
    {
        static constexpr auto BLK = "\e[4;30m";
        static constexpr auto RED = "\e[4;31m";
        static constexpr auto GRN = "\e[4;32m";
        static constexpr auto YEL = "\e[4;33m";
        static constexpr auto BLU = "\e[4;34m";
        static constexpr auto MAG = "\e[4;35m";
        static constexpr auto CYN = "\e[4;36m";
        static constexpr auto WHT = "\e[4;37m";
    }

    // Regular background
    namespace RegBg
    {
        static constexpr auto BLK = "\e[40m";
        static constexpr auto RED = "\e[41m";
        static constexpr auto GRN = "\e[42m";
        static constexpr auto YEL = "\e[43m";
        static constexpr auto BLU = "\e[44m";
        static constexpr auto MAG = "\e[45m";
        static constexpr auto CYN = "\e[46m";
        static constexpr auto WHT = "\e[47m";
    }

    // High intensty background 
    namespace HiBg
    {
        static constexpr auto BLK = "\e[0;100m";
        static constexpr auto RED = "\e[0;101m";
        static constexpr auto GRN = "\e[0;102m";
        static constexpr auto YEL = "\e[0;103m";
        static constexpr auto BLU = "\e[0;104m";
        static constexpr auto MAG = "\e[0;105m";
        static constexpr auto CYN = "\e[0;106m";
        static constexpr auto WHT = "\e[0;107m";
    }

    // High intensty text
    namespace Hi
    {
        static constexpr auto BLK = "\e[0;90m";
        static constexpr auto RED = "\e[0;91m";
        static constexpr auto GRN = "\e[0;92m";
        static constexpr auto YEL = "\e[0;93m";
        static constexpr auto BLU = "\e[0;94m";
        static constexpr auto MAG = "\e[0;95m";
        static constexpr auto CYN = "\e[0;96m";
        static constexpr auto WHT = "\e[0;97m";
    }

    // Bold high intensity text
    namespace BldHi
    {
        static constexpr auto BLK = "\e[1;90m";
        static constexpr auto RED = "\e[1;91m";
        static constexpr auto GRN = "\e[1;92m";
        static constexpr auto YEL = "\e[1;93m";
        static constexpr auto BLU = "\e[1;94m";
        static constexpr auto MAG = "\e[1;95m";
        static constexpr auto CYN = "\e[1;96m";
        static constexpr auto WHT = "\e[1;97m";
    }


    namespace ALIAS = ANSI::Reg;

    static constexpr auto RESET = "\e[0m";
    static constexpr auto BLK   = ALIAS::BLK;
    static constexpr auto RED   = ALIAS::RED;
    static constexpr auto GRN   = ALIAS::GRN;
    static constexpr auto YEL   = ALIAS::YEL;
    static constexpr auto BLU   = ALIAS::BLU;
    static constexpr auto MAG   = ALIAS::MAG;
    static constexpr auto CYN   = ALIAS::CYN;
    static constexpr auto WHT   = ALIAS::WHT;

}
