// Projeto 2 - Lista de Imagens
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "imagem.h"

void helper()
{
    printf("=============| Editor de listas de imagens |=============\n");
    printf(" [criar nome.bin imagem.pgm] Cria arquivo com uma lista contendo o nome da imagem passada.\n");
    printf(" [inserir nome.bin imagem.pgm] Insere na lista um novo nome de imagem.\n");
    printf(" [remover nome.bin imagem.pgm] Remove da lista, caso exista, um nome de imagem.\n");
    printf(" [print nome.bin] Imprime na tela a lista de imagens ordenadas alfabeticamente.\n");
    printf(" [limiar nome.bin (limiar)] Limiariza todas as imagens contidas na lista.\n");
    printf(" [inversa nome.bin] Inverte todas as imagens contidas na lista.\n");
    printf(" [vertical nome.bin] Espelha verticalmente as imagens da lista.\n");
    printf(" [horizontal nome.bin] Espelha horizontalmente as imagens da lista.\n");
}

void finish(bool check, char *filename, dimg I)
{
    if (check)
    {
        // Se tudo ocorrer de acordo a lista é gravado no arquivo.
        bool aux = to_file(filename, I);
        if (aux)
        {
            printf("\n Operação realizada com sucesso. \n");
        }
        else
        {
            printf("\n Erro: Falha ao salvar arquivo. \n");
        }
        return;
    }
    printf("\n Erro: Operação falhou. \n");
}

void cmd_invalido()
{
    printf("\n Erro: Comando inválido. Digite help para ajuda. \n");
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "help") == 0)
        {
            helper();
            return 0;
        }
        cmd_invalido();
        return 0;
    }
    if (argc > 2 && argc < 5) // 3 , 4
    {
        dimg I = new_img();
        bool check = true;
        // Função de criação de lista
        if (strcmp(argv[1], "criar") == 0 && argc == 4)
        {
            char imagem[50];
            sscanf(argv[3], "%s", imagem);
            check = insertImg(I, imagem);
            finish(check, argv[2], I);
            del_list(I);
            return 0;
        }

        check = from_file(argv[2], I);
        if (check)
        {
            // Funções de manipulação de lista
            check = false; // Reinicia variável de checagem
            if (strcmp(argv[1], "inserir") == 0 && argc == 4)
            {
                char imagem[50];
                sscanf(argv[3], "%s", imagem);
                check = insertImg(I, imagem);
            }

            if (strcmp(argv[1], "print") == 0 && argc == 3)
            {
                check = print_list(I);
            }

            // Funções de manipulação de imagem
            if ((strcmp(argv[1], "limiar") == 0) && (argc == 4))
            {
                // Aplica um limiar nas imagens da lista com base em valor fornecido pelo usuário.
                int lim = 0;
                sscanf(argv[3], "%d", &lim);
                check = limiar_lista(I, lim);
            }
            if ((strcmp(argv[1], "inversa") == 0) && (argc == 3))
            // Inverte as cores das imagens da lista.
            {
                check = inversa_lista(I);
            }
            if ((strcmp(argv[1], "vertical") == 0) && (argc == 3))
            // Espelha as imagens da lista verticalmente.
            {
                check = vertical_lista(I);
            }
            if ((strcmp(argv[1], "horizontal") == 0) && (argc == 3))
            // Espelha as imagens da lista horizontalmente.
            {
                check = horizontal_lista(I);
            }

            if (strcmp(argv[1], "remover") == 0 && argc == 4)
            // Remove uma imagem da lista.
            {
                char imagem[50];
                char filename[50];
                sscanf(argv[3], "%s", imagem);
                sscanf(argv[2], "%s", filename);
                check = removeImg(I, imagem, filename);
            }
        }

        finish(check, argv[2], I);
        del_list(I);
        return 0;
    }
    cmd_invalido();
    return 0;
}