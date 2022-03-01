#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DIM_CMD 20
#define DIM_START 100
#define DIM_ROW 1024

typedef struct action_s{
    /*variabile change usato come booleano 1=change, 0=delete*/
    int change;
    int ind1;
    int ind2;
    char** old_rows;
    int old_last_row;
    struct action_s * next;
} action_t;

/*******************************************************************/

action_t * push(action_t *, int, int, int, int);
action_t* pop(action_t*, action_t**);

/******************************************************************/

void acquisisci(char*);
void acquisisci_e_rialloca(char**);

int max(int, int);
int min(int, int);

/*****************************************************************/

action_t* change(char***, int*, int*, int, int, action_t*);
action_t* delete(char***, int*, int, int, action_t*);
void print(char*[], int, int, int);
action_t* undo(char***, int*, int, action_t*, action_t**);
action_t* redo(char***, int*, int, action_t*, action_t**);


int main() {
    /*variabili per gestire il programma*/
    /*vettore di puntatori per gestire le righe*/
    char** doc;
    int dim_doc = DIM_START;

    /*variabile per tenere conto dell'ultima riga occupata*/
    int last_row;

    /*inizializzo le due pile per gestire le azioni con undo/redo. Queste saranno le due teste*/
    action_t* done_stack = NULL;
    action_t* undone_stack = NULL;
    /*variabili che tengono conto dei comandi nella pila done_stack e undone_stack*/
    int comandi_done;
    int comandi_undone;


    /*variabili per acquisire i comandi*/
    char cmd[DIM_CMD+1];
    char last;
    int len_cmd, ind1, ind2;
    char* string_ind2;
    int num_reps;

    /*variabile che tiene conto del numero di undo redo per comandi successivi in modo da non fare tante chiamate inutili
     * undo>0 chiamo undo, undo<0 chiamo redo*/
    int num_undo;
    num_undo = 0;
    comandi_done = 0;
    comandi_undone = 0;

    /*inizializzo il documento (vettore di puntatori per le righe) con 50 righe*/
    doc=(char**)malloc(DIM_START*sizeof(char*));
    last_row = 0;

    /*continuo a inserire comandi fino a che non scrivo q*/

    do {
        /*trovo l'ultima lettera per distinguere i vari comandi*/
        acquisisci(cmd);
        len_cmd = (int)strlen(cmd);
        last = cmd[len_cmd - 1];
        switch (last){
            case 'c':
                /*ricopio il tratto fino alla prima virgola in ind1 e dalla virgola in poi in ind2*/
                ind1 = (int)strtol(cmd, &string_ind2, 10);
                ind2 = (int)strtol(&string_ind2[1], (char**) NULL, 10);

                /*con questo if cerco di accorpare undo e redo solo quando c'è un comando diverso*/
                if (num_undo>0){
                    /*aggiorno il numero di comandi nelle due pile*/
                    comandi_done = comandi_done - num_undo;
                    comandi_undone = comandi_undone + num_undo;

                    done_stack = undo(&doc,&last_row, num_undo, done_stack, &undone_stack);
                    num_undo = 0;
                }
                else if(num_undo < 0){
                    num_undo = - num_undo;
                    /*aggiorno il numero di comandi nelle due pile*/
                    comandi_done = comandi_done + num_undo;
                    comandi_undone = comandi_undone - num_undo;

                    /*passo alla redo num_undo cambiato di segno */
                    done_stack = redo(&doc,&last_row, num_undo, done_stack, &undone_stack);
                    num_undo = 0;
                }

                /*svuoto undone_stack perchè le azioni sono eliminate definitivamente*/
                if(undone_stack) {
                    comandi_undone = 0;
                    undone_stack = NULL;
                }
                done_stack = change(&doc,&last_row, &dim_doc, ind1, ind2, done_stack);
                comandi_done++;
                break;

            case 'd':
                /*ricopio il tratto fino alla prima virgola in ind1 e dalla virgola in poi in ind2*/
                ind1 = (int)strtol(cmd, &string_ind2, 10);
                ind2 = (int)strtol(&string_ind2[1], (char**) NULL, 10);

                /*con questo if cerco di accorpare undo e redo solo quando c'è un comando diverso*/
                if (num_undo>0){
                    /*aggiorno il numero di comandi nelle due pile*/
                    comandi_done = comandi_done - num_undo;
                    comandi_undone = comandi_undone + num_undo;

                    done_stack = undo(&doc,&last_row, num_undo, done_stack, &undone_stack);
                    num_undo = 0;
                }
                else if(num_undo < 0){
                    num_undo = - num_undo;
                    /*aggiorno il numero di comandi nelle due pile*/
                    comandi_done = comandi_done + num_undo;
                    comandi_undone = comandi_undone - num_undo;

                    /*passo alla redo num_undo cambiato di segno */
                    done_stack = redo(&doc,&last_row, num_undo, done_stack, &undone_stack);
                    num_undo = 0;
                }

                /*svuoto undone_stack perchè le azioni sono eliminate definitivamente*/
                if(undone_stack) {
                    comandi_undone = 0;
                    undone_stack = NULL;
                }

                done_stack = delete(&doc, &last_row, ind1, ind2, done_stack);
                comandi_done++;
                break;

            case 'p':
                /*ricopio il tratto fino alla prima virgola in ind1 e dalla virgola in poi in ind2*/
                ind1 = (int)strtol(cmd, &string_ind2, 10);
                ind2 = (int)strtol(&string_ind2[1], (char**) NULL, 10);

                /*con questo if cerco di accorpare undo e redo solo quando c'è un comando diverso*/
                if (num_undo>0){
                    /*aggiorno il numero di comandi nelle due pile*/
                    comandi_done = comandi_done - num_undo;
                    comandi_undone = comandi_undone + num_undo;

                    done_stack = undo(&doc,&last_row, num_undo, done_stack, &undone_stack);
                    num_undo = 0;
                }
                else if(num_undo < 0){
                    num_undo = - num_undo;
                    /*aggiorno il numero di comandi nelle due pile*/
                    comandi_done = comandi_done + num_undo;
                    comandi_undone = comandi_undone - num_undo;

                    /*passo alla redo num_undo cambiato di segno */
                    done_stack = redo(&doc,&last_row, num_undo, done_stack, &undone_stack);
                    num_undo = 0;
                }

                print(doc, last_row, ind1, ind2);
                break;

            case 'u':
                /*metto il numero di undo in una variabile num_reps*/
                num_reps = (int)strtol(cmd, (char**) NULL, 10);

                num_undo = min(num_undo + num_reps, comandi_done);
                break;

            case 'r':
                /*metto il numero di redo in una variabile num_reps*/
                num_reps = (int)strtol(cmd, (char**) NULL, 10);

                num_undo = max(num_undo - num_reps, -comandi_undone);
                break;

            case 'q':
                break;

            default:   ;

        }



    }while(cmd[0]!='q');


    return 0;
}

/*******************************************************************************************/

/*push genera e spinge in cima alla pila la nuova azione*/
action_t * push(action_t * head, int change, int ind1, int ind2, int old_last_row){
    action_t * e;
    int num_righe_cambiate;


    if(change ||  ind2<old_last_row)
        /*se è una change o una delete nei margini alloco lo spazio per tutte le righe perchè poi facendo undo
         * mi servirà tutto quello spazio*/
        num_righe_cambiate = ind2 - ind1 + 1;
        /*nel caso sia una delete distinguo i casi particolari*/
    else if(ind1>old_last_row)  /*caso di delete inefficace*/
        num_righe_cambiate=0;
    else {  /* if (ind2>old_last_row) che è diventato quindi ind2==old_last_row e ind1<old_last_row*/
        /*nel caso di delete oltre i margini del documento lo riporto nei margini*/
        num_righe_cambiate = ind2 - ind1 + 1;
    }


    /*alloco lo spazio per il nuovo nodo azione e lo lego alla testa della lista*/
    e=malloc(sizeof(action_t));
    e->change = change;
    e->ind1 = ind1;
    e->ind2 = ind2;
    e->old_last_row = old_last_row;
    /*metto un if per tenere conto delle delete in area vuota*/
    if(num_righe_cambiate>0) {
        /*alloco lo spazio per un array di indirizzi di char per tenere conto delle righe modificate*/
        e->old_rows = malloc(num_righe_cambiate * sizeof(char *));
    }
    else  /*se non c'è stato cambiamento imposto le old_raws a NULL (caso di delete vuota)*/
        e->old_rows = NULL;
    e->next = head;
    head = e;

    return head;
}

/*spila il primo elemento del primo stack passato per copia e lo attacca al secondo stack passato per indirizzo,
 * ritorna poi la testa del primo stack*/
action_t* pop(action_t* first_stack, action_t** ind_second_stack){
    action_t * ptr;
    action_t * second_stack;

    second_stack = *ind_second_stack;

    /*il puntatore punta all'elemento che voglio spostare dal done all'undone*/
    ptr = first_stack;

    /*faccio puntare la testa della lista all'elemento dopo che diventa così il primo*/
    first_stack = first_stack -> next;

    /*eseguo poi una push nell'undone_stack*/
    ptr ->next = second_stack;
    *ind_second_stack = ptr;

    return first_stack;
}



/**************************************************************************************************************/


void acquisisci(char *str){
    //dichiarazioni
    char carattere;

    //acquisizione
    while((carattere=(char)getchar_unlocked())!=10 && carattere!=EOF)
        *(str++) = carattere;

    //aggiunta \0
    *str = 0;
}


void acquisisci_e_rialloca(char **string){
    //dichiarazioni
    char carattere;
    int dim;
    char * str;
    /*ho fatto un passaggio per indirizzo ma converto per fare conti agevoli e poter modificare str e usarlo come pointer*/
    str = *string;

    /*faccio partire la dimensione della stringa a 2 perchè in ogni caso ci sarà il carattere \0 e il while fatto così
     * salta un giro di dim++ */
    dim=2;
    //acquisizione
    while((carattere=(char)getchar_unlocked())!=10 && carattere!=EOF) {
        *(str++) = carattere;
        dim++;
    }

    //aggiunta \0
    *str = 0;

    /*rialloco la stringa per far occupare solo lo spazio necessario*/
    *string = realloc(*string, dim * sizeof(char));

}


int max (int a, int b){
    if (a>b)
        return a;
    return b;
}

int min (int a, int b){
    if (a<b)
        return a;
    return b;
}

/****************************************************************************************************************/

action_t* change(char*** ind_doc, int* ind_last_row, int* dim_doc, int ind1, int ind2, action_t* done_stack){
    int i;
    int last_row;
    char** doc;
    /*ricopio il contenuto di doc passato per indirizzo per fare conti agevoli*/
    doc = *ind_doc;

    /*ricopio il contenuto dell'indirizzo di last_row passato per indirizzo per fare conti agevoli*/
    last_row=*ind_last_row;

    /*creo un nuovo nodo azione per tenere l'azione corrente nel done_stack, all'interno della push ho anche
     * riservato lo spazio per il vettore di puntatori old_rows, da riempire*/
    done_stack = push(done_stack, 1, ind1, ind2, last_row);

    /*rialloca il vettore di puntatori per le righe se necessario tenendo un margine grande quanto DIM_START
     * e mette la nuova dimensione in dim_doc. inoltre ricopio il nuovo doc per fare passaggio per indirizzo*/
    if(ind2>*dim_doc) {
        doc = realloc(doc, (ind2 + DIM_START) * sizeof(char *));
        *dim_doc = ind2 + DIM_START;
        *ind_doc = doc;

    }


    /*alloco la memoria per tutte le righe nuove e salvo le vecchie in done_stack->old_rows passando l'indirizzo*/
    for(i=ind1; i<=ind2 ; i++){
        if(i<=last_row) {
            /*copio la riga in old_rows passando l'indirizzo*/
            done_stack->old_rows[i - ind1] = doc[i - 1];
        }
        doc[i-1]=(char*)malloc(DIM_ROW*sizeof(char));
        /*prendo le righe di testo e le metto nelle nuove righe create, rimodellate poi a dimensione minima*/
        acquisisci_e_rialloca(&doc[i-1]);

    }


    /*riassegno il valore last_row solo se è cambiato: passaggio per indirizzo*/
    if(last_row < ind2)
        *ind_last_row = ind2;


    /*acquisisco il punto fnale e il carattere invio*/
    getchar_unlocked();
    getchar_unlocked();

    return done_stack;

}

action_t* delete(char*** ind_doc, int* ind_last_row, int ind1, int ind2, action_t* done_stack){
    int i;
    int last_row;
    char** doc;
    int num_righe_cancellate;
    /*ricopio il contenuto di doc passato per indirizzo per fare conti agevoli*/
    doc = *ind_doc;

    /*ricopio il contenuto dell'indirizzo di last_row passato per indirizzo per fare conti agevoli*/
    last_row=*ind_last_row;

    /*i casi 0,xd valgono come 1,xd*/
    if(ind1==0)
        ind1++;
    /*se vuole cancellare oltre l'ultima riga faccio cancellare fino all'ultima*/
    if(ind2>last_row)
        ind2 = last_row;
    /*creo un nuovo nodo azione per tenere l'azione corrente nel done_stack, all'interno della push ho anche
    * riservato lo spazio per il vettore di puntatori old_rows, da riempire*/
    done_stack = push(done_stack, 0, ind1, ind2, last_row);

    /*salvo le righe da cancellare in old_rows*/
    if(ind1>last_row || ind2==0)   ;
        /*delete non ha alcun effetto*/
        /*distinguo i casi in cui devo cancellare righe centrali o finali*/
    else if(ind2==last_row){  /*che comprende tutti i casi ind2>last_row*/
        for(i=ind1; i<=last_row ; i++)
            /*copio la riga in old_rows passando l'indirizzo*/
            done_stack->old_rows[i - ind1] = doc[i - 1];
        /*modifico solo il last row dato che ho cancellato le ultime righe*/
        last_row = ind1-1;
    }
    else {  /*sono nel caso di cancellamento di righe centrali, quindi devo fare uno scorrimento*/
        for (i = ind1; i <= ind2; i++)
            /*copio la riga in old_rows passando l'indirizzo*/
            done_stack->old_rows[i - ind1] = doc[i - 1];
        num_righe_cancellate = ind2 - ind1 + 1;
        last_row = last_row - num_righe_cancellate;
        for(i=ind1; i<=last_row ; i++)
            doc[i-1]= doc[i-1 + num_righe_cancellate];
    }

    /*faccio il passaggio per indirizzo di last_row*/
    *ind_last_row = last_row;
    /*faccio il passaggio per indirizzo di doc*/
    *ind_doc = doc;

    return done_stack;
}

void print(char* doc[], int last_row, int ind1, int ind2){
    int i;

    /*se dice di stampare la riga zero stampo un punto*/
    if(ind1==0) {
        printf(".\n");
        if (ind2 == 0) {
            return;
        }
        ind1=1;
    }

    /*se indica una zona dove non ci sono righe stampo solo punti*/
    if(ind1>last_row)
        for(i=ind1; i<=ind2; i++)
            printf(".\n");
        /*distinguo i casi in cui devo stampare dei punti o meno*/
    else if(ind2>last_row){
        for(i=ind1; i<=last_row ; i++)
            puts(doc[i-1]);
        for(i=last_row; i<ind2; i++)
            printf(".\n");
    }
    else
        for(i=ind1; i<=ind2; i++)
            puts(doc[i-1]);

}

action_t* undo(char*** ind_doc, int* ind_last_row, int reps, action_t* done_stack, action_t** ind_undone_stack){
    int i,j;
    int last_row;
    int old_last_row;
    int ind1, ind2;
    char** doc;
    char* swap;
    int num_righe_cambiate;


    /*ricopio il contenuto di doc passato per indirizzo per fare conti agevoli*/
    doc = *ind_doc;

    /*ricopio il contenuto dell'indirizzo di last_row passato per indirizzo per fare conti agevoli*/
    last_row=*ind_last_row;

    /*faccio undo reps volte*/
    for(j=1; j<=reps; j++) {

        /*eseguo undo solo se la testa non punta a NULL*/
        if (done_stack) {
            /*ricopio i valori dell'ultima azione compiuta in variabili di più facile accesso*/
            old_last_row = done_stack->old_last_row;
            ind1 = done_stack->ind1;
            ind2 = done_stack->ind2;
            /*divido i casi se si tratta di change o di delete*/
            if (done_stack->change) {  /*caso change*/
                if (old_last_row > ind2) {  /*in questo caso sostituisco tutte le righe*/
                    for (i = ind1; i <= ind2; i++) {
                        swap = doc[i - 1];
                        doc[i - 1] = done_stack->old_rows[i - ind1];
                        done_stack->old_rows[i - ind1] = swap;
                    }

                }
                else if(old_last_row < ind1){ /*devo "eliminare" le righe e cambiare il last row*/
                    for (i = ind1; i <= ind2; i++)
                        done_stack->old_rows[i - ind1] = doc[i-1];
                    /*old_last_row sarà ind1-1*/
                    last_row = old_last_row;

                }
                else {  /*sostituisco da ind1 a old_last_row, cambio il last_row*/
                    for (i = ind1; i <= old_last_row; i++) {
                        swap = doc[i - 1];
                        doc[i - 1] = done_stack->old_rows[i - ind1];
                        done_stack->old_rows[i - ind1] = swap;
                    }
                    for (i = old_last_row+1; i <= ind2; i++)
                        done_stack->old_rows[i - ind1] = doc[i-1];
                    last_row = old_last_row;

                }

            }
            else {  /*caso delete*/
                if ( done_stack->old_rows == NULL) {  /*caso di delete nulla sposto solo l'azione in undone_stack*/

                }
                else if(old_last_row > ind2) {
                    /*devo spostare tutte le righe del documento e inserire le vecchie righe cancellate */
                    num_righe_cambiate = ind2 - ind1 + 1;
                    /*con questo for sposto avanti tutte le righe*/
                    for(i=last_row; i>=ind1; i--)
                        doc[i+num_righe_cambiate-1] = doc[i-1];
                    /*con questo for ripristino le righe che erano state cancellate*/
                    for(i=ind1; i<=ind2 ; i++){
                        doc[i-1]= done_stack->old_rows[i-ind1];
                    }
                    /*ripristino poi il vecchio last_row che sarà last_row + num_righe_cambiate*/
                    last_row = old_last_row;

                }
                else{  /*old_last_row==ind2 ripristina di seguito le vecchie righe e cambia last_row*/
                    /*ind1 sarà = a last_row + 1*/
                    for(i=ind1; i<=ind2 ; i++)
                        doc[i-1]= done_stack->old_rows[i-ind1];
                    /*old_last_row sarà last_row + ind2 - ind1 + 1 */
                    last_row = old_last_row;
                }

            }

            /*dopo aver modificato l'azione la tolgo da done_stack e la metto in undone_stack*/
            done_stack = pop(done_stack, ind_undone_stack);

        }
            /* se la lista è vuota esco dal for senza esguire i restanti undo*/
        else {
            *ind_last_row = last_row;
            *ind_doc = doc;
            return done_stack;
        }
    }
    *ind_last_row = last_row;
    *ind_doc = doc;
    return done_stack;
}


action_t* redo(char*** ind_doc, int* ind_last_row, int reps, action_t* done_stack, action_t** ind_undone_stack){
    int i,j;
    int old_last_row;
    int ind1, ind2;
    char** doc;
    char* swap;
    int num_righe_cambiate;
    action_t* undone_stack;

    /*ricopio il contenuto di ind_undone_stack per conti agevoli*/
    undone_stack = *ind_undone_stack;

    /*ricopio il contenuto di doc passato per indirizzo per fare conti agevoli*/
    doc = *ind_doc;


    /*faccio redo reps volte*/
    for(j=1; j<=reps; j++) {

        /*eseguo redo solo se la testa non punta a NULL*/
        if (undone_stack) {
            /*ricopio i valori dell'ultima azione annulata in variabili di più facile accesso*/
            /*old_last_raw è uguale al last_raw attuale*/
            old_last_row = undone_stack->old_last_row;
            ind1 = undone_stack->ind1;
            ind2 = undone_stack->ind2;
            /*divido i casi se si tratta di change o di delete*/
            if (undone_stack->change) {  /*caso change*/
                if (old_last_row > ind2) {  /*in questo caso sostituisco tutte le righe*/
                    for (i = ind1; i <= ind2; i++) {
                        swap = doc[i - 1];
                        doc[i - 1] = undone_stack->old_rows[i - ind1];
                        undone_stack->old_rows[i - ind1] = swap;
                    }

                }
                    /*da qui ind2 è maggiore di last_row quindi diventerà il nuovo last_row */
                else if(old_last_row < ind1){ /*devo rimettere le righe e cambiare il last row*/
                    for (i = ind1; i <= ind2; i++)
                        doc[i-1] = undone_stack->old_rows[i - ind1];
                    *ind_last_row = ind2;

                }
                else {  /*sostituisco da ind1 a old_last_row, inserisco da old_last_row a ind2, cambio il last_row*/
                    for (i = ind1; i <= old_last_row; i++) {
                        swap = doc[i - 1];
                        doc[i - 1] = undone_stack->old_rows[i - ind1];
                        undone_stack->old_rows[i - ind1] = swap;
                    }
                    for (i = old_last_row+1; i <= ind2; i++)
                        doc[i-1] = undone_stack->old_rows[i - ind1];
                    *ind_last_row = ind2;

                }

            }
            else {  /*caso delete*/
                if (undone_stack->old_rows == NULL) {  /*caso di delete nulla sposto solo l'azione in done_stack*/

                }
                else if(old_last_row > ind2) {
                    /*devo spostare indietro tutte le righe del documento in modo da coprire le righe cancellate e
                     * cambiare last_row (le righe cancellate sono già salvate in old_rows)*/
                    num_righe_cambiate = ind2 - ind1 + 1;
                    /*con questo for sposto indietro tutte le righe*/
                    for(i=ind1; i<=old_last_row - num_righe_cambiate ; i++)
                        doc[i-1]= doc[i-1 + num_righe_cambiate];
                    /*cambio poi il last_row*/
                    *ind_last_row = old_last_row - num_righe_cambiate;

                }
                else{  /*old_last_row==ind2 cambia solo last_row, le righe sono già salvate in old_rows*/
                    *ind_last_row = ind1 - 1;
                }

            }

            /*dopo aver modificato l'azione la tolgo da undone_stack e la metto in done_stack*/
            undone_stack = pop(undone_stack, &done_stack);

        }
            /* se la lista è vuota esco dal for senza esguire i restanti redo*/
        else {

            *ind_doc = doc;
            *ind_undone_stack = undone_stack;
            return done_stack;
        }
    }

    *ind_doc = doc;
    *ind_undone_stack = undone_stack;
    return done_stack;
}
-----------------------------25346961372042367141123475277--
