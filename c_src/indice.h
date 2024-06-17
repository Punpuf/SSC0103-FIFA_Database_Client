#ifndef INDICE_H
#define INDICE_H

#include "cabecalho_indice.h"
#include "registro_indice.h"

// índice sobre os ids dos registros
// carregado diretamente na memória durante
// a execução do programa
// somenta acessa o disco ao carregar o registro
// e ao salvar ele
typedef struct _indice indice_t;

indice_t *indice_instanciar(cabecalho_indice_t *cabecalho, registro_indice_t **registros, int nro_regitros, int capacidade);

// libera a memória alocada para o registro
// não modifica o arquivo de registro
void indice_liberar(indice_t **indice);

// tenta carragar os dados do arquivo de registro
// retorna NULL se o índice estiver inconsistente
indice_t *indice_ler(FILE *arquivo);

// salva o registro no arquivo
void indice_escrever(indice_t *indice, FILE *arquivo);

// define os registros do índice a serem utilizados
// chamar esta função toma posse do vetor de registros
// e ordena o vetor com base no id de cada registro
void indice_set_registros(indice_t *indice, registro_indice_t **registros, int nro_registros, int capacidade);

// insere um registro de índice no índice
void indice_inserir_registro(indice_t *indice, registro_indice_t *registro);

// insere um registro de índice com o id especificado no índice
void indice_remover_registro(indice_t *indice, int id);

// busca um registro de índice com o id especificado
// retorna NULL se o registro não foi encontrado
registro_indice_t *indice_buscar_id(indice_t *indice, int id);

#endif