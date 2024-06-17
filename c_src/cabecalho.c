#include "cabecalho.h"

#define TIPO_STATUS char
#define TIPO_TOPO long
#define TIPO_PROX_BYTE_OFFSET long
#define TIPO_NRO_REG_ARQ int
#define TIPO_NRO_REG_REM int

struct _cabecalho
{
    TIPO_STATUS status;
    TIPO_PROX_BYTE_OFFSET topo;
    TIPO_TOPO prox_byte_offset;
    TIPO_NRO_REG_ARQ nro_reg_arq;
    TIPO_NRO_REG_REM nro_reg_rem;
};

cabecalho_t *cabecalho_instanciar()
{
    cabecalho_t *cabecalho = malloc(sizeof(cabecalho_t));
    cabecalho->status = CABECALHO_STATUS_INCONSISTENTE;
    cabecalho->topo = -1;
    cabecalho->prox_byte_offset = 0;
    cabecalho->nro_reg_arq = 0;
    cabecalho->nro_reg_rem = 0;
    return cabecalho;
}

void cabecalho_liberar(cabecalho_t **cabecalho)
{
    free(*cabecalho);
    *cabecalho = NULL;
}

cabecalho_t *cabecalho_ler(FILE *arquivo)
{
    cabecalho_t *cabecalho = cabecalho_instanciar();
    fread(&cabecalho->status, sizeof(char), 1, arquivo);
    fread(&cabecalho->topo, sizeof(long), 1, arquivo);
    fread(&cabecalho->prox_byte_offset, sizeof(long), 1, arquivo);
    fread(&cabecalho->nro_reg_arq, sizeof(int), 1, arquivo);
    fread(&cabecalho->nro_reg_rem, sizeof(int), 1, arquivo);
    return cabecalho;
}

void cabecalho_escrever(FILE *arquivo, cabecalho_t *cabecalho)
{
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(long), 1, arquivo);
    fwrite(&cabecalho->prox_byte_offset, sizeof(long), 1, arquivo);
    fwrite(&cabecalho->nro_reg_arq, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nro_reg_rem, sizeof(int), 1, arquivo);
}

char cabecalho_get_status(cabecalho_t *cabecalho)
{
    return cabecalho->status;
}

void cabecalho_set_status(cabecalho_t *cabecalho, char status)
{
    cabecalho->status = status;
}

long cabecalho_get_topo(cabecalho_t *cabecalho)
{
    return cabecalho->topo;
}

void cabecalho_set_topo(cabecalho_t *cabecalho, long topo)
{
    cabecalho->topo = topo;
}

long cabecalho_get_prox_byte_offset(cabecalho_t *cabecalho)
{
    return cabecalho->prox_byte_offset;
}

void cabecalho_set_prox_byte_offset(cabecalho_t *cabecalho, long prox_byte_offset)
{
    cabecalho->prox_byte_offset = prox_byte_offset;
}

int cabecalho_get_nro_reg_arq(cabecalho_t *cabecalho)
{
    return cabecalho->nro_reg_arq;
}

void cabecalho_set_nro_reg_arq(cabecalho_t *cabecalho, int nro_reg_arq)
{
    cabecalho->nro_reg_arq = nro_reg_arq;
}

int cabecalho_get_nro_reg_rem(cabecalho_t *cabecalho)
{
    return cabecalho->nro_reg_rem;
}

void cabecalho_set_nro_reg_rem(cabecalho_t *cabecalho, int nro_reg_rem)
{
    cabecalho->nro_reg_rem = nro_reg_rem;
}
