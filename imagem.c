#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>
#include "lista.h"
#include "imagem.h"

imgb read_img(FILE *fimg)
/*
    Lê uma imagem em formato PGM de um arquivo e a armazena na estrutura de dados imgb.
*/
{
    // Tipo de imagem. Vamos fixar essa linha como "P2".
    char ver[5];
    fscanf(fimg, "%s", ver);
    // Define as dimensões da imagem com base no arquivo.
    imgb I;
    fscanf(fimg, "%d %d \n", &I.h, &I.w);
    fscanf(fimg, "%d", &I.cinza);

    // Aloca dinamicamente memória para a matriz de pixels da imagem com base na altura e largura.
    I.pixel = malloc(I.h * I.w * sizeof(int));

    // Verifica se a alocação de memória deu certo.
    if (I.pixel != NULL)
    {
        for (int i = 0; i < I.h; i++)
            for (int j = 0; j < I.w; j++)
            {
                fscanf(fimg, "%d ", &I.pixel[i * I.w + j]);
            }
    }
    else
    {
        printf("\n Erro: Falha na alocação de memória.\n");
    }

    // Retorna a estrutura imgb que contém as informações da imagem.
    return I;
}

imgb copia_img(imgb *I)
/*
    Copia os dados de uma struct de imagem em uma segunda struct.
*/
{

    imgb J;
    J.h = I->h;
    J.w = I->w;
    J.cinza = I->cinza;
    J.pixel = malloc(J.h * J.w * sizeof(int));

    if (J.pixel != NULL)
    {
        for (int i = 0; i < J.h; i++)
        {
            for (int j = 0; j < J.w; j++)
            {
                J.pixel[i * J.w + j] = I->pixel[i * I->w + j];
            }
        }
    }

    return J;
}
void free_imgb(imgb *I)
/*
    Libera memória em imgb.
*/
{
    if (I->pixel != NULL)
    {
        free(I->pixel);
        I->pixel = NULL;
    }
}

void write_imgb(char *filename, imgb *I)
/*
    Escreve imgb a um arquivo com nome filename.
*/
{
    FILE *fp = fopen(filename, "w");
    if (fp != NULL)
    {
        if (I->pixel != NULL)
        {
            // Informações de cabeçalho e seguido das dimensões e do valor máximo de tons de cinza.
            fprintf(fp, "P2\n");
            fprintf(fp, "%d %d \n", I->h, I->w);
            fprintf(fp, "%d \n", I->cinza);

            // Dados dos pixels: escreve os valores dos pixels da imagem no arquivo.
            for (int i = 0; i < I->h; i++)
            {
                for (int j = 0; j < I->w; j++) // Percorre a matriz.
                {
                    fprintf(fp, "%d ", I->pixel[i * I->w + j]); // Copia o valor da respectiva posição da matriz no arquivo.
                }
                fprintf(fp, "\n");
            }
            printf("Processo feito com sucesso!\n");
        }
        else
        {
            printf("Imagem de entrada é NULL. \n");
        }
    }
    else
    {
        printf("Erro: Não foi possível criar arquivo de saída.\n");
    }
}

bool limiar(imgb *I, int lim)
/*
    Aplica limiarização a cada pixel.
*/
{
    if (I->pixel != NULL)
    {
        #pragma omp parallel for
        for (int i = 0; i < I->h; i++)
        {
            for (int j = 0; j < I->w; j++)
            {
                // Verifica se o valor do pixel é menor que o limiar.
                if (I->pixel[i * I->w + j] < lim)
                {
                    // Se for menor que o limiar, define o valor do pixel como 0.
                    I->pixel[i * I->w + j] = 0;
                }
                else
                {
                    // Caso contrário, define o valor do pixel como o valor máximo de tons de cinza da imagem.
                    I->pixel[i * I->w + j] = I->cinza;
                }
            }
        }
        return true;
    }
    else
    {
        printf("Erro: Imagem de entrada é NULL.");
        return false;
    }
}

bool limiar_lista(dimg I, int lim)
/*
    Gera o loop para limiarizar todas as imagens da lista.
*/
{
    img aux = I->first;
    while (aux != NULL)
    {
        // Definindo o novo nome do arquivo com a tag "_limiar".
        char novo_nome[61];
        strcpy(novo_nome, aux->nome);
        char *ponto = strrchr(novo_nome, '.'); // Busca pelo ponto no nome do arquivo.
        if (ponto != NULL)
            *ponto = '\0';                // Remove a extensão .pgm.
        strcat(novo_nome, "_limiar.pgm"); // Adiciona a tag.

        FILE *pgm = fopen(aux->nome, "r");
        if (pgm == NULL)
        {
            printf("\n Erro: Erro ao abrir o arquivo %s\n", aux->nome);
        }
        else
        {
            imgb imgl = read_img(pgm);

            if (!limiar(&imgl, lim))
            {
                return false;
            }

            write_imgb(novo_nome, &imgl);
            free_imgb(&imgl);
            fclose(pgm);
        }

        aux = aux->next;
    }
    return true;
}

bool inversa(imgb *I)
/*
    Inverte as cores dos pixels.
*/
{
    if (I->pixel != NULL)
    {
    #pragma omp parallel for
        // Loop pra percorrer todos os pixels.
        for (int i = 0; i < I->h; i++)
        {
            for (int j = 0; j < I->w; j++)
            {
                // Aplica a inversão, substituindo o valor do pixel pela diferença entre o valor máximo de tons de cinza e o valor atual do pixel.
                I->pixel[i * I->w + j] = I->cinza - I->pixel[i * I->w + j];
            }
        }
        return true;
    }
    else
    {
        printf("Imagem de entrada é NULL.");
        return false;
    }
}

bool inversa_lista(dimg I)
/*
    Gera o loop para inverter todas as imagens da lista.
*/
{
    img aux = I->first;
    while (aux != NULL)
    {
        // Definindo o novo nome do arquivo com a tag "_inversa".
        char novo_nome[61]; // Ajuste o tamanho conforme necessário.
        strcpy(novo_nome, aux->nome);
        char *ponto = strrchr(novo_nome, '.'); // Busca pelo ponto no nome do arquivo.
        if (ponto != NULL)
            *ponto = '\0';                 // Remove a extensão .pgm.
        strcat(novo_nome, "_inversa.pgm"); // Adiciona a tag.

        FILE *pgm = fopen(aux->nome, "r");
        if (pgm == NULL)
        {
            printf("\n Erro: Erro ao abrir o arquivo %s\n", aux->nome);
        }
        else
        {
            imgb imgi = read_img(pgm);

            if (!inversa(&imgi))
            {
                return false;
            }

            write_imgb(novo_nome, &imgi);
            free_imgb(&imgi);
            fclose(pgm);
        }
        aux = aux->next;
    }
    return true;
}

bool vertical(imgb *I)
/*
    Espelha verticalmente a imagem.
*/
{
    // Copia I em J
    imgb J = copia_img(I);
    if (I->pixel != NULL)
    {
        // Loop para percorrer todos os  pixels.
        #pragma omp parallel for
        for (int i = 0; i < I->h; i++)
        {
            for (int j = 0; j < I->w; j++)
            {
                // Espelha os pixels de I, com base na segunda struct J para evitar que a imagem se espelhe duas vezes.
                I->pixel[i + I->h * j] = J.pixel[(J.h - i - 1) + j * J.h];
            }
        }
        return true;
    }
    else
    {
        printf("Imagem de entrada é NULL.");
        return false;
    }
}

bool vertical_lista(dimg I)
/*
    Gera o loop para espelhar verticalmente todas as imagens da lista.
*/
{
    img aux = I->first;
    while (aux != NULL)
    {
        // Definindo o novo nome do arquivo com a tag "_inversa".
        char novo_nome[61]; // Ajuste o tamanho conforme necessário.
        strcpy(novo_nome, aux->nome);
        char *ponto = strrchr(novo_nome, '.'); // Busca pelo ponto no nome do arquivo.
        if (ponto != NULL)
            *ponto = '\0';                  // Remove a extensão .pgm.
        strcat(novo_nome, "_vertical.pgm"); // Adiciona a tag.

        FILE *pgm = fopen(aux->nome, "r");
        if (pgm == NULL)
        {
            printf("\n Erro: Erro ao abrir o arquivo %s\n", aux->nome);
        }
        else
        {
            imgb imgv = read_img(pgm);

            if (!vertical(&imgv))
            {
                return false;
            }

            write_imgb(novo_nome, &imgv);
            free_imgb(&imgv);
            fclose(pgm);
        }
        aux = aux->next;
    }
    return true;
}

bool horizontal(imgb *I)
/*
    Espelha horizontalmente a imagem.
*/
{
    if (I->pixel != NULL)
    {
        imgb J = copia_img(I);
        // Loop para percorrer todos os  pixels.
        #pragma omp parallel for
        for (int i = 0; i < I->h; i++)
        {
            // Copia I em J.
            for (int j = 0; j < I->w; j++)
            {
                // Espelha os pixels de I, com base na segunda struct J para evitar que a imagem se espelhe duas vezes.
                I->pixel[i + I->h * j] = J.pixel[i + J.h * (J.w - j - 1)];
            }
        }
        return true;
    }
    else
    {
        printf("Imagem de entrada é NULL.");
        return false;
    }
}

bool horizontal_lista(dimg I)
/*
    Gera o loop para espelhar horizontalmente todas as imagens da lista.
*/
{
    img aux = I->first;
    while (aux != NULL)
    {
        // Definindo o novo nome do arquivo com a tag "_inversa".
        char novo_nome[61]; // Ajuste o tamanho conforme necessário.
        strcpy(novo_nome, aux->nome);
        char *ponto = strrchr(novo_nome, '.'); // Busca pelo ponto no nome do arquivo.
        if (ponto != NULL)
            *ponto = '\0';                    // Remove a extensão .pgm.
        strcat(novo_nome, "_horizontal.pgm"); // Adiciona a tag.

        FILE *pgm = fopen(aux->nome, "r");
        if (pgm == NULL)
        {
            printf("\n Erro: Erro ao abrir o arquivo %s\n", aux->nome);
        }
        else
        {
            imgb imgh = read_img(pgm);

            if (!horizontal(&imgh))
            {
                return false;
            }

            write_imgb(novo_nome, &imgh);
            free_imgb(&imgh);
            fclose(pgm);
        }
        aux = aux->next;
    }
    return true;
}