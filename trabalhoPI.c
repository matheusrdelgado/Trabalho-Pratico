#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FUNCIONARIOS 50
#define MAX_REFEICOES 100
#define MAX_ESCOLHAS 100

typedef struct {
    char numero[5];
    char nome[50];
    char nif[15];
    char telefone[15];
} Funcionario;

typedef struct {
    char dia[10];
    char prato_peixe[50];
    int calorias_peixe;
    char prato_carne[50];
    int calorias_carne;
    char prato_dieta[50];
    int calorias_dieta;
    char prato_vegetariano[50];
    int calorias_vegetariano;
} Refeicao;

typedef struct {
    char dia[10];
    char numero_funcionario[5];
    char tipo_prato[2];  // carne (C), peixe (P), dieta (D), ou vegan (V)
} Escolha;

int carregarFuncionarios(const char *ficheiro, Funcionario funcionarios[]);
int carregarEmenta(const char *ficheiro, Refeicao refeicoes[]);
int carregarEscolhas(const char *ficheiro, Escolha escolhas[]);
void mostrarRefeicoesDia(const char *dia, Funcionario funcionarios[], int numFuncionarios, 
                         Escolha escolhas[], int numEscolhas, Refeicao refeicoes[], int numRefeicoes);
void listarUtentesDespesas(Funcionario funcionarios[], int numFuncionarios, Escolha escolhas[], int numEscolhas);
int inserirFuncionario(const char *ficheiro, Funcionario funcionarios[], int maxFuncionarios);
int mostrarEmenta(const char *ficheiro, Refeicao refeicoes[]);


int carregarFuncionarios(const char *ficheiro, Funcionario funcionarios[]) {
    FILE *file = fopen("C:\\Users\\mathe\\CODE\\Programacao Imperativa\\Trabalho Pratico\\funcionarios.csv", "r");
    if (!file) {
        printf("Erro ao abrir o ficheiro %s\n", ficheiro);
        return 0;
    }

    char linha[100];
    int count = 0;
    while (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "%[^;];%[^;];%[^;];%[^\n]", 
               funcionarios[count].numero, 
               funcionarios[count].nome, 
               funcionarios[count].nif, 
               funcionarios[count].telefone);
        count++;
    }
    fclose(file);
    return count;
}

int inserirFuncionario(const char *ficheiro, Funcionario funcionarios[], int maxFuncionarios) {
    int nrFuncionario = carregarFuncionarios(ficheiro, funcionarios);
    if(nrFuncionario < 0){
        printf("\n Numero invalido! Tente novamente, por favor.");
        return -1;
    }

    int nrFuncionario;
    printf("\nIntroduza o numero do funcionario que pretende inserir:\n");
    scanf("%d", nrFuncionario);

    for (int i = 0; i < nrFuncionario; i++){ //Verifica se ja existe
        if(atoi(funcionarios[i].numero == nrFuncionario)){
        printf("Numero de funcionario ja registado!\n");
        return -1;
        }
    }

    printf("Introduza o nome do funcionario:\n");
    scanf(" %[^;]", funcionarios[nrFuncionario].nome);
    printf("Introduza o NIF do funcionario:\n");
    scanf(" %[^;]", funcionarios[nrFuncionario].nif);
    printf("Introduza o telefone do funcionario:\n");
    scanf(" %[^;]", funcionarios[nrFuncionario].telefone);

    sprinf(funcionarios[nrFuncionario].numero, "%d", nrFuncionario); //Guardar como string
    nrFuncionario++;

    //Guarda os dados no ficheiro
    FILE *file = fopen(ficheiro, "w");
    if(!file){
        printf("Erro ao guardar o funcionario no ficheiro.\n");
        return -1;
    }
    for (int i = 0; i < nrFuncionario; i++){
        fprintf(file, "%s;%s;%s;%s\n", funcionarios[i].numero,
        funcionarios[i].nome,
        funcionarios[i].nif,
        funcionarios[i].telefone);
    }
    
    fclose(file);
    printf("Funcionario inserido com sucesso!\n");
    return 0;
    
}


int carregarEmenta(const char *ficheiro, Refeicao refeicoes[]) {
    FILE *file = fopen("C:\\Users\\mathe\\CODE\\Programacao Imperativa\\Trabalho Pratico\\Ementa.txt", "r");
    if (!file) {
        printf("Erro ao abrir o ficheiro %s\n", ficheiro);
        return 0;
    }

    char linha[200];
    int count = 0;
    while (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "%[^;];%[^;];%d;%[^;];%d;%[^;];%d;%[^;];%d",
               refeicoes[count].dia, 
               refeicoes[count].prato_peixe, &refeicoes[count].calorias_peixe,
               refeicoes[count].prato_carne, &refeicoes[count].calorias_carne,
               refeicoes[count].prato_dieta, &refeicoes[count].calorias_dieta,
               refeicoes[count].prato_vegetariano, &refeicoes[count].calorias_vegetariano);
        count++;
    }
    fclose(file);
    return count;
}

int mostrarEmenta(const char *ficheiro, Refeicao refeicoes[]){
    FILE *file = fopen(ficheiro, "r");
    if(!file){
        printf("Erro ao abrir o ficheiro %s\n", ficheiro);
        return 0;
    }
    char linha[300];
    int count = 0;
    while (fgets(linha, sizeof(linha), file)){
        sscanf(linha, "%[^;]; %[^;]; %[^;]; %d; %[^;]; %d; %[^;]; %d; %[^;]; %d",
               refeicoes[count].dia,
               refeicoes[count].prato_peixe, &refeicoes[count].calorias_peixe,
               refeicoes[count].prato_carne, &refeicoes[count].calorias_carne,
               refeicoes[count].prato_dieta, &refeicoes[count].calorias_dieta,
               refeicoes[count].prato_vegetariano, &refeicoes[count].calorias_vegetariano);
        count++;
    }
    fclose(file);
    return count;

}

int carregarEscolhas(const char *ficheiro, Escolha escolhas[]) {
    FILE *file = fopen("C:\\Users\\mathe\\CODE\\Programacao Imperativa\\Trabalho Pratico\\escolhas.txt", "r");
    if (!file) {
        printf("Erro ao abrir o ficheiro %s\n", ficheiro);
        return 0;
    }

    char linha[50];
    int count = 0;
    while (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "%[^;];%[^;];%[^\n]", 
               escolhas[count].dia, 
               escolhas[count].numero_funcionario, 
               escolhas[count].tipo_prato);
        count++;
    }
    fclose(file);
    return count;
}

void mostrarRefeicoesDia(const char *dia, Funcionario funcionarios[], int numFuncionarios, 
                         Escolha escolhas[], int numEscolhas, Refeicao refeicoes[], int numRefeicoes) {
    printf("Refeições para o dia %s:\n", dia);

    for (int i = 0; i < numEscolhas; i++) {
        if (strcmp(escolhas[i].dia, dia) == 0) {
            // Procura o funcionário
            char nomeFuncionario[50] = "Desconhecido";
            for (int j = 0; j < numFuncionarios; j++) {
                if (strcmp(funcionarios[j].numero, escolhas[i].numero_funcionario) == 0) {
                    strcpy(nomeFuncionario, funcionarios[j].nome);
                    break;
                }
            }

            // Procurar prato correspondente
            char prato[50] = "Desconhecido";
            for (int k = 0; k < numRefeicoes; k++) {
                if (strcmp(refeicoes[k].dia, dia) == 0) {
                    if (strcmp(escolhas[i].tipo_prato, "P") == 0) strcpy(prato, refeicoes[k].prato_peixe);
                    else if (strcmp(escolhas[i].tipo_prato, "C") == 0) strcpy(prato, refeicoes[k].prato_carne);
                    else if (strcmp(escolhas[i].tipo_prato, "D") == 0) strcpy(prato, refeicoes[k].prato_dieta);
                    else if (strcmp(escolhas[i].tipo_prato, "V") == 0) strcpy(prato, refeicoes[k].prato_vegetariano);
                }
            }

            // Exibir resultado
            printf("Funcionário: %s, Nome: %s, Prato: %s\n", escolhas[i].numero_funcionario, nomeFuncionario, prato);
        }
    }
}

// Listagem de utentes ordenados por número de funcionário e cálculo da despesa
void listarUtentesDespesas(Funcionario funcionarios[], int numFuncionarios, Escolha escolhas[], int numEscolhas) {
    int refeicoesServidas[MAX_FUNCIONARIOS] = {0};
    float precoPorRefeicao = 6.0;

    // Contar refeições por funcionário
    for (int i = 0; i < numEscolhas; i++) {
        for (int j = 0; j < numFuncionarios; j++) {
            if (strcmp(escolhas[i].numero_funcionario, funcionarios[j].numero) == 0) {
                refeicoesServidas[j]++;
            }
        }
    }

    printf("Listagem de utentes e despesas:\n");
    for (int i = numFuncionarios - 1; i >= 0; i--) {
        printf("%s, %s: %d refeições, Total: %.2f\n",
               funcionarios[i].numero, funcionarios[i].nome,
               refeicoesServidas[i], refeicoesServidas[i] * precoPorRefeicao);
    }
}