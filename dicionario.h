//
// Created by vitor on 13/05/23.
//

#ifndef UNTITLED43_DICIONARIO_H
#define UNTITLED43_DICIONARIO_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#define remove_k(dict, chave, type) (type) remove_key(dict, chave)
#define search(ptr, chave, type) (type) pesquisa(ptr, chave)

typedef struct obj{

    void *valor;
    void *(*cp_function)(void *);
    int (*hash)(const void *);
    void *(*print_data)(void *);


}obj;


typedef struct key{

    char *chave;
    void *conteudo;
    struct key*prox;

    size_t tam_conteudo;
    void *(*wich_clear_conteudo)(void *);
    void *(*cp_function)(void *);
    int (*hash)(const void *);
}key;

typedef struct dict{

    key ** keys;
    size_t tamanho;
    int quantidade_chaves;

}dict;

typedef struct keys_dict{

    char **mat;
    int lines;
}keys_dict;
void *clear_memory(void *ptr);
key *pesquisa_celula(dict *ptr, char *chave);
void *alocaMemoria(void *ptr,size_t tam);
dict *CriaDicionario(size_t tam);
size_t hash_key(const char *key);
void *pesquisa(dict *ptr, char *key);
int add_key(dict *ptr, char *key, void *valor, size_t tam_valor, void *(*with_clear)(void *), void *(*cp_function)(void *), int (*hash)(const void *));
void *remove_key(dict *ptr,
                 char *key);
int att_value(dict *ptr, char *chave, void *valor, size_t tam_valor, void *(*with_clear)(void *), void *(*cp_function)(void *), int (*hash)(const void *));

void *clear_dicionario(void *ptr);
keys_dict get_keys(dict *ptr);
dict *copy_dict(dict *ptr);
dict *mescla_dicionarios(dict *ptr, dict *ptr1);
int dicts_equality(dict *ptr, dict *ptr2);
void clear_keys(keys_dict *ptr);
key * iterator_keys(dict *ptr);
int hash_dict(dict *ptr);
int hash_int(int *num);
char * add_str(const char *);
#endif //UNTITLED43_DICIONARIO_H
