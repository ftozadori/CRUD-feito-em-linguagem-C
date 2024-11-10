#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <conio.c>
#include <windows.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO "usuarios.txt"  // Nome do arquivo onde os usu�rios ser�o salvos
#define MAX_USUARIOS 100
#define SHIFT 10 * 2    // Deslocamento da tabela ASCII para criptografia

char nome[MAX_USUARIOS][15];  // vari�veis globais
char senha[MAX_USUARIOS][15];
FILE *arquivo;
int op;

void incluir();
void menu();
void listar();
void alterar();
void excluir();
void limparTela();
void criptografarSenha(char* senha);
void descriptografarSenha(char* senha);
int arquivotxt(int i);

int main() {
    setlocale(LC_ALL, "portuguese");
    menu();  
    return 0;
}

void limparTela() {
    system("cls"); 
    textcolor(7);   
    textbackground(9); 
}

void menu() {  // fun��o do menu
    while (1) {
        limparTela();
        gotoxy(5, 7);  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
        gotoxy(5, 8);  printf("----SISTEMA DE CADASTRO-----");
        gotoxy(5, 9);  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
        gotoxy(5, 10); printf("|1-Incluir                 |");
        gotoxy(5, 11); printf("|2-Alterar                 |");
        gotoxy(5, 12); printf("|3-Excluir                 |");
        gotoxy(5, 13); printf("|4-Listar                  |");
        gotoxy(5, 14); printf("|5-Cr�ditos                |");
        gotoxy(5, 15); printf("|6-Sair                    |");
        gotoxy(5, 16); printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
        gotoxy(5, 17); printf("Escolha uma op��o: ");
        
        scanf("%d", &op);
        
        switch (op) {
            case 1: incluir(); break;
            case 2: alterar(); break;
            case 3: excluir(); break;
            case 4: listar(); break;
            case 5: 
                limparTela();
                gotoxy(5, 7);  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                gotoxy(5, 8);  printf("         CR�DITOS          \n");
                gotoxy(5, 9);  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                gotoxy(5, 10); printf("Sistema de Cadastro em Linguagem C\n");
                gotoxy(5, 11); printf("Desenvolvido por: Felipe Tozadori Silva--------Gabriel Cesar--------Julia Aparecida Cardoso\n");
                gotoxy(5, 12); printf("UNICESUMAR=-=-=-=A&P 2024 2bim. \n");
                gotoxy(5, 13); printf("Pressione qualquer tecla para voltar ao menu...\n");
                getch();  // Aguarda o pressionamento de qualquer tecla
                system("cls");
				system("color 0F");
                break;
            case 6:
			    system("cls");
                printf("========================OBRIGADO POR USAR NOSSOS SERVI�OS========================\n");
                exit(0);
            default: 
                printf("Op��o invalida, tente novamente, pressione qualquer tecla e tente novamente.\n");
                getch();  // Aguarda o pressionamento de qualquer tecla
                system("cls");
				system("color 0F");
                break;
        }
    }
}

void incluir() {  // fun��o para incluir cadastro
    static int linha = 0;  // vari�vel para controlar a linha do cadastro
    if (linha >= MAX_USUARIOS) {
        printf("Limite de cadastros atingido.\n");
        return;
    }

    char username[15];
    int usuarioExistente = 0; 

    do {
        limparTela();
        gotoxy(5, 7);  printf("Crie um username: ");
        gotoxy(5, 8);  scanf("%s", username);

        // Verifica se o nome de usu�rio j� est� cadastrado
        usuarioExistente = 0; 
        for (int i = 0; i < linha; i++) {
            if (strcmp(nome[i], username) == 0) {
                usuarioExistente = 1; 
                gotoxy(5, 9); 
                printf("Erro: O username \"%s\" ja existe! Tente outro.\n", username);
                break;
            }
        }

        if (!usuarioExistente) {  // Se o usu�rio n�o existe, permite o cadastro
            strcpy(nome[linha], username);  // Armazena o nome de usu�rio
            gotoxy(5, 9);  printf("Crie uma senha: ");
            gotoxy(5, 10); scanf("%s", senha[linha]);

            // Criptografar a senha
            criptografarSenha(senha[linha]);

            gotoxy(5, 11); printf("\nCadastro realizado com sucesso!\n");  
            linha++; 
            arquivotxt(linha);    
        }

        gotoxy(5, 12); printf("Digite 1 para continuar ou qualquer outro n�mero para retornar ao menu: ");
        scanf("%d", &op);
    } while(op == 1); 

    system("cls");
    system("color 0F");
    menu();  
}


void listar() {  // fun��o de listagem de usu�rios
    int i=0;
	
	limparTela();
    arquivo = fopen(ARQUIVO, "r");
    gotoxy(5, 7);  printf("Lista de usuarios:\n");
    while(fgets(nome[i], 30 , arquivo) && fgets(senha[i], 30 , arquivo) != NULL){ 	  	
    if (nome[i][0] != '\0') {  // verifica se o nome foi preenchido
        char senhaTemp[15]; 
        strcpy(senhaTemp, senha[i]);  // Faz uma c�pia da senha original
        descriptografarSenha(senhaTemp);// Descriptografa a senha tempor�ria para exibi��o
        printf("Nome: %s",nome[i]);
		printf("Senha:�%s\n\n",senhaTemp);  // Exibe a senha descriptografada
        i++;
    }
}
    getch();
	fclose(arquivo);
    system("cls");
	system("color 0F");
    menu();  // retorna ao menu principal
}    

void alterar() {  // Fun��o para alterar os dados do usu�rio diretamente no arquivo
    char username[15];
    int encontrado = 0;
    FILE *arquivoOriginal, *arquivoTemp;
    char nomeTemp[15], senhaTemp[15];
    
    limparTela();
    gotoxy(5, 7); printf("Digite o username a ser alterado: ");
    scanf("%s", username);

    arquivoOriginal = fopen(ARQUIVO, "r");
    if (arquivoOriginal == NULL) {
        gotoxy(5, 8); printf("Erro ao abrir o arquivo!\n");
        return;
    }

    arquivoTemp = fopen("temp.txt", "w");
    if (arquivoTemp == NULL) {
        gotoxy(5, 8); printf("Erro ao criar arquivo tempor�rio!\n");
        fclose(arquivoOriginal);
        return;
    }

    while (fgets(nomeTemp, sizeof(nomeTemp), arquivoOriginal) && fgets(senhaTemp, sizeof(senhaTemp), arquivoOriginal)) {    
        nomeTemp[strcspn(nomeTemp, "\n")] = 0;
        senhaTemp[strcspn(senhaTemp, "\n")] = 0;

        if (strcmp(nomeTemp, username) == 0) {  // Verifica se o username foi encontrado
            gotoxy(5, 8); printf("Cadastro encontrado!\n");
            gotoxy(5, 9); printf("Digite o novo username: ");
            scanf("%s", nomeTemp);
            gotoxy(5, 10); printf("Digite a nova senha: ");
            scanf("%s", senhaTemp);

            // Criptografar a nova senha
            criptografarSenha(senhaTemp);

            // Atualiza o nome e senha alterados no arquivo tempor�rio
            fprintf(arquivoTemp, "%s\n", nomeTemp);
            fprintf(arquivoTemp, "%s\n", senhaTemp);

            gotoxy(5, 11); printf("Cadastro alterado com sucesso!\n");
            encontrado = 1;
        } else {
            // Se n�o for o usu�rio a ser alterado, copia para o arquivo tempor�rio
            fprintf(arquivoTemp, "%s\n", nomeTemp);
            fprintf(arquivoTemp, "%s\n", senhaTemp);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    // Se o usu�rio foi encontrado e alterado, renomeia o arquivo tempor�rio
    if (encontrado) {
        remove(ARQUIVO);  // Remove o arquivo original
        rename("temp.txt", ARQUIVO);  // Renomeia o arquivo tempor�rio para o nome do arquivo original
    } else {
        remove("temp.txt");  // Se o usu�rio n�o foi encontrado, exclui o arquivo tempor�rio
        gotoxy(5, 8); printf("Usu�rio n�o encontrado!\n");
    }

    gotoxy(5, 12); 
    printf("\nDigite 1 para alterar outro usuario ou qualquer outro numero para retornar ao menu: ");
    scanf("%d", &op);
    if (op == 1) {
        alterar();  // Repete a opera��o de altera��o
    } else {
        system("cls");
        system("color 0F");
        menu();  // Retorna ao menu principal
    }
}

void excluir() {  // fun��o de exclus�o
    char username[15];
    int encontrado = 0;
    FILE *arquivoOriginal, *arquivoTemp;
    char nomeTemp[15], senhaTemp[15];

    limparTela();
    gotoxy(5, 7);  printf("Digite o username a ser exclu�do: ");
    scanf("%s", username);

    // Abre o arquivo original para leitura
    arquivoOriginal = fopen(ARQUIVO, "r");
    if (arquivoOriginal == NULL) {
        gotoxy(5, 8); printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Cria um arquivo tempor�rio para escrever os dados atualizados
    arquivoTemp = fopen("temp.txt", "w");
    if (arquivoTemp == NULL) {
        gotoxy(5, 8); printf("Erro ao criar arquivo tempor�rio!\n");
        fclose(arquivoOriginal);
        return;
    }

    // L� o arquivo original linha por linha e copia para o arquivo tempor�rio, excluindo o usu�rio
    while (fgets(nomeTemp, sizeof(nomeTemp), arquivoOriginal) && fgets(senhaTemp, sizeof(senhaTemp), arquivoOriginal)) {
        nomeTemp[strcspn(nomeTemp, "\n")] = 0;  // Remove o '\n' da string
        senhaTemp[strcspn(senhaTemp, "\n")] = 0;  // Remove o '\n' da string

        if (strcmp(nomeTemp, username) != 0) {  // Se o username n�o for o do usu�rio a ser exclu�do
            // Copia para o arquivo tempor�rio
            fprintf(arquivoTemp, "%s\n", nomeTemp);
            fprintf(arquivoTemp, "%s\n", senhaTemp);
        } else {
            // Encontrou o usu�rio, ent�o n�o copia para o arquivo tempor�rio
            encontrado = 1;
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    // Se o usu�rio foi encontrado e exclu�do, renomeia o arquivo tempor�rio
    if (encontrado) {
        remove(ARQUIVO);  // Remove o arquivo original
        rename("temp.txt", ARQUIVO);  // Renomeia o arquivo tempor�rio para o nome do arquivo original
        gotoxy(5, 8); printf("Usu�rio exclu�do com sucesso!\n");
    } else {
        remove("temp.txt");  // Se o usu�rio n�o foi encontrado, exclui o arquivo tempor�rio
        gotoxy(5, 8); printf("Usu�rio n�o encontrado!\n");
    }

    gotoxy(5, 9); 
    printf("\nDigite 1 para excluir outro usuario ou qualquer outro n�mero para retornar ao menu: ");
    scanf("%d", &op);
    if (op == 1) {
        excluir();  // Repete a opera��o de exclus�o
    } else {
        system("cls");
        system("color 0F");
        menu();  // Retorna ao menu principal
    }
}

void criptografarSenha(char* senha) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] += SHIFT;  // desloca cada caractere no c�digo ASCII
    }
}

void descriptografarSenha(char* senha) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] -= SHIFT;  // desfaz o deslocamento feito na criptografia
    }
}

int arquivotxt(int x){
	
	
	arquivo = fopen(ARQUIVO, "a+");  	
    fprintf(arquivo,"%s\n", nome[x-1]);
    fprintf(arquivo,"%s\n", senha[x-1]);
	fclose(arquivo);
	return x;
}
