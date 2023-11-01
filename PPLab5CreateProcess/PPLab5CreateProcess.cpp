#define _CRT_SECURE_NO_DEPRECATE 
#include <stdio.h>
#include <windows.h>

const int imageSize = 1900; //размер файла в байтах
const int coefficient = 1;
const int maxThreads = 8; //кол-во процессов по заданию
const int threadStep = imageSize / maxThreads;

int main()
{
    FILE* original_file;
    FILE* new_file;
    unsigned char f[1900];

    original_file = fopen("C://Users//707//Downloads//corrupted_binary.jpg", "rb");
    fread(f, sizeof(f), 1, original_file);

    HANDLE hProcess[maxThreads];
    STARTUPINFO startInfo[maxThreads];
    PROCESS_INFORMATION processInfo[maxThreads];

    const char* narrowChildProgram = "C://Users//707//source//repos//ChildProgram//x64//Debug//ChildProgram.exe";
    wchar_t wideChildProgram[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, narrowChildProgram, -1, wideChildProgram, MAX_PATH);

    for (int i = 0; i < maxThreads; i++) {
        memset(&startInfo[i], 0, sizeof(STARTUPINFO));
        memset(&processInfo[i], 0, sizeof(PROCESS_INFORMATION));

        if (CreateProcessW(NULL, wideChildProgram, NULL, NULL, FALSE, 0, NULL, NULL, &startInfo[i], &processInfo[i])) {
            hProcess[i] = processInfo[i].hProcess;
            SIZE_T bytesWritten;
            WriteProcessMemory(processInfo[i].hProcess, f, f, sizeof(f), &bytesWritten);
            ResumeThread(processInfo[i].hThread);
        }
    }

    WaitForMultipleObjects(maxThreads, hProcess, TRUE, INFINITE);

    for (int i = 0; i < maxThreads; i++) {
        SIZE_T bytesRead;
        ReadProcessMemory(processInfo[i].hProcess, f, f, sizeof(f), &bytesRead);

        //CloseHandle(hProcess[i]);
        //CloseHandle(processInfo[i].hProcess);
    }

    new_file = fopen("C://Users//707//Downloads//changed_binary_createprocess.jpg", "wb");
    fwrite(f, sizeof(f), 1, new_file);

    fclose(original_file);
    fclose(new_file);

    return 0;
}
