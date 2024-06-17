#ifndef CABECALHO_H
#define CABECALHO_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// olhar o arquivo .c se quiser verificar a paridade do valor
// os defines utilizados no arquivo .c não foram movidos
// para cá para que eles não "poluam" o escopo global
#define TAMANHO_CABECALHO 25
#define CABECALHO_STATUS_BOM '1'
#define CABECALHO_STATUS_INCONSISTENTE '0'

// a struct do cabecalho ajuda a gerenciar o cabeçalho do arquivo
// os dados são carregados na memória, então
// é necessário chamar cabecalho_escrever para
// efetivar mudanças no arquivo
typedef struct _cabecalho cabecalho_t;

// cria um novo objeto que gerencia o cabeçalho do arquivo
// esta função não lê nem escreve no arquivo
cabecalho_t *cabecalho_instanciar();

// libera a memória alocada para o objeto que gerencia o cabeçalho
void cabecalho_liberar(cabecalho_t **cabecalho);

// carrega os dados do cabeçalho do arquivo
// na posição atual do cursor do arquivo
cabecalho_t *cabecalho_ler(FILE *arquivo);

// escreve os dados do cabeçalho no arquivo
// na posição atual do cursor do arquivo
void cabecalho_escrever(FILE *arquivo, cabecalho_t *cabecalho);

// obtém o status do cabeçalho
//
// retorna CABECALHO_STATUS_BOM se o cabeçalho está consistente
// retorna CABECALHO_STATUS_INCONSISTENTE caso o contrário
char cabecalho_get_status(cabecalho_t *cabecalho);
void cabecalho_set_status(cabecalho_t *cabecalho, char status);

// obtem o topo da lista de registros removidos
// o valor retornado é o byte offset desse registro
long cabecalho_get_topo(cabecalho_t *cabecalho);
void cabecalho_set_topo(cabecalho_t *cabecalho, long topo);

// retorna o próximo byte offset livre para a adição de novos registros
long cabecalho_get_prox_byte_offset(cabecalho_t *cabecalho);
void cabecalho_set_prox_byte_offset(cabecalho_t *cabecalho, long prox_byte_offset);

// retorna o número de registros utilizáveis no arquivo
int cabecalho_get_nro_reg_arq(cabecalho_t *cabecalho);
void cabecalho_set_nro_reg_arq(cabecalho_t *cabecalho, int nro_reg_arq);

// retorna o número de registros logicamente removidos dentro do arquivo
int cabecalho_get_nro_reg_rem(cabecalho_t *cabecalho);
void cabecalho_set_nro_reg_rem(cabecalho_t *cabecalho, int nro_reg_rem);

#endif