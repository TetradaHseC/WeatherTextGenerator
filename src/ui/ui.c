#include <printf.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ui.h"
#include "../../res/strings.h"

#define scase(case_) if (strcmp(SWITCH_STRING_ARGUMENT, case_) == 0)

// region console width
int console_width =
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    #include <sys/ioctl.h>
    #include <stdio.h>
    #include <unistd.h>

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    w.ws_col;
#elif defined(Q_OS_MSDOS)
    #include <windows.h>

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    80;
#endif
// endregion

char *inputFile = NULL;
char *outputFile = NULL;

enum Commands { InputCommand, OutputCommand, InputRecentCommand, OutputRecentCommand, StartCommand, ExitCommand, NotACommand };

enum Commands GetCommand();

void PrintInfo();

void EnterFilepath(char **dest);

void OnNewRecent(char *recentStorage, char *newRecent);

void OnGetRecent(char *recentStorage);

void UploadRecent();

void OnInputCommand() {
    EnterFilepath(&inputFile);
    OnNewRecent("input_recent", inputFile);
}

void OnOutputCommand() {
    EnterFilepath(&outputFile);
    OnNewRecent("output_recent", outputFile);
}

void OnInputRecentCommand() {

}

void OnOutputRecentCommand() {

}

void OnNotACommand() {
    printf("Такой команды нет, загляните в инфо сверху\n");
}

int Start() {
    bool isRunning = true;

    UploadRecent();

    while (isRunning) {
        system(
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
        "clear"
#elif defined(Q_OS_MSDOS)
        "cls"
#else
#define NO_CLEAR
        ""
#endif
        );
#ifdef NO_CLEAR
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
#endif

        PrintInfo();

        switch (GetCommand()) {
            case InputCommand:
                OnInputCommand();
                break;
            case OutputCommand:
                OnOutputCommand();
                break;
            case StartCommand:
                isRunning = false;
                break;
            case ExitCommand:
                free(inputFile);
                free(outputFile);
                exit(0);
            default:
                OnNotACommand();
                break;
        }
    }

    return 0;
}

void PrintInfo() {
    printf(ui_info_text,
           inputFile == NULL ? "None" : inputFile,
           outputFile == NULL ? "None" : outputFile);
}

enum Commands GetCommand() {
    char command[5] = { 0 };
    scanf("%s", command);

#define SWITCH_STRING_ARGUMENT command
    scase("i")
        return InputCommand;
    scase("o")
        return OutputCommand;
    scase("ri")
        return InputRecentCommand;
    scase("ro")
        return OutputRecentCommand;
    scase("s")
        return StartCommand;
    scase("exit")
        return ExitCommand;
#undef SWITCH_STRING_ARGUMENT

    return NotACommand;
}

void EnterFilepath(char **dest) {
    printf("Введите название файла(с путём до него, конечно):\n  "); // TODO: res + locals
    char temp[500];
    fflush(stdin);
    gets(temp);
    free(*dest);
    *dest = calloc(strlen(temp) + 1, sizeof(char));
    sprintf(*dest, "%s", temp);
}

void OnNewRecent(char *recentStorage, char *newRecent) {
    char *fullPath = calloc(strlen(recentStorage) + strlen("../res/") + 1, sizeof(char));
    strcat(fullPath, "../res/");
    strcat(fullPath, recentStorage);
    FILE *storage = fopen(fullPath, "w");
    free(fullPath);

    fprintf(storage, "%s\n", newRecent);
    fclose(storage);
}

void UploadRecent() {
    // region input
    char fullPath[22] = "../res/input_recent";
    FILE *storage = fopen(fullPath, "r");

    inputFile = calloc(200, sizeof(char));
    fgets(inputFile, 200, storage);
    if (strlen(inputFile) > 0) {
        if (inputFile[strlen(inputFile) - 1] == '\n')
            inputFile[strlen(inputFile) - 1] = 0;
    } else
        sprintf(inputFile, "None");
    fclose(storage);
    // endregion

    // region output
    sprintf(fullPath, "%s", "../res/output_recent");
    storage = fopen(fullPath, "r");

    outputFile = calloc(200, sizeof(char));
    fgets(outputFile, 200, storage);
    if (strlen(outputFile) > 0) {
        if (outputFile[strlen(outputFile)-1] == '\n')
        outputFile[strlen(outputFile) - 1] = 0;
    }else
        sprintf(outputFile, "None");
    fclose(storage);
    // endregion
}
