typedef struct
{
    int h, w; // Altura e largura da imagem.
    int cinza; // Quantidade de tons de cinza.
    int *pixel; // Matriz de pixels.
} imgb;

// Funções essenciais de manipulação de imagem.
imgb read_img(FILE *fimg);
imgb copia_img(imgb *I);
void free_imgb(imgb *I);
bool limiar(imgb *I, int lim);
bool limiar_lista(dimg I, int lim);
void write_imgb(char *filename, imgb *I);
// Funções de Loop e Edição das Imagens.
bool inversa(imgb *I);
bool inversa_lista(dimg I);
bool vertical(imgb *I);
bool vertical_lista(dimg I);
bool horizontal(imgb *I);
bool horizontal_lista(dimg I);


