/*

Copyright 2013 Allie Saia <allie@fcraft.net>
    https://github.com/astelyn

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "mmu.h"

uint8_t read_8(mmu_t* mmu, uint16_t addr)
{
#ifdef _DEBUG
    return mmu->ram[addr];
#else
    /* TODO */
    return 0;
#endif
}

uint16_t read_16(mmu_t* mmu, uint16_t addr)
{
#ifdef _BIG_ENDIAN
    return read_8(mmu, addr + 1) + (read_8(mmu, addr) << 8);
#else
    return read_8(mmu, addr) + (read_8(mmu, addr + 1) << 8);
#endif
}

void write_8(mmu_t* mmu, uint16_t addr, uint8_t val)
{
#ifdef _DEBUG
    mmu->ram[addr] = val;
#else
    /* TODO */
    return 0;
#endif
}

void write_16(mmu_t* mmu, uint16_t addr, uint16_t val)
{
#ifdef _BIG_ENDIAN
    write_8(mmu, addr + 1, val & 0xFF);
    write_8(mmu, addr, (val >> 8) & 0xFF);
#else
    write_8(mmu, addr, val & 0xFF);
    write_8(mmu, addr + 1, (val >> 8) & 0xFF);
#endif
}

