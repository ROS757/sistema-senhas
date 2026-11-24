#include <stdio.h>
#include <stdlib.h>

#define MAX 100          // maximo de senhas por categoria
#define URGENTE 0        // a ordem das categorias e a ordem de prioridade
#define PREFERENCIAL 1
#define NORMAL 2
#define FEMININO 1
#define MASCULINO 2

// ---------- Dados do sistema ----------
// Uma fila para cada categoria: fila[0] = Urgente, fila[1] = Preferencial, fila[2] = Normal
int fila[3][MAX];
int inicio[3] = {0, 0, 0};   // posicao da proxima senha a ser chamada
int fim[3] = {0, 0, 0};      // quantidade de senhas emitidas
char letras[3] = {'U', 'P', 'N'};
char nomes[3][15] = {"Urgente", "Preferencial", "Normal"};

// Atendimento em andamento
int emAtendimento = 0;       // 1 = tem paciente sendo atendido, 0 = nao tem
int senhaAtual = 0;
int categoriaAtual = 0;

// ---------- Funcoes de leitura e validacao ----------

void limparBuffer() {
    int c;
    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

// le um numero inteiro e so aceita valores entre min e max
int lerInteiro(char texto[], int min, int max) {
    int valor, ok;
    do {
        printf("%s", texto);
        ok = scanf("%d", &valor);
        if (ok == EOF) {
            printf("\nEntrada encerrada. Saindo do programa...\n");
            exit(0);
        }
        limparBuffer();
        if (ok != 1 || valor < min || valor > max) {
            printf("Valor invalido! Digite um numero de %d a %d.\n", min, max);
            ok = 0;
        }
    } while (ok != 1);
    return valor;
}

// retorna 1 para Sim e 0 para Nao
int lerSimNao(char pergunta[]) {
    printf("%s\n", pergunta);
    return lerInteiro("1-Sim  2-Nao: ", 1, 2) == 1;
}

// ---------- Modulo 1: Registro de senhas ----------

void exibirMenu() {
    printf("\n===== SISTEMA DE SENHAS - ATENDIMENTO =====\n");
    printf("1 - Registrar paciente e emitir senha\n");
    printf("2 - Consultar proxima senha a ser chamada\n");
    printf("3 - Chamar paciente\n");
    printf("4 - Identificar senha em atendimento\n");
    printf("5 - Finalizar atendimento\n");
    printf("6 - Exibir painel de chamada\n");
    printf("0 - Sair\n");
}

// ---------- Modulo 2: Classificacao das senhas ----------

int classificarPaciente(int genero, int idade, int gestante, int lactante, int pne, int obeso) {
    if (idade > 80) {
        return URGENTE;
    }
    if (gestante == 1 || lactante == 1 || pne == 1 || obeso == 1) {
        return PREFERENCIAL;
    }
    if (genero == FEMININO && idade >= 60) {
        return PREFERENCIAL;
    }
    if (genero == MASCULINO && idade >= 65) {
        return PREFERENCIAL;
    }
    return NORMAL;
}

void emitirSenha(int cat) {
    if (fim[cat] >= MAX) {
        printf("A fila %s esta cheia!\n", nomes[cat]);
    } else {
        fila[cat][fim[cat]] = fim[cat] + 1;
        fim[cat]++;
        printf("\nSenha emitida: %c%03d (%s)\n", letras[cat], fim[cat], nomes[cat]);
    }
}

void registrarPaciente() {
    int genero, idade, cat;
    int gestante = 0, lactante = 0, pne, obeso;

    printf("\n--- REGISTRO DO PACIENTE ---\n");
    printf("Genero: 1-Feminino  2-Masculino\n");
    genero = lerInteiro("Opcao: ", 1, 2);
    idade = lerInteiro("Idade: ", 0, 120);

    if (genero == FEMININO) {
        gestante = lerSimNao("A paciente esta gestante?");
        lactante = lerSimNao("A paciente e lactante?");
    }
    pne = lerSimNao("Possui necessidade especial?");
    obeso = lerSimNao("Possui condicao de obesidade?");

    cat = classificarPaciente(genero, idade, gestante, lactante, pne, obeso);
    emitirSenha(cat);
}

// ---------- Modulo 3: Registro de atendimento ----------

// percorre as categorias na ordem de prioridade e retorna a primeira que tem gente esperando
int proximaCategoria() {
    int i;
    for (i = 0; i < 3; i++) {
        if (inicio[i] < fim[i]) {
            return i;
        }
    }
    return -1;
}

void consultarProxima() {
    int cat = proximaCategoria();

    if (cat == -1) {
        printf("Nao ha senhas aguardando chamada.\n");
    } else {
        printf("Proxima senha a ser chamada: %c%03d (%s)\n", letras[cat], fila[cat][inicio[cat]], nomes[cat]);
    }
}

void chamarPaciente() {
    int cat;

    if (emAtendimento == 1) {
        printf("Finalize o atendimento da senha %c%03d antes de chamar outro paciente.\n", letras[categoriaAtual], senhaAtual);
    } else {
        cat = proximaCategoria();
        if (cat == -1) {
            printf("Nao ha senhas aguardando chamada.\n");
        } else {
            senhaAtual = fila[cat][inicio[cat]];
            categoriaAtual = cat;
            inicio[cat]++;
            emAtendimento = 1;
            printf("Chamando a senha %c%03d (%s)\n", letras[cat], senhaAtual, nomes[cat]);
        }
    }
}

void identificarAtendimento() {
    if (emAtendimento == 1) {
        printf("Senha em atendimento: %c%03d (%s)\n", letras[categoriaAtual], senhaAtual, nomes[categoriaAtual]);
    } else {
        printf("Nenhuma senha em atendimento no momento.\n");
    }
}

// ---------- Modulo 4: Painel de chamada ----------

void exibirPainel() {
    int i, j;

    printf("\n============ PAINEL DE CHAMADA ============\n");
    if (emAtendimento == 1) {
        printf("Senha chamada: %c%03d\n", letras[categoriaAtual], senhaAtual);
        printf("Categoria: %s\n", nomes[categoriaAtual]);
    } else {
        printf("Senha chamada: nenhuma\n");
    }

    printf("\nProximas senhas aguardando chamada:\n");
    for (i = 0; i < 3; i++) {
        printf("%s (%d em espera): ", nomes[i], fim[i] - inicio[i]);
        if (inicio[i] == fim[i]) {
            printf("-");
        }
        for (j = inicio[i]; j < fim[i]; j++) {
            printf("%c%03d ", letras[i], fila[i][j]);
        }
        printf("\n");
    }
    printf("===========================================\n");
}

void finalizarAtendimento() {
    if (emAtendimento == 0) {
        printf("Nao ha atendimento em andamento.\n");
    } else {
        printf("Atendimento da senha %c%03d finalizado.\n", letras[categoriaAtual], senhaAtual);
        emAtendimento = 0;
        printf("Fila atualizada:\n");
        exibirPainel();
    }
}

// ---------- Menu ----------

void executarOpcao(int opcao) {
    switch (opcao) {
        case 1: registrarPaciente(); break;
        case 2: consultarProxima(); break;
        case 3: chamarPaciente(); break;
        case 4: identificarAtendimento(); break;
        case 5: finalizarAtendimento(); break;
        case 6: exibirPainel(); break;
        case 0: printf("Encerrando o sistema...\n"); break;
    }
}

int main() {
    int opcao;

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha: ", 0, 6);
        executarOpcao(opcao);
    } while (opcao != 0);

    return 0;
}
