#include "vm.hpp"
#include <cstdio>
#include <cstdlib>


int main( int argc, char **argv )
{
    if (argc != 2)
    {
        printf("Usage: vvm [FILE]\n");
        return 1;
    }

    FILE *fh = fopen(argv[1], "r");
    if (!fh)
    {
        printf("Cannot open file \"%s\"\n", argv[1]);
        return 1;
    }

    fseek(fh, 0, SEEK_END);
    size_t size = ftell(fh);
    auto *buf = new uint8_t[size];

    fseek(fh, 0, SEEK_SET);
    size_t res = fread(buf, 1, size, fh);
    fclose(fh);

    vanevm::VmCtx ctx;
    ctx.execute((uint32_t*)buf, size);

    return 0;
}
