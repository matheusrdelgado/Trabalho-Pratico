#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FUNCIONARIOS 50
#define MAX_REFEICOES 100
#define MAX_ESCOLHAS 100

typedef struct {
    char numero[5];
    char nome[156];
    char nif[15];
    char telefone[15];
} Funcionario;

typedef struct {
    char dia[10];
    char data[20];
    char prato_peixe[100];
    int calorias_peixe;
    char prato_carne[100];
    int calorias_carne;
    char prato_dieta[100];
    int calorias_dieta;
    char prato_vegan[100];
    int calorias_vegan;
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
int inserirFuncionario(const char *ficheiro, Funcionario funcionarios[]);
int mostrarEmenta(const char *ficheiro, Refeicao refeicoes[]);
void removerFuncionario(const char *ficheiro);
void registarEscolha(const char *ficheiro, Escolha escolhas[], int *numEscolhas, int maxEscolhas);
void editarFuncionario(const char *ficheiro, Funcionario funcionarios[]);
void mostrarFuncionarios(const char *ficheiro, Funcionario funcionarios[]);

int main() {
    Funcionario funcionarios[MAX_FUNCIONARIOS];
    Refeicao refeicoes[MAX_REFEICOES];
    Escolha escolhas[MAX_ESCOLHAS];
    
    int numFuncionarios = 0;
    int numRefeicoes = 0;
    int numEscolhas = 0;
    
    char ficheiroFuncionarios[] = "C:\\Users\\mathe\\CODE\\Programacao Imperativa\\Trabalho Pratico\\funcionarios.csv";
    char ficheiroEmenta[] = "C:\\Users\\mathe\\CODE\\Programacao Imperativa\\Trabalho Pratico\\Ementa.txt";
    char ficheiroEscolhas[] = "C:\\Users\\mathe\\CODE\\Programacao Imperativa\\Trabalho Pratico\\escolhas.txt";

    char opcao;
    do {
        printf("\nMenu:\n");
        printf("1 - Mostrar todos os funcionarios registrados\n");
        printf("2 - Inserir novo funcionario\n");
        printf("3 - Mostrar ementa semanal\n");
        printf("4 - Mostrar escolhas dos funcionarios\n");
        printf("5 - Mostrar refeicoes de um dia\n");
        printf("6 - Listar utentes e despesas\n");
        printf("7 - Remover funcionario\n");
        printf("8 - Registar escolha de refeicao\n");
        printf("9 - Editar dados de funcionario\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf(" %c", &opcao);
        getchar();

        switch (opcao) {
            case '1':
                mostrarFuncionarios(ficheiroFuncionarios, funcionarios);
                break;

            case '2':
                if (inserirFuncionario(ficheiroFuncionarios, funcionarios) == 0) {
                    numFuncionarios++;
                }
                break;

            case '3':
                numRefeicoes = carregarEmenta(ficheiroEmenta, refeicoes);
                if (numRefeicoes > 0) {
                    printf("Ementa semanal:\n");
                    for (int i = 0; i < numRefeicoes; i++) {
                        printf("%s, %s: Peixe: %s (%d cal), Carne: %s (%d cal), Dieta: %s (%d cal), Vegan: %s (%d cal)\n",
                               refeicoes[i].dia, refeicoes[i].data, refeicoes[i].prato_peixe, refeicoes[i].calorias_peixe,
                               refeicoes[i].prato_carne, refeicoes[i].calorias_carne,
                               refeicoes[i].prato_dieta, refeicoes[i].calorias_dieta,
                               refeicoes[i].prato_vegan, refeicoes[i].calorias_vegan);
                    }
                } else {
                    printf("Nenhuma ementa encontrada.\n");
                }
                break;

            case '4':
                numEscolhas = carregarEscolhas(ficheiroEscolhas, escolhas);
                printf("%d escolhas carregadas com sucesso.\n", numEscolhas);
                break;

            case '5': {
                char dia[10];
                printf("Digite o dia (ex: Segunda): ");
                scanf(" %s", dia);
                getchar();
                mostrarRefeicoesDia(dia, funcionarios, numFuncionarios, escolhas, numEscolhas, refeicoes, numRefeicoes);
                break;
            }

            case '6':
                listarUtentesDespesas(funcionarios, numFuncionarios, escolhas, numEscolhas);
                break;

            case '7':
                removerFuncionario(ficheiroFuncionarios);
                break;

            case '8':
                registarEscolha(ficheiroEscolhas, escolhas, &numEscolhas, MAX_ESCOLHAS);
                break;

            case '9':
                editarFuncionario(ficheiroFuncionarios, funcionarios);
                break;

            case '0':
                printf("Saindo do programa.\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != '0');

    return 0;
}

void removerFuncionario(const char *ficheiro) {
    char numero[5];
    Funcionario funcionarios[MAX_FUNCIONARIOS];
    int numFuncionarios = carregarFuncionarios(ficheiro, funcionarios);
    printf("Digite o numero do funcionario a ser removido: ");
    scanf("%s", numero);

    int indice = -1;
    for (int i = 0; i < numFuncionarios; i++) {
        if (strcmp(funcionarios[i].numero, numero) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Funcionario nao encontrado.\n");
        return;
    }

    // Usando memmove para deslocar os elementos
    memmove(&funcionarios[indice], &funcionarios[indice + 1], sizeof(Funcionario) * (numFuncionarios - indice));

    (numFuncionarios)--;

    FILE *file = fopen(ficheiro, "w");
    if (!file) {
        printf("Erro ao atualizar o ficheiro de funcionarios.\n");
        return;
    }

    for (int i = 0; i < numFuncionarios; i++) {
        fprintf(file, "%s;%s;%s;%s\n", funcionarios[i].numero, funcionarios[i].nome, funcionarios[i].nif, funcionarios[i].telefone);
    }

    fclose(file);
    printf("\nFuncionario removido com sucesso.\n");
}


void registarEscolha(const char *ficheiro, Escolha escolhas[], int *numEscolhas, int maxEscolhas) {
    if (*numEscolhas >= maxEscolhas) {
        printf("\nLimite maximo de escolhas atingido.\n");
        return;
    }

    Escolha novaEscolha;
    const char *diasValidos[] = {"2feira", "3feira", "4feira", "5feira", "6feira"};
    int diaValido = 0;
    do { //ciclo para garantir que o utente escreva um dia da semana valido
        printf("\nDigite o dia (2feira, 3feira, 4feira, 5feira, 6feira): \n");
        scanf(" %s", novaEscolha.dia);

        diaValido = 0;
        for (int i = 0; i < 5; i++) {
            if (strcmp(novaEscolha.dia, diasValidos[i]) == 0) {
                diaValido = 1;
                break;
            }
        }

        if (!diaValido) {
            printf("\nDia invalido. Por favor, insira um dia valido.\n");
        }
    } while (!diaValido);

   do { //garante que vai digitar um numero de funcionario valido
    printf("Digite o numero do funcionario (4 digitos, 0000 a 9999): ");
    scanf(" %s", novaEscolha.numero_funcionario);

    if (strlen(novaEscolha.numero_funcionario) == 4 && 
        strspn(novaEscolha.numero_funcionario, "0123456789") == 4) { //strspn vai verificar se os caracteres sao digitos de 0 a 9
        break;
    }
    printf("Numero invalido. Por favor, insira um numero valido (4 digitos).\n");
} while (1);

do { //garante que vai escolher uma opcao valida
    printf("Digite o tipo de prato (C - Carne, P - Peixe, D - Dieta, V - Vegean): ");
    scanf(" %s", novaEscolha.tipo_prato);

    if (strlen(novaEscolha.tipo_prato) == 1 && 
        strchr("CPDV", toupper(novaEscolha.tipo_prato[0]))) { //strchr verifica se o caracter esta em "CPDV". toupper torna minuscula em maiuscula
        break;
    }
    printf("Tipo de prato invalido. Por favor, escolha C, P, D ou V.\n");
} while (1);

    escolhas[*numEscolhas] = novaEscolha; //para guardar a escolha do utente
    (*numEscolhas)++;

    FILE *file = fopen(ficheiro, "a");
    if (!file) {
        printf("Erro ao registrar escolha no ficheiro.\n");
        return;
    }

    fprintf(file, "%s;%s;%s\n", novaEscolha.dia, novaEscolha.numero_funcionario, novaEscolha.tipo_prato);
    fclose(file);

    printf("Escolha registrada com sucesso.\n");
}

void mostrarFuncionarios(const char *ficheiro, Funcionario funcionarios[]) {
    int numFuncionarios = carregarFuncionarios(ficheiro, funcionarios);
    if (numFuncionarios == 0) {
        printf("Nenhum funcionário registrado.\n");
        return;
    }

    printf("\nLista de funcionarios registrados:\n");
    printf("Numero de funcionario | Nome  | NIF  | Telefone\n");
    for (int i = 0; i < numFuncionarios; i++) {
        printf("%s | %s | %s | %s\n", 
               funcionarios[i].numero, 
               funcionarios[i].nome, 
               funcionarios[i].nif, 
               funcionarios[i].telefone);
    }
}

int carregarFuncionarios(const char *ficheiro, Funcionario funcionarios[]) {
    FILE *file = fopen(ficheiro, "r");
    if (!file) {
        printf("Erro ao abrir o ficheiro %s\n", ficheiro);
        return 0;
    }

    char linha[100];
    int count = 0;
    while (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "%[^;];%[^;];%[^;];%[^\n]", 
               funcionarios[count].numero,  // Mantém como string
               funcionarios[count].nome, 
               funcionarios[count].nif, 
               funcionarios[count].telefone);
        count++;
    }
    fclose(file);
    return count;
}


int inserirFuncionario(const char *ficheiro, Funcionario funcionarios[]) {
    int nrFuncionario = carregarFuncionarios(ficheiro, funcionarios);
    if (nrFuncionario >= MAX_FUNCIONARIOS) {
        printf("Limite maximo de funcionarios atingido.\n");
        return -1;
    }
    if(nrFuncionario < 0){
        printf("\n Numero invalido! Tente novamente, por favor.");
        return -1;
    }

    printf("\nIntroduza o numero do funcionario que pretende inserir:\n");
    char nrFuncionarioStr[5];  // Mudando de int para string
    scanf(" %s", nrFuncionarioStr);

    for (int i = 0; i < nrFuncionario; i++) { 
        if (strcmp(funcionarios[i].numero, nrFuncionarioStr) == 0) {  // Comparando como string
            printf("Numero de funcionario ja registado!\n");
            return -1;
        }
    }

    printf("Introduza o nome do funcionario:\n");
    scanf(" %156[^\n]", funcionarios[nrFuncionario].nome); //%156[^\n] permite um nome ate 156 caracteres e le tudo ate a quebra de linha
    printf("Introduza o NIF do funcionario:\n");
    scanf(" %s", funcionarios[nrFuncionario].nif);
    printf("Introduza o telefone do funcionario:\n");
    scanf(" %s", funcionarios[nrFuncionario].telefone);

    // Atribui o número de funcionário como string
    strcpy(funcionarios[nrFuncionario].numero, nrFuncionarioStr);  
    nrFuncionario++;

    FILE *file = fopen(ficheiro, "w");
    if (!file) {
        printf("Erro ao guardar o funcionario no ficheiro.\n");
        return -1;
    }
    for (int i = 0; i < nrFuncionario; i++) {
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
    FILE *file = fopen(ficheiro, "r");
    if (!file) {
        printf("Erro ao abrir o ficheiro %s\n", ficheiro);
        return 0;
    }

    char linha[300];
    int count = 0;

    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "%[^;];%[^;];%[^;];%d;%[^;];%d;%[^;];%d;%[^;];%d",
                   refeicoes[count].dia,
                   refeicoes[count].data,
                   refeicoes[count].prato_peixe, &refeicoes[count].calorias_peixe,
                   refeicoes[count].prato_carne, &refeicoes[count].calorias_carne,
                   refeicoes[count].prato_dieta, &refeicoes[count].calorias_dieta,
                   refeicoes[count].prato_vegan, &refeicoes[count].calorias_vegan) == 10) {
            count++;
        } else {
            printf("Erro ao interpretar a linha: %s\n", linha);
        }
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
               refeicoes[count].dia, refeicoes[count].data,
               refeicoes[count].prato_peixe, &refeicoes[count].calorias_peixe,
               refeicoes[count].prato_carne, &refeicoes[count].calorias_carne,
               refeicoes[count].prato_dieta, &refeicoes[count].calorias_dieta,
               refeicoes[count].prato_vegan, &refeicoes[count].calorias_vegan);
        count++;
    }
    fclose(file);
    return count;

}

int carregarEscolhas(const char *ficheiro, Escolha escolhas[]) {
    FILE *file = fopen(ficheiro, "r");
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
    printf("Refeicoes do dia %s:\n", dia);

    for (int i = 0; i < numEscolhas; i++) {
        if (strcmp(escolhas[i].dia, dia) == 0) {
            // Procura o funcionário
            char nomeFuncionario[50] = "Desconhecido";
            for (int j = 0; j < numFuncionarios; j++) {
                if (strcmp(funcionarios[j].numero, escolhas[i].numero_funcionario) == 0) {  // Comparando como string
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
                    else if (strcmp(escolhas[i].tipo_prato, "V") == 0) strcpy(prato, refeicoes[k].prato_vegan);
                }
            }

            // Exibir resultado
            printf("Funcionário: %s, Nome: %s, Prato: %s\n", escolhas[i].numero_funcionario, nomeFuncionario, prato);
        }
    }
}


// Lista os utentes ordenando por número de funcionário e cálculo da despesa
void listarUtentesDespesas(Funcionario funcionarios[], int numFuncionarios, Escolha escolhas[], int numEscolhas) {
    int refeicoesServidas[MAX_FUNCIONARIOS] = {0};
    float precoPorRefeicao = 6.0;

    // Conta as refeições por funcionário
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
void editarFuncionario(const char *ficheiro, Funcionario funcionarios[]) {
    int numFuncionarios = carregarFuncionarios(ficheiro, funcionarios);
    char numero[5];
    printf("Digite o numero do funcionario a ser editado: ");
    scanf(" %s", numero);

    int indice = -1;
    for (int i = 0; i < numFuncionarios; i++) {
        if (strcmp(funcionarios[i].numero, numero) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Funcionario nao encontrado.\n");
        return;
    }

    printf("Funcionario encontrado: %s, %s, %s\n", 
           funcionarios[indice].nome, 
           funcionarios[indice].nif, 
           funcionarios[indice].telefone);

    printf("Digite o novo nome (ou pressione Enter para manter): ");
    char novoNome[50];
    getchar();
    fgets(novoNome, sizeof(novoNome), stdin);
    if (novoNome[0] != '\n') {
        novoNome[strcspn(novoNome, "\n")] = '\0'; // percorre a str1 e verifica se algum caracter pertence a str 2 e se pertencer vai retornar o indice
        // - se nenhum pertencer, retorna str1
        strcpy(funcionarios[indice].nome, novoNome);
    }

    printf("Digite o novo NIF (ou pressione Enter para manter): ");
    char novoNif[15];
    fgets(novoNif, sizeof(novoNif), stdin);
    if (novoNif[0] != '\n') {
        novoNif[strcspn(novoNif, "\n")] = '\0';
        strcpy(funcionarios[indice].nif, novoNif);
    }

    printf("Digite o novo telefone (ou pressione Enter para manter): ");
    char novoTelefone[15];
    fgets(novoTelefone, sizeof(novoTelefone), stdin);
    if (novoTelefone[0] != '\n') {
        novoTelefone[strcspn(novoTelefone, "\n")] = '\0';
        strcpy(funcionarios[indice].telefone, novoTelefone);
    }

    FILE *file = fopen(ficheiro, "w");
    if (!file) {
        printf("Erro ao salvar as alteracoes no ficheiro.\n");
        return;
    }

    for (int i = 0; i < numFuncionarios; i++) {
        fprintf(file, "%s;%s;%s;%s\n", 
                funcionarios[i].numero, 
                funcionarios[i].nome, 
                funcionarios[i].nif, 
                funcionarios[i].telefone);
    }

    fclose(file);
    printf("Dados do funcionario atualizados com sucesso.\n");
}
