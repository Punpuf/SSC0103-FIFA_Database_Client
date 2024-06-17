#include "registro_indice.h"

struct _registro_indice
{
    int id;
    long byte_offset;
};

registro_indice_t *registro_indice_instanciar(int id, long byte_offset)
{
    registro_indice_t *registro = malloc(sizeof(registro_indice_t));
    registro->id = id;
    registro->byte_offset = byte_offset;
    return registro;
}

void registro_indice_liberar(registro_indice_t **registro)
{
    free(*registro);
    *registro = NULL;
}

registro_indice_t *registro_indice_ler(FILE *arquivo)
{
    // lê os dados e cria a instância
    int id;
    long byte_offset;
    fread(&id, sizeof(int), 1, arquivo);
    fread(&byte_offset, sizeof(long), 1, arquivo);
    return registro_indice_instanciar(id, byte_offset);
}

void registro_indice_escrever(FILE *arquivo, registro_indice_t *registro)
{
    fwrite(&registro->id, sizeof(int), 1, arquivo);
    fwrite(&registro->byte_offset, sizeof(long), 1, arquivo);
}

int registro_indice_get_id(registro_indice_t *registro)
{
    return registro->id;
}

void registro_indice_set_id(registro_indice_t *registro, int id)
{
    registro->id = id;
}

long registro_indice_get_byte_offset(registro_indice_t *registro)
{
    return registro->byte_offset;
}

void registro_indice_set_byte_offset(registro_indice_t *registro, long byte_offset)
{
    registro->byte_offset = byte_offset;
}
