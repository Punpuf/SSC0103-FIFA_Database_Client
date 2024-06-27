#include "indice.h"

struct _indice
{
    cabecalho_indice_t *cabecalho;
    registro_indice_t **registros;
    int capacidade;
    int nro_registros;
};

// ordena os registros do índice com base no critério de comparação interno
void _ordenar_registros(indice_t *indice);

// compara dois registros, retornando um valor negativo se a < b
// um valor positivo se a > b e 0 se a == b
int _comparar_registros(registro_indice_t *a, registro_indice_t *b);

// realiza busca binária no índice para encontrar um registro
// equivalente ao registro fornecido, utilizando a função de comparação interna.
//
// retorna o índice do registro no vetor de registros ou -1 se não encontrado
int _buscar_registro(indice_t *indice, registro_indice_t *alvo);

indice_t *indice_instanciar(cabecalho_indice_t *cabecalho, registro_indice_t **registros, int nro_regitros, int capacidade)
{
    indice_t *indice = malloc(sizeof(indice_t));
    indice->cabecalho = cabecalho;
    indice->registros = registros;
    indice->nro_registros = nro_regitros;
    indice->capacidade = capacidade;
    return indice;
}

void indice_liberar(indice_t **indice)
{
    indice_t *i = *indice;
    if (i->cabecalho != NULL)
        free(i->cabecalho);

    if (i->registros != NULL)
    {
        for (int j = 0; j < i->nro_registros; j++)
        {
            registro_indice_liberar(&i->registros[j]);
        }
        free(i->registros);
    }

    free(*indice);
    *indice = NULL;
}

indice_t *indice_ler(FILE *arquivo, int nro_registros)
{
    if (ftell(arquivo) != 0)
    {
        fseek(arquivo, 0, SEEK_SET);
    }

    cabecalho_indice_t *cabecalho = cabecalho_indice_ler(arquivo);
    char status = cabecalho_indice_get_status(cabecalho);

    // aborta se o arquivo de índice não está consistente
    if (status == CABECALHO_INDICE_STATUS_INCONSISTENTE)
    {
        cabecalho_indice_liberar(&cabecalho);
        return NULL;
    }

    long inicio = ftell(arquivo);
    fseek(arquivo, 0, SEEK_END);
    long fim = ftell(arquivo);

    int capacidade = nro_registros;
    registro_indice_t **registros = malloc(capacidade * sizeof(registro_indice_t *));

    long offset = inicio;

    fseek(arquivo, inicio, SEEK_SET);
    // lê todos os registros de índice no arquivo
    for (int i = 0; i < nro_registros; i++)
    {
        registros[i] = registro_indice_ler(arquivo);
        offset += TAMANHO_REGISTRO_INDICE;
    }

    indice_t *indice = indice_instanciar(cabecalho, registros, nro_registros, capacidade);

    return indice;
}

void indice_escrever(indice_t *indice, FILE *arquivo)
{
    cabecalho_indice_t *cabecalho = indice->cabecalho;
    // indica que o arquivo está sob modificação
    cabecalho_indice_set_status(cabecalho, CABECALHO_INDICE_STATUS_INCONSISTENTE);
    fseek(arquivo, 0, SEEK_SET);
    cabecalho_indice_escrever(arquivo, cabecalho);

    // escreve todos os registros de índice
    for (int i = 0; i < indice->nro_registros; i++)
    {
        registro_indice_t *registro = indice->registros[i];
        registro_indice_escrever(arquivo, registro);
    }

    long tamanho = ftell(arquivo);
    // o arquivo está pronto
    cabecalho_indice_set_status(cabecalho, CABECALHO_INDICE_STATUS_BOM);
    fseek(arquivo, 0, SEEK_SET);
    cabecalho_indice_escrever(arquivo, cabecalho);
}

void indice_set_registros(indice_t *indice, registro_indice_t **registros, int nro_registros, int capacidade)
{
    if (indice->registros != NULL)
        free(indice->registros);

    indice->registros = registros;
    indice->nro_registros = nro_registros;
    indice->capacidade = capacidade;
    _ordenar_registros(indice);
}

void indice_inserir_registro(indice_t *indice, registro_indice_t *registro)
{
    // aumenta a capacidade do índice se necessário
    if (indice->capacidade < indice->nro_registros + 1)
    {
        indice->capacidade *= 2;
        indice->registros = realloc(indice->registros, indice->capacidade * sizeof(registro_indice_t *));
    }

    // a posição na qual o novo registro será inserido
    int posicao = indice->nro_registros;

    // desloca todos os registros maiores que o novo registro
    // para a direita, a fim de inserir ele na posição correta
    while (posicao > 0 && _comparar_registros(registro, indice->registros[posicao - 1]) < 0)
    {
        indice->registros[posicao] = indice->registros[posicao - 1];
        posicao--;
    }

    indice->registros[posicao] = registro;
    indice->nro_registros++;
}

void indice_remover_registro(indice_t *indice, int id)
{
    registro_indice_t *registro = registro_indice_instanciar(id, 0);
    int posicao = _buscar_registro(indice, registro);

    registro_indice_liberar(&registro);

    if (posicao == -1)
        return;

    // desloca todos os registros maiores que o novo registro
    // para a esquerda
    // efetivamente removendo o registro do índice
    for (int i = posicao; i < indice->nro_registros - 1; i++)
    {
        indice->registros[i] = indice->registros[i + 1];
    }

    // corrige a quantidade de registros existentes
    indice->nro_registros--;
}

registro_indice_t *indice_buscar_id(indice_t *indice, int id)
{
    // registro temporário para comparação na busca
    registro_indice_t *temp = registro_indice_instanciar(id, 0);

    int posicao = _buscar_registro(indice, temp);
    registro_indice_liberar(&temp);

    if (posicao == -1)
        return NULL;

    return indice->registros[posicao];
}

int _comparar_registros(registro_indice_t *a, registro_indice_t *b)
{
    // o índice é ordenado com base nos ids, então
    // é só comparar os ids dos registros
    return registro_indice_get_id(a) - registro_indice_get_id(b);
}

// algoritmo de merge sort para ordenar os registros
void _ordenar_registros_rec(registro_indice_t **registros, registro_indice_t **aux, int inicio, int fim)
{
    // vetores de tamanho 1 ou menor sempre
    // estão ordenados
    if (fim - inicio <= 1)
    {
        return;
    }

    // ordena as metades do vetor
    int meio = (inicio + fim) / 2;
    _ordenar_registros_rec(registros, aux, inicio, meio);
    _ordenar_registros_rec(registros, aux, meio, fim);

    // faz merge das duas metades,
    // colcando os dados no vetor auxiliar
    int i = inicio;
    int j = meio;
    for (int k = inicio; k < fim; k++)
    {
        // escolhe o menor dos dois registros
        if (i < meio && (j >= fim || _comparar_registros(registros[i], registros[j]) < 0))
        {
            aux[k] = registros[i];
            i++;
        }
        else
        {
            aux[k] = registros[j];
            j++;
        }
    }

    // copia os dados de volta para o vetor original
    for (int k = inicio; k < fim; k++)
    {
        registros[k] = aux[k];
    }
}

void _ordenar_registros(indice_t *indice)
{
    // ponto de entrada para a função recursiva
    registro_indice_t **aux = malloc(indice->nro_registros * sizeof(registro_indice_t *));
    _ordenar_registros_rec(indice->registros, aux, 0, indice->nro_registros);
    free(aux);
}

int _buscar_registro(indice_t *indice, registro_indice_t *alvo)
{
    int inicio = 0;
    int fim = indice->nro_registros;

    while (inicio < fim)
    {
        int meio = (inicio + fim) / 2;
        registro_indice_t *registro = indice->registros[meio];
        int cmp = _comparar_registros(registro, alvo);

        if (cmp == 0) // iguais
        {
            return meio;
        }
        else if (cmp > 0) // registro > alvo
        {
            fim = meio;
        }
        else // registro < alvo
        {
            inicio = meio + 1;
        }
    }

    return -1;
}
