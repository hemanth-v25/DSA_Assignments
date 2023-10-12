#include<stdio.h>
#include<stdlib.h>

struct Node {
    int i,j;
    float p;
    struct Node *next,*prev;
};

struct Node* get_node(int a,int b,float c){
    struct Node* temp=(struct Node*)malloc(sizeof(struct Node));
    temp->i=a;
    temp->j=b;
    temp->p=c;
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}

float power(float base, int exp){
    int res=1;
    while(exp--){
        res*=base;
    }
    return res;
}

void delList(struct Node** head_ref){
    struct Node* current = *head_ref;
    struct Node* next = NULL;

    while (current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    *head_ref = NULL;
}

void insert_poly(int a,int b,float c,struct Node** head_ref){
    struct Node *e,*f;
    f=*head_ref;
    if(c==0) return;
    e=get_node(a,b,c);
    if(f==NULL||(f->i+f->j)<(a+b) || ((f->i+f->j==a+b)&&(f->i<a))){
        e->next=*head_ref;
        if(f!=NULL) (*head_ref)->prev=e;
        *head_ref=e;
    }    
    else{
        while(f->next!=NULL &&((f->i+f->j>a+b)||((f->i+f->j==a+b) && (f->i>a)))){
            f=f->next;
        }
        if(f->next==NULL){
            if((f->i+f->j>a+b)||((f->i+f->j==a+b) && (f->i>a))){
                f->next=e;
                e->prev=f;
            }
            else if(f->i==a && f->j==b){
                f->p=f->p+c;
                if(f->p==0){
                    if(f->prev!=NULL){
                        f->prev->next=NULL;
                        free(f);
                    }
                    else{
                        *head_ref=NULL;
                        free(f);
                    }
                }
            }
            else{
                f->prev->next=e;
                e->prev=f->prev;
                e->next=f;
                f->prev=e;
            }
            return;
        }
        if(f->i==a && f->j==b){
            f->p=f->p+c;
            if(f->p==0 && f->prev==NULL){
                *head_ref=f->next;
                f->next->prev=NULL;
                free(f);
            }
            if(f->p==0 && f->prev!=NULL && f->next!=NULL){
                f->prev->next=f->next;
                f->next->prev=f->prev;
                free(f);
            }    
            return;
        }
        else if(f==*head_ref){
            f->next=e;
            e->prev=f;
        }
        else{
            f->prev->next=e;
            e->next=f;
            e->prev=f->prev;
            f->prev=e;  
        }      
    }
}

struct Node* poly_add(struct Node* a,struct Node* b){
    struct Node *c;
    c=NULL;
    if(a==NULL && b==NULL) return c;
    while(a!=NULL && b!=NULL){
        insert_poly(a->i,a->j,a->p,&c);
        insert_poly(b->i,b->j,b->p,&c);
        b=b->next;
        a=a->next;
    }
    while(a!=NULL){
        insert_poly(a->i,a->j,a->p,&c);
        a=a->next;
    }
    while(b!=NULL){
        insert_poly(b->i,b->j,b->p,&c);
        b=b->next;
    }
    return c;
}

struct Node* poly_mul(struct Node* a,struct Node* b){
    struct Node *c,*x;
    x=a;
    c=NULL;
    if(a==NULL && b==NULL) return c;
    while(b!=NULL){
        while(a!=NULL){
            insert_poly(a->i+b->i,a->j+b->j,(a->p)*(b->p),&c);
            a=a->next;
        }
        a=x;
        b=b->next;
    }
    return c;
}

void get_poly(struct Node** x){
    int length = 10000;
    float * data = malloc(length * sizeof(float)); 
    int count = 0; 
    int j=0;
    int flag=0;
    int sign=1;
    char c; 
    while((c = getchar()) != '\n'){ 
        if(count+1 >= length){
            data = realloc(data, (length += 10000) * sizeof(float));
        }
        else if(c !='(' && c!=')'){
            if(c!=','){
                if(c=='-') sign=-1;
                else if((count+1)%3!=0){
                    data[count] = atof(&c)+data[count]*10;
                    data[count]*=sign;
                }
                else {
                    if(c=='.') flag=1;
                    if(flag==0 && c!='.') {
                        data[count]*=sign;
                        data[count]=atof(&c)+data[count]*10;
                        data[count]*=sign;
                    }
                    else if(flag==1 && c!='.') {
                        data[count]*=sign;
                        data[count]=atof(&c)/power(10,j+1)+data[count];
                        data[count]*=sign;
                        j++;
                    }
                }
            }
            else {
                j=0;
                sign=1;
                flag=0;
                count++;
            }
            
        }
    }
    for(int i=0;i<=(count+1)/3;i++){
        insert_poly(data[3*i],data[3*i+1],data[3*i+2],x);
    }
}

int main(){
    int i,j,x,y,k,p;
    char op;
    struct Node *A,*B,*C;
    A=B=C=NULL;
    char c[1];
    c[0]='y'; 
    while(c[0]=='y'){
        printf("\nPlease enter the operator(+/*):");
        char op[1];
        scanf("%s",op);
        getchar();

        printf("\nPlease enter polynomial A:");
        get_poly(&A);
        printf("\nPlease enter polynomial B:");
        get_poly(&B);

       
        if(op[0]=='*') {
            C=poly_mul(A,B);
            printf("\nA*B:");
        }
        else if(op[0]=='+') {
            C=poly_add(A,B);
            printf("\nA+B:");
        }
        else printf("\nPlease Enter a Valid Operator");
        delList(&A);
        delList(&B);
        struct Node* temp=C;
        while(temp->next!=NULL){
            printf("(%d,%d,%g),",temp->i,temp->j,temp->p);
            temp=temp->next;
        }
        printf("(%d,%d,%g)\n",temp->i,temp->j,temp->p);
        delList((&C));
        
        printf("\nDo you want to continue:(y/n):");
        scanf("%s",c);        
    }
    return 0;
}