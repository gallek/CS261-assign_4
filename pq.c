/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <assert.h>


#include "pq.h"
#include "dynarray.h"

#define HEAP_CAP 16
/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
	struct dynarray* dyn;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* p = malloc(sizeof(struct pq));
	assert(p);
  	p->dyn=dynarray_create(HEAP_CAP);
	return p;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	assert(pq);
	dynarray_free(pq->dyn);
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	assert(pq);
	return dynarray_size(pq->dyn)==0; //gives the size of the array to show if it is empty or not
}

struct pq_val{ //struct to actually access an element in the priority queue
	int prior;
	void* val;
};

/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	assert(pq);
	struct pq_val* pv=malloc(sizeof(struct pq_val)); //creates the struct to access the elements
	pv->val=value; //gets the value
	pv->prior=priority; //gets the priority number
	dynarray_insert(pq->dyn, -1, pv); //inserts the element into the heap array
	int elem=dynarray_size(pq->dyn)-1; //gets where in the heap to inser the next element
	int par=0; //parent value
	struct pq_val* temp;
	while(par>=0){ 
		par=(elem-1)/2; //how to get the correct parent (where to start)
		temp=(struct pq_val*)dynarray_get(pq->dyn, par); //the temp gets the parents vals so it acts as the parent
		if(temp->prior > pv->prior){ //percolates the newly added element in order to make sure that every parent has a smaller priority than children 
			dynarray_set(pq->dyn, par, pv);//at the parent index, set the index with the inserted value
			dynarray_set(pq->dyn, elem, temp); //set with the parent original information
			elem=par; //get the new parent information
		}
		else{
			break;
		}
	}
	temp=NULL; //set to NULL if the parent value is less than 0
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  	assert(pq);
	assert(dynarray_size(pq->dyn)>0); //makes sure that an error will be thrown if there are no elements in the dynarray
	struct pq_val* first=NULL; //creates a struct to access the first element
	first=(struct pq_val*)dynarray_get(pq->dyn, 0); //gets the actual element in the first position 
	if(first!=NULL){ //if there is an element
		return first->val; //return the value
	}
	else{
		return NULL;
	}
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	assert(pq); //makes sure the struct is not empty
	assert(dynarray_size(pq->dyn)>0); //makes sure that an error will be thrown if there are no elements in the dynarray
        struct pq_val* first=NULL; //creates a struct to access the first element
        first=(struct pq_val*)dynarray_get(pq->dyn, 0); //gets the actual element in the first position
        if(first!=NULL){ //if there is an element
		return first->prior; //return the priority integer
	}
	else{
		return 0;
	}
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	assert(pq); //makes sure struct is not empty
	assert(dynarray_size(pq->dyn)>0);//makes sure that an error will be thrown if there are no elements in the dynarray
	struct pq_val* first=NULL; //access first element
	struct pq_val* last = NULL; //access last element

	first=(struct pq_val*)dynarray_get(pq->dyn, 0);//the highest priority element is at the 0 index so this should be what the first element shoudl be set to
	last=(struct pq_val*)dynarray_get(pq->dyn, -1); //the last element has the lowest priority

	dynarray_set(pq->dyn, 0, last); //replaces the highest priority element
	dynarray_remove(pq->dyn, -1); //removes tht element from the array after information was already copied over
	last=NULL;//set back to last not accessing anything

	int elem=0; 
	int left=0;//left child element
	int right=0; //right child element
	int min=0;
	
	struct pq_val* t1; //temp to get an element
	struct pq_val* t2; //temp to get an element 
	
	while(1){ //while true
		left=(2*elem)+1; //set to left
		right=(2*elem)+2; //set to right
	
		if(left>dynarray_size(pq->dyn)-1) 
			break;
		else if(left==dynarray_size(pq->dyn)-1)
			min=left;
		else{
			if(((struct pq_val*)dynarray_get(pq->dyn, left))->prior >((struct pq_val*)dynarray_get(pq->dyn, right))->prior)//makes sure every node's priority value is smaller than the children (percolate)
				min=right; //if left is bigger than right, the minimum is the right
			else
				min=left; //if right is bigger than left, the minimum is the left
		}
		if((((struct pq_val*)dynarray_get(pq->dyn, elem))->prior)>(((struct pq_val*)dynarray_get(pq->dyn, min))->prior)){ //continues percolation
			t1=(struct pq_val*)dynarray_get(pq->dyn,min); //temp gets the min information
			t2=(struct pq_val*)dynarray_get(pq->dyn, elem); //temp 2 gets the element information
			dynarray_set(pq->dyn, min, t2);
			dynarray_set(pq->dyn, elem, t1);
			elem=min;
		}
		else
			break;
	}
	if(first!=NULL){
		void*val=first->val;
		free(first); //freees memory
		return val;
	}
	else{
		return NULL;
	}
}
