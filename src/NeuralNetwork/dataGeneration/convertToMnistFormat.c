#include "../../ImageProcessing/ImageProcess.h"
#include <dirent.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define IMAGE_W 28
#define IMAGE_H 28
#define MAGIC 42

#define CHARS_IN_INT32 sizeof(int32_t) / sizeof(char)

void writeImg(FILE *file, char *img_path) {
    SDL_Surface *img = SDL_Start(img_path);

    size_t img_size = IMAGE_H * IMAGE_W;
    if (IMAGE_W != img->w || IMAGE_H != img->h)
        errx(1, "image size mismatch, should be %dx%d, found %dx%d", IMAGE_W,
             IMAGE_H, img->w, img->h);
    unsigned char *buffer = malloc(img_size * sizeof(unsigned char));

    const SDL_PixelFormat *format = img->format;
    Uint32 *pixtab = img->pixels;
    Uint8 r, g, b, a;
    for (size_t x = 0; x < img_size; ++x) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
        buffer[x] = r;
    }

    if (fwrite(buffer, sizeof(unsigned char), img_size, file) != img_size)
        errx(1, "error writing an image");

    SDL_FreeSurface(img);
    free(buffer);
}

void WriteInt32(FILE *file, int32_t *to_write, size_t len) {
    size_t buff_len = len * CHARS_IN_INT32;
    unsigned char *buffer = malloc(buff_len * sizeof(char));

    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < CHARS_IN_INT32; j++) {
            buffer[i * CHARS_IN_INT32 + CHARS_IN_INT32 - j - 1] = to_write[i];
            to_write[i] >>= 8;
        }
    }

    if (fwrite(buffer, sizeof(char), buff_len, file) != buff_len)
        errx(1, "error writing headers");

    free(buffer);
}

void WriteImages(char *path, int32_t magic, int32_t qtt, char **files_name) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        errx(1, "error opening file %s", path);
    }

    // 0: magic number ; 1: number of images ; 2: image height ; 3: image width
    int32_t *headers = malloc(4 * sizeof(int32_t));
    headers[0] = magic;
    headers[1] = qtt;
    headers[2] = IMAGE_H;
    headers[3] = IMAGE_W;

    printf("Writting images (magic: %d) -> qtt: %d  h: %d  w: %d\n", headers[0],
           headers[1], headers[2], headers[3]);
    WriteInt32(file, headers, 4);
    free(headers);

    for (int32_t i = 0; i < qtt; ++i)
        writeImg(file, files_name[i]);

    fclose(file);
}

void WriteLabels(char *path, int32_t magic, int32_t qtt, char *labels) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        errx(1, "error opening file %s", path);
    }

    // 0: magic number ; 1: number of labels
    int32_t *headers = malloc(2 * sizeof(int32_t));
    headers[0] = magic;
    headers[1] = qtt;
    printf("Writting labels (magic: %d) -> qtt: %d\n", headers[0], headers[1]);
    WriteInt32(file, headers, 2);

    free(headers);

    if (fwrite(labels, sizeof(unsigned char), qtt, file) != (size_t)qtt)
        errx(1, "error writing labels");

    fclose(file);
}

int32_t numberOfFiles(DIR *dp) {
    struct dirent *ep;
    int32_t count = 0;
    while ((ep = readdir(dp)) != NULL) {
        if (strcmp(".", ep->d_name) == 0 || strcmp("..", ep->d_name) == 0)
            continue;
        ++count;
    }
    return count;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s <dirpath/> <new file name>\n", argv[0]);
        return 1;
    }
    DIR *dp;
    struct dirent *ep;
    char *dirpath;
    int32_t qtt = 0;

    for (int i = 0; i <= 9; ++i) {
        asprintf(&dirpath, "%s%d/", argv[1], i);
        dp = opendir(dirpath);
        if (dp == NULL)
            errx(1, "couldn't open this directory or directory missing the "
                    "required subdirectory (0/ 1/ 2/ ... 9/)");

        qtt += numberOfFiles(dp);

        (void)closedir(dp);
    }

    char *labels = malloc(qtt * sizeof(char));
    char **files_name = malloc(qtt * sizeof(char *));

    int j = 0;
    for (int i = 0; i <= 9; ++i) {
        asprintf(&dirpath, "%s%d/", argv[1], i);
        dp = opendir(dirpath);
        if (dp == NULL)
            errx(1, "error reopening directory");

        while ((ep = readdir(dp)) != NULL) {
            if (strcmp(".", ep->d_name) == 0 || strcmp("..", ep->d_name) == 0)
                continue;
            asprintf(files_name + j, "%s%s", dirpath, ep->d_name);
            labels[j] = i;
            ++j;
        }

        (void)closedir(dp);
    }

    char *save_name;
    asprintf(&save_name, "%s.img", argv[2]);
    WriteImages(save_name, MAGIC, qtt, files_name);

    asprintf(&save_name, "%s.label", argv[2]);
    WriteLabels(save_name, MAGIC, qtt, labels);

    free(labels);
    free(files_name);
    return 0;
}
