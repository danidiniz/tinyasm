#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"mem.h"
#include"cpu.h"
#define SIZE_OF_INST 30

void creatingDecodeList(struct decode decode_list[NUM_INST]) {
    decode_list[0].inst_name = "INVALID";
    decode_list[0].opcode = 0x0;

    decode_list[1].inst_name = "READ";
    decode_list[1].opcode = 0x1;

    decode_list[2].inst_name = "WRITE";
    decode_list[2].opcode = 0x2;

    decode_list[3].inst_name = "LOAD";
    decode_list[3].opcode = 0x3;

    decode_list[4].inst_name = "STORE";
    decode_list[4].opcode = 0x4;

    decode_list[5].inst_name = "ADD";
    decode_list[5].opcode = 0x5;

    decode_list[6].inst_name = "SUB";
    decode_list[6].opcode = 0x6;

    decode_list[7].inst_name = "MUL";
    decode_list[7].opcode = 0x7;

    decode_list[8].inst_name = "DIV";
    decode_list[8].opcode = 0x8;

    decode_list[9].inst_name = "MOD";
    decode_list[9].opcode = 0x9;

    decode_list[10].inst_name = "BRANCH";
    decode_list[10].opcode = 0xa;

    decode_list[11].inst_name = "BRANCHNEG";
    decode_list[11].opcode = 0Xb;

    decode_list[12].inst_name = "BRANCHZERO";
    decode_list[12].opcode = 0xc;

    decode_list[13].inst_name = "HALT";
    decode_list[13].opcode = 0xd;
}

/* retorna um opcode, isto e, uma palavra de memoria com a instrucao e o operando */
word_t getOpcodeCPU(const cpu_t cpu){
   return cpu.mem;
};

/* Inicia o funcionamento da CPU, zera o contador de programa e limpa a memoria (e zera o acc tb)*/
void init(cpu_t *cpu) {
    printf("O acumulador, contador de programa e memoria foram zerados.\n");
    cpu->acc = 0;
    cpu->pc = 0; //zera o contador
    clearMem(cpu->mem); //limpa a memória
};

/* Roda um programa previamente carregado na
memoria da CPU*/
void run(cpu_t *cpu) {
    /* Criando um arquivo texto pra salvar os printf */
    FILE *log;
    log = fopen(".//log.txt", "w");

    int i;
    for (i=0; i<256; i++) {
        cpu->pc = cpu->mem[i+1];
        int ir = cpu->mem[i];
        int instructionToLoad = 0;
        int *pInstructionToLoad = &instructionToLoad;
        int memPosition = 0;
        int *pMemPosition = &memPosition;

//        printf("Linha %d\n", i);
        separatingOpcode(ir, pInstructionToLoad, pMemPosition);
//        printf("----------Apos o metodo separatingOpcode:----------\n");
//        printf("instructionToLoad: %d, memPosition: %d\n\n", instructionToLoad, memPosition);
        fprintf(log, "Lendo endereco de memoria %d\nOpcode: %08x | Instrucao: %s | ", i, cpu->mem[i], decode_list[instructionToLoad]);
        fprintf(log, "Endereco de memoria: %d\n", memPosition); /* não faço a minima fucking ideia do porque ele printa a memposition errada se eu coloco essa linha no fprintf de cima... só sei que desse jeito funciona */
//        printf("Lendo linha %d do arquivo .ty\nOpcode: %08x | Instrucao: %s | ", i, cpu->mem[i], decode_list[instructionToLoad]);
//        printf("Endereco de memoria: %d\n", memPosition);

        switch(instructionToLoad) {
        case INVALID:
            fprintf(log, "Desculpe, sua instrucao eh invalida.\n");
            break;
        case READ:
            printf("Insira o valor a ser colocado no acc: ");
            scanf("%d", &cpu->acc);
            printf("\n");
            fprintf(log, "Valor atual do acc: %d\n", cpu->acc);
            break;
        case WRITE:
            fprintf(log, "Valor atual no acc: %d\n",cpu->acc);
            break;
        case LOAD:
            cpu->acc = cpu->mem[memPosition];
            fprintf(log, "Valor carregado pro acc: %d\n", cpu->acc);
            break;
        case STORE:
            cpu->mem[memPosition] = cpu->acc;
            fprintf(log, "Salvou o acc(%d) no endereco de memoria %d.\n", cpu->acc, memPosition);
            break;
        case ADD:
            fprintf(log, "Adicionou acc(%d) + endereco %d(%d) = %d\n",cpu->acc, memPosition, cpu->mem[memPosition], (cpu->acc + cpu->mem[memPosition]));
            cpu->acc = (cpu->acc) + (cpu->mem[memPosition]);
            break;
        case SUB:
            fprintf(log, "Subtraiu acc(%d) - endereco %d(%d) = %d\n",cpu->acc, memPosition, cpu->mem[memPosition], (cpu->acc - cpu->mem[memPosition]));
            cpu->acc = (cpu->acc) - (cpu->mem[memPosition]);
            break;
        case MUL:
            fprintf(log, "Multiplicou acc(%d) * endereco %d(%d) = %d\n",cpu->acc, memPosition, cpu->mem[memPosition], (cpu->acc * cpu->mem[memPosition]));
            cpu->acc = (cpu->acc) * (cpu->mem[memPosition]);
            break;
        case DIV:
            fprintf(log, "Dividiu acc(%d) / endereco %d(%d) = %d\n",cpu->acc, memPosition, cpu->mem[memPosition], (cpu->acc / cpu->mem[memPosition]));
            cpu->acc = (cpu->acc) / (cpu->mem[memPosition]);
            break;
        case MOD:
            fprintf(log, "MOD acc(%d) % endereco %d(%d) = %d\n",cpu->acc, memPosition, cpu->mem[memPosition], (cpu->acc % cpu->mem[memPosition]));
            cpu->acc = (cpu->acc) % (cpu->mem[memPosition]);
            break;
        case BRANCH:
            i = memPosition;
            i--; /* -1 por causa do i++ no final.. */
            fprintf(log, "Fluxo transferido para linha %d do arquivo.\n", (memPosition + 1) );
            break;
        case BRANCHNEG:
            if (cpu->acc < 0){
                i = memPosition;
                i--; /* -1 por causa do i++ no final.. */
                fprintf(log, "Fluxo transferido para linha %d do arquivo.\n", (memPosition + 1) );
            }
            break;
        case BRANCHZERO:
            if (cpu->acc == 0){
                i = memPosition;
                i--; /* -1 por causa do i++ no final.. */
                fprintf(log, "Fluxo transferido para linha %d do arquivo.\n", (memPosition + 1) );
            }
            break;
        case HALT:
            fprintf(log, "Seu programa terminou.\n");
            return;
            //exitCPU(cpu);
            break;
        }
       fprintf(log, "\n");
    }

    fclose(log);
};

void exec(cpu_t *cpu);

/* Sai da cpu */
void exitCPU(const cpu_t cpu);

/* Mostra o conteudo da CPU */
void dumpCPU(const cpu_t  cpu) {
    printf("Memoria:\n");
    dumpMem(cpu.mem);
    printf("Acumulador: %d\n", cpu.acc);
    printf("Contador de programa: %d\n", cpu.pc);
};



int creatingOpcode(int instruct, int ender) {
    int primDigt = 0x00;
    int tercDigt = 0x00;
    unsigned int result = (primDigt<<32) | (instruct<<16) | (tercDigt<<8) | ender;
    return result;
}
void separatingOpcode(int opcode, int *instructSep, int *enderSep){
//    printf("----------Metodo separatingOpcode:----------\nOpcode: %08x\nParametro instructSep: %d\nParametro passado enderSep: %d\n", opcode, *instructSep, *enderSep);
    int number = opcode;
    *enderSep = number & 0xff;
    int tercDigtSep = (number >> 8) & 0xff;
    *instructSep = (number >> 16) & 0xff;
    int primDigtSep = (number >> 32) & 0xff;
//    printf("----------Fim do metodo separatingOpcode:----------\nOpcode: %08x\ninstructSep: %d\nenderSep: %d\n", opcode, *instructSep, *enderSep);
}


/* Le um arquivo com um programa em linguagem de montagem e compila gerando um codigo objeto*/
void assembleProgram(char *src_file_name, char *obj_file_name, struct decode decode_list[NUM_INST]){
    FILE *src;
    src = fopen(src_file_name, "r");
    FILE *obj;
    obj = fopen(obj_file_name, "w");

    if( (src != NULL) && (obj != NULL) )
    {
    char line[SIZE_OF_INST];
    char instruc[SIZE_OF_INST];
    int instruc_decoded = 0;

    int endereco = 0;
    int *pEndereco = &endereco;


    int temChar = 0;
    int *pTemChar = &temChar;

    int temNumero = 0;
    int *pTemNumero = &temNumero;

    int contador = 0;
    int *pContador = &contador;

    resetarArrayDaLinha(line);
    resetarArrayDaLinha(instruc);

    while(fgets( line, SIZE_OF_INST, src )!= NULL)
    {
        /* Metodo para verificar se lina possui CHAR, NUMERO, OU OS DOIS */
        verificarSeTemCharENumero(line, pContador, pTemChar, pTemNumero);

        int j;
        /* Se tiver CHAR e NUMERO */
        if( (temChar == 1) && (temNumero == 1) )
        {
            separarCharNumero(line, instruc, pEndereco);

//            printf("Instruct apos o metodo: %s\n", instruc);
            for (j = 0; j < NUM_INST; j++)
            {
                if ( (strcmp(decode_list[j].inst_name, instruc)) == 0)
                {
//                    printf("Decode list[%d]: %s == Instruction %s\n", j, decode_list[j].inst_name, instruc);
                    instruc_decoded = decode_list[j].opcode;
                    j = NUM_INST;
                }
            }
            int instructionOpCode_ready = creatingOpcode(instruc_decoded, endereco);
            fprintf(obj, "%08x\n", instructionOpCode_ready);
        }
        /* Se tiver apenas CHAR */
        else if( (temChar == 1) && (temNumero == 0) )
        {
            *pEndereco = 0;
            separarCharNumero(line, instruc, pEndereco);

            for (j = 0; j < NUM_INST; j++)
            {
                if (strcmp(decode_list[j].inst_name, instruc) == 0)
                {
//                    printf("Decode list[%d]: %s == Instruction %s\n", j, decode_list[j].inst_name, instruc);
                    instruc_decoded = decode_list[j].opcode;
                    j = NUM_INST;
                }
            }
            int instructionOpCode_ready = creatingOpcode(instruc_decoded, endereco);
            fprintf(obj, "%08x\n", instructionOpCode_ready);
        }
        /* Se tiver apenas NUMERO */
        else if( (temChar == 0) && (temNumero == 1) )
        {
            separarCharNumero(line, '---', pEndereco);

            int valorSozinho = 0;
            int instructionOpCode_ready = creatingOpcode(valorSozinho, endereco);
            fprintf(obj, "%08x\n", instructionOpCode_ready);
        }

        /* Reseta valores antes de ler a proxima linha */
        temChar = 0;
        temNumero = 0;
        contador++;
        resetarArrayDaLinha(line);
        resetarArrayDaLinha(instruc);
    }   /* Fim do while */
    fclose(src);
    fclose(obj);
    }
    else
    {
        printf("\nErro ao tentar carregar arquivo.\n");
    }
};

/* Carrega em memoria os bytes de um programa executável*/
void loadProgram(cpu_t *cpu, char *file_name){
    FILE *f_exec = fopen(file_name,"r");
    if(f_exec != NULL)
    {
        int opcodeExtracted = 0;
        int contador = 0;

        while( fscanf(f_exec, "%08x", &opcodeExtracted) != EOF )
        {
            cpu->mem[contador] = opcodeExtracted;
            contador++;
        }
        fclose(f_exec);
    }
    else
    {
        printf("\nErro ao tentar carregar %s.\n", file_name);
    }
};

void separarCharNumero(char line[SIZE_OF_INST], char instruct[], int *ende)
{
    int contadorDeLetras = 0;
    int i;
    for(i = 0; i < SIZE_OF_INST; i++)
    {
        /* Letra */
        if( (line[i] >= 65) && (line[i] <= 90) )
        {
            instruct[i] = line[i];
            contadorDeLetras++;
        }
        /* Se o caractere for NUMERO */
        else if( (line[i] >= 48) && (line[i] <= 57) )
        {
            *ende = line[i] - '0'; /* Primeiro algarismo */
            int pos_primeiro_algarismos = i;
            int pos_ultimo_algarismos;

            /* Caso o numero tenha mais de 1 algarismo */
            int j, contadorDeAlgarismos = 1;
            for(j = i+1; j < SIZE_OF_INST; j++)
            {
                if( (line[j] >= 48) && (line[j] <= 57) )
                {
                    contadorDeAlgarismos++;
                }
                else
                {
                    pos_ultimo_algarismos = j-1;
                    j = SIZE_OF_INST;
                }
                i++;
            }
            int k;
            for(k = pos_primeiro_algarismos+1; k <= pos_ultimo_algarismos; k++)
            {
                *ende = ((*ende)*10) + (line[k] - '0');
            }
//            printf("\nQuantidade de algarismos: %d | Numero: %d\n", contadorDeAlgarismos, *ende);
        }
        else if( line[i] == 32 )
        {

        }
        else
        {
//            printf("Nao ha mais caracteres.\n\n");
            i = SIZE_OF_INST;
        }
    }
    /*
    printf("Array instruct: ");
    int a = 0;
    for(a = 0; a < contadorDeLetras; a++)
    {
        printf(" %c ", instruct[a]);
    }
    printf("\n");
    printf("Fim da verificacao.\nInstrucao: %s\nEndereco: %d\n", instruct, *ende); */
}

void verificarSeTemCharENumero(char line[NUM_INST], int *contador, int *temChar, int *temNumero)
{
//    printf("\n---------------------\n");
//    printf("\nVerificando linha %d: %s", *contador, line);
    int i;
    for(i = 0; i < SIZE_OF_INST; i++)
    {
//              printf("%c(%d), ", line[i], line[i]);
        /* Se o caractere for CHAR */
        if( (line[i] >= 65) && (line[i] <= 90) )
        {
            *temChar = 1;
        }
        /* Se o caractere for NUMERO */
        else if( (line[i] >= 48) && (line[i] <= 57) )
        {
            *temNumero = 1;
        }
        /* Se o caractere for ESPACO */
        else if( line[i] == 32)
        {
        }
        /* Se nao for nenhum dos 3, sai do for e le a proxima a linha */
        else
        {
            i = SIZE_OF_INST;
        }
    }
}

void resetarArrayDaLinha(char line[SIZE_OF_INST])
{
    int i = 0;
    for(i = 0; i < SIZE_OF_INST; i++)
    {
        line[i] = NULL;
    }
}
