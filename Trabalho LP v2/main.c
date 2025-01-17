#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// Define a quantidade inicial de clientes e o tamanho m�ximo do nome
#define QUANTIDADECLIENTES 2
#define TAMNOME 50

// Enumera��o para os tipos de clientes, come�ando em 1 para consist�ncia
typedef enum {
    Regular = 1,
    Esporadico,
    Ocasional
} TipoCliente;

// Estrutura para armazenar informa��es do cliente
typedef struct TCAutomovel {
    int numerocliente;
    char nome[TAMNOME];
    char nif[10];
    float valorcompras;
    TipoCliente tipocliente;
    int datadia, datemes, dataano;
    struct TCAutomovel* proximo; // Ponteiro para o pr�ximo cliente na lista ligada // **Altera��o: Adicionado para lista ligada**
} TCAutomovel;

// Fun��o para verificar se um ano � bissexto
int ebissexto(int ano) {
    return ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0));
}

// Fun��o para validar a data
int datavalida(char* data) {
    int dia, mes, ano;

    // Verificar se os valores de dia, m�s e ano s�o v�lidos
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return 0;
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 2000 || ano > 2026) return 0;

    // Verificar os dias m�ximos para os meses espec�ficos
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;
    if (mes == 2) {
        if (ebissexto(ano) && dia > 29) return 0;
        if (!ebissexto(ano) && dia > 28) return 0;
    }

    return 1;
}

// Fun��o para validar o NIF
int validarNIF(char* nif) {
    if (strlen(nif) != 9) return 0;

    for (int i = 0; i < 9; i++) {
        if (!isdigit(nif[i])) return 0;
    }
    int soma = 0;
    for (int i = 0; i < 8; i++) {
        soma += (nif[i] - '0') * (9 - i);
    }
    int resto = soma % 11;
    int digcontrol = (resto < 2) ? 0 : 11 - resto;

    return digcontrol == (nif[8] - '0');
}

// Fun��o para exibir as informa��es de um cliente
void MostrarCliente(TCAutomovel* cliente) { // **Altera��o: Par�metro agora � ponteiro**
    if (cliente == NULL) return; // **Altera��o: Verifica��o de ponteiro NULL**

    printf("\n------ Cliente %d ------\n", cliente->numerocliente);
    printf("Nome: %s\n", cliente->nome);
    printf("NIF: %s\n", cliente->nif);
    printf("Valor Compras: %.2f\n", cliente->valorcompras);

    char* tipoStr;
    if (cliente->tipocliente == Regular) tipoStr = "Regular";
    else if (cliente->tipocliente == Esporadico) tipoStr = "Espor�dico";
    else tipoStr = "Ocasional";
    printf("Tipo: %s\n", tipoStr);

    printf("Data Registo: %02d/%02d/%04d\n", cliente->datadia, cliente->datemes, cliente->dataano);
    printf("\n------------------------\n");
}

// Fun��o para inserir um novo cliente na lista ligada
void InserirCliente(TCAutomovel** head, int* numClientes) { // **Altera��o: Par�metro agora � ponteiro para ponteiro**
    // Alocar mem�ria para o novo cliente
    TCAutomovel* novoCliente = (TCAutomovel*)malloc(sizeof(TCAutomovel));
    if (!novoCliente) {
        printf("Erro na aloca��o de mem�ria.\n");
        exit(1);
    }

    novoCliente->numerocliente = (*numClientes) + 1;
    novoCliente->proximo = NULL;

    // Limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // **Corre��o: Substitu�do fseek por loop para limpar buffer**

    // Inserir Nome
    printf("Nome: ");
    if (fgets(novoCliente->nome, TAMNOME, stdin) != NULL) { // **Corre��o: Verifica��o do retorno de fgets**
        int len = strlen(novoCliente->nome);
        if (len > 0 && novoCliente->nome[len - 1] == '\n') {
            novoCliente->nome[len - 1] = '\0';
        }
    }

    // Inserir NIF
    int nifvalido = 0;
    while (!nifvalido) {
        printf("NIF: ");
        scanf("%9s", novoCliente->nif); // **Corre��o: Limitar a entrada a 9 caracteres**
        nifvalido = validarNIF(novoCliente->nif);
        if (!nifvalido) {
            printf("\nNIF inv�lido. Insira novamente.\n");
        }
    }

    // Inserir Valor de Compras
    printf("Valor de Compras: ");
    scanf("%f", &novoCliente->valorcompras);

    // Inserir Tipo de Cliente
    int tipodecliente;
    printf("Tipo de Cliente (1 - Regular, 2 - Espor�dico, 3 - Ocasional): ");
    scanf("%d", &tipodecliente);
    while (tipodecliente < 1 || tipodecliente > 3) {
        printf("Valor inv�lido!\nEscolha um tipo de Cliente (1 - Regular, 2 - Espor�dico, 3 - Ocasional): ");
        scanf("%d", &tipodecliente);
    }
    novoCliente->tipocliente = (TipoCliente)tipodecliente;

    // Inserir Data de Registo
    char data[11];
    printf("Data [dd/mm/aaaa]: ");
    scanf("%s", data);
    while (!datavalida(data)) {
        printf("Data inv�lida! Insira novamente com o seguinte formato dd/mm/aaaa: ");
        scanf("%s", data);
    }
    sscanf(data, "%d/%d/%d", &novoCliente->datadia, &novoCliente->datemes, &novoCliente->dataano); // **Corre��o: Substitu�do scanf por sscanf**

    // Inserir no final da lista ligada
    if (*head == NULL) { // **Corre��o: Inserir no in�cio se a lista estiver vazia**
        *head = novoCliente;
    }
    else {
        TCAutomovel* atual = *head;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoCliente;
    }

    (*numClientes)++;

    printf("Cliente inserido com sucesso! Foi-lhe atribu�do o n�mero de cliente %d.\n", novoCliente->numerocliente);
}

// Fun��o para gravar os clientes em um ficheiro bin�rio
void GravarClientes(TCAutomovel* head, const char* filename) { // **Corre��o: Par�metro agora � ponteiro para lista ligada**
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Erro ao abrir o ficheiro.\n");
        return;
    }

    TCAutomovel* atual = head;
    while (atual != NULL) {
        // Temporariamente definir o ponteiro 'proximo' como NULL para evitar gravar endere�os de mem�ria
        TCAutomovel temp = *atual;
        temp.proximo = NULL; // **Corre��o: N�o gravar ponteiros na estrutura**
        fwrite(&temp, sizeof(TCAutomovel), 1, file);
        atual = atual->proximo;
    }

    fclose(file);
    printf("Clientes gravados com sucesso em %s.\n", filename);
}

// Fun��o para mostrar clientes por tipo
void MostrarClientesPorTipo(TCAutomovel* head, TipoCliente tipo) { // **Corre��o: Par�metro agora � ponteiro para lista ligada**
    char* tipoStr;
    if (tipo == Regular) tipoStr = "Regular";
    else if (tipo == Esporadico) tipoStr = "Espor�dico";
    else tipoStr = "Ocasional";

    printf("\nClientes do tipo %s:\n", tipoStr);
    int encontrados = 0;
    TCAutomovel* atual = head;
    while (atual != NULL) {
        if (atual->tipocliente == tipo) {
            MostrarCliente(atual);
            encontrados = 1;
        }
        atual = atual->proximo;
    }
    if (!encontrados) {
        printf("Ainda n�o foram registrados clientes do tipo %s.\n", tipoStr);
    }
}

// Fun��o para listar todos os clientes
void ListarClientes(TCAutomovel* head) { // **Corre��o: Par�metro agora � ponteiro para lista ligada**
    if (head == NULL) {
        printf("N�o existem clientes registados.\n");
        return;
    }
    TCAutomovel* atual = head;
    while (atual != NULL) {
        MostrarCliente(atual);
        atual = atual->proximo;
    }
}

// Fun��o para ler clientes de um ficheiro bin�rio e adicionar � lista ligada
int LerClientes(TCAutomovel** head, const char* filename) { // **Corre��o: Par�metro agora � ponteiro para ponteiro**
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Erro ao abrir o ficheiro.\n");
        return 0;
    }

    int numClientes = 0;
    TCAutomovel temp;

    while (fread(&temp, sizeof(TCAutomovel), 1, file)) {
        // Criar um novo n� com os dados lidos
        TCAutomovel* novoCliente = (TCAutomovel*)malloc(sizeof(TCAutomovel));
        if (!novoCliente) {
            printf("Erro na aloca��o de mem�ria.\n");
            fclose(file);
            exit(1);
        }
        *novoCliente = temp;
        novoCliente->proximo = NULL; // **Corre��o: Definir 'proximo' como NULL**

        // Inserir no final da lista ligada
        if (*head == NULL) {
            *head = novoCliente;
        }
        else {
            TCAutomovel* atual = *head;
            while (atual->proximo != NULL) {
                atual = atual->proximo;
            }
            atual->proximo = novoCliente;
        }

        numClientes++;
    }

    fclose(file);
    printf("Clientes lidos com sucesso de %s.\n", filename);
    return numClientes;
}

// Fun��o para ordenar os clientes por ano de registro (Bubble Sort)
void OrdenarClientes(TCAutomovel** head, int ordem) { // **Corre��o: Par�metro agora � ponteiro para ponteiro**
    if (*head == NULL) {
        printf("N�o existem clientes para ordenar.\n");
        return;
    }

    int trocou;
    TCAutomovel* atual;
    TCAutomovel* ultimo = NULL;

    do {
        trocou = 0;
        atual = *head;

        while (atual->proximo != ultimo) {
            int cmp;
            if (ordem == 1) { // Ordem crescente
                cmp = atual->dataano > atual->proximo->dataano;
            }
            else { // Ordem decrescente
                cmp = atual->dataano < atual->proximo->dataano;
            }

            if (cmp) {
                // Trocar os dados dos clientes
                TCAutomovel temp = *atual;
                *atual = *(atual->proximo);
                *(atual->proximo) = temp;

                // Ajustar os ponteiros ap�s a troca
                TCAutomovel* tempProximo = atual->proximo->proximo;
                atual->proximo->proximo = atual;
                atual->proximo = tempProximo;

                trocou = 1;
            }
            else {
                atual = atual->proximo;
            }
        }
        ultimo = atual;
    } while (trocou);

    printf("Clientes ordenados com sucesso.\n");
}

// Fun��o para calcular e exibir o valor total de compras de cada cliente
void CalcularValorTotalCompras(TCAutomovel* head) { // **Corre��o: Par�metro agora � ponteiro para lista ligada**
    if (head == NULL) {
        printf("Nenhum cliente registrado para calcular o valor total de compras.\n");
        return;
    }

    float valorTotal = 0.0f;
    TCAutomovel* atual = head;
    while (atual != NULL) {
        printf("Cliente: %d - Nome: %s - Valor total de compras: %.2f\n",
               atual->numerocliente,
               atual->nome,
               atual->valorcompras);
        valorTotal += atual->valorcompras;
        atual = atual->proximo;
    }
    printf("\nValor total de compras de todos os clientes: %.2f\n", valorTotal);
}

// Fun��o para remover um cliente por n�mero de registo usando lista ligada
void RemoverCliente(TCAutomovel** head, int* numClientes, int numerocliente) { // **Corre��o: Par�metro agora � ponteiro para ponteiro**
    if (*head == NULL) { // **Corre��o: Verifica��o se a lista est� vazia**
        printf("A lista est� vazia. Nenhum cliente para remover.\n");
        return;
    }

    TCAutomovel* atual = *head;
    TCAutomovel* anterior = NULL;

    // Procurar o cliente a remover
    while (atual != NULL && atual->numerocliente != numerocliente) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) { // **Corre��o: Cliente n�o encontrado**
        printf("O cliente com o n�mero %d n�o foi encontrado.\n", numerocliente);
        return;
    }

    // Remover o cliente encontrado
    if (anterior == NULL) { // **Corre��o: Cliente a remover � o primeiro da lista**
        *head = atual->proximo;
    }
    else {
        anterior->proximo = atual->proximo; // **Corre��o: Atualizar o ponteiro do cliente anterior**
    }

    free(atual);
    (*numClientes)--;

    // Atualizar numerocliente para manter a sequ�ncia
    TCAutomovel* temp = *head;
    int contador = 1;
    while (temp != NULL) {
        temp->numerocliente = contador;
        contador++;
        temp = temp->proximo;
    }

    printf("O cliente com o n�mero %d foi removido com sucesso.\n", numerocliente);
}

// Fun��o para exibir o menu e obter a op��o do usu�rio
int menu() {
    int opcao;
    printf("\n          MENU\n");
    printf("[1] Inserir Informa��o dos Clientes\n");
    printf("[2] Listar todos os Registos de Clientes inseridos\n");
    printf("[3] Listar Clientes por tipo de Cliente\n");
    printf("[4] Gravar Registos de Clientes no Ficheiro\n");
    printf("[5] Remover Clientes por N�mero de Registo\n");
    printf("[6] Ler Registos do Ficheiro e Mostrar\n");
    printf("[7] Ler do Ficheiro, Ordenar, Gravar e Mostrar\n");
    printf("[8] Calcular o Valor Total de Compras de cada cliente\n");
    printf("[0] Sair\n");
    printf(" - Escolha uma op��o: ");
    scanf("%d", &opcao);

    return opcao;
}

// Fun��o principal
int main() {
    setlocale(LC_ALL, "Portuguese"); // **Corre��o: Para utilizar acentos**

    TCAutomovel* head = NULL; // **Corre��o: In�cio da lista ligada**
    int numClientes = 0;
    int opcao;

    do {
        opcao = menu();
        switch (opcao) {
        case 1: {
            // Inserir cliente
            InserirCliente(&head, &numClientes); // **Corre��o: Passar ponteiro para ponteiro**
            break;
        }
        case 2: {
            // Listar todos os clientes
            ListarClientes(head);
            break;
        }
        case 3: {
            // Listar clientes por tipo
            int tipoInput;
            printf("\nEscolha o tipo de Cliente para listar (1 - Regular, 2 - Espor�dico, 3 - Ocasional): ");
            scanf("%d", &tipoInput);
            if (tipoInput < 1 || tipoInput > 3) {
                printf("\nTipo inv�lido! Escolha entre 1, 2 ou 3.\n");
            }
            else {
                MostrarClientesPorTipo(head, (TipoCliente)tipoInput);
            }
            break;
        }
        case 4: {
            // Gravar clientes no ficheiro
            GravarClientes(head, "clientes.bin");
            break;
        }
        case 5: {
            // Remover clientes por n�mero de registo
            if (head == NULL) { // **Corre��o: Verifica��o se a lista est� vazia**
                printf("N�o existem clientes para remover.\n");
                break;
            }
            int numeroCliente;
            printf("Digite o n�mero do cliente a ser removido: ");
            scanf("%d", &numeroCliente);
            RemoverCliente(&head, &numClientes, numeroCliente); // **Corre��o: Passar ponteiro para ponteiro**
            break;
        }
        case 6: {
            // Ler clientes do ficheiro
            int novosClientes = LerClientes(&head, "clientes.bin"); // **Corre��o: Passar ponteiro para ponteiro**
            numClientes += novosClientes;
            ListarClientes(head);
            break;
        }
        case 7: {
            // Ler, ordenar, gravar e mostrar
            int novosClientes = LerClientes(&head, "clientes.bin"); // **Corre��o: Passar ponteiro para ponteiro**
            numClientes += novosClientes;
            if (novosClientes > 0) {
                int ordem;
                printf("Escolha a ordem para ordenar os clientes (1 - Crescente, 2 - Decrescente): ");
                scanf("%d", &ordem);
                while (ordem != 1 && ordem != 2) {
                    printf("Ordem inv�lida! Escolha 1 para Crescente ou 2 para Decrescente: ");
                    scanf("%d", &ordem);
                }
                OrdenarClientes(&head, ordem); // **Corre��o: Passar ponteiro para ponteiro**
                GravarClientes(head, "clientes_ordenados.bin");
                ListarClientes(head);
            }
            break;
        }
        case 8: {
            // Calcular o valor total de compras de cada cliente
            CalcularValorTotalCompras(head);
            break;
        }
        case 0: {
            printf("A sair...\n");
            break;
        }
        default: {
            printf("Op��o inv�lida!\n");
            break;
        }
    } while (opcao != 0);

    // Libera��o da mem�ria restante da lista antes de finalizar
    TCAutomovel* atual = head;
    while (atual != NULL) {
        TCAutomovel* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    return 0;
}
    //a