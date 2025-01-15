#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define QUANTIDADECLIENTES 2
#define TAMNOME 50

typedef enum {
    Regular,
    Esporadico,
    Ocasional
} TipoCliente;

typedef struct {
    int numerocliente;
    char nome[TAMNOME];
    int nif;
    float valorcompras;
    TipoCliente tipocliente;
    int datadia, datames, dataano;
} TCAutomovel;

// Função para verificar se um ano é bissexto
int ebissexto(int ano) {
    return ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0));
}

// Função para validar a data
int datavalida(char* data) {
    int dia, mes, ano;
    
    // Verificar se os valores de dia, mês e ano são válidos
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return 0;
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 2000 || ano > 2026) return 0;

    // Verificar os dias máximos para os meses específicos
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;
    if (mes == 2) {
        if (ebissexto(ano) && dia > 29) return 0;
        if (!ebissexto(ano) && dia > 28) return 0;
    }

    return 1;
}

void InsereCliente(TCAutomovel* lista, int* numClientes) {
    // Alocar memória para o novo cliente
    TCAutomovel novoCliente;
    novoCliente.numerocliente = *numClientes + 1;

    // Ler os dados do novo cliente
    fseek(stdin, 0, SEEK_END); // limpar buffer do scanf

    printf("Nome: ");
    fgets(novoCliente.nome, TAMNOME, stdin);
    int len = strlen(novoCliente.nome);
    if (len > 0 && novoCliente.nome[len + 1] == '\n') {
        novoCliente.nome[len - 1] = '\0';
    }

    printf("NIF: ");
    scanf("%d", &novoCliente.nif);

    printf("Valor de Compras: ");
    scanf("%f", &novoCliente.valorcompras);

    // Tipo de cliente
    int tipodecliente;
    printf("Tipo de Cliente (1 - Regular, 2 - Esporadico, 3 - Ocasional): ");
    scanf("%d", &tipodecliente);
        while (tipodecliente < 1 || tipodecliente >3) {
            printf("Valor invalido!\nEscolha um tipo de Cliente (1 - Regular, 2 - Esporadico, 3 - Ocasional):");
            scanf("%d", &tipodecliente);
        }

    if (tipodecliente == 1) novoCliente.tipocliente = Regular;
    else if (tipodecliente == 2) novoCliente.tipocliente = Esporadico;
    else novoCliente.tipocliente = Ocasional;

    // Ler e validar data (dia/mes/ano)
    char data[11];
    printf("Data [dd/mm/aaaa]: ");
    scanf("%s", data);
    while (!datavalida(data)) {
        printf("Data inválida! Por favor, insira novamente [dd/mm/aaaa]: ");
        scanf("%s", data);
    }
    sscanf(data, "%d/%d/%d", &novoCliente.datadia, &novoCliente.datames, &novoCliente.dataano);

    // Adicionar o novo cliente ao vetor
    lista[*numClientes] = novoCliente;
    (*numClientes)++;
    printf("Cliente inserido com sucesso! (num = %d)\n", novoCliente.numerocliente);
}

void GravarClientes(TCAutomovel* lista, int numClientes, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Erro ao abrir o ficheiro.\n");
        return;
    }
    fwrite(lista, sizeof(TCAutomovel), numClientes, file);
    fclose(file);
    printf("Clientes gravados com sucesso em %s.\n", filename);
}

void MostrarCliente(TCAutomovel cliente) {
    printf("\n------ Cliente %d ------\n", cliente.numerocliente);
    printf("Nome: %s\n", cliente.nome);
    printf("NIF: %d\n", cliente.nif);
    printf("Valor Compras: %.2f\n", cliente.valorcompras);

    char* tipoStr;
    if (cliente.tipocliente == Regular) tipoStr = "Regular"; //ver isto, colocar o do outro codigo
    else if (cliente.tipocliente == Esporadico) tipoStr = "Esporadico";
    else tipoStr = "Ocasional";
    printf("Tipo: %s\n", tipoStr);

    printf("Data Registo: %02d/%02d/%04d\n", cliente.datadia, cliente.datames, cliente.dataano);
    printf("\n------------------------\n");
}

void ListarClientes(TCAutomovel* lista, int numClientes) {
    if (numClientes == 0) {
        printf("Nao existem clientes registados.\n");
        return;
    }
    for (int i = 0; i < numClientes; i++) {
        MostrarCliente(lista[i]);
    }
}

int LerClientes(TCAutomovel* lista, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Erro ao abrir o ficheiro.\n");
        return 0;
    }
    int numClientes = 0;
    while (fread(&lista[numClientes], sizeof(TCAutomovel), 1, file)) {
        numClientes++;
    }
    fclose(file);
    printf("Clientes lidos com sucesso de %s.\n", filename);
    return numClientes;
}

void OrdenarClientes(TCAutomovel* lista, int numClientes, int ordem) {
    for (int i = 0; i < numClientes - 1; i++) {
        for (int j = i + 1; j < numClientes; j++) {
            int cmp = ordem == 1 ?
                (lista[i].dataano > lista[j].dataano) :
                (lista[i].dataano < lista[j].dataano);
            if (cmp) {
                TCAutomovel temp = lista[i];
                lista[i] = lista[j];
                lista[j] = temp;
            }
        }
    }
    printf("Clientes ordenados com sucesso.\n");
}

int menu() {
    int opcao;
    printf("\n          MENU\n");
    printf("[1] Inserir informacaoo dos clientes\n");
    printf("[2] Listar todos os registos de clientes inseridos\n");
    printf("[3] Listar Clientes por tipo de cliente\n");
    printf("[4] Gravar registos de clientes no ficheiro\n");
    printf("[5] Remover clientes por numero de registo\n");
    printf("[6] Ler registos do ficheiro e mostrar\n");
    printf("[7] Ler do ficheiro, ordenar, gravar e mostrar\n");
    printf("[8] Calcular o valor total de compras de cada cliente\n");
    printf("[0] Sair\n");
    printf(" - Escolha uma opcao: ");
    scanf("%d", &opcao);

    return opcao;
}

int main() {
    TCAutomovel* lista = malloc(QUANTIDADECLIENTES * sizeof(TCAutomovel)); // Supondo um máximo de 100 clientes
    int numClientes = 0;
    int opcao;

    do {
        opcao = menu();
        switch (opcao) {
        case 1:
            // Inserir cliente
            InsereCliente(lista, &numClientes);
            break;
        case 2:
            // Listar clientes
            ListarClientes(lista, numClientes);
            break;
        case 4:
            // Gravar clientes no ficheiro
            GravarClientes(lista, numClientes, "clientes.bin");
            break;
        case 6:
            // Ler clientes do ficheiro
            numClientes = LerClientes(lista, "clientes.bin");
            ListarClientes(lista, numClientes);
            break;
        case 7:
            // Ler, ordenar, gravar e mostrar
            numClientes = LerClientes(lista, "clientes.bin");
            OrdenarClientes(lista, numClientes, 1); // 1 para ordem crescente
            GravarClientes(lista, numClientes, "clientes_ordenados.bin");
            ListarClientes(lista, numClientes);
            break;
        case 0:
            printf("A sair...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }

        if (numClientes % QUANTIDADECLIENTES == 0 && numClientes != 0) {
            lista = realloc(lista, (numClientes + QUANTIDADECLIENTES) * sizeof(TCAutomovel)); //n da, acho q o problema esta na lista
        }
    } while (opcao != 0);

    free(lista);
    return 0;
}


