#include<stdio.h>
#include<stdlib.h>
// #include<math.h> //Used in PageRank for computing squareroot
int max_size = -999999;

#define MAX(a,b) (((a)>(b))?(a):(b))

//Struct for storing egdes
struct AdjListNode{
    int v;
    double w;
    struct AdjListNode* next;
};

//struct for storing vertices and weights 
struct Node{
    int v;
    int u;
    int l;
    double w;
    struct Node *next;
};

//Adjacency list using linked lists
struct AdjList {
    int v;
    int count;
    struct AdjList *next;
    struct AdjListNode *head;
};


/*
i,j:vertices
k:weight
creates graph with given data of type i,j,k
*/
void createGraph(int i,int j,int k,struct AdjList **G){
    if(*G==NULL) {
        struct AdjListNode *temp=(struct AdjListNode*)malloc(sizeof(struct AdjListNode));
        *G = (struct AdjList*)malloc(sizeof(struct AdjList));
        (*G)->head=temp;
        (*G)->count=1;
        (*G)->v=i;
        (*G)->next=NULL;
        temp->v=j;
        temp->w=1;
        temp->next=NULL;
    }
    else{
        struct AdjList* g = *G;
        struct AdjListNode *temp1=(struct AdjListNode*)malloc(sizeof(struct AdjListNode));
        temp1->v=j;
        temp1->w=1;
        temp1->next=NULL;
        while(g!=NULL){
            int flag=1;
            if(j==g->head->v) flag=0;
            if(g->v == i && flag){
                temp1->next=g->head;
                g->head=temp1;
                g->count=g->count+1;
                return;
            }
            g=g->next;
        }
        struct AdjList* temp=(struct AdjList*)malloc(sizeof(struct AdjList));
        temp->v=i;
        temp->head=temp1;
        temp->count=1;
        temp->next=*G;
        *G=temp;  

    }
}

/*
i,j:vertices
k:weight
length:
Inserts Node(i,j,k) into a list of decresing order of k
*/
void insert_list(int i,int j, double k,int length, struct Node **c){
    struct Node *temp=*c;
    struct Node* temp1=(struct Node*)malloc(sizeof(struct Node));
    temp1->next=NULL;
    temp1->u=i;
    temp1->v=j;
    temp1->w=k;
    temp1->l=length;
    int flag=0;
    
    if(*c==NULL || (temp->w<k) || (temp->u>i && temp->v>=j && temp->w==k) || (temp->u==i && temp->v>j && temp->w==k)){
        if(*c==NULL) *c=temp1;
        else if((temp->w<k) || (temp->u>i && temp->v>=j && temp->w==k)|| (temp->u==i && temp->v>j && temp->w==k)){
            temp1->next=temp;
            *c=temp1;
        }
    }
    else{
        while(temp->next!=NULL && temp->next->w>k){
            temp=temp->next;
        }
        while(temp->next!=NULL && (temp->next->w==k) && ((temp->next->u<i) || (temp->next->u==i && temp->next->v<j))){
            temp=temp->next;
        }
        temp1->next=temp->next;
        temp->next=temp1;
    }
}

//Given data it creates a graph
void readGraph(struct AdjList** g)
{
    FILE *fp;
    int i,j;
    double k;
    fp=fopen("../contact-high-school-proj-graph.txt","r");
    if(fp == NULL) {
      perror("Error in opening file");
      return;
   }
    while(!feof(fp))
    {
        fscanf(fp,"%d %d %lf",&i, &j, &k);
        max_size=MAX(MAX(i,j),max_size);
        createGraph(i,j,k,g);
        createGraph(j,i,k,g);
    }

}

//prints graph
void printGraph(struct AdjList **g){
    struct AdjList *temp1=*g;
    int i=0;
    FILE * fp;
    /* open the file for writing*/
    fp = fopen ("adlist.txt","w");
    while(temp1!=NULL ){
        struct AdjListNode *temp=temp1->head;
    while(temp!=NULL && temp1->v==327){
        // printf("%d %d\n",temp1->v,temp->v);
        
        fprintf (fp, "%d %d\n",temp1->v, temp->v);

        temp=temp->next;
        i++;
    }
    temp1=temp1->next;
    // i++;
    }
    printf("%d",i);
    fclose (fp);
    return;
}
//creates new vertex in Adjacency list
void push(int v,struct AdjListNode** a){
    // struct AdjListNode *temp=*a;
    struct AdjListNode *temp1=malloc(sizeof(struct AdjListNode));
    temp1->v=v;
    temp1->w=1;
    temp1->next=NULL;
    if(*a==NULL){
        *a=temp1;
    }
    else{
        temp1->next=*a;
        *a=temp1;
    }
}

//checks is there is an edge between v and a->v
int ispresent(int v,struct AdjListNode **a){
    struct AdjListNode* temp=*a;
    while(temp!=NULL){
        if(temp->v==v) return 1;
        temp=temp->next;
    }
    return 0;
}

// Union of two linked lists
double get_Union(struct AdjListNode* a,struct AdjListNode* b){
    double k=0;
    struct AdjListNode *c=NULL;
    while(a!=NULL){
        push(a->v,&c);
        a=a->next;
        k++;
    }
    while(b!=NULL){
        if(!ispresent(b->v,&c)) k++;
        b=b->next;
    }
    return k;
}
//Intersection of two linked lists
double get_Intersection(struct AdjListNode*a,struct AdjListNode* b){
    double k=0;
    struct AdjListNode *c=NULL;
    while(a!=NULL){
        push(a->v,&c);
        a=a->next;
    }
    while(b!=NULL){
       if(ispresent(b->v,&c)) k++;
       b=b->next;
    }
    return k;
}
//Jaccards Coefficient

void JC(struct AdjList** G,int K){
    struct AdjList* temp=*G;
    struct Node* C=NULL;
    int U[max_size+1];
    while(temp!=NULL){
        for(int k=0;k<=max_size;k++){
            U[k]=0;
        }
        U[temp->v]=1;
        struct AdjListNode *temp1=temp->head;
        while(temp1!=NULL){
            U[temp1->v]=1;
            temp1=temp1->next;
        }
        temp1=temp->head;
        for(int i=1;i<=max_size;i++){
            if(U[i]==0){
                struct AdjList* temp2=*G;
                while(temp2!=NULL && temp2->v!=i){
                    temp2=temp2->next;
                }
                if(temp2!=NULL && temp2->v==i){
                    struct AdjListNode *temp3=temp2->head;
                    temp3=temp2->head;
                    double p,q;
                    p = get_Union(temp1,temp3);
                    q = get_Intersection(temp1,temp3);
                    // printf("%d %d\n",p,q);
                    if(temp->v<i) insert_list(temp->v,i,q/p,1,&C);
                }

            }
        }
        temp=temp->next;
    }
    FILE * fp;
    int i;
    /* open the file for writing*/
    fp = fopen ("Jaccard.txt","w");
    for(int i=0;C!=NULL && i<K;i++){
        fprintf (fp, "%d %d %lf\n",C->u, C->v, C->w);
        C=C->next;
    }
    fclose (fp);
    return;
}
//power of matrix
void mul(int n,int a[n][n][6],int p){
    int i,j,k;
    for(int i=1;i<n;i++){
        for(int j=1;j<n;j++){
            a[i][j][p-1]=0;
            for(int k=1;k<n;k++){
                a[i][j][p-1]+=a[i][k][0]*a[k][j][p-2];
            }
        }
    }
}
//power of transition probability matrix
void mul_p(int n,double a[n][n][6],int p){
    int i,j,k;
    for(int i=1;i<n;i++){
        for(int j=1;j<n;j++){
            a[i][j][p-1]=0;
            for(int k=1;k<n;k++){
                a[i][j][p-1]+=a[i][k][0]*a[k][j][p-2];
            }
        }
    }
}
//creates transition probability matrix
void create_pt(int n,double a[n][n][6],struct AdjList** G){
    struct AdjList *temp=*G;
    while(temp!=NULL){
        struct AdjListNode *temp1=temp->head;
        for(int i=1;i<n;i++){
            a[temp->v][i][0] = ispresent(i,&temp1)/(double)(temp->count);
        }
        temp=temp->next;
    }
}
//creates Adacency matrix
void create_am(int n,int a[n][n][6],struct AdjList** G){
    struct AdjList *temp=*G;
    while(temp!=NULL){
        struct AdjListNode *temp1=temp->head;
        for(int i=1;i<n;i++){
            a[temp->v][i][0] = ispresent(i,&temp1);
        }
        temp=temp->next;
    }
}

float power(float a,int p){
    float res=1;
    while(p--){
        res*=a;
    }
    return res;
}
//Katz score
void KS(struct AdjList **G,int K){
    int A[max_size+1][max_size+1][6];
    struct Node* c=NULL;
    
    create_am(max_size+1,A,G);
    for(int i=2;i<=6;i++){
        mul(max_size+1,A,i);
    }
    for(int i=1;i<=max_size;i++){
        for(int j=i+1;j<=max_size;j++){
            if(A[i-1][j-1][0]==0){
                double score=0;
                for(int k=2;k<=6;k++){
                    score+=power(0.1,k)*A[i-1][j-1][k-1];
                    
                }
                insert_list(i,j,score,1,&c);

            }
            // fprintf(mp,"%f ",A[i][j][2]);
        }
        // fprintf(mp,"\n");
    }
    FILE * fp;
    int i;
    /* open the file for writing*/
    fp = fopen ("Katz.txt","w");
    for(int i=0;c!=NULL && i<K;i++){
        fprintf (fp, "%d %d %lf\n",c->u, c->v, c->w);
        c=c->next;
    }
    fclose (fp);
    // fclose(mp);
    return;

}
//Commute time
void HT(struct AdjList **G,int K){
    double p[max_size+1][max_size+1][6];
    int A[max_size+1][max_size+1][6];
    create_am(max_size+1,A,G);
    struct Node* c=NULL;
    create_pt(max_size+1,p,G);
  
    for(int i=2;i<=6;i++){
        mul_p(max_size+1,p,i);
    }
    for(int i=2;i<=max_size;i++){
        for(int j=i+1;j<=max_size;j++){
            if(A[i-1][j-1][0]==0){
                double R=0;
                for(int k=2;k<=6;k++){
                    R+=k*(p[i-1][j-1][k-1]+p[j-1][i-1][k-1]);
                }
                insert_list(i-1,j-1,-R,6,&c);
            }
        }
    }
    FILE * fp;
    int i;
    /* open the file for writing*/
    fp = fopen ("HittingTime.txt","w");
    for(int i=0;c!=NULL && i<K;i++){
        fprintf (fp, "%d %d %lf\n",c->u, c->v, c->w);
        c=c->next;
    }
    fclose (fp);
    return;

}


void mat_p(int n,float a[n][n][10],int p){
    int i,j,k;
    if(p<=10){
        for(int i=1;i<n;i++){
            for(int j=1;j<n;j++){
                a[i][j][p-1]=0;
                for(int k=1;k<n;k++){
                    a[i][j][p-1]+=a[i][k][0]*a[k][j][p-2];
                }
            }
        }
    }
    else{
        for(int i=1;i<n;i++){
            for(int j=1;j<n;j++){
                a[i][j][p-1]=0;
                for(int k=1;k<n;k++){
                    a[i][j][p-1]+=a[i][k][9]*a[k][j][p-11];
                }
            }
        }
    }
}

void create_p(int n,float a[n][n][10],struct AdjList** G){
    struct AdjList *temp=*G;
    while(temp!=NULL){
        struct AdjListNode *temp1=temp->head;
        for(int i=1;i<n;i++){
            a[temp->v][i][0] = ispresent(i,&temp1)/(float)(temp->count);
        }
        temp=temp->next;
    }
}

void mat_cpy(int n,double a[n][n],double b[n][n]){
    for(int i=1;i<n;i++){
        for(int j=1;j<n;j++){
            b[i][j]=a[i][j];
        }
    }
}
//Accurate commute time
void HT_opt(struct AdjList **G,int K){
    float p[max_size+1][max_size+1][10];
    int A[max_size+1][max_size+1][6];
    create_am(max_size+1,A,G);
    struct Node* c=NULL;
    create_p(max_size+1,p,G);
    for(int i=2;i<=10;i++){
        mat_p(max_size+1,p,i);
    }

    for(int i=2;i<=max_size;i++){
        for(int j=i+1;j<=max_size;j++){
            if(A[i-1][j-1][0]==0){
                double R1=0;
                int k=2;
                double e=10;
                while(e>0.01 && k<20){
                    double R2=0;
                    R1+=k*(p[i-1][j-1][k-1]+p[j-1][i-1][k-1]);
                    e=(k+1)*(p[i-1][j-1][k]+p[j-1][i-1][k]);
                    k++;
                }
                insert_list(i-1,j-1,-R1,k-1,&c);
            }
        }
    }
    FILE * fp;
    int i;
    /* open the file for writing*/
    fp = fopen ("HittingTimeAccurate.txt","w");
    while(c!=NULL){
        fprintf (fp, "%d %d %lf %d\n",c->u, c->v, c->w,c->l);
        c=c->next;
    } 
    fclose (fp);
    return;
}

// double norm(int n,double a[n]){
//     double res=0;
//     for(int i=0;i<n;i++){
//         res+=a[i]*a[i];
//     }
//     return sqrt(res);
// }

double norm_d(int n,double a[n],double b[n]){
    double res=0;
    for(int i=0;i<n;i++){
        res+=(a[i]-b[i])*(a[i]-b[i]);
    }
    res;
}

void create_pg(int n,double p[n][n],int v,struct AdjList** G){
    struct AdjList *temp=*G;
    while(temp!=NULL){
        struct AdjListNode *temp1=temp->head;
        for(int i=1;i<n+1;i++){
            if(i==v) p[temp->v][i] = 0.2;
            else if(i!=v && !ispresent(v,&temp1) && ispresent(i,&temp1)) p[temp->v][i] = 0.8/(double)(temp->count);
            else if(i!=v && ispresent(v,&temp1) && ispresent(i,&temp1)) p[temp->v][i] = 0.8/(double)(temp->count-1);
            else if(i!=v && !ispresent(i,&temp1)) p[temp->v][i]=0;
        }
        temp=temp->next;
    }
    
}

void pg_mul(int n,double pi[n],double res[n],double p[n+1][n+1]){
    for(int i=0;i<n;i++){
        res[i]=0;
        for(int j=0;j<n;j++){
            res[i]+=pi[j]*p[j+1][i+1];
        }
    }
}

void arrcpy(int n,double a[n],double b[n]){
    for(int i=0;i<n;i++){
        b[i]=a[i];
    }
}

//power iteration method to compute stationary distribution
void power_iter(int n,double pi[n],double res[n],double p[n+1][n+1]){
    double e=1.0;
    while(e>1/(double)power(10,6)){
        double temp[n];
        arrcpy(n,pi,temp);
        pg_mul(max_size,pi,res,p);
        e=norm_d(n,res,temp);
        arrcpy(n,res,pi);
    }
}


void pagerank(struct AdjList** G,int K){
    struct Node* c=NULL;
    double p[max_size+1][max_size+1];
    double pg[max_size+1][max_size+1];
    for(int i=0;i<max_size+1;i++){
        // double r =0;
        for(int j=0;j<max_size+1;j++){
            pg[i][j]=0;
        }
    }
    for(int i=2;i<=max_size+1;i++){
        double pi[max_size];
        create_pg(max_size+1,p,i-1,G);
        for(int k=0;k<max_size;k++){
            pi[k] = 1/(double)max_size;
        }
        double res[max_size];
        power_iter(max_size,pi,res,p);
        for(int j=2;j<=max_size+1;j++){
            if(p[i-1][j-1]==0){
                pg[i-1][j-1]=res[j-2];
                // printf("%d %d %lf ",i-1,j-1,res[j-2]);
            }
            // printf("\n");
        }
    }

    

    for(int i=1;i<=max_size;i++){
        for(int j=1+i;j<=max_size;j++){
            if(p[i][j]==0) insert_list(i,j,(pg[i][j]+pg[j][i])/(double)2,1,&c);
        }
    }
    

    FILE * fp;
    int i=0;
    /* open the file for writing*/
    fp = fopen ("PageRank.txt","w");
    while(i<K && c!=NULL){
        fprintf (fp, "%d %d %lf\n",c->u, c->v, c->w);
        c=c->next;
        i++;
    } 
    fclose (fp);
    return;
    

}


int main(){
    int K;
    printf("Enter K:");
    scanf("%d",&K);
    struct AdjList* G=NULL; 
    readGraph(&G);
    JC(&G,K);
    KS(&G,K);
    HT(&G,K);
    HT_opt(&G,K);
    pagerank(&G,K);
    printGraph(&G);
    return 0;

}