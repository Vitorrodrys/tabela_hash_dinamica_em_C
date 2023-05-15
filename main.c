#include <stdio.h>
#include "dicionario.h"

void printa_oi(){
    printf("oi");
}
typedef struct funcao{

    void (*f)(void);

}funcao;
int main() {


    dict *dicionario = CriaDicionario(500000);

    char *chave = alocaMemoria(NULL, sizeof(char)* strlen("sofia")+sizeof(char)*10);

    funcao a;
    a.f = printa_oi;
    void *f = printa_oi;
    add_key(dicionario, "da um comprimento", &a, sizeof(funcao), clear_memory, NULL, NULL);

    (search(dicionario, "da um comprimento", funcao *))->f();

    add_key(
            dicionario, "outro dicionario", CriaDicionario(60000), sizeof(dict), clear_dicionario, (void *)copy_dict,
            (void *)hash_dict);
    add_key(
            dicionario, "outro dicionario1", CriaDicionario(600051), sizeof(dict), clear_dicionario, (void *)copy_dict,
            (void *)hash_dict);
    add_key(
            search(dicionario, "outro dicionario1", dict*), "outro dicionario3", CriaDicionario(651), sizeof(dict), clear_dicionario, (void *)copy_dict,
            (void *)hash_dict);

    char numero[10];
    int i;
    for( i = 0; i<2000; i++){
        chave = strcpy( chave,"sofia");
        snprintf(numero, sizeof(numero), "%d", i);
        strcat(chave, numero);

        add_key(dicionario, chave, &i, sizeof(int), clear_memory, NULL, (void *)hash_int);

    }
    for (;i<100000;i++){
        chave = strcpy(chave,"vitor");
        snprintf(numero, sizeof(numero), "%d", i);
        strcat(chave, numero);
        add_key(search(dicionario, "outro dicionario", dict *), chave, &i, sizeof(int), clear_memory, NULL,(void *) hash_int);
        add_key(search(dicionario, "outro dicionario1", dict *), chave, &i, sizeof(int), clear_memory, NULL,(void *) hash_int);
        add_key(
                search(dicionario, "outro dicionario1", dict*), chave, &i, sizeof(dict), clear_memory, NULL,
                (void *) hash_int);
    }


    dict *dicionario2 = copy_dict(dicionario);
    add_key(dicionario2, "essa chave com certeza nao tem no outro dicionario", "teste", sizeof(char)*strlen("teste")+sizeof(char), clear_memory, NULL, NULL);
    if(dicts_equality(dicionario, dicionario2)){
        printf("1");
    }



    printf("%d\n", *search(
            search(dicionario, "outro dicionario", dict *), "vitor80000", int *));

    clear_dicionario(dicionario);
    clear_dicionario(dicionario2);


 
    return 0;
}
