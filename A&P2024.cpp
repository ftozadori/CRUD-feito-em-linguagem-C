#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <conio.c>
#include <windows.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO "usuarios.txt"  // Nome do arquivo onde os usuários serão salvos
#define MAX_USUARIOS 100
#define SHIFT 10 * 2    // Deslocamento da tabela ASCII para criptografia

char nome[MAX_USUARIOS][15];  // variáveis globais
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

void menu() {  // função do menu
    while (1) {
        limparTela();
        gotoxy(5, 7);  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
        gotoxy(5, 8);  printf("----SISTEMA DE CADASTRO-----");
        gotoxy(5, 9);  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
        gotoxy(5, 10); printf("|1-Incluir                 |");
        gotoxy(5, 11); printf("|2-Alterar                 |");
        gotoxy(5, 12); printf("|3-Excluir                 |");
        gotoxy(5, 13); printf("|4-Listar                  |");
        gotoxy(5, 14); printf("|5-Créditos                |");
        gotoxy(5, 15); printf("|6-Sair                    |");
        gotoxy(5, 16); printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
        gotoxy(5, 17); printf("Escolha uma opção: ");
        
        scanf("%d", &op);
        
        switch (op) {
            case 1: incluir(); break;
            case 2: alterar(); break;
            case 3: excluir(); break;
            case 4: listar(); break;
            case 5: 
                limparTela();
                gotoxy(5, 7);  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                gotoxy(5, 8);  printf("         CRéDITOS          \n");
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
                printf("========================OBRIGADO POR USAR NOSSOS SERVIÇOS========================\n");
                exit(0);
            default: 
                printf("Opção invalida, tente novamente, pressione qualquer tecla e tente novamente.\n");
                getch();  // Aguarda o pressionamento de qualquer tecla
                system("cls");
				system("color 0F");
                break;
        }
    }
}

void incluir() {  // função para incluir cadastro
    static int linha = 0;  // variável para controlar a linha do cadastro
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

        // Verifica se o nome de usuário já está cadastrado
        usuarioExistente = 0; 
        for (int i = 0; i < linha; i++) {
            if (strcmp(nome[i], username) == 0) {
                usuarioExistente = 1; 
                gotoxy(5, 9); 
                printf("Erro: O username \"%s\" ja existe! Tente outro.\n", username);
                break;
            }
        }

        if (!usuarioExistente) {  // Se o usuário não existe, permite o cadastro
            strcpy(nome[linha], username);  // Armazena o nome de usuário
            gotoxy(5, 9);  printf("Crie uma senha: ");
            gotoxy(5, 10); scanf("%s", senha[linha]);

            // Criptografar a senha
            criptografarSenha(senha[linha]);

            gotoxy(5, 11); printf("\nCadastro realizado com sucesso!\n");  
            linha++; 
            arquivotxt(linha);    
        }

        gotoxy(5, 12); printf("Digite 1 para continuar ou qualquer outro número para retornar ao menu: ");
        scanf("%d", &op);
    } while(op == 1); 

    system("cls");
    system("color 0F");
    menu();  
}


void listar() {  // função de listagem de usuários
    int i=0;
	
	limparTela();
    arquivo = fopen(ARQUIVO, "r");
    gotoxy(5, 7);  printf("Lista de usuarios:\n");
    while(fgets(nome[i], 30 , arquivo) && fgets(senha[i], 30 , arquivo) != NULL){ 	  	
    if (nome[i][0] != '\0') {  // verifica se o nome foi preenchido
        char senhaTemp[15]; 
        strcpy(senhaTemp, senha[i]);  // Faz uma cópia da senha original
        descriptografarSenha(senhaTemp);// Descriptografa a senha temporária para exibição
        printf("Nome: %s",nome[i]);
		printf("Senha:ö%s\n\n",senhaTemp);  // Exibe a senha descriptografada
        i++;
    }
}
    getch();
	fclose(arquivo);
    system("cls");
	system("color 0F");
    menu();  // retorna ao menu principal
}    

void alterar() {  // Função para alterar os dados do usuário diretamente no arquivo
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
        gotoxy(5, 8); printf("Erro ao criar arquivo temporário!\n");
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

            // Atualiza o nome e senha alterados no arquivo temporário
            fprintf(arquivoTemp, "%s\n", nomeTemp);
            fprintf(arquivoTemp, "%s\n", senhaTemp);

            gotoxy(5, 11); printf("Cadastro alterado com sucesso!\n");
            encontrado = 1;
        } else {
            // Se não for o usuário a ser alterado, copia para o arquivo temporário
            fprintf(arquivoTemp, "%s\n", nomeTemp);
            fprintf(arquivoTemp, "%s\n", senhaTemp);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    // Se o usuário foi encontrado e alterado, renomeia o arquivo temporário
    if (encontrado) {
        remove(ARQUIVO);  // Remove o arquivo original
        rename("temp.txt", ARQUIVO);  // Renomeia o arquivo temporário para o nome do arquivo original
    } else {
        remove("temp.txt");  // Se o usuário não foi encontrado, exclui o arquivo temporário
        gotoxy(5, 8); printf("Usuário não encontrado!\n");
    }

    gotoxy(5, 12); 
    printf("\nDigite 1 para alterar outro usuario ou qualquer outro numero para retornar ao menu: ");
    scanf("%d", &op);
    if (op == 1) {
        alterar();  // Repete a operação de alteração
    } else {
        system("cls");
        system("color 0F");
        menu();  // Retorna ao menu principal
    }
}

void excluir() {  // função de exclusão
    char username[15];
    int encontrado = 0;
    FILE *arquivoOriginal, *arquivoTemp;
    char nomeTemp[15], senhaTemp[15];

    limparTela();
    gotoxy(5, 7);  printf("Digite o username a ser excluído: ");
    scanf("%s", username);

    // Abre o arquivo original para leitura
    arquivoOriginal = fopen(ARQUIVO, "r");
    if (arquivoOriginal == NULL) {
        gotoxy(5, 8); printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Cria um arquivo temporário para escrever os dados atualizados
    arquivoTemp = fopen("temp.txt", "w");
    if (arquivoTemp == NULL) {
        gotoxy(5, 8); printf("Erro ao criar arquivo temporário!\n");
        fclose(arquivoOriginal);
        return;
    }

    // Lê o arquivo original linha por linha e copia para o arquivo temporário, excluindo o usuário
    while (fgets(nomeTemp, sizeof(nomeTemp), arquivoOriginal) && fgets(senhaTemp, sizeof(senhaTemp), arquivoOriginal)) {
        nomeTemp[strcspn(nomeTemp, "\n")] = 0;  // Remove o '\n' da string
        senhaTemp[strcspn(senhaTemp, "\n")] = 0;  // Remove o '\n' da string

        if (strcmp(nomeTemp, username) != 0) {  // Se o username não for o do usuário a ser excluído
            // Copia para o arquivo temporário
            fprintf(arquivoTemp, "%s\n", nomeTemp);
            fprintf(arquivoTemp, "%s\n", senhaTemp);
        } else {
            // Encontrou o usuário, então não copia para o arquivo temporário
            encontrado = 1;
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    // Se o usuário foi encontrado e excluído, renomeia o arquivo temporário
    if (encontrado) {
        remove(ARQUIVO);  // Remove o arquivo original
        rename("temp.txt", ARQUIVO);  // Renomeia o arquivo temporário para o nome do arquivo original
        gotoxy(5, 8); printf("Usuário excluído com sucesso!\n");
    } else {
        remove("temp.txt");  // Se o usuário não foi encontrado, exclui o arquivo temporário
        gotoxy(5, 8); printf("Usuário não encontrado!\n");
    }

    gotoxy(5, 9); 
    printf("\nDigite 1 para excluir outro usuario ou qualquer outro número para retornar ao menu: ");
    scanf("%d", &op);
    if (op == 1) {
        excluir();  // Repete a operação de exclusão
    } else {
        system("cls");
        system("color 0F");
        menu();  // Retorna ao menu principal
    }
}

void criptografarSenha(char* senha) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] += SHIFT;  // desloca cada caractere no código ASCII
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
