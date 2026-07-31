#include "vm.hpp"
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static FILE *infile, *outfile;

const char *isopname( const char *s )
{
    using namespace vanevm;

    // edge case: strncmp("u32pushX", "u32push", strlen(u32push)) == 0
    // #define X(Nm) printf("%s,%d\n",  #Nm, strncmp(s, #Nm, strlen(#Nm))); if (strncmp(s, #Nm, strlen(#Nm) == 0)) return #Nm;
    #define X(Nm) if (strncmp(s, #Nm, strlen(#Nm)) == 0) return #Nm;
    VVM_OPNAMES
    #undef X

    return nullptr;
}

uint32_t getop( const char *opname )
{
    using namespace vanevm;

    #define X(Nm) if (strncmp(opname, #Nm, strlen(#Nm)) == 0) return OP_##Nm;
    VVM_OPNAMES
    #undef X

    return 0;
}


void lexer( const char *src )
{
    while (*src)
    {
        if (*src == ' ' || *src == '\n')
        {
            src++;
        }

        if (const char *name = isopname(src))
        {
            uint32_t op = getop(name);
            fwrite(&op, sizeof(op), 1, outfile);
            src += strlen(name);
        }
    }
}


int main( int argc, char **argv )
{
    if (argc != 3)
    {
        printf("Usage: vasm [INFILE] [OUTFILE]\n");
        return 1;
    }

    const char *inpath = argv[1];
    const char *outpath = argv[2];

    if (!(infile = fopen(inpath, "r")))
    {
        printf("Cannot open file \"%s\"\n", inpath);
        return 1;
    }

    if (!(outfile = fopen(outpath, "w")))
    {
        printf("Cannot open file \"%s\"\n", outpath);
        return 1;
    }

    fseek(infile, 0, SEEK_END);
    size_t size = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    char *buf = new char[size];
    size_t res = fread(buf, 1, size, infile);
    fclose(infile);

    lexer(buf);
    fclose(outfile);

    return 0;
}


