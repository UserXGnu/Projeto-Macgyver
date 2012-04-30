#ifndef USERLIST_H
#define USERLIST_H
#include "shell_fnct.h"

#define MAX			64
#define MIN			16
#define null		NULL
#define TRUE	    1
#define FALSE 		0
#define	USRFILE		"/usr/etc/macgyver/userfile.dat"
#define CNTFILE		"/usr/etc/macgyver/cntfile.dat"
#define LOGFILE		"/usr/etc/macgyver/logfile.txt"


typedef struct userFile pUser;
struct userFile {
    int id;
    char name[MAX];
    char passwd[MIN];
    pUser * prox;
};

// :: Estrutura da fila (lista) ::

typedef struct pFila {
    pUser * begin;
    pUser * end;
    int cont;
} sFila;

// iniciando a lista ::
void startList(sFila * f){
    f->begin = NULL;
    f->end = NULL;
    f->cont = 0;
}

// registrando novo usuário ::
void registerToList(sFila * f){
    pUser * temp = (pUser *) calloc(1, sizeof(pUser));
    int aux = 0;
    char * passwd;
    
    dump_line(stdin);
    temp->id = f->cont;
    printf("-==[Type username]==-[ ");
    fgets(temp->name, MAX, stdin);
    str_format(temp->name);
    passwd = getpass("-==[Type passwd ]==-[ ");
    strcpy(temp->passwd, passwd);
    passwd = getpass("-==[Re-Type passwd ]==-[ ");
    if(!strcmp(temp->passwd, passwd)){
        if (f->cont == 0) {
    
            f->end = temp;
            f->begin = temp;
            temp->prox = null;
            f->cont++;
    
        } else if (f->cont > 0) {
    
            f->end->prox = temp;
            temp->prox = null;
            f->end = temp;
            f->cont++;
    
        } else {
            fatal("with dinamyc allocation");
        }
        printf("\n\t-==[!!] Registered with success [!!]==-\n");
    }else{
        printf("\t\n-==[ Password doesn't match ]==-\n");
    }
    wait();
}

// procurando elemento da lista ::
void search(sFila * f, char * id){
    pUser * temp = f->begin;
    int found = 0;
    
    while(temp != NULL){
        if(!(strcmp(temp->name, id))){
            printf("\n\t-==[!!] FOUND [!!]==-\n");
            printf("\n\t-==[Username: %s ]==-", temp->name);
            printf("\n\t-==[Passwd: %s ]==-\n", temp->passwd);
            found = 1;
        }
        temp = temp->prox;
    }
    if(found  == 0){
        printf("\n\t-==[ User doestn't registered yet ]==-\n");
    }
}

int loginVerify(sFila * f, char * id, char * passwd){
    pUser * temp = f->begin;
    int found = 0;
    
    while(temp != NULL){
        if(!(strcmp(temp->name, id))){
			if(!(strcmp(temp->passwd, passwd))){
	            printf("\n\t-==[!!] FOUND [!!]==-\n");
	            printf("\n\t-==[Username: %s ]==-", temp->name);
				return TRUE;
			}
            found = 1;
        }
        temp = temp->prox;
    }
    if(found  == 0){
        fprintf(stderr, "\n\t-==[ User doestn't registered ]==-\n");
		return FALSE;
    }
}

void escritor(sFila * f, FILE * fd){
  pUser * temp;
  pUser * ant;
  temp = f->begin;
  int cont;
  if(!(fd = fopen(CNTFILE, "w+"))){
    fatal("opening the cont file");
  }
  while(temp != NULL){
    ant = temp;
    temp = temp->prox;
  }
  cont = ant->id;
  if(fwrite(&cont, sizeof(int), 1, fd)!= 1){
    fatal("writing on the cont file");
  }
  fclose(fd);
  fflush(fd);
  
  if(!(fd = fopen(USRFILE, "w+"))){
    fatal("opening the file");
  }
  temp = f->begin;
  while(temp != NULL){
    if(fwrite(temp, sizeof(pUser), 1, fd) != 1){
        fatal("writing on the file");
    }
    temp = temp->prox;
  }
  printf("\n\t-==[ Saved ]==-\n");
  fclose(fd);
  fflush(fd);
  wait();
}

// inserindo na lista ::
void enfileirar(sFila * f, pUser * user){
      if (user == null) {
        return;
    } else if (f->cont == 0) {
        f->begin = user;
        f->end = user;
        user->prox = null;
        f->cont++;
    } else {
        f->end->prox = user;
        user->prox = null;
        f->end = user;
        f->cont++;
    }
}

// lendo arquivo e enfileirando ::
void recover(sFila * f, FILE * fd){
    int cont;
    char c;
    if(!(fd = fopen(CNTFILE, "r+"))){
        fatal("reading cont file");
    }
    if(!(fread(&cont, sizeof(int), 1, fd))){
        fatal("reading cont file");
    }
    fclose(fd);
    fflush(fd);
    
    if(!(fd = fopen(USRFILE, "r+"))){
        fatal("reading the file");
    }
    while(f->cont <= cont){
        pUser * temp = (pUser *) calloc(1, sizeof(pUser));
        if(fread(temp, sizeof(pUser), 1, fd)){
            enfileirar(f, temp);
        }else{
            fatal("reading the file 1");
        }
    }
    fclose(fd);
    fflush(fd);
}

// FREE na lista ::
void listFree(sFila* f) {
    pUser * temp;
    temp = f->begin;
    while (temp != null) {
        pUser * savior = temp;
        temp = temp->prox;
        free(savior);
        f->cont
                --;
    }
    f->begin = null;
    f->end = null;
}

// imprime o menu :: 
int menu(sFila * f, int op, char * user, char * passwd){
    FILE * fd;
    int option;
    char id[MAX];
    unsigned short int whiler = 0;
	
	if(op == 1){
		recover(f, fd);
		return loginVerify(f, user, passwd);
	}

    while(whiler == 0){
	    clear();
    	printf("-=======================-\n");
    	printf("-==[ User Menu Table ]==-\n");
    	printf("-=======================-\n");
    	printf("-==[ 2 - search user ]==-\n");
    	printf("-==[ 4 - recover DB  ]==-\n");
	printf("-==[ 5 - quit        ]==-\n");
    	printf("-=======================-\n");
    	printf("-==[ Option ]==-==[ ]==-\n");
    	move(20,10);
    	scanf("%d", &option);
        switch(option){
            case 1:{
                registerToList(f);
                break;
            }case 2:{
                printf("\n");
                printf("\t-==[Type the ID name]==-\n");
                printf("\t-==[ID:             ]==-\n");
                move(16, 13);
				dump_line(stdin);
                fgets(id, sizeof(id), stdin);
				str_format(id);
                search(f, id);
                wait();
                break;
            }case 3:{
                escritor(f, fd);
                break;
            }case 4:{
                recover(f, fd);
                break;
            }case 5:{
                whiler = 1;
             break;
            }default: whiler = 1; break;
        }
    }
}

#endif
