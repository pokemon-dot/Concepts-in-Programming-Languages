# Malloc_free.c file

Implements the “allocate” and “free” functions. It also ensures that adjacent free blacks are merged together.

~~~C
#include<stddef.h>
~~~

**stddef.h** is required because the definition for size_t datatype is found there.

~~~C
char memory[20000];
~~~

This is the declaration of the array which is considered as our memory. We get a contiguous allocation of memory by using an array.
Here, I assumed a block of memory of size 20,000 bytes char memory[20000];  (assuming that the storage for a character is 1 byte in the machine) and all the data structures and the allocated memory blocks reside in this same chunk of memory.

~~~C
struct block *freeList=(void*)memory;
~~~

Here we initialize a pointer of type **"meta_block"**, named freeList pointing to the starting address of the chunk of memory we created before. This freeList pointer will point to the start of the linked list of metadata blocks.The starting address of the array (memory) should be casted to type void so that we are able to allocate blocks of memory which are of different datatypes.(int, char, float etc.)

~~~C
/*Initializing the block of memory*/
void initialize()
{
 freeList->size=20000-sizeof(struct block); 
 freeList->free=1;
 freeList->next=NULL;
}
~~~

This is where we initialize the first metadata block, update it to refer to the next block of memory.
The size of the block that it refers to is (20000 bytes- the_size_of_one_metadata_block)
To indicate that the block is not yet allocated, we set the free flag to 1.
And the first metadata block has no next metadata block yet. So we set next to NULL.

~~~C
/*Making way for a new block allocation by splitting a free block -- (Assume first fit algorithm)*/
void split(struct block *fitting_slot,size_t size)
{
 struct block *new=(void*)((void*)fitting_slot+size+sizeof(struct block));
 new->size=(fitting_slot->size)-size-sizeof(struct block);
 new->free=1;
 new->next=fitting_slot->next;
 fitting_slot->size=size;
 fitting_slot->free=0;
 fitting_slot->next=new;
}
~~~

We use the **First-fit-algorithm** to find a free block to allocate memory. Assume that we get a request to allocate a block of memory of size 500 bytes. Starting from the first metadata block we can go on searching for the first block which has enough space to allocate. If we get the free block sizes to be 250, 130 and 750 in order, we will allocate the block of size 750.

If we find a free block which exactly fits the required size, we don't need to do the splitting. So this function is only required if we have what is more than required.

The arguments accepted by this function are --- A pointer to the metadata block which refers to the block of memory whose size is more than required.(fitting_slot) and the required size of the memory chunk to be allocated.

Here we create a new metadata block pointer called "new". And it should point to the location provided by passing(setting aside) a block of memory which is equal to **the_size_of_the_metadata_block_we_considered + the_size_requested_to_be_allocated**

Then this new pointer points to the metadata block referring to the next free chunk.
As you can see from the code, I have set the attributes of both the new and fitting_slot metadata blocks accordingly.

The malloc and free functions are named here as ``MyAlloc()`` and ``MyFree()``.

~~~C
if(!(freeList->size))
{ 
 initialize();
 printf("Memory initialized\n");
}
~~~

This condition will initialize the memory if it is not initialized at the beginning. This set of statements will be executed only if the size of the first metadata block is not set yet. That means, if the memory is still not initialized.

~~~C
while(((curr->size<noOfBytes)||(curr->free==0))&&(curr->next!=NULL))
~~~

If this condition is met, the metadata block we checked cannot be used for the allocation. So we execute the following statements. That is you go on checking one metadata block at a time

~~~C
if(curr->size==noOfBytes)
~~~

If this condition is met, the metadata block we checked refers to a chunk of memory that exactly fits the required size. So set the free flag to 0, indicating that it is allocated. Then return the starting address of the block of allocated memory.

~~~C
else if(curr->size>(noOfBytes+sizeof(struct block)))
~~~

If this condition is met, the metadata block we checked refers to a chunk of memory that is of size greater than what is required. So invoke the ``split()`` function to allocate only the block which is required and then return the starting address of the block of allocated memory.

There can be a situation where you have consecutive blocks that are set free by deallocating after they were previously allocated. This results in external fragmentation which will cause the ``MyAlloc()`` function to return a NULL pointer although we have enough memory to allocate. Therefore we use a function called ``merge()`` to join the consecutive free blocks by removing the metadata blocks lying in between.

After we allocate memory for some purpose, it is a really good practice to deallocate that memory if you have finished using it, so that it can be used by another person for his memory requirement.
We use the ``MyFree()`` function for that.

It takes the pointer to a block of memory previously allocated as a parameter.

~~~C
if(((void*)memory<=ptr)&&(ptr<=(void*)(memory+20000)))
~~~

Here we check whether the address to which the pointer given as an argument to the function actually lies within the address range of the memory array that we used for the purpose. If yes,we simply set the free flag in the metadata block to 1 indicating that it is free and scan through and merge the consecutive blocks that are free, if any.
