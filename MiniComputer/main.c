#include <stdio.h>
#include <stdlib.h>
#include"mem.h"
#include"cpu.h"
#include"help.h"

struct decode decode_list[NUM_INST];
struct helpComands comands[NUM_OF_COMANDS];

char file[30] = {".//teste.ty"}, fileObj[30] = {".//object.ty"};

int main () {
    cpu_t cpu;
    cpu_t *pCpu = &cpu;

    printf("Arquivo carregado: %s\nArquivo objeto carregado: %s\n", file, fileObj);
    init(pCpu);
    creatingDecodeList(decode_list);
    assembleProgram(file, fileObj, decode_list);
    loadProgram(pCpu, ".//object.ty");
    printf("\n");
    run(pCpu);
    dumpCPU(cpu);

    printf("\n");

    initializeComands(comands);
    showComands(comands);

    printf("\n");

    char comand[50];

    while((strcmp(comand, "/exit")) != 0)
    {
        scanf("%s", comand);
        printf("\n");
        executeComand(comands, comand, cpu, pCpu);
    }
    return 0;
}
