#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "lista.h"

dimg new_img()
// Cria a lista com um elemento inicial
{
    dimg I = malloc(sizeof(dImg)); // Armazena o ponteiro para o endereço alocado
    I->last = -1;
    I->first = NULL; // É atribuido como o primeiro elemento da lista
    return I;
}

bool del_list(dimg I)
{
    if (I != NULL)
    {
        img current = I->first;
        while (current != NULL)
        {
            // cuidado para não perder o próximo elemento
            I->first = current->next;
            I->last--;
            free(current);
            // Atualiza current
            current = I->first;
        }
        free(I);
        return true;
    }
    return false;
}

int size(dimg I)
{
    if (I != NULL)
        return I->last + 1;
    return 0;
}

bool empty(dimg I)
{
    return size(I) == 0;
}

bool full(dimg I)
{
    return size(I) == MAX_LIST_SIZE;
}

bool print_list(dimg I)
/*
    Imprime elementos da lista.
*/
{
    if (empty(I))
    {
        printf("\n Erro: Lista vazia.\n");
        return false;
    }

    for (img i = I->first; i != NULL; i = i->next)
        printf("%s ", i->nome);
    return true;
}

bool pgm(char *imagem)
{
    if (strcmp(imagem + strlen(imagem) - strlen(".pgm"), ".pgm"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int search(dimg I, char *imagem)
// Procura uma imagem na lista e retorna a posição
{
    int pos = -1;

    img aux = I->first;
    while (aux != NULL)
    {
        pos += 1;
        if (strcmp(aux->nome, imagem) == 0)
        {
            return pos;
        }
        aux = aux->next;
    }
    pos = -1;
    return pos;
}

img pos(dimg I, char *imagem)
// Define a posição anterior à qual o novo elemento será inserido
{
    img aux = I->first;
    if (strcmp(aux->nome, imagem) > 0)
    {
        return NULL;
    }
    while (aux->next != NULL)
    {
        if (strcmp(aux->next->nome, imagem) > 0) // carácter da primeira String for maior que da segunda String B > A
        {
            return aux;
        }
        aux = aux->next;
    }
    return aux; // Aponta a ultima posição do vetor (Caso especial)
}

bool insertImg(dimg I, char *imagem)
// Insere um elemento na lista
{
    if (pgm(imagem))
    {
        printf("\n Erro: Insira uma imagem com a extensão '.pgm'. \n");
        return false;
    }
    if (full(I))
    {
        printf("\n Erro: Lista está cheia.\n ");
        return false;
    }

    if (search(I, imagem) != -1)
    {
        printf("\n Erro: Imagem já inserida na lista. \n");
        return false;
    }

    img novo = malloc(sizeof(struct Img)); // Aloca o espaço da nova imagem
    strcpy(novo->nome, imagem);
    novo->next = NULL;

    if (I->last == -1)
    {
        I->first = novo;
        I->last++;
        return true;
    }

    img aux = I->first;

    if (size(I) == 1)
    {                                            // Caso especial, um unico elemento
        if (strcmp(I->first->nome, imagem) <= 0) // Se o primeiro elemento for menor ou igual a zero, alfabeticamente ele vem antes
        {
            aux->next = novo; // O antigo ultimo item recebe como próximo elemento o novo img
        }
        else
        // O novo elemento entra na primeira posição
        {
            novo->next = aux;
            aux->next = NULL;
            I->first = novo;
        }
        I->last++;
        return true;
    }

    img pivo = pos(I, imagem);

    if (pivo == NULL)
    {
        novo->next = I->first;
        I->first = novo;
        return true;
    }
    if (pivo->next == NULL) // Caso especial, o elemento é inserido no final da lista
    {
        pivo->next = novo;
        I->last++;
        return true;
    }

    novo->next = pivo->next; // Caso geral, o elemento é inserido entre o pivo e seu próximo elemento
    pivo->next = novo;
    I->last++;

    return true;
}

bool removeImg(dimg I, char *imagem, char *filename)
/*
    Remove item uma imagem.
*/
{
    if (empty(I))
    {
        printf("\n Erro: Lista vazia.\n");
        return false;
    }

    int pos = search(I, imagem);
    img aux = NULL;
    I->last--;

    if (pos == -1)
    {
        printf("Erro: Arquivo não pertence a lista.");
        return false;
    }

    if (pos == 0) // Caso particular, primeiro elemento
    {
        aux = I->first->next; // aux recebe o segundo elemento
        free(I->first);       // libera a memoria do first
        I->first = aux;       // o novo first recebe o antigo segundo elemento
        return true;
    }

    aux = I->first;
    img prev = NULL; // armazena anterior
    int k = 0;
    while (k < pos)
    {
        prev = aux;
        k++;
        aux = aux->next;
    }
    prev->next = aux->next;
    free(aux);
    return true;
}

bool to_file(char *filename, dimg I)
/*
    Escreve conteúdo de I para um arquivo.
*/
{
    if (empty(I))
    {
        printf("\n Erro: Lista vazia.\n");
        return false;
    }

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        printf("Erro: Não foi possível criar arquivo.");
        fclose(fp);
        return false;
    }

    img aux = I->first;
    while (aux != NULL)
    {
        fwrite(aux, sizeof(struct Img), 1, fp);
        aux = aux->next;
    }
    fclose(fp);
    return true;
}

bool from_file(char *filename, dimg I)
/*
    Lê uma L de um arquivo.
*/
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Erro: Não foi possível abrir arquivo.");
        fclose(fp);
        return false;
    }

    img aux = (img)malloc(sizeof(struct Img));
    img last_img = NULL;
    while (fread(aux, sizeof(struct Img), 1, fp))
    {
        I->last++;
        if (I->first == NULL)
        {
            I->first = malloc(sizeof(struct Img));
            last_img = I->first;
        }
        else
        {
            last_img->next = malloc(sizeof(struct Img));
            last_img = last_img->next;
        }
        strcpy(last_img->nome, aux->nome);
        last_img->next = NULL;
    }
    fclose(fp);
    return true;
}
