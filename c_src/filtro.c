#include "filtro.h"

// campos nulos não são usados para filtrar os registros
struct _filtro
{
    int id;
    int idade;
    char *nome_jogador;
    int tamanho_nome;
    char *nacionalidade;
    int tamanho_nacionalidade;
    char *nome_clube;
    int tamanho_nome_clube;
};

// compara s1 (string do filtro) com s2 (string do registro)
// retornando true se elas são iguais
bool _strings_sao_iguais(char *s1, int t1, char *s2, int t2);

filtro_t *filtro_instanciar()
{
    filtro_t *filtro = malloc(sizeof(filtro_t));
    // definindo todos os campos como nulos
    // assim qualquer registro passa pelo filtro
    filtro->id = -1;
    filtro->idade = -1;
    filtro->nome_jogador = NULL;
    filtro->tamanho_nome = 0;
    filtro->nacionalidade = NULL;
    filtro->tamanho_nacionalidade = 0;
    filtro->nome_clube = NULL;
    filtro->tamanho_nome_clube = 0;
    return filtro;
}

void filtro_liberar(filtro_t **filtro)
{
    filtro_t *f = *filtro;

    // as strings nem sempre são definidas
    if (f->nome_jogador != NULL)
        free(f->nome_jogador);
    if (f->nacionalidade != NULL)
        free(f->nacionalidade);
    if (f->nome_clube != NULL)
        free(f->nome_clube);
    free(f);
    *filtro = NULL;
}

bool filtro_tem_id(filtro_t *filtro)
{
    return filtro->id != -1;
}

int filtro_get_id(filtro_t *filtro)
{
    return filtro->id;
}

void filtro_set_id(filtro_t *filtro, int id)
{
    filtro->id = id;
}

bool filtro_tem_idade(filtro_t *filtro)
{
    return filtro->idade != -1;
}

int filtro_get_idade(filtro_t *filtro)
{
    return filtro->idade;
}

void filtro_set_idade(filtro_t *filtro, int idade)
{
    filtro->idade = idade;
}

bool filtro_tem_nome_jogador(filtro_t *filtro)
{
    return filtro->nome_jogador != NULL;
}

char *filtro_get_nome_jogador(filtro_t *filtro, int *out_tamanho)
{
    *out_tamanho = filtro->tamanho_nome;
    return filtro->nome_jogador;
}

void filtro_set_nome_jogador(filtro_t *filtro, char *nome, int tamanho)
{
    // é responsabilidade do filtro
    // liberar a memória da string anterior
    if (filtro->nome_jogador != NULL)
        free(filtro->nome_jogador);
    filtro->nome_jogador = nome;
    filtro->tamanho_nome = tamanho;
}

bool filtro_tem_nacionalidade(filtro_t *filtro)
{
    return filtro->nacionalidade != NULL;
}

char *filtro_get_nacionalidade(filtro_t *filtro, int *out_tamanho)
{
    *out_tamanho = filtro->tamanho_nacionalidade;
    return filtro->nacionalidade;
}

void filtro_set_nacionalidade(filtro_t *filtro, char *nacionalidade, int tamanho)
{
    // é responsabilidade do filtro
    // liberar a memória da string anterior
    if (filtro->nacionalidade != NULL)
        free(filtro->nacionalidade);

    filtro->nacionalidade = nacionalidade;
    filtro->tamanho_nacionalidade = tamanho;
}

bool filtro_tem_nome_clube(filtro_t *filtro)
{
    return filtro->nome_clube != NULL;
}

char *filtro_get_nome_clube(filtro_t *filtro, int *out_tamanho)
{
    *out_tamanho = filtro->tamanho_nome_clube;
    return filtro->nome_clube;
}

void filtro_set_nome_clube(filtro_t *filtro, char *nome_clube, int tamanho)
{
    // é responsabilidade do filtro
    // liberar a memória da string anterior
    if (filtro->nome_clube != NULL)
        free(filtro->nome_clube);

    filtro->nome_clube = nome_clube;
    filtro->tamanho_nome_clube = tamanho;
}

bool filtro_se_aplica(filtro_t *filtro, registro_t *registro)
{
    // retorna falso caso qualquer uma das restrições do
    // filtro rejeite o registro
    if (filtro_tem_id(filtro) && filtro->id != registro_get_id(registro))
        return false;

    if (filtro_tem_idade(filtro) && filtro->idade != registro_get_idade(registro))
        return false;

    if (filtro_tem_nome_jogador(filtro))
    {
        int tamanho;
        char *nome = registro_get_nome_jogador(registro, &tamanho);
        if (!_strings_sao_iguais(nome, tamanho, filtro->nome_jogador, filtro->tamanho_nome))
            return false;
    }

    if (filtro_tem_nacionalidade(filtro))
    {
        int tamanho;
        char *nacionalidade = registro_get_nacionalidade(registro, &tamanho);
        if (!_strings_sao_iguais(nacionalidade, tamanho, filtro->nacionalidade, filtro->tamanho_nacionalidade))
            return false;
    }

    if (filtro_tem_nome_clube(filtro))
    {
        int tamanho;
        char *nome_clube = registro_get_nome_clube(registro, &tamanho);
        if (!_strings_sao_iguais(nome_clube, tamanho, filtro->nome_clube, filtro->tamanho_nome_clube))
            return false;
    }

    return true;
}

bool _strings_sao_iguais(char *s1, int t1, char *s2, int t2)
{
    // strings iguais têm tamanhos iguais
    if (t1 != t2)
        return false;

    // só s2 pode ser NULL, já que antes de usar
    // s1, verificamos se o filtro possui o campo
    if (s2 == NULL)
        return false;

    // memcmp compara o conteúdo das duas strings
    return memcmp(s1, s2, t1) == 0;
}
