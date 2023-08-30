#ifndef LIST_H
#define LIST_H
#include <stdlib.h> // if you want to use some other allocator, just implement something called malloc, remalloc and free ig (and remove this import)
#include <assert.h> // same with assert

// Cool macro stuff lol
// https://stackoverflow.com/questions/56657752/c-macro-expansion-within-a-macro

#ifndef DEFAULT_LIST_CAP
// Not sure what exactly to put here, but oh well :D Power of two seems nice though
#define DEFAULT_LIST_CAP (16)
#endif // DEFAULT_LIST_CAP

// GENERATOR FOR THE STRUCTURE BY ITSELF (USEFUL FOR CICULAR REFERENCES)
#define GENERATE_LIST_STRUCT_FOR_TYPE(CONTENT_TYPE)\
typedef struct CONTENT_TYPE##List {\
    CONTENT_TYPE* content;\
    size_t size;\
    size_t capacity;\
} CONTENT_TYPE##List;

// GENERATOR FOR THE FUNCTIONS BY ITSELF (USEFUL FOR CICULAR REFERENCES)
#define GENERATE_LIST_FUNCTIONS_FOR_TYPE(CONTENT_TYPE)\
CONTENT_TYPE##List CONTENT_TYPE##List_new(size_t initial_capacity) {\
    return (CONTENT_TYPE##List) {\
        .content = malloc(sizeof(CONTENT_TYPE) * initial_capacity),\
        .size = 0,\
        .capacity = initial_capacity\
    };\
}\
\
void CONTENT_TYPE##List_push(CONTENT_TYPE##List* this, CONTENT_TYPE item) {\
    if (this->size == this->capacity) {\
        CONTENT_TYPE* tmp = realloc(this->content, sizeof(CONTENT_TYPE)*this->capacity*2);\
        assert(tmp && "Buy more RAM lol");\
        this->content = tmp;\
        this->capacity *= 2;\
    }\
    this->content[this->size++] = item;\
}\
\
void CONTENT_TYPE##List_free(CONTENT_TYPE##List* this) {\
    free((void*) this->content);\
    this->content = NULL;\
    this->size = 0;\
    this->capacity = 0;\
}

#define GENERATE_LIST_FOR_TYPE(CONTENT_TYPE)\
GENERATE_LIST_STRUCT_FOR_TYPE(CONTENT_TYPE)\
GENERATE_LIST_FUNCTIONS_FOR_TYPE(CONTENT_TYPE)

#define NEW_LIST_OF_TYPE(CONTENT_TYPE) CONTENT_TYPE##List_new(DEFAULT_LIST_CAP)

#endif // LIST_H