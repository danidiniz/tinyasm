#include"help.h"
#include"mem.h"
#include"cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initializeComands(struct helpComands comands[NUM_OF_COMANDS])
{
    comands[0].comandName = "/dump";
    comands[0].comandDescription = "Mostra todos enderecos de memoria.";

    comands[1].comandName = "/initCpu";
    comands[1].comandDescription = "Inicializa a cpu.";

    comands[2].comandName = "/exit";
    comands[2].comandDescription = "Termina a execucao.";

    comands[3].comandName = "/help";
    comands[3].comandDescription = "Mostra todos comandos disponiveis.";

    comands[4].comandName = "/log";
    comands[4].comandDescription = "Mostra o log.";

    comands[5].comandName = "/newFile";
    comands[5].comandDescription = "Carrega um novo arquivo na memoria.";
}

void showComands(struct helpComands comands[NUM_OF_COMANDS])
{
    printf("Lista de comandos:\n");
    int i;
    for(i = 0; i < NUM_OF_COMANDS; i++)
    {
        printf("%s: %s\n", comands[i].comandName, comands[i].comandDescription);
    }
}

void executeComand(struct helpComands comandsList[NUM_OF_COMANDS], char comand[], const cpu_t cpu, cpu_t *pCpu)
{
    int i;
    for(i = 0; i < NUM_OF_COMANDS; i++)
    {
        if (strcmp(comandsList[i].comandName, comand) == 0)
        {
            if((strcmp(comand, "/dump")) == 0)
            {
                dumpCPU(cpu);
            }
            else if((strcmp(comand, "/initCpu")) == 0)
            {
                init(pCpu);
            }
            else if((strcmp(comand, "/exit")) == 0)
            {
                return;
            }
            else if((strcmp(comand, "/help")) == 0)
            {
                showComands(comandsList);
            }
            else if((strcmp(comand, "/log")) == 0)
            {
                FILE *log;
                log = fopen(".//log.txt", "r");
                char linha[500];
                int contador = 0;
                if(log != NULL)
                {
                    while(fgets(linha, 500, log ) != NULL)
                    {
                        printf("%s", linha);
                        contador++;
                    }
                    if(contador == 0)
                    {
                        printf("Para ver o log, feche o programa e veja o arquivo log na pasta.\n");
                    }
                    fclose(log);
                }
            }
            else if((strcmp(comand, "/newFile")) == 0)
            {
                char fileName[50], objectFileName[50];
                printf("Nome do arquivo: ");
                scanf("%s", fileName);
                printf("\nNome do arquivo objeto: ");
                scanf("%s", objectFileName);
                FILE *newFile;
                newFile = fopen(fileName, "r");
                FILE *objectFile;
                objectFile = fopen(objectFileName, "r");
                printf("\n");
                if( (newFile != NULL) && (objectFile != NULL) )
                {
                    resetarLog();
                    init(pCpu);
                    struct decode decode_list[NUM_INST];
                    creatingDecodeList(decode_list);
                    assembleProgram(fileName, objectFileName, decode_list);
                    loadProgram(pCpu, objectFileName);
                    run(pCpu);
                    fclose(newFile);
                    fclose(objectFile);
                }
                else
                {
                    printf("Nao encontrou arquivos.\n");
                }

            }
        }
    }
    printf("\n");
}

void resetarLog()
{
    FILE *log;
    log = fopen(".//log.txt", "rw");
    char linha[500];
    if(log != NULL)
    {
        while(fgets(linha, 500, log ) != NULL)
        {
            fprintf(log, " ");
        }
        fclose(log);
    }

}
