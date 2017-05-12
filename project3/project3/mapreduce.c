/*
 * Implementation file for simple MapReduce framework.  Fill in the functions
 * and definitions in this file to complete the assignment.
 *
 * Place all of your implementation code in this file.  You are encouraged to
 * create helper functions wherever it is necessary or will make your code
 * clearer.  For these functions, you should follow the practice of declaring
 * them "static" and not including them in the header file (which should only be
 * used for the *public-facing* API.
 */


/* Header includes */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "mapreduce.h"


/* Size of shared memory buffers */
#define MR_BUFFER_SIZE 1024
struct args;
struct args{
	struct map_reduce* mr;
	const char *in, *out; 
	int i;//for the id
};
//the path is for creating thread
void *pathmap (void *path){
	struct args* a;
	a = (struct args*)path;
	int IN =open(a->in, O_RDONLY);
	a->mr->maps[a->i]=a->mr->map(a->mr, IN, a->i, a->mr->tott);	//store the mapping status for test28
	close(IN);
	return NULL;
}
//the path for reducing thread
void *pathreduce (void *path){
	struct args* a = (struct args*)path;
	int OU= open(a->out, O_WRONLY | O_CREAT | O_TRUNC);
	a->mr->rs=a->mr->reduce(a->mr, OU, a->mr->tott);
	close(OU);
	return NULL;
}

/* Allocates and initializes an instance of the MapReduce framework */
struct map_reduce *mr_create(map_fn map, reduce_fn reduce, int threads)
{
	// allocate space for whole map reduce structure
	struct map_reduce *mr = (struct map_reduce *)malloc(sizeof(struct map_reduce));
	//if fail
	if (mr==NULL){	return NULL;}
	// pointers pointing to kvpairs for the buffers
	mr->buff = (char *)malloc(MR_BUFFER_SIZE*sizeof(char));
	if(mr->buff==NULL){return NULL;}
	// counter in order to check if the buffer is already full
	mr->count = (int *)malloc((threads)*sizeof(int));
	if(mr->count==NULL){return NULL;}
	// threads and its count
	mr->t = (pthread_t *)malloc((threads)*sizeof(pthread_t));
	if(mr->t==NULL){return NULL;}
	//for the files
	mr->infile= (FILE**)malloc(threads*sizeof(FILE)); 
	//each thread open it once
	if(mr->infile==NULL){return NULL;}
	mr->outfile= (FILE*)malloc(sizeof(FILE));
	if(mr->outfile==NULL){return NULL;}
	mr->a=malloc((threads+1)*sizeof(struct args));	//argument for threads + reduce threads
	if(mr->a==NULL){return NULL;}
	mr->maps=malloc(threads*sizeof(int));
	// map and reduce, the function should now be connected to the pointer
	int i=0;
	mr->map=map;
	mr->reduce=reduce;	
	mr->tott=threads;	
	return mr;	
}

/* Destroys and cleans up an existing instance of the MapReduce framework */
void mr_destroy(struct map_reduce *mr)
{
	free(mr->buff);
	free(mr->count);
	free(mr->t);
	free(mr->infile);
	free(mr->outfile);
	free(mr->a);
	free(mr->maps);
	free(mr);
}
/* Begins a multithreaded MapReduce operation */
int mr_start(struct map_reduce *mr, const char *inpath, const char *outpath)
{
	int i;
	// for test30:
	int test30=open(inpath, O_RDONLY);
	if(test30==-1){return 1;}
	close(test30);
	for (i = 0; i < mr->tott; i++) {
		//since each thread should have a different open file, open every single time
		//create thread for fuction path map
		mr->a[i].mr=mr;
		mr->a[i].i=i;
		mr->a[i].in=inpath;
//		mr->infile[i] = fopen(inpath, "r");
//		mr->a->in = fileno(mr->infile[i]);
		if(pthread_create(&mr->t[i], NULL, pathmap, (void *)&mr->a[i])){
			return 1;
		}
	}
	//create thread for function pathreduce
	//I'm lazy so just used index 0	
	mr->a[0].mr=mr;
	mr->a[0].out=outpath;
//	mr->outfile = fopen(outpath, "w");
//	mr->a->out = fileno(mr->outfile);
	if (pthread_create(&mr->r, NULL, pathreduce, (void *)&mr->a[0])){
		return 1;
	}	
	
	return 0;
}

/* Blocks until the entire MapReduce operation is complete */
int mr_finish(struct map_reduce *mr)
{
	int i, j=0;
	for (i = 0; i < mr->tott; i++) {
		if(pthread_join(mr->t[i], NULL) )
		j++;
	}
	for (i=0; i<mr->tott; i++){
		if(mr->maps[i]!=0) {j++;}
	}
	
	if(pthread_join(mr->r, NULL)){
		j++;}
	if(mr->rs!=0) {j++;}
	if (j==0){return 0;}
	else{return 1;}
}

/* Called by the Map function each time it produces a key-value pair */
int mr_produce(struct map_reduce *mr, int id, const struct kvpair *kv)
{

	return 0;
}

/* Called by the Reduce function to consume a key-value pair */
int mr_consume(struct map_reduce *mr, int id, struct kvpair *kv)
{
	return 0;
}

