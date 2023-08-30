#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "../include/List.h"

char* read_file_into_string(const char* source_path){
    FILE* file = fopen(source_path, "rb"); // If this isn't in binary mode, windows line endings will find you... and give you buffer overflows
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* raw_string = malloc((file_size + 1) * sizeof(char)); // +1 for \0 at the end
    raw_string[file_size] = '\0';
    fread(raw_string, sizeof(char), file_size, file);

    fclose(file); // If this fails... welp... whatever

    return raw_string;
}

typedef uint8_t Byte;
typedef size_t USize;

GENERATE_LIST_FOR_TYPE(Byte);

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '<' || c == '>' || c == '[' || c == ']' || c == ',' || c == '.');
}

bool validateProgram(char* program) {
    int32_t numOpenBrackets = 0;

    for (size_t i = 0; program[i]; ++i) {
        char c = program[i];
        if (c == '[') ++numOpenBrackets;
        else if (c == ']') --numOpenBrackets;

        if (numOpenBrackets < 0) {
            printf("ERROR: Unmatched closing bracket\n");
            return false;
        }
    }

    if (numOpenBrackets != 0) {
        printf("ERROR: Unmatched opening bracket\n");
        return false;
    }

    return true;
}

void run(char* program) {
    ByteList memory = NEW_LIST_OF_TYPE(Byte);
    ByteList_push(&memory, 0);
    int64_t memPointer = 0;

    char c;

    for (size_t programCounter = 0; (c = program[programCounter]); ++programCounter) {
        if (!isOperator(c)) continue;
        switch (c) {
            case '+':{
                ++memory.content[memPointer];
            } break;

            case '-':{
                --memory.content[memPointer];
            } break;

            case '.':{
                putchar(memory.content[memPointer]);
            } break;

            case ',':{
                memory.content[memPointer] = getchar();
            } break;

            case '<':{
                --memPointer;
                assert(memPointer >= 0 && "Negative memory adresses not implemented yet");
            } break;

            case '>':{
                ++memPointer;
                if (memPointer == memory.size) {
                    ByteList_push(&memory, 0);
                }
            } break;

            case '[':{
                if (memory.content[memPointer] == 0) {
                    int32_t numOpenBrackets = 1;
                    while (numOpenBrackets != 0) {
                        ++programCounter;
                        if (program[programCounter] == '[') ++numOpenBrackets;
                        else if (program[programCounter] == ']') --numOpenBrackets;
                    }
                }
            } break;

            case ']':{
                if (memory.content[memPointer] != 0) {
                    int32_t numClosedBrackets = 1;
                    while (numClosedBrackets != 0) {
                        --programCounter;
                        if (program[programCounter] == ']') ++numClosedBrackets;
                        else if (program[programCounter] == '[') --numClosedBrackets;
                    }
                }
            } break;
        }
    }
}

int main(int argc, char** argv) {

    // Skip the program argument
    argv++;

    if (!*argv) {
        printf("ERROR: No program provided as an argument!\n");
        printf("Try: brainfuck \"<your program>\"\n");
        return 1;
    }

    char* program = read_file_into_string(*argv);

    if (!program) {
        fprintf(stderr, u8"ERROR: Could not open file \"%s\": %s\n", *argv, strerror(errno));
        return 1;
    }

    if (!validateProgram(program)) {
        fprintf(stderr, u8"ERROR: Not a valid brainfuck program");
        return 2;
    }

    run(program);

    return 0;
}
