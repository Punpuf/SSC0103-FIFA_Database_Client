#include "registro.h"

struct _registro
{
    char removido;
    int tamanho_registro;
    long prox;
    int id;
    int idade;
    int tam_nome_jogador;
    char *nome_jogador;
    int tam_nacionalidade;
    char *nacionalidade;
    int tam_nome_clube;
    char *nome_clube;
};

// garante que o tamanho do registro está definido
// se ele não estiver, calcula o tamanho do registro
// com base nos seus campos
void _registro_garantir_tamanho(registro_t *registro);

// escreve um campo de tamanho variável do tipo string
// ele também cuida dos casos onde a string é nula
void _escrever_string(FILE *arquivo, char *string, int tamanho);

// lê um campo de tamanho variável do tipo string
// ele também cuida dos casos onde a string é nula
char *_ler_string(FILE *arquivo, int *out_tamanho);

registro_t *registro_instanciar(int id, int idade, char *nome_jogador, int tam_nome_jogador, char *nacionalidade, int tam_nacionalidade, char *nome_clube, int tam_nome_clube)
{
    registro_t *registro = malloc(sizeof(registro_t));
    registro_set_removido(registro, false);
    registro->prox = -1;
    registro->id = id;
    registro->idade = idade;
    registro->tamanho_registro = -1;
    registro_set_nome_jogador(registro, nome_jogador, tam_nome_jogador);
    registro_set_nacionalidade(registro, nacionalidade, tam_nacionalidade);
    registro_set_nome_clube(registro, nome_clube, tam_nome_clube);

    return registro;
}

void registro_liberar(registro_t **registro)
{
    registro_t *reg = *registro;

    if (reg == NULL)
        return;
    // só libera a memória de campos não nulos
    if (reg->nome_jogador != NULL)
        free(reg->nome_jogador);
    if (reg->nacionalidade != NULL)
        free(reg->nacionalidade);
    if (reg->nome_clube != NULL)
        free(reg->nome_clube);

    free(reg);
    *registro = NULL;
}

registro_t *registro_ler(FILE *arquivo)
{
    registro_t *registro = registro_instanciar(-1, -1, NULL, 0, NULL, 0, NULL, 0);
    // valor padrão caso não possa ser lido
    registro_set_removido(registro, true);

    fread(&registro->removido, sizeof(char), 1, arquivo);
    fread(&registro->tamanho_registro, sizeof(int), 1, arquivo);
    fread(&registro->prox, sizeof(long), 1, arquivo);

    // se o registro foi logicamente removido,
    // carregar os dados não é necessário,
    // mas talvez seja bom expor uma forma de carregar
    // esses dados de forma explícita
    if (!registro_get_removido(registro))
    {
        fread(&registro->id, sizeof(int), 1, arquivo);
        fread(&registro->idade, sizeof(int), 1, arquivo);

        registro->nome_jogador = _ler_string(arquivo, &registro->tam_nome_jogador);
        registro->nacionalidade = _ler_string(arquivo, &registro->tam_nacionalidade);
        registro->nome_clube = _ler_string(arquivo, &registro->tam_nome_clube);
    }

    return registro;
}

void registro_escrever(FILE *arquivo, registro_t *registro)
{
    // garante que o tamanho do registro está definido
    _registro_garantir_tamanho(registro);

    fwrite(&registro->removido, sizeof(char), 1, arquivo);
    fwrite(&registro->tamanho_registro, sizeof(int), 1, arquivo);
    fwrite(&registro->prox, sizeof(long), 1, arquivo);

    if (registro_get_removido(registro))
        return;

    fwrite(&registro->id, sizeof(int), 1, arquivo);
    fwrite(&registro->idade, sizeof(int), 1, arquivo);

    _escrever_string(arquivo, registro->nome_jogador, registro->tam_nome_jogador);
    _escrever_string(arquivo, registro->nacionalidade, registro->tam_nacionalidade);
    _escrever_string(arquivo, registro->nome_clube, registro->tam_nome_clube);
}

bool registro_get_removido(registro_t *registro)
{
    return registro->removido == '1';
}

void registro_set_removido(registro_t *registro, bool removido)
{
    registro->removido = removido ? '1' : '0';
}

int registro_get_tamanho_registro(registro_t *registro)
{
    _registro_garantir_tamanho(registro);
    return registro->tamanho_registro;
}

void registro_set_tamanho_registro(registro_t *registro, int tamanho)
{
    registro->tamanho_registro = tamanho;
}

long registro_get_prox(registro_t *registro)
{
    return registro->prox;
}

void registro_set_prox(registro_t *registro, long prox)
{
    registro->prox = prox;
}

int registro_get_id(registro_t *registro)
{
    return registro->id;
}

void registro_set_id(registro_t *registro, int id)
{
    registro->id = id;
}

int registro_get_idade(registro_t *registro)
{
    return registro->idade;
}

void registro_set_idade(registro_t *registro, int idade)
{
    registro->idade = idade;
}

char *registro_get_nome_jogador(registro_t *registro, int *out_tamanho)
{
    *out_tamanho = registro->tam_nome_jogador;
    return registro->nome_jogador;
}

void registro_set_nome_jogador(registro_t *registro, char *nome, int tamanho)
{
    registro->tam_nome_jogador = nome == NULL ? 0 : tamanho;
    registro->nome_jogador = nome;
}

char *registro_get_nacionalidade(registro_t *registro, int *out_tamanho)
{
    *out_tamanho = registro->tam_nacionalidade;
    return registro->nacionalidade;
}

void registro_set_nacionalidade(registro_t *registro, char *nacionalidade, int tamanho)
{
    registro->tam_nacionalidade = nacionalidade == NULL ? 0 : tamanho;
    registro->nacionalidade = nacionalidade;
}

char *registro_get_nome_clube(registro_t *registro, int *out_tamanho)
{
    *out_tamanho = registro->tam_nome_clube;
    return registro->nome_clube;
}

void registro_set_nome_clube(registro_t *registro, char *nome_clube, int tamanho)
{
    registro->tam_nome_clube = nome_clube == NULL ? 0 : tamanho;
    registro->nome_clube = nome_clube;
}

void _registro_garantir_tamanho(registro_t *registro)
{
    if (registro->tamanho_registro != -1)
        return;
    registro->tamanho_registro = sizeof(registro->removido) + sizeof(registro->tamanho_registro) + sizeof(registro->prox) + sizeof(registro->id) + sizeof(registro->idade) + sizeof(registro->tam_nome_jogador) + registro->tam_nome_jogador + sizeof(registro->tam_nacionalidade) + registro->tam_nacionalidade + sizeof(registro->tam_nome_clube) + registro->tam_nome_clube;
}

void _escrever_string(FILE *arquivo, char *string, int tamanho)
{
    fwrite(&tamanho, sizeof(int), 1, arquivo);
    if (string != NULL)
    {
        fwrite(string, sizeof(char), tamanho, arquivo);
    }
}

char *_ler_string(FILE *arquivo, int *out_tamanho)
{
    int tamanho = 0;
    fread(&tamanho, sizeof(int), 1, arquivo);
    *out_tamanho = tamanho;

    // strings vazias são tratadas como nulas
    if (tamanho == 0)
    {
        return NULL;
    }

    char *string = malloc(sizeof(char) * tamanho);
    fread(string, sizeof(char), tamanho, arquivo);

    return string;
}
