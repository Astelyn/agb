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

#ifndef MMU_H
#define MMU_H

#include <stdint.h>

typedef struct {
    uint8_t* ram;
} mmu_t;

/* Read 1 byte of memory from a given address */
uint8_t read_8(mmu_t* mmu, uint16_t addr);

/* Read 2 bytes of memory from a given address */
uint16_t read_16(mmu_t* mmu, uint16_t addr);

/* Write 1 byte of memory to a given address */
void write_8(mmu_t* mmu, uint16_t addr, uint8_t val);

/* Write 2 bytes of memory to a given address */
void write_16(mmu_t* mmu, uint16_t addr, uint16_t val);

#endif

