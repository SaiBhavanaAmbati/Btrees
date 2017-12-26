#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#define t1 7
#define t2 8
struct key 
{
  int sr_no;
  char country[5];
  char type[5];
  int a;
  int b;
};
struct node
{
	struct key elem[t1];
	int link[t2];
	int degree;
	char leaf;
	
};
void btree_insert(FILE * f, struct key k, int * r, int *size);
void btree_splitchild(FILE * f, int parent, int i, int *size);
void btree_insert_nonfull(FILE * f, struct key  k, int index, int *size);
//void btree_delete (struct node **arr, struct key k );
//void merge(struct node ** arr1, int i);
int search (FILE * f, int index, int k);
int main()
{
	int root=0;
	int size_arr = 1;
	int i,j;
	FILE *fptr;
	FILE *f1;
	f1=fopen("in1.csv","r");
	if ((fptr = fopen("input.bin","wb+")) == NULL){
       		printf("Error! opening file");
	    exit(1);
	   }
	struct node head,h1;
	head.degree =0;
	head.leaf='1';
	for(i=0;i<t2;i++)
		head.link[i]=-1;
   	fwrite(&head, sizeof(struct node), 1, fptr);
	for(i=0;i<1000000;i++)
	{
		struct key temp;
		fscanf(f1,"%d,%[^,],%[^,],%d,%d", &temp.sr_no, temp.country, temp.type, &temp.a, &temp.b);
		
		btree_insert(fptr, temp,&root, &size_arr );
		

	}
	int x;
	printf("1000000 elements inserted\n Enter a value between 0 and 1000000\n");
	scanf("%d",&x);
	
	//for(i=0;i<1000000;i++)
		int m;
		m=search(fptr,root,x);
		struct node tree;
		int k=0;
		if(m!=-1)
		{
		fseek(fptr, (sizeof(struct node))*m , SEEK_SET);
		fread(&tree, sizeof(struct node), 1, fptr);
		while((k<tree.degree) && (tree.elem[k].sr_no!=i))
			k++;
		printf("element entered:%d\nelement found:  node no.:%d\nindex of the element in the node:%d\n",tree.elem[k].sr_no,m,k);
		}
		else
			printf("not found\n");
//	}
	
	fclose(fptr);
	
	
}
void btree_insert(FILE * f, struct key k, int * r, int *size)
{
	int i;
	struct node h,temp,temp1;
	
	fseek(f, (sizeof(struct node))*(*r) , SEEK_SET);
	fread(&h, sizeof(struct node), 1, f);
	
	
	if(h.degree == t1)
	{
		
		
		struct node n,n1;
		n.leaf = '0';
		n.degree = 0;
		n.link[0]=(*r);
		
		fseek(f, (sizeof(struct node))*(*size) , SEEK_SET);
		fwrite(&n, sizeof(struct node), 1, f);
		(*size) = (*size)+1;
		(*r)=(*size)-1;
		btree_splitchild(f,((*size)-1),0,size);
		btree_insert_nonfull(f,k,*r,size);
	
	}
	else
	{
	btree_insert_nonfull(f,k,*r,size);
	
	
	}
	
}
void btree_splitchild(FILE *f, int parent, int i, int *size)
{
	
	struct node new1;
	new1.degree =(t2/2)-1;
	(*size) = (*size)+1;
	struct node r1, arr1;
	struct node *r, *arr;
	int child;
	
	fseek(f, (sizeof(struct node))*(parent) , SEEK_SET);
	fread(&arr1, sizeof(struct node), 1, f);
	
	arr=&arr1;
	child=arr->link[i];
	
	fseek(f, (sizeof(struct node))*(child) , SEEK_SET);
	fread(&r1, sizeof(struct node), 1, f);
	r=&r1;
	new1.leaf=r->leaf;
	int j;
	for(j=0;j<t2;j++)
		new1.link[j]=-1;
	for(j=0;j<((t2/2)-1);j++)
		new1.elem[j]=r->elem[j+((t2/2))];
	
	if(r->leaf=='0')
	{
		for(j=0;j<=((t2/2)-1);j++)
			new1.link[j]=r->link[j+(t2/2)];
	}
	int m;

	fseek(f, (sizeof(struct node))*((*size)-1) , SEEK_SET);
	fwrite(&new1, sizeof(struct node), 1, f);
	
	r->degree = (t2/2)-1;
	for(j=(arr->degree)+1; j>i+1; j--)
		arr->link[j]=arr->link[j-1];
	arr->link[i+1]=(*size)-1;
	
	for(j=(arr->degree); j>i; j--)
		{arr->elem[j]=arr->elem[j-1];} 
	
	arr->elem[i]=r->elem[((t2/2)-1)];
	arr->degree = arr->degree + 1;
	
	fseek(f, (sizeof(struct node))*(parent) , SEEK_SET);
	fwrite(arr, sizeof(struct node), 1, f);
	fseek(f, (sizeof(struct node))*(child) , SEEK_SET);
	fwrite(r, sizeof(struct node), 1, f);
	
	
}
int search (FILE * f , int  index, int k)
{
	int i=0;
	struct node a;
	
	fseek(f, (sizeof(struct node))*(index) , SEEK_SET);
	fread(&a, sizeof(struct node), 1, f);
	
	while((i<a.degree) && ( k > (a.elem[i].sr_no)))
		{ i++;}
	if((i<a.degree) && ( k == (a.elem[i].sr_no)))
		{return index;}
	else 
	{
		if(a.leaf=='1')
			return -1;
		else
			{return search(f, a.link[i],k);}
	}
}
void btree_insert_nonfull(FILE * f, struct key  k, int index, int *size)
{
	struct node  arr1,temp;
	
	fseek(f, ((sizeof(struct node))*(index)) , SEEK_SET);
	fread(&arr1, sizeof(struct node), 1, f);
	
	int i =arr1.degree-1;
	
	if((arr1.leaf) == '1' )
	{
		
		while((i>0) && ((k.sr_no) < (arr1.elem[i].sr_no)) )
		{	
			arr1.elem[i+1].sr_no=arr1.elem[i].sr_no;
			i=i-1;
		}
		
		arr1.elem[i+1] = k;
		arr1.degree = arr1.degree + 1;
		fseek(f, ((sizeof(struct node))*(index)) , SEEK_SET);
		fwrite(&arr1, sizeof(struct node), 1, f);
			
	}
	else
	{
		
		while((i>0) && ((k.sr_no) < (arr1.elem[i].sr_no)) )
			{ i=i-1;}
		
		i=i+1;
		int child=arr1.link[i];
		
		fseek(f, ((sizeof(struct node))*(child)) , SEEK_SET);
		fread(&temp, sizeof(struct node), 1, f);
		
		if( temp.degree == t1)
		{	
			btree_splitchild(f,index,i,size);
			fseek(f, ((sizeof(struct node))*(index)) , SEEK_SET);
			fread(&arr1, sizeof(struct node), 1, f);
			if((k.sr_no) > (arr1.elem[i].sr_no))
				{i++;}
			
		}
		
		 child=arr1.link[i];
		
		btree_insert_nonfull(f,k,child,size);
		
	}
	
}
