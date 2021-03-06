#include <stdio.h>

#define pixel_width 512
#define pixel_heigth 512
#define pixel_rgb 3


typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    unsigned short int pixel[pixel_width][pixel_heigth][pixel_rgb];
    unsigned int width;
    unsigned int height;
} Image;


Image read_pixelImage();
Image gray_scale(Image img);
Image filter_sepia(Image img);
void blur(unsigned int height, unsigned short int pixel[512][512][3], 
          int size, unsigned int width);
Image rotation(Image img);
Image mirroring(Image img);
void invert_colors(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height);
                    Image crop_image(Image img, int coordinate_x,
                     int coordinate_y, int width, int height);
Image rotate_right90(Image img);
void print_image(Image img);










int main() {
    Image img;

    img = read_pixelImage();

    int n_options;
    scanf("%d", &n_options);

    for(int i = 0; i < n_options; ++i) {
        int options;
        scanf("%d", &options);

        switch(options) {
            case 1: { // Gray scale
                img = gray_scale(img);
                break;
            }
            case 2: { // Filter sepia
                img = filter_sepia(img);
                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                blur(img.height, img.pixel, size, img.width);
                break;
            }
            case 4: { // Rotation
                img = rotation(img);
                break;
            }
            case 5: { // Mirroring
                img = mirroring(img);
                break;
            }
            case 6: { // Color inversion
                invert_colors(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Crop Image
                int coordenate_x, coordenate_y;
                scanf("%d %d", &coordenate_x, &coordenate_y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = crop_image(img, coordenate_x, coordenate_y, width, height);
                break;
            }
        }
    print_image(img);
    }
    return 0;
}

Image read_pixelImage(){
        char p3[4];
        scanf("%s", p3);
        Image img;

        // Read width height and color of image
        int max_color;
        scanf("%u %u %d", &img.width, &img.height, &max_color);

        for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }
    return img;

}

Image gray_scale(Image img) {

    for (unsigned int i =  0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int average = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];
            average /= 3;
            img.pixel[i][j][0] = average;
            img.pixel[i][j][1] = average;
            img.pixel[i][j][2] = average;
        }
    }

    return img;
}

Image filter_sepia(Image img){
    for (unsigned int x = 0; x < img.height; ++x) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[x][j][0];
            pixel[1] = img.pixel[x][j][1];
            pixel[2] = img.pixel[x][j][2];

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][0] = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][1] = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][2] = menor_r;
            }   
        }
        return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int size, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel average = {0, 0, 0};

            int smaller_height = (height - 1 > i + size/2) ? i + size/2 : height - 1;
            int min_w = (width - 1 > j + size/2) ? j + size/2 : width - 1;
            for(int x = (0 > i - size/2 ? 0 : i - size/2); x <= smaller_height; ++x) {
                for(int y = (0 > j - size/2 ? 0 : j - size/2); y <= min_w; ++y) {
                    average.red += pixel[x][y][0];
                    average.green += pixel[x][y][1];
                    average.blue += pixel[x][y][2];
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            pixel[i][j][0] = average.red;
            pixel[i][j][1] = average.green;
            pixel[i][j][2] = average.blue;
        }
    }
}

Image mirroring(Image img){
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img.width, height = img.height;

    if (horizontal == 1){
        width /= 2;
    }
    else {
        height /= 2;
    }

    for (int i2 = 0; i2 < height; ++i2) {
        for (int j = 0; j < width; ++j) {
            int x = i2, y = j;

            if (horizontal == 1){ 
                y = img.width - 1 - j;
            }
            else{ 
                x = img.height - 1 - i2;
            }

            Pixel aux1;
            aux1.red = img.pixel[i2][j][0];
            aux1.green = img.pixel[i2][j][1];
            aux1.blue = img.pixel[i2][j][2];

            img.pixel[i2][j][0] = img.pixel[x][y][0];
            img.pixel[i2][j][1] = img.pixel[x][y][1];
            img.pixel[i2][j][2] = img.pixel[x][y][2];

            img.pixel[x][y][0] = aux1.red;
            img.pixel[x][y][1] = aux1.green;
            img.pixel[x][y][2] = aux1.blue;
            }
        }
        return img;
}

Image rotation(Image img){
    int times = 0;
    scanf("%d", &times);
    times %= 4;
    for (int j = 0; j < times; ++j) {
        img = rotate_right90(img);
    }
    return img;
}

void invert_colors(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image crop_image(Image img, int coordinate_x, int coordinate_y, int width, int height) {
    Image cut_img;

    cut_img.width = width;
    cut_img.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cut_img.pixel[i][j][0] = img.pixel[i + coordinate_y][j + coordinate_x][0];
            cut_img.pixel[i][j][1] = img.pixel[i + coordinate_y][j + coordinate_x][1];
            cut_img.pixel[i][j][2] = img.pixel[i + coordinate_y][j + coordinate_x][2];
        }
    }

    return cut_img;
}

Image rotate_right90(Image img) {
    Image rotated;
    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j][0] = img.pixel[x][y][0];
            rotated.pixel[i][j][1] = img.pixel[x][y][1];
            rotated.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotated;
}

void print_image( Image img){
   
     // Print type of image
    printf("P3\n");
    // Print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // Print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);

        }
        printf("\n");
    }

}



