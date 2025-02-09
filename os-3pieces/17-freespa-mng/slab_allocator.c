#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLAB_SIZE 1024 // Total memory size for the slab
#define OBJECT_SIZE 64  // Size of each object in the slab

typedef struct Slab {
    void *memory;
    int free_list[SLAB_SIZE / OBJECT_SIZE];
} Slab;

Slab *initialize_slab() {
    Slab *slab = (Slab *)malloc(sizeof(Slab));
    slab->memory = malloc(SLAB_SIZE);
    for (int i = 0; i < (SLAB_SIZE / OBJECT_SIZE); i++) {
        slab->free_list[i] = 1; // Mark all objects as free
    }
    return slab;
}

void *allocate_object(Slab *slab) {
    for (int i = 0; i < (SLAB_SIZE / OBJECT_SIZE); i++) {
        if (slab->free_list[i]) {
            slab->free_list[i] = 0; // Mark as allocated
            return (char *)slab->memory + (i * OBJECT_SIZE);
        }
    }
    return NULL; // No free space
}

void free_object(Slab *slab, void *ptr) {
    int index = ((char *)ptr - (char *)slab->memory) / OBJECT_SIZE;
    slab->free_list[index] = 1; // Mark object as free
}

void destroy_slab(Slab *slab) {
    free(slab->memory);
    free(slab);
}

int main() {
    Slab *slab = initialize_slab();
    
    void *obj1 = allocate_object(slab);
    void *obj2 = allocate_object(slab);
    printf("Allocated objects at %p and %p\n", obj1, obj2);
    
    free_object(slab, obj1);
    printf("Freed object at %p\n", obj1);
    
    void *obj3 = allocate_object(slab);
    printf("Reallocated object at %p\n", obj3);
    
    destroy_slab(slab);
    return 0;
}
