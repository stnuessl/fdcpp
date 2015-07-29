/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Steffen Nuessle
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cstring>
#include <iostream>

#include <fdcpp/easy/mmap.hpp>

// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <stdio.h>
// 
// int c()
// {
//     int fd, err;
//     char *addr;
//     size_t size = 128;
//     
//     fd = open("/tmp/mmap.txt", O_CREAT | O_RDWR, 0644);
//     if (fd < 0) {
//         perror("open");
//         goto fail;
//     }
//     
//     err = ftruncate(fd, size);
//     if (err < 0) {
//         perror("ftruncate");
//         goto fail;
//     }
//     
//     addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//     if (addr == MAP_FAILED) {
//         perror("mmap");
//         goto fail;
//     }
//     
//     close(fd);
//     
//     std::strncpy(addr, "Hello, World!\n", size);
//     
//     munmap(addr, size);
//     exit(EXIT_SUCCESS);
//     
// fail:
//     if (fd >= 0)
//         close(fd);
//     exit(EXIT_FAILURE);
// }

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    auto file = fd::file("/tmp/easy_mmap_test.txt", O_CREAT | O_RDWR, 0644);
    file.ftruncate(128);
    
    auto mmap = fd::easy::mmap(file, PROT_READ | PROT_WRITE, MAP_SHARED);
    
    std::strncpy(mmap, "Hello, World!\n", mmap.size());
    
    std::cout << "Ok\n";
    
    return 0;
}