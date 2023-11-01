#define _CRT_SECURE_NO_DEPRECATE 
#include <stdio.h>
#include <windows.h>

const int imageSize = 1900;
const int coefficient = 1;
const int maxThreads = 8;
const int threadStep = imageSize / maxThreads;

void change_bytes(int start, int end, unsigned char buffer[]) {
    for (int i = start; i > end; i -= threadStep) {
        buffer[i] += (i * coefficient) & 255;
        printf("%d ", (int)buffer[i]);
    }
}

void program() {
    unsigned char f[1900];
    SIZE_T bytesRead;
    ReadProcessMemory(GetCurrentProcess(), f, f, sizeof(f), &bytesRead);
    change_bytes(imageSize - GetCurrentProcessId(), 0, f);
    SIZE_T bytesWritten;
    WriteProcessMemory(GetCurrentProcess(), f, f, sizeof(f), &bytesWritten);
}
