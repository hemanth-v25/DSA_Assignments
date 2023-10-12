#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LENGTH_OF_ARRAY 10000

struct Node{
    int arr[8];
    struct Node *next;
};

void insert_val(int a,int n,struct Node **head_ref){
    struct Node *temp;
    temp=*head_ref;
    if(temp==NULL || n%8==0){
        if(temp==NULL && n<8) {
            *head_ref=(struct Node*)malloc(sizeof(struct Node));
            (*head_ref)->arr[n%8]=a;
            (*head_ref)->next=NULL;
        }
        else{
            while(temp->next!=NULL) temp=temp->next;
            temp->next=(struct Node*)malloc(sizeof(struct Node));
            temp->next->arr[n%8]=a;
        }
    }
    else{
        while(temp->next!=NULL) temp=temp->next;
        temp->arr[n%8]=a;

    }
}

void delList(struct Node** head_ref){
    struct Node* current = *head_ref;
    struct Node* next = NULL;

    while (current != NULL) 
    {
        next = current->next;
        free(current);
        current = next;
    }
    *head_ref = NULL;
}

int  get_list(struct Node **a){
    if(*a!=NULL) {
        int count=0;
        struct Node *temp=*a;
        while(temp!=NULL){

        }
    }
    int length = 10;
    char * data = malloc(length * sizeof(char)); 
    int count = 0; 
    char c; 
    while((c = getchar()) != '\n'){ 
        if(count >= length)
            data = realloc(data, (length += 10) * sizeof(char));
        data[count++] = c;
    }
    int n=0;
    char *token=strtok(data,",");
    while(token != NULL){
        if(token[0]=='$') return n;
        else{
            insert_val(atoi(token),n,a);
            token = strtok(NULL, ",");
            n++;
        }
    }
}

int get_array(int arr[],int l){
    // int length = 10;
    // char * data = malloc(length * sizeof(char)); 
    // int count = 0; 
    // char c; 
    // while((c = getchar()) != '\n'){ 
    //     if(count >= length)
    //         data = realloc(data, (length += 10) * sizeof(char));
    //     data[count++] = c;
    // }
    char data[10000];
    fgets(data,10000,stdin);
    int n=0;
    char *token=strtok(data,",");
    while(token != NULL){
        if(token[0]=='$') return n;
        else{
            arr[n]=atoi(token);
            token = strtok(NULL, ",");
            n++;
        }
    }
}

struct Node* find(struct Node* head,int *pattern,int *text,int l,int p,int t,int *count) {
    struct Node* temp=head;
    struct Node* ptr=head;
    struct Node* x=NULL;
    int i,j;
    i=j=0;
    if(p==0 && t!=0){
        while(*count<t) {
            insert_val(text[*count],*count,&x);
            (*count)++;
        }
        while(j<l && temp!=NULL){
            insert_val(temp->arr[j%8],*count,&x);
            j++;
            (*count)++;
            if(j%8==0) temp=temp->next;
        }
        return x;
    }
    else if(p!=0 && t==0){
        *count=l+t;
        return temp;
    }
    else if(p==0 && t==0){
        *count=l;
        return temp;
    }
    else if(t!=0 && p!=0){
        int end=0;
        while(temp!=NULL && j<l){
            i=0;
            if(temp->arr[j%8]==pattern[i]){
                int start=j;
                struct Node* ptr2=temp;
                j++;
                if(j%8==0) temp=temp->next; 
                i++;
                while(i<p && temp->arr[j%8]==pattern[i] && j<l){
                    j++;
                    i++;
                    if(j%8==0) temp=temp->next;
                }
                if(i==p){
                    for(int h=start;h<start+t;h++){
                        insert_val(text[h-start],*count,&x);
                        (*count)++;
                    }
                    j=start+p;
                    i=0;
                }
                else if(i!=p){
                    i=0;
                    temp=ptr2;
                    insert_val(temp->arr[start%8],*count,&x);
                    (*count)++;
                    j=start+1;
                    if(j%8==0) {
                        temp=temp->next;
                    }
                }
            }
            else if(temp->arr[j%8]!=pattern[i]) {
                insert_val(temp->arr[j%8],*count,&x);
                (*count)++;
                j++;
                
                if(j%8==0) {
                    temp=temp->next;
                }
            }
        }
    }
    return x;
}

int main(){
    struct Node *A;
    A=NULL;

    int pattern[MAX_LENGTH_OF_ARRAY];
    int text[MAX_LENGTH_OF_ARRAY];
        
    printf("Please enter the sequence S:");
    int l = get_list(&A);
    char c='y';

    while(c=='y')  {      
        int pattern_size=0;
        int text_size=0;
        int count=0;
        
        printf("\nPlease enter the pattern to be replaced:");
        pattern_size=get_array(pattern,MAX_LENGTH_OF_ARRAY);

        printf("\nPlease enter the replacement text:");
        text_size=get_array(text,MAX_LENGTH_OF_ARRAY);

        struct Node* temp=find(A,pattern,text,l,pattern_size,text_size,&count);
        struct Node* temp2=temp;
        
        int i=0;
        printf("\nSequence ater replacement:");
        while(temp!=NULL && i<count){
            printf("%d,",temp->arr[i%8]);
            i++;
            if(i%8==0) temp=temp->next;
        }
        // if(A!=temp) delList(&A);
        A=temp2;
        l=count;
        
        printf("$\n");
        printf("\nDo you want to continue? (y/n):");
        scanf("%c",&c);
        getchar();

    }
    return 0;
}