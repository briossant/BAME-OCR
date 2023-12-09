#ifndef INTERFACE
#define INTERFACE

typedef struct ThreadParameters {
    int auto_rotate;
    char *filename;
    int step_index;
    double angle;
    char *filename_resolved;
    void (*show_img)(void);
    void (*raise_error)(char *);
} ThreadParameters;

void *BAME(void *data);

#endif
