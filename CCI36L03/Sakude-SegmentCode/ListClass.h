#ifndef __LIST_CLASS
#define __LIST_CLASS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum {CHAR_T, STRING_T, INT_T, FLOAT_T, DOUBLE_T } basic_type;

#define N_VERTEX 50
#define MEM_INC 20
#define OP_SUCCESS 1
#define OP_FAILURE -1
template <class T>  
class LIST_CLASS {
public:
   int n,vector_size,vector_inc;
   T *elem;

   LIST_CLASS(int init_size, int incr);
   ~LIST_CLASS(){delete elem;};
   void Reset(){n=0;};
	T GetElem(int i);  // Get element index i
	int Add(T el); // Add el as last element

	int SetElem(int index, T el); // Replace data at index i with el
	int AddAtIndex(int i,T el); // Add el at index i (last if i>n) 
	int RemoveThis(T el); // Remove el 
			//NOTE:: DO NOT USE RemoveThis for complex type T (struct or class)
			//   T must be simple type, such as pointer, int, float, double
	int GetIndex(T el); // Get index of element el
				//NOTE:: DO NOT USE GetIndex for complex type T

    int GetNElem();  // Get number of elements
	int RemoveIndex(int i); // Remove element of index i, return SUCCESS or FAILURE

	T TRemoveIndex(int i); // Remove and return  element i
	T Remove();  // remove last element
	void Copy(LIST_CLASS<T> *from);
	void SetAll(T el);
	void Print(char *message, basic_type type);
	T GetLastElem();
	void Save(char *filename, void SaveElem(FILE *, T el));
	void Load(char *filename, void LoadElem(FILE *, T el));
};


template <class T>
class QUEUE:public LIST_CLASS<T>
{
public:
	QUEUE(int size, int inc):LIST_CLASS<T>(size,inc){};
	void Add(T elem); // add in the first index elem 
	T GetElem();   // Get last element 

	// Remove() is the same (last elem) 
};


template<class T>
LIST_CLASS<T>::LIST_CLASS(int init_size,int incr)
{
   n=0;
   if (init_size<=0)
      init_size=MEM_INC;
   if (incr<=0)
	   incr=MEM_INC;
   
   vector_size=init_size;
   vector_inc=incr;
   elem= new T[vector_size];
}
// add a vertex index in the end of the list 

template<class T>
int LIST_CLASS<T>::Add(T el)
{
  if (n==vector_size) // reallocate space
    {
        vector_size+=vector_size;
	    elem = (T *)realloc( elem,vector_size* sizeof(T) );
		if (elem==NULL)
			return OP_FAILURE;
     }

  elem[n]=el;
  n++;
  return OP_SUCCESS;
}

template<class T>
int LIST_CLASS<T>::RemoveThis(T el)
{   int i=GetIndex(el);

     if (i!=OP_FAILURE)
		{
			if (i<n) // if not last element, move, advance all following elements  
		      memmove(&elem[i], &elem[i+1], sizeof(T)*(n-i-1));
            n--;  // decrease n
		    return OP_SUCCESS;
		}
   return OP_FAILURE;
}

template<class T>
int LIST_CLASS<T>::GetIndex(T el)
{  int i;
	if (n>0)
    for (i=0; i<n; i++)  // search for the element el in vector
		if (elem[i]==el)  // if found
		return i;
  return OP_FAILURE;;
}

template<class T>
T LIST_CLASS<T>::GetElem(int i)
{ T a;
  
  if (i>=0 && i<n)
	  return elem[i];
  // index is ot valid: set NULL to a
  memset(&a,0x00,sizeof(T));
  return (a);
}
template<class T>
int LIST_CLASS<T>::GetNElem()
{
  return n;
}
template<class T>
int LIST_CLASS<T>::RemoveIndex(int i)
{ 
   if (i>=0 && i<n)
   {
    	if (i<n-1) // if not last element, move, advance all following elements  
		      memmove(&elem[i], &elem[i+1], sizeof(T)*(n-i-1));
        n--;  // decrease n
        return OP_SUCCESS;
   }	
  return OP_FAILURE;
}
template<class T>
T LIST_CLASS<T>::TRemoveIndex(int i)
{ T el;
if (i>=0){
   el = GetElem(i);
   RemoveIndex(i);
   return el;
}
 return el;
}


template<class T>
T LIST_CLASS<T>::Remove()
{
  return TRemoveIndex(n-1);
}

template<class T>
T LIST_CLASS<T>::GetLastElem()
{
  return GetElem(n-1);
}
template<class T>
int LIST_CLASS<T>::AddAtIndex(int i,T el) // Add el at index i (last if i>n) 
{
  if (i>=0) 
  {
	   if (n+1==vector_size) // reallocate space
    {
        vector_size+=vector_size;
	    elem = (T *)realloc( elem,vector_size* sizeof(T) );
		if (elem==NULL)
			return OP_FAILURE;
     }

     if (i<n)
	 {
	  	memmove(&elem[i+1], &elem[i], sizeof(T)*(n-i));
		elem[i]=el;
		n++;
	 }
	 else Add(el); 
	 return OP_SUCCESS;
  }
  return OP_FAILURE;
}

template<class T>
int LIST_CLASS<T>::SetElem(int i,T el) // Replace data with el at index i 
{
  if (i>=0 &&i<n)
	 {
		elem[i]=el;
	    return OP_SUCCESS;
  }
  return OP_FAILURE;
}

template<class T>
void LIST_CLASS<T>::Copy(LIST_CLASS<T> *from)
{  
	
	n = from->n;
	memmove(elem,from->elem,from->n*sizeof(T));

}

template<class T> 
void LIST_CLASS<T>::Print(char *message,basic_type type)
{
   int i;
   printf("%s : ",message);
   for (i=0; i<n; i++)
	   switch (type){
	case CHAR_T: printf("%c ",(char)elem[i]);
		break;
	case STRING_T: printf("%s ",(char *)elem[i]);
		break;		
	case INT_T: printf("%d ",(int)elem[i]);
		break;
	case FLOAT_T: printf("%f ",(float)elem[i]);
		break;
	case DOUBLE_T: printf("%lf ",(double)elem[i]);
		break;
   }
   printf("\n"); 
	    
}
template<class T> 
void LIST_CLASS<T>::SetAll(T el)
{
   int i;
 
   for (i=0; i<n; i++)
	  SetElem(i,el);
	    
}
template<class T> 
void LIST_CLASS<T>::Save(char *filename, void SaveElem(FILE *fil, T el))
{
	FILE *file;
	fopen_s(&file, filename,"w");
    if (file)
	{
	fprintf_s(file,"%d\n",n);	
        
   	for (int i=0; i<n; i++)
	{ SaveElem(file,elem[i]);
	}
	}
	fclose(file);
}
  
template<class T> 
void LIST_CLASS<T>::Load(char *filename, void LoadElem(FILE *fil, T el))
{
	FILE *file;
	fopen_s(&file, filename,"r");
    if (file)
	{
	fscanf_s(file,"%d",&n);		
        
   	for (int i=0; i<n; i++)
	{ LoadElem(file,elem[i]);
	}
	}
	fclose(file);
}
  
		    


template <class T>
void QUEUE<T>::Add(T elem)
{
	AddAtIndex(0,elem);
}
template <class T>
T QUEUE<T>::GetElem()
{
	return LIST_CLASS<T>::GetElem(GetNElem()-1);
}



#endif

/*
#include "vector.h"
  LIST_CLASS<int> int_vect(10,5);
 typedef  LIST_CLASS<float> float_vect_type;
  LIST_CLASS<float> *T_ptr_type;  // use pointer to structure 
  
   LIST_CLASS<int> *vect = new LIST_CLASS<int>(20,10);

point_vect_type v(10,5);

  */