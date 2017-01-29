#include <stdio.h>
#include <stdlib.h>


//definição de um nó.
typedef struct node{
    int n; 										//Número de chaves
    int *chaves; 						//Vetor de Chaves
    struct node **filhos; 			//Filhos (Vetor de ponteiros)
    struct node *ifilhos;
    int eFolha; 								//0 = não é folha, 1 = é folha
}node;


// Ponteiro para nó que será a raíz da Arvore
typedef struct bTree{
    node *raiz;
}bTree;

//Função criabTree(bTree *t):
//Cria a arvore, alocando espaço para o nó raiz, coloca número de elmentos = 0
void criabTree(bTree *t,int T){
    node *x;

    x = (node*) calloc(1, sizeof(node)); 	//Aloca primeiro nó
    x->chaves = (int*) calloc(2*T, sizeof(int));//Alocando vetor de chaves
    x->filhos = (node*) calloc(2*T+1, sizeof(node));//Alocando vetor de ponteiro iFilhos
    x->n = 0;									//número de elementos = 0
    x->eFolha = 1;								//eFolha = true

    t->raiz = x;								//coloca esse nó como raíz
}

//Função divideFilhobTree(node *x, int i)
//divide o nó de acordo com o parametro i (que diz onde selecionar)
void divideFilhobTree(node *x, int i,int T){
    node *z, *y;
    int j;

    z = (node*) calloc(1, sizeof(node));  	//Aloca espaço para o novo nó
    z->chaves = (int*) calloc(2*T, sizeof(int)); //Alocando vetor de chaves
    z->filhos = (node*) calloc(2*T+1, sizeof(node));//Alocando vetor de ponteiro iFilhos
    y = x->filhos[i];							//troca o filho de X na posição i de lugar

    z->eFolha = y->eFolha;						//se o nó original era folha, o novo também será

    z->n = T-1;									//número de elementos do novo nó = T - 1

    for (j = 1; j < T; j++){                    //Copia as chaves da metade pra frente para o novo nó
            z->chaves[j] = y->chaves[j+T];
    }

    if (!y->eFolha){							//Se o nó não for folha:
        for (j = 1; j <= T; j++){				//Copia os filhos do nó original a partir de T+1 até 2T para o novo nó
            z->filhos[j] = y->filhos[j+T];
        }
    }
    y->n = T-1;									//número de elementos = T-1

    for (j = (x->n)+1; j >i; j-- ){				//desloca os filhos em 1 posiçao
        x->filhos[j+1] = x->filhos[j];
    }

    x->filhos[i+1] = z;							//Filho a direita de X passa a ser o novo nó

    for (j= x->n; j >= i; j--){					//desloca as chaves de x em 1 posição
        x->chaves[j+1] = x->chaves[j];
    }

    x->chaves[i] = y->chaves[T];				//sobe a chave do meio
    x->n = (x->n)+1;							//corrige o número de chaves em X (já que uma chave subiu)
}


//Função void inserebTree(bTree *t, int k)
//Essa função é basicamente o controle da inserção de chaves, "decidindo" se a chave será inserida em um nó existente
//ou se será alocado um novo nó para arrumar a arvore(crescimento da arvore na raiz).
//RETORNO: A função não tem retorno
void inserebTree(bTree *t, int k,int T){
    node *r, *s;

    r = t->raiz; 								//Definimos r como a raiz da arvore
    if (r->n == 2*T - 1){							//Se a raiz tiver o número maximo de elementos:
        s = (node*) malloc(sizeof(node));//Aloca um novo nó s
        s->chaves = (int*) calloc(2*T, sizeof(int));//Alocando vetor de chaves
        s->filhos = (node*) calloc(2*T+1, sizeof(node));//Alocando vetor de ponteiro iFilhos
        s->eFolha = 0;							//Atribui a ele o estado e não folha
        s->n = 0;								//Atribui a ele o número de elementos zero
        s->filhos[1] = r;						//Torna-se a raiz filha desse novo nó
        t->raiz = s;							//Torna-se esse nó a nova raiz da arvore

        divideFilhobTree(s, 1,T);				//Chama-se a função de dividir o nó na posição 1
        inserebTreeNaoCheio(s, k, T);			//Chama-se a função para inserir a chave k no novo nó que nunca estará cheio (nessa etapa)
    }
    else										//Se a raiz não tiver o número maximo de elementos
        inserebTreeNaoCheio(r, k, T);			//Chama-se a função para inserir a chave k em r já que ela não está cheia
}


//Função node* busca_bTree(node *x, int k, int *pos)
//Essa função tem como objetivo buscar uma chave na arvore retornando o nó e a posição em que a chave se encontra
//RETORNO: A função retorna o nó em que a chave foi encontrada(se foi encontrada) e a posição é passada por referência
node* busca_bTree(node *x, int k, int *pos){
    int i = 1;									//Inicializa-se uma variavel index no valor 1 (primeira posição de um nó)

    while(i <= x->n && k > x->chaves[i]){		//Incrementa-se o i até a chave que ele indexe seja maior que a chave a ser inserida...
        i += 1;									//... ou até que se chegue na última posição do vetor
    }

    if (i <= x->n && k == x->chaves[i]){		//Se quando a condição acima parar o elemento for a chave busca:
        *pos = i;								//Retorna-se a posição i por referência
        /*printf("\n********** Elemento encontrado **********\n");
        printf("\t\tNumero: %d\n\t\tPosicao: %d\n", x->chaves[*pos], *pos);
        printf("*****************************************\n");
        */
        return x;								//Retorna-se o nó

    }
    else if (x->eFolha){						//Se não for a chave buscada e o nó for folha
        *pos = -1;								//Retorna-se -1 como posição por referência
        /*printf("\n********* Elemento nao encontrado **********\n");
        */
        return NULL;							//Retorna-se NULL no lugar do nó, já que a chave não foi encontrada e não há filhos para buscar
    }
    else										//Senão busca-se a chave no filho de posição i (já que k seria menor que o elemento seguinte do nó atual)
        return busca_bTree(x->filhos[i], k, pos);//Chama-se a busca no filho de posição indexada i

}


//Função inserebTreeNaoCheio (node *x, int k){
//insere em um nó não cheio
// É a única função que de fato insere chaves na árvore, pois a função inserebTree faz todas as verificações e divisões
// e então chama a função inserebTreeNaoCheio
void inserebTreeNaoCheio (node *x, int k, int T){
    int i, pos;
    i = x->n;									//O index i recebe o número de elementos do nó

    busca_bTree(x, k, &pos);					//Verifica se a chave a ser inserida já não está na árvore

    if (pos == -1){								//Se pos = -1, quer dizer que a chave não existe, então eu a insiro
        if(x->eFolha){							//Se x for folha:
            while (i >= 1 && k < x->chaves[i]){ //Enquanto i for maior ou igual a 1 E a chave a ser inserida menor que a chave na posição i
                x->chaves[i+1] = x->chaves[i];	//Abre espaço pra chave ser inserida na posição correta
                i -=1;
            }
            x->chaves[i+1] = k;					//Coloco a chave na posição correta
			int pos_ins = i+1;
            x->n = (x->n)+1;					//atualiza a quantidade de chaves no nó
            printf ("Inserido %d no no: ", k);
            for (i = 0; i < x->n; i++){			//Imprime todas as chaves contidas no nó onde a chave foi inserida
                printf("%d - ", x->chaves[i+1]);
            }
            printf ("Na posicao: %d \n", pos_ins);


        }
        else {									//Se o nó x não for folha:
            while (i >= 1 && k < x->chaves[i]){ //Enquanto o index for maior ou igual a 1 E a chave a ser inserida menor que a chave na posição i
                i -=1;
            }
            i += 1;								//Ajusto o i para ter a posição correta
            if (x->filhos[i]->n == 2*T-1){		//Se o número de elementos no filho de x na posição i for o máximo (2T - 1):
                divideFilhobTree(x, i,T);		//Divido  o nó
                if (k > x->chaves[i])			//Se a chave a ser inserida for maior que a chave de x na posição i
                    i+= 1;						//Incrementa-se o index para inserir no filho da direita da chave
            }
            inserebTreeNaoCheio(x->filhos[i], k, T);//insiro na posição correta
        }
    }
}


int main(int argc, char *argv[]){
    int pos;
    int T;

    scanf("%d",&T);

    bTree arvore;
    criabTree(&arvore,T);

    inserebTree(&arvore, 1,T);
    inserebTree(&arvore, 7,T);
    inserebTree(&arvore, 9,T);
    inserebTree(&arvore, 24,T);
    inserebTree(&arvore, 40,T);
    inserebTree(&arvore, 41,T);
    inserebTree(&arvore, 45,T);
    inserebTree(&arvore, 60,T);
    inserebTree(&arvore, 75,T);
    inserebTree(&arvore, 90,T);
    inserebTree(&arvore, 50,T);
    inserebTree(&arvore, 65,T);



    return 0;
}
