#include "dicionario.h"
key *pesquisa_celula(dict *ptr, char *chave);

char *add_str(const char *name){

    char *n = alocaMemoria(NULL, sizeof(char)* strlen(name)+sizeof(char));
    strcpy(n, name);
    return n;
}
void *clear_memory(void *ptr){
    free(ptr);
    return NULL;
}

size_t hash_key(const char *key){

    size_t sum = 0;
    while(*key != '\0'){

        sum +=((*key)<<10)/23;
        key++;
    }
    return sum;
}

void *alocaMemoria(void *ptr, size_t tam){
    void *ptrRealloc = realloc(ptr, tam);

    if(!ptrRealloc){
        fprintf(stderr, "ERRO: Memoria indisponivel!\n");
        exit(1);
    }
    return ptrRealloc;
}
dict *CriaDicionario(size_t tam){

    if(tam  == 0){
        return NULL;
    }
    dict *new = alocaMemoria(NULL, sizeof(dict));
    *new = (dict){
                calloc(tam*sizeof(key*),1),
                tam,
                0
                };

    if(new->keys == NULL){
        fprintf(stderr, "ERRO: Memoria indisponivel!\n");
        exit(1);
    }

    return new;
}

void *pesquisa(dict *ptr, char *chave){
    size_t indice = hash_key(chave)%ptr->tamanho;

    key *aux = ptr->keys[indice];
    if(aux == NULL){
        return NULL;
    }

    while(aux->prox != NULL && strcmp(aux->chave, chave)){
        aux = aux->prox;
    }
    if(!strcmp(aux->chave, chave)){
        return aux->conteudo;
    }
    return NULL;
}

int add_key(dict *ptr, char *chave, void *valor, size_t tam_valor, void *(*with_clear)(void *), void *(*cp_function)(void *), int (*hash)( const void *)) {

    if (pesquisa(ptr, chave)) {
        return 0;
    }
    size_t indice = hash_key(chave) % ptr->tamanho;

    if (ptr->keys[indice] == NULL) {

        ptr->keys[indice] = alocaMemoria(NULL, sizeof(key));
        ptr->keys[indice]->chave = add_str(chave);

        if (cp_function == NULL) {
            ptr->keys[indice]->conteudo = alocaMemoria(NULL, tam_valor);
            memcpy(ptr->keys[indice]->conteudo, valor, tam_valor);
        } else {

            ptr->keys[indice]->conteudo = cp_function(valor);
            //free(valor);
        }
        ptr->keys[indice]->prox = NULL;
        ptr->keys[indice]->wich_clear_conteudo = with_clear == NULL ? clear_memory : with_clear;
        ptr->keys[indice]->tam_conteudo = tam_valor;
        ptr->keys[indice]->cp_function = cp_function;
        ptr->keys[indice]->hash = hash == NULL? hash_key:hash;
        ptr->quantidade_chaves++;

        return 1;
    }
    key *aux = ptr->keys[indice];
    while (aux->prox != NULL) {
        aux = aux->prox;

    }
    aux->prox = alocaMemoria(NULL, sizeof(key));
    aux->prox->chave = alocaMemoria(NULL, sizeof(char) * strlen(chave) + sizeof(char));
    strcpy(aux->prox->chave, chave);

    if (cp_function == NULL) {
        aux->prox->conteudo = alocaMemoria(NULL, tam_valor);
        memcpy(aux->prox->conteudo, valor, tam_valor);
    } else{
        aux->prox->conteudo = cp_function(aux->prox->conteudo);
    }

    aux->prox->tam_conteudo = tam_valor;
    aux->prox->prox = NULL;
    aux->prox->wich_clear_conteudo = with_clear == NULL? clear_memory:with_clear;
    aux->prox->cp_function = cp_function;
    aux->prox->hash =  hash == NULL? hash_key:hash;
    ptr->quantidade_chaves++;
    return 1;
}
void *remove_key(dict *ptr, char *chave){

    if(!pesquisa(ptr, chave)){
        return NULL;
    }

    size_t indice = hash_key(chave)%ptr->tamanho;

    if(!strcmp(ptr->keys[indice]->chave, chave)){
        key *aux = ptr->keys[indice];
        ptr->keys[indice] = ptr->keys[indice]->prox;
        void *retorno;
        if (aux->cp_function ==NULL) {
            retorno = alocaMemoria(NULL, aux->tam_conteudo);
            memmove(retorno, aux->conteudo, aux->tam_conteudo);
        }else{
            retorno = aux->cp_function(aux->conteudo);
        }
        aux->conteudo = aux->wich_clear_conteudo(aux->conteudo);
        aux = clear_memory(aux);
        ptr->quantidade_chaves--;
        return retorno;
    }
    key *aux = ptr->keys[indice];
    while(aux->prox!=NULL && strcmp(aux->prox->chave, chave)){
        aux = aux->prox;
    }

    if(aux->prox!= NULL){
        key *aux3 = aux->prox;
        aux->prox = aux->prox->prox;
        void *retorno = alocaMemoria(NULL, aux3->tam_conteudo);
        memmove(retorno, aux3->conteudo, aux3->tam_conteudo);
        aux3->conteudo = aux3->wich_clear_conteudo(aux3->conteudo);
        aux3 = clear_memory(aux3);
        ptr->quantidade_chaves--;
        return retorno;


    }

    return NULL;

}
void *clear_key(key*ptr){

    if(ptr->prox != NULL){
        ptr->prox = clear_key(ptr->prox);


    }
    ptr->conteudo = ptr->wich_clear_conteudo(ptr->conteudo);
    return clear_memory(ptr);
}
void *clear_dicionario(void *ptrG){

    dict *ptr = (dict*)ptrG;

    for(int i = 0; i<ptr->tamanho; i++){
        if(ptr->keys[i]!= NULL){

            ptr->keys[i] = clear_key(ptr->keys[i]);
        }

    }
    ptr->keys = clear_memory(ptr->keys);
    ptr->tamanho =0;
    ptr->quantidade_chaves = 0;
    ptr = clear_memory(ptr);
    return NULL;
}
void add_keys_indice(key *ptr, keys_dict *mat){

    while(ptr != NULL){
        mat->mat = alocaMemoria(mat->mat, sizeof(char*)*(mat->lines+1));
        mat->mat[mat->lines] = add_str(ptr->chave);
        mat->lines++;

        ptr = ptr->prox;
    }

}
keys_dict get_keys(dict *ptr){
    keys_dict names = {NULL, 0};

    for (int i = 0; i<ptr->tamanho; i++){

            add_keys_indice(ptr->keys[i], &names);
    }
    return names;



}
int att_value(dict *ptr, char *chave, void *valor, size_t tam_valor, void *(*with_clear)(void *), void *(*cp_function)(void *), int (*hash)(const void *)){

    if(!pesquisa(ptr, chave)){
        return 0;
    }

    size_t indice = hash_key(chave)%ptr->tamanho;

    key *aux = ptr->keys[indice];

    while(strcmp(aux->chave, chave)){
        aux = aux->prox;
    }
    aux->conteudo = aux->wich_clear_conteudo(aux->conteudo);
    if(cp_function == NULL) {
        aux->conteudo = alocaMemoria(NULL, tam_valor);
        memmove(aux->conteudo, valor, tam_valor);
    }else{
        aux->conteudo = cp_function(valor);
    }
    aux->wich_clear_conteudo = with_clear;
    aux->cp_function = cp_function;
    aux->tam_conteudo = tam_valor;
    aux->hash = hash;
    return 1;




}
key *pesquisa_celula(dict *ptr, char *chave){

    if(!pesquisa(ptr, chave)){
        return NULL;
    }

    size_t indice = hash_key(chave)%ptr->tamanho;

    key *aux = ptr->keys[indice];

    while(strcmp(aux->chave, chave)){
        aux= aux->prox;
    }
    return aux;
}
dict* copy_dict(dict *ptr){

    dict *new = CriaDicionario(ptr->tamanho);

    keys_dict keys = get_keys(ptr);
    key *aux;
    for(int i = 0; i<keys.lines; i++){

        aux = pesquisa_celula(ptr, keys.mat[i]);
        add_key(new, keys.mat[i], aux->conteudo, aux->tam_conteudo, aux->wich_clear_conteudo, aux->cp_function, aux->hash);


    }
    clear_keys(&keys);

    return new;

}
dict *mescla_dicionarios(dict *ptr, dict *ptr1){
    dict *new = CriaDicionario(ptr->tamanho > ptr1->tamanho?ptr->tamanho:ptr1->tamanho);

    keys_dict keys = get_keys(ptr);
    keys_dict  keys2 = get_keys(ptr1);

    key *aux;
    for(int i = 0;i<keys.lines; i++){

        aux = pesquisa_celula(ptr, keys.mat[i]);
        add_key(new, keys.mat[i], aux->conteudo, aux->tam_conteudo, aux->wich_clear_conteudo, aux->cp_function, aux->hash);

    }
    for(int i = 0; i<keys2.lines; i++){
        aux = pesquisa_celula(ptr1, keys2.mat[i]);
        add_key(new, keys2.mat[i], aux->conteudo, aux->tam_conteudo, aux->wich_clear_conteudo, aux->cp_function, aux->hash);
    }
    clear_keys(&keys);
    clear_keys(&keys2);
    return new;
}
int comparaBytes(void *ptr, void *ptr2, size_t size){
    unsigned char *ptrC = ptr;
    unsigned char *ptrC1 = ptr2;

    for(int i = 0; i<size; i++){
        if(ptrC[i]-ptrC1[i]){
            return 0;
        }
    }
    return 1;
}
int dicts_equality(dict *ptr, dict *ptr2){

    keys_dict keys1 = get_keys(ptr);
    keys_dict keys2 = get_keys(ptr2);
    if(keys1.lines != keys2.lines){
        clear_keys(&keys1);
        clear_keys(&keys2);
        return 0;
    }

    for(int i = 0; i<keys1.lines; i++){
        if(strcmp(keys1.mat[i], keys2.mat[i])){
            clear_keys(&keys1);
            clear_keys(&keys2);
            return 0;
        }
    }
    key *aux1;
    key *aux2;

    for(int i = 0; i< keys1.lines; i++){
        aux1 = pesquisa_celula(ptr, keys1.mat[i]);
        aux2 = pesquisa_celula(ptr2, keys2.mat[i]);

        if(aux1->tam_conteudo != aux2->tam_conteudo || aux1->hash(aux1->conteudo) != aux2->hash(aux2->conteudo)){
            clear_keys(&keys1);
            clear_keys(&keys2);
            return 0;
        }


    }
    clear_keys(&keys1);
    clear_keys(&keys2);
    return 1;
}
void clear_keys(keys_dict *ptr){
    for(int i = 0; i<ptr->lines;i++){
        ptr->mat[i] = clear_memory(ptr->mat[i]);

    }
    ptr->mat = clear_memory(ptr->mat);
    ptr->lines = 0;
}
key * iterator_keys(dict *ptr){

    static keys_dict keys;
    static int index = 0;

    if(index == 0){
        keys = get_keys(ptr);
    }
    if(index ==keys.lines){
        clear_keys(&keys);
        index = 0;
        return NULL;
    }
    key *aux = pesquisa_celula(ptr, keys.mat[index]);
    index++;

    return aux;



}
int hash_dict(dict *ptr){
    key *aux = iterator_keys(ptr);
    size_t soma=0;
    while(aux != NULL){
        soma += aux->hash(aux->conteudo);
        aux = iterator_keys(ptr);
    }
}
int hash_int(int *num){
   unsigned char *ptr = (char *)num;

   size_t soma = 0;

   for(int i = 0; i<sizeof(int); i++){
       soma+=((*ptr) << 4)/23;
       ptr++;
   }
   return soma;
}