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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <pthread.h>
#include "mapreduce.h"
#include <assert.h>

/* Size of shared memory buffers */
#define MR_BUFFER_SIZE 1024
struct args;
struct args{
	struct map_reduce* mr;
	int in, out; 
	int i;//for the id
};
//the path is for creating thread
void *pathmap (void *path){
	struct args* a;
	a = (struct args*)path;
	a->mr->maps[a->i]=a->mr->map(a->mr, a->in, a->i, a->mr->tott);	//store the mapping status for test28
        shutdown(a->mr->sockfd_c[a->i],SHUT_RDWR);
	return NULL;
}
//the path for reducing thread
void *pathreduce (void *path){
	struct args* a = (struct args*)path;
        socklen_t clilen=sizeof(a->mr->c[0]);
        int i;
        //third step, accept. accept all of them, so make forloop
        for(i=0;i<a->mr->tott; i++){
                a->mr->sockfd_c[i]=accept(a->mr->sockfd_s, (struct sockaddr *) &(a->mr->c[i]), &clilen);
		if(a->mr->sockfd_c[i]==-1){
                        //if fail, close client
			close(a->mr->a[i].in);
                        close(a->mr->sockfd_c[i]);
                        return NULL;
                }
        }
        //reduce
	a->mr->rs=a->mr->reduce(a->mr, a->out, a->mr->tott);
	return NULL;
}

/* Allocates and initializes an instance of the MapReduce framework */
struct map_reduce *mr_create(map_fn map, reduce_fn reduce, int threads)
{
	// allocate space for whole map reduce structure
	struct map_reduce *mr = (struct map_reduce *)malloc(sizeof(struct map_reduce));
	//if fail
	if (mr==NULL){	return NULL;}
	
	// threads and its count
	mr->t = (pthread_t *)malloc((threads)*sizeof(pthread_t));
	if(mr->t==NULL){return NULL;}
	
	mr->a=malloc(threads*sizeof(struct args));	//argument for threads + reduce threads
	if(mr->a==NULL){return NULL;}

	mr->maps=malloc(threads*sizeof(int));
	// map and reduce, the function should now be connected to the pointer
	
	mr->map=map;
	mr->reduce=reduce;	
	mr->tott=threads;	

        mr->sockfd_c=malloc(threads*(sizeof(int)));
        mr->c=malloc(threads*(sizeof(struct sockaddr_in)));

	return mr;	
}

/* Destroys and cleans up an existing instance of the MapReduce framework */
void mr_destroy(struct map_reduce *mr)
{
	free(mr->t);
	free(mr->a);
	free(mr->maps);
	free(mr->sockfd_c);
	free(mr->c);
	free(mr);
}
/* Begins a multithreaded MapReduce operation */
int mr_start(struct map_reduce *mr, const char *path, const char *ip, uint16_t port)
{
	if(mr==NULL){return 1;} //if fail
	int i, j;
	struct sockaddr_in saddr;
        struct sockaddr_in caddr;
	bzero((char*)&saddr, sizeof(saddr));
	bzero((char*)&caddr, sizeof(caddr));
        //keep failing reduce.... 
	caddr.sin_family=AF_INET;
        caddr.sin_port=htons(port);
        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(port);

if(mr->reduce==NULL){
	for (i = 0; i < mr->tott; i++) {
		if (inet_aton(ip, &caddr.sin_addr)==0){return 1;}//fail
		//since each thread should have a different open file, open every single time
		//create thread for fuction path map
		mr->a[i].mr=mr;
		mr->a[i].i=i;
		mr->a[i].in= open(path, O_RDONLY);
		if(mr->a[i].in==-1){
			close(mr->a[i].in);
			return 1;
		}
		mr->c[i]=caddr;
                //create socket
		mr->sockfd_c[i]=socket(AF_INET, SOCK_STREAM, 0);
                if (mr->sockfd_c[i]==-1){
			for(j=0;j<=i;j++){
				close(mr->sockfd_c[j]);
				close(mr->a[j].in);
				return 1;
			}
		}    // fail
                //this connects the client socket to server
                if (connect(mr->sockfd_c[i], (const struct sockaddr *)&saddr, sizeof(saddr))==-1){
                        //close all sockets if any fail- attempt to fix file left open
                        for(j=0;j<=i;j++){
				close(mr->sockfd_c[j]);
				close(mr->a[j].in);
        	                return 1;
			}
                } //fail

		if(pthread_create(&mr->t[i], NULL, pathmap, (void *)&mr->a[i])){
                        for(j=0;j<=i;j++){
				close(mr->sockfd_c[j]);
				close(mr->a[j].in);
				return 1;
			}
		}
	}
}
//server
else{
	if (inet_aton(ip, &saddr.sin_addr)==0){return 1;}//fail
	//create thread for function pathreduce
	//I'm lazy so just used index 0	
	mr->a[0].mr=mr;
	mr->a[0].out= open(path, O_WRONLY | O_CREAT | O_TRUNC);
	if(mr->a[0].out==-1){
		close(mr->a[0].out);
		return 1;
	}
	mr->s=saddr;
        mr->sockfd_s=socket(AF_INET, SOCK_STREAM, 0);
	//create socket
        if (mr->sockfd_s==-1){
		close(mr->sockfd_s);
		close(mr->a[0].out);
		return 1;
	}
	//this allows using same addresses 
        if (setsockopt(mr->sockfd_s, SOL_SOCKET, (SO_REUSEADDR | SO_REUSEPORT), &(int){1}, sizeof(int))==-1){
		close(mr->sockfd_s);
		close(mr->a[0].out);
		return 1;
	}
        //bind socket
        if (bind(mr->sockfd_s, (const struct sockaddr *)&saddr, sizeof(saddr))==-1){
		close(mr->sockfd_s);
		close(mr->a[0].out);
		return 1;
	}
        //in order to solve the "file descripter left open"
        //listen socket
        if (listen(mr->sockfd_s, 5)==-1){
		close(mr->sockfd_s);
		close(mr->a[0].out);
		return 1;
	}

	if (pthread_create(&mr->r, NULL, pathreduce, (void *)&mr->a[0])){
		close(mr->sockfd_s);
		close(mr->a[0].out);
		return 1;
	}	
}	
	return 0;
}
int mr_finish(struct map_reduce *mr)
{
        int i, j=0;
        //for client
        if (mr->reduce==NULL){
                for (i = 0; i < mr->tott; i++) {
                        if(pthread_join(mr->t[i], NULL) ){j++;}
                        if(mr->maps[i]!=0){j++;}           
                        if(close(mr->sockfd_c[i])!=0){j++;}
			close(mr->a[i].in);
                }
        }
        //for server:
        if(mr->map==NULL){
                if(mr->rs!=0) {j++;}
                if(pthread_join(mr->r, NULL)){j++;}
                if(close(mr->sockfd_s)!=0){j++;}
                for(i=0;i<mr->tott; i++){if(close(mr->sockfd_c[i])!=0){j++;}}
		close(mr->a[0].out);
        }

        if(j==0){return 0;}
 
        else{return 1;}
}

/* Called by the Map function each time it produces a key-value pair */
int mr_produce(struct map_reduce *mr, int id, const struct kvpair *kv)
{	
	if(kv==NULL){return -1;}
        //first, it's about the key
        uint32_t s= htonl(kv->keysz);
        if (send(mr->sockfd_c[id], &s, sizeof(uint32_t), 0)==-1){return -1;}
        if (send(mr->sockfd_c[id], kv->key, kv->keysz, 0)==-1){return -1;}

        //second, it's about the value
        uint32_t v=htonl(kv->valuesz);
        if (send(mr->sockfd_c[id], &v, sizeof(uint32_t), 0)==-1){return -1;}
        if (send(mr->sockfd_c[id], kv->value, kv->valuesz, 0)==-1){return -1;}
        return 1;
}

/* Called by the Reduce function to consume a key-value pair */
int mr_consume(struct map_reduce *mr, int id, struct kvpair *kv)
{
	if(kv==NULL){return -1;}
        uint32_t sk;//size for the key
        //use recv instead of read, since the field is clearer to manipulate
        if (recv (mr->sockfd_c[id], &sk, sizeof(uint32_t), 0)==0){return 0;}
	kv->keysz=ntohl(sk);
        if (recv (mr->sockfd_c[id], kv->key, kv->keysz, 0)==0){return 0;}
        
	if (recv (mr->sockfd_c[id], &sk, sizeof(uint32_t), 0)==0){return 0;}
	kv->valuesz=ntohl(sk);
        if (recv (mr->sockfd_c[id], kv->value, kv->valuesz, 0)==0){return 0;}
        return 1;
}


