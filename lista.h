#define MAX_LIST_SIZE 80

struct Img
{
    char nome[50];    // nome da imagem
    struct Img *next; // aponta para a próxima imagem
};

typedef struct Img *img;

typedef struct
{
    img first; // primeira imagem da lista
    int last;
} dImg;

typedef dImg *dimg;

// Funções Essenciais para as listas.
dimg new_img();
bool del_list(dimg I);
int size(dimg I);
bool empty(dimg I);
bool full(dimg I);
bool print_list(dimg I);
// Funções para obter informações das imagens.
bool pgm(char *imagem);
img pos(dimg I, char *imagem);
// Funções para manipular imagens da lista
bool insertImg(dimg I, char *imagem);
bool removeImg(dimg I, char *imagem, char *filename);
// Funções para gravar e receber dados de/em um arquivo.
bool to_file(char *filename, dimg I);
bool from_file(char *filename, dimg I);