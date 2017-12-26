#include<stdio.h>
#include<stdlib.h>

struct key 
{
  int sr_no;
  char *country;
  char *type;
  int a;
  int b;
};

struct node
{
	struct key * elem;
	int* link;
	int degree;
	char leaf;
	
};

int t;
void btree_insert(struct node ** arr, struct key k, int * r, int *size);
void btree_splitchild(struct node ** arr, int parent, int i, int *size);
void btree_insert_nonfull(struct node ** arr1, struct key  k, int index, int *size);
void btree_delete (struct node **arr, int k, int index, int *size, int *r );
void merge(struct node **arr, int index, int pos, int *r);
int search (struct node * a, int index, int k);
void fill(struct node **arr, int index, int i, int *size, int *r);
int main()
{
	
	t=4;
	//printf("%d\n",t);
	FILE *f;
	f=fopen("in1.csv","r");
	struct node * tree;
	struct node *head;
	head=(struct node *)malloc(sizeof(struct node));
	int root=0;
	head->elem = (struct key *)malloc(sizeof(struct key)* (2*t - 1));
	head->link = (int *)malloc(sizeof(int *)*(2*t));
	int i,j;
	for(i=0;i<(2*t);i++)
		head->link[i] = -1;
	head->degree =0;
	head->leaf='1';
	tree = head;
	int size_arr = 1;
	for(i=0;i<1000000;i++)
	{
		struct key temp;
		temp.country = (char *)malloc((sizeof(char))*5);
		temp.type = (char *)malloc((sizeof(char))*3);
		fscanf(f,"%d,%[^,],%[^,],%d,%d", &temp.sr_no, temp.country, temp.type, &temp.a, &temp.b);
		
		btree_insert(&tree, temp, &root, &size_arr );
		

	}
	
	int c=1,x,m;
	while(c)
	{
		printf("Enter 1 for search and 2 for delete, 0 to exit\n");
		scanf("%d",&c);
		if(c==1)
		{
			printf("Enter value to search\n");
			scanf("%d",&x);
			m=search(tree,root,x);
			//printf("%d\n",m);
			int k=0;
		
			if(m!=-1)
			{while((k<tree[m].degree) && (tree[m].elem[k].sr_no!=x))
				k++;
			printf("element %d found at node %d index %d\n",x,m, k);
			}
			else
				printf("%d not found\n",x);
			
		}
		if(c==2)
		{	printf("Enter value to delete\n");
			scanf("%d",&x);
			btree_delete(&tree, x,root, &size_arr,&root );

		}
		if(c==0)
			exit(1);
	}
	
	
}
void btree_insert(struct node ** arr, struct key k, int * r, int *size)
{
	int i;
	struct node *h;
	h= &(*arr)[(*r)];
	struct  node *tempo;
	if(h->degree == (2*t - 1))
	{
		(*size) = (*size)+1;
		if(tempo=(struct node*)realloc(*arr,(sizeof(struct node)*(*size))))
			*arr=tempo;
		struct node * n;
		n = (struct node *)malloc(sizeof(struct node));
		n->elem = (struct key *)malloc(sizeof( struct key) * (2*t -1));
		n->link = (int *)malloc(sizeof( int) * (2*t));

		n-> leaf = '0';
		n->degree = 0;
		n->link[0]=(*r);
		(*arr)[(*size)-1]=*n;
		(*r)=(*size)-1;
		btree_splitchild(arr,((*size)-1),0,size);
		
		btree_insert_nonfull(arr,k,*r,size);
	
	}
	else
	{
	
	btree_insert_nonfull(arr,k,*r,size);
	
	
	}
	
}
void btree_insert_nonfull(struct node ** arr1, struct key  k, int index, int *size)
{
	int i =( (*arr1)[index]).degree-1;
	
	if((((*arr1)[index]).leaf) == '1' )
	{
		
		while((i>0) && ((k.sr_no) < ((&((*arr1)[index]))->elem[i].sr_no)) )
		{	
			(&((*arr1)[index]))->elem[i+1].sr_no=(&((*arr1)[index]))->elem[i].sr_no;
			i=i-1;
		}
		
		(&((*arr1)[index]))->elem[i+1] = k;
		(&((*arr1)[index]))->degree = (&((*arr1)[index]))->degree + 1;
			
	}
	else
	{
		
		while((i>0) && ((k.sr_no) < ((&((*arr1)[index]))->elem[i].sr_no)) )
			{ i=i-1; }
		i=i+1;
		int child=( (*arr1)[index]).link[i];
		if(( (*arr1)[child]).degree == ((2*t)-1))
		{	
			
			btree_splitchild(arr1,index,i,size);
			
			if((k.sr_no) > ((*arr1)[index].elem[i].sr_no))
				i++;
			
		}
		child=( (*arr1)[index]).link[i];
		btree_insert_nonfull(arr1,k,child,size);
		
	}
	
}
void btree_splitchild(struct node ** arr, int parent, int i, int *size)
{
	struct node * new1;
	new1 = (struct node *)malloc(sizeof(struct node));
	new1->elem = (struct key *)malloc(sizeof(struct key)* (2*t - 1));
	new1->link = (int *)malloc(sizeof(int)*(2*t));
	new1->degree =t-1;
	(*size) = (*size)+1;
	(*arr)=realloc(*arr, sizeof(struct node)*(*size));
	struct node * r;
	int child;
	child=(*arr)[parent].link[i];
	r=&((*arr)[child]);
	new1->leaf=r->leaf;
	int j;
	for(j=0;j<(2*t);j++)
		new1->link[j]=-1;
	for(j=0;j<t-1;j++)
		new1->elem[j]=r->elem[j+t];
	
	if(r->leaf=='0')
	{
		for(j=0;j<=t-1;j++)
			new1->link[j]=r->link[j+t];
	}
	(*arr)[(*size)-1]=*new1;
	r->degree = t-1;
	for(j=((*arr)[parent].degree)+1; j>i+1; j--)
		(&((*arr)[parent]))->link[j]=(&((*arr)[parent]))->link[j-1];
	(&((*arr)[parent]))->link[i+1]=(*size)-1;
	
	for(j=((*arr)[parent].degree); j>i; j--)
		{(&((*arr)[parent]))->elem[j]=(&((*arr)[parent]))->elem[j-1];} 
	
	(&((*arr)[parent]))->elem[i]=r->elem[t-1];
	
	(&((*arr)[parent]))->degree = (&((*arr)[parent]))->degree + 1;
	
	
	
}
int search (struct node * a, int  index, int k)
{
	int i=0;
	while((i<a[index].degree) && ( k > (a[index].elem[i].sr_no)))
		{ i++;}
	if((i<a[index].degree) && ( k == (a[index].elem[i].sr_no)))
		{return index;}
	else 
	{
		if(a[index].leaf=='1')
			return -1;
		else
			{return search(a, a[index].link[i],k);}
	}
}
void btree_delete (struct node **arr, int k, int index, int *size, int *rootn )
{
	struct node * r1, *temp1;
	
	struct key temp, pred, succ;
	r1 = &(*arr)[index];
	int i=0,j,l;
	
	while((i< r1->degree) && ( k > (r1->elem[i].sr_no)))
		{i++;}
	
	if((i< r1->degree) && ( k == (r1->elem[i].sr_no)))
	{	
		if(r1->leaf=='1')
		{
			for(j=i;j<r1->degree-1;j++)
				r1->elem[j]=r1->elem[j+1];
			r1->degree = r1->degree-1;
			printf(" %d deleted\n",k);	
		}
		else
		{
			if(((*arr)[r1->link[i]]).degree > (t-1))
			{
				
				*temp1=(*arr)[(r1->link[i])];
				while((temp1->leaf)!='1')
					{
					l=temp1->degree;
					*temp1=(*arr)[(temp1->link[l])];
					}
				pred=temp1->elem[((temp1->degree)-1)];
				r1->elem[i]=pred;
				btree_delete(arr, pred.sr_no,r1->link[i],size, rootn);
				
			}
			if(((*arr)[r1->link[i]]).degree == (t-1) && ((*arr)[r1->link[i+1]]).degree > (t-1))
			{
				*temp1=(*arr)[(r1->link[i+1])];
				while((temp1->leaf)!='1')
					{
					*temp1=(*arr)[(temp1->link[0])];
					}
				succ=temp1->elem[0];
				r1->elem[i]=succ;
				btree_delete(arr, succ.sr_no,r1->link[i+1],size, rootn);
			}
			else
			{
				merge(arr, index, i, rootn);
				btree_delete(arr, k, r1->link[i],size, rootn);
			}
		}
	}
	else 
	{
		if(r1->leaf=='1')
		{
			printf("elem not found\n");
			return;
		}
		int flag;
		
		flag = ((i== r1->degree)? 1 : 0);
		if(((*arr)[(r1->link[i])]).degree < t)
			fill(arr,index,i,size,rootn);
		if(flag && (i>(r1->degree)))
			btree_delete(arr, k, r1->link[i-1],size, rootn);
	
		else
			btree_delete(arr, k, r1->link[i],size, rootn);	
	}
		
}
void merge(struct node **arr, int index, int pos, int *rootn)
{
	
	int child, sib;
	struct node *r1;
	r1 = &(*arr)[index];
	child=r1->link[pos];
	sib=r1->link[pos+1];
	int deg_child=(*arr)[child].degree;
	int deg_sib=(*arr)[sib].degree;
	(*arr)[child].elem[deg_child]=(*arr)[index].elem[pos];
	int i;
	for(i=0;i<deg_sib;i++)
		(*arr)[child].elem[i+t]=(*arr)[sib].elem[i];
	if(((*arr)[child].leaf)=='0')
	{
		for(i=0;i<=deg_sib;i++)
		(*arr)[child].link[i+t]=(*arr)[sib].link[i];
	}
	for(i=pos+1;i<r1->degree;i++)
		r1->elem[i-1]=r1->elem[i];
	for(i=pos+2;i<=r1->degree;i++)
		r1->link[i-1]=r1->link[i];
	(*arr)[child].degree=deg_child+deg_sib+1;
	(*arr)[index].degree= (*arr)[index].degree - 1;
	if((index==*rootn) && ((*arr)[index].degree == 0))
		*rootn=index;
	
	
}
void fill(struct node **arr, int index, int i, int *size, int *rootn)
{
	struct node *r;
	r=&(*arr)[index];
	//printf("hiii %d  %d\n",i, index);
	if((i!=0) && ((*arr)[r->link[i-1]]).degree >=t )
	{
		struct node * child, *sibling;
		*child=(*arr)[r->link[i]];
		*sibling=(*arr)[r->link[i-1]];
		int j;
		for(j=child->degree-1; j>=0; --j)
			child->elem[j+1]=child->elem[j];
		if(child->leaf=='0')
		{
			for(j=child->degree; j>=0; --j)
				child->link[j+1]=child->link[j];
		}
		child->elem[0]=r->elem[i-1];
		if(r->leaf=='0')
			child->link[0]=sibling->link[(sibling->degree)];
		r->elem[i-1]=sibling->elem[(sibling->degree-1)];
		child->degree=child->degree+1;
		sibling->degree=sibling->degree-1;
	}
	else
	{
		if((i!=(r->degree)) && ((*arr)[r->link[i+1]]).degree >=t )
		{
			struct node * child, *sibling;
			*child=(*arr)[r->link[i]];
			*sibling=(*arr)[r->link[i+1]];
			int j;
			child->elem[child->degree]=r->elem[i];
			if(child->leaf=='0')
				{
					child->link[(child->degree)+1]=sibling->link[0];
				}
			r->elem[i]=sibling->elem[0];
			for(j=1;j<sibling->degree;++i)
				sibling->elem[j-1] = sibling->elem[j];
			if(sibling->leaf=='0')
				{
					for(int j=1; j<=sibling->degree; ++j)
            					sibling->link[j-1] = sibling->link[j];	
				}
			child->degree=child->degree+1;
			sibling->degree=sibling->degree-1;
		}
		else
		{
			if(i!=r->degree)
				merge(arr,index,i, rootn);
			else
				merge(arr,index,i-1, rootn);
		}
	}


}
