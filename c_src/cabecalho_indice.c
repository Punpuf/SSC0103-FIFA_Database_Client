#include "cabecalho_indice.h"

struct _cabecalho_indice
{
    char status;
};

cabecalho_indice_t *cabecalho_indice_instanciar()
{
    cabecalho_indice_t *cabecalho = malloc(sizeof(cabecalho_indice_t));
    cabecalho->status = CABECALHO_INDICE_STATUS_INCONSISTENTE;
    return cabecalho;
}

void cabecalho_indice_liberar(cabecalho_indice_t **cabecalho)
{
    free(*cabecalho);
    *cabecalho = NULL;
}

char cabecalho_indice_get_status(cabecalho_indice_t *cabecalho)
{
    return cabecalho->status;
}

void cabecalho_indice_set_status(cabecalho_indice_t *cabecalho, char status)
{
    cabecalho->status = status;
}

cabecalho_indice_t *cabecalho_indice_ler(FILE *arquivo)
{
    cabecalho_indice_t *cabecalho = cabecalho_indice_instanciar();

    fread(&cabecalho->status, sizeof(char), 1, arquivo);
    return cabecalho;
}

void cabecalho_indice_escrever(FILE *arquivo, cabecalho_indice_t *cabecalho)
{
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
}