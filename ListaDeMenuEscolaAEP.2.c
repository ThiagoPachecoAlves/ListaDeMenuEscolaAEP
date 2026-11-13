#include <stdio.h>
#include <string.h>
 typedef struct {
    char dia[20];
    char prato[80];
    char ingredientes[200];
    char observacao[200];
} Refeicao;

 typedef struct {
    char nome[60];
    char funcao[30];
} Usuario;

 typedef struct {
    Refeicao refeicoes[7];
    Usuario usuarios[20];
    int total;
    int totalUsuarios;
} Sistema;

Sistema dados = {0};
 void salvar() {
    FILE *arquivo = fopen("semana.dat", "wb");
    if (arquivo != NULL) {
        fwrite(&dados, sizeof(dados), 1, arquivo);
        fclose(arquivo);
    } else {
        printf("Erro ao salvar os dados.\n");
    }
}
void carregar() {
    FILE *arquivo = fopen("semana.dat", "rb");
    if (arquivo != NULL) {
        if (fread(&dados, sizeof(dados), 1, arquivo) != 1 ||
            dados.total < 0 || dados.total > 7 ||
            dados.totalUsuarios < 0 || dados.totalUsuarios > 20) {
            dados.total = 0;
            dados.totalUsuarios = 0;
            printf("Arquivo invalido. Iniciando uma semana vazia.\n");
        }
    fclose(arquivo);
    }
}
int identificar(char nome[], char funcao[]) {
    int i;
    for (i = 0; i < dados.totalUsuarios; i++) {
        if (strcmp(dados.usuarios[i].nome, nome) == 0) {
            if (strcmp(dados.usuarios[i].funcao, funcao) != 0) {
                printf("Funcao diferente da cadastrada para este nome.\n");
                return 0;
            }
            return 1;
        }
    } 
     if (dados.totalUsuarios == 20) {
        printf("Limite de usuarios atingido.\n");
        return 0;
    }
    strcpy(dados.usuarios[dados.totalUsuarios].nome, nome);
    strcpy(dados.usuarios[dados.totalUsuarios].funcao, funcao);
    dados.totalUsuarios++;
    salvar();
    return 1;
}
 void listar() {
    int i;
    if (dados.total == 0) printf("Nenhuma refeicao cadastrada.\n");
    for (i = 0; i < dados.total; i++) {
        printf("\n%d - Dia: %s\n", i + 1, dados.refeicoes[i].dia);
        printf("Refeicao: %s\n", dados.refeicoes[i].prato);
        printf("Ingredientes e dosagens: %s\n", dados.refeicoes[i].ingredientes);
        if (dados.refeicoes[i].observacao[0] != '\0')
            printf("Observacao: %s\n", dados.refeicoes[i].observacao);
    }
}
 void cadastrar() {
    char dia[20];
    int i, pos = -1;
    printf("\nDia da semana: ");
    scanf(" %19[^\n]", dia);
    for (i = 0; i < dados.total; i++) {
        if (strcmp(dados.refeicoes[i].dia, dia) == 0) pos = i;
    }
    if (pos == -1) {
        if (dados.total == 7) {
            printf("A semana ja possui 7 registros.\n");
            return;
        }
        pos = dados.total;
        dados.total++;
    }
    strcpy(dados.refeicoes[pos].dia, dia);
    printf("Nome da refeicao: ");
    scanf(" %79[^\n]", dados.refeicoes[pos].prato);
    printf("Ingredientes e dosagens (ex.: arroz 5g, feijao 3g): ");
    scanf(" %199[^\n]", dados.refeicoes[pos].ingredientes);
    dados.refeicoes[pos].observacao[0] = '\0';
    salvar();
    printf("Refeicao salva!\n");
}
 void avaliar() {
    int numero;
    listar();
    if (dados.total == 0) return;
    printf("\nNumero da refeicao para observar: ");
    if (scanf("%d", &numero) != 1) return;
    if (numero < 1 || numero > dados.total) {
        printf("Refeicao nao encontrada.\n");
        return;
    }
    printf("Observacao de coerencia: ");
    scanf(" %199[^\n]", dados.refeicoes[numero - 1].observacao);
    salvar();
    printf("Observacao salva!\n");
}
 int main() {
    char nome[60], funcao[30];
    int continuar = 1;
    carregar();
    while (continuar == 1) {
        printf("\nDigite seu nome: ");
        if (scanf(" %59[^\n]", nome) != 1) break;
        printf("Digite sua funcao (cozinheira, nutricionista ou coordenadora): ");
        if (scanf(" %29[^\n]", funcao) != 1) break;

        if (strcmp(funcao, "coordenador") == 0) strcpy(funcao, "coordenadora");
        if (strcmp(funcao, "cozinheira") != 0 &&
            strcmp(funcao, "nutricionista") != 0 &&
            strcmp(funcao, "coordenadora") != 0) {
            printf("Funcao invalida. Use letras minusculas.\n");
        } else if (identificar(nome, funcao)) {
            if (strcmp(funcao, "cozinheira") == 0) cadastrar();
            else if (strcmp(funcao, "nutricionista") == 0) avaliar();
            else if (strcmp(funcao, "coordenadora") == 0) listar();
        }
        printf("\nOutro usuario? (1 = sim, 0 = sair): ");
        if (scanf("%d", &continuar) != 1) break;
    }
    printf("Programa encerrado.\n");
    return 0;
}
