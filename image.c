#include "main.h"

double findDist(vec a, vec b) {
    double r = sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
    return r;
}

vec addVec(vec a, vec b) {
    vec sum = {a.x + b.x, a.y + b.y};
    return sum;
}

imageError paintPixel(image* img, vec loc, pixel color) {
    uint32_t x = (uint32_t)round(loc.x);
    uint32_t y = (uint32_t)round(loc.y);

    if(x < img->info.width && y < img->info.height) {
        img->array[y][x] = color;
    }
    else {
        return IWAR_OOB;
    }

    return IERR_NONE;
}

pixel getPixel(image* img, vec loc) {
    uint32_t x = (uint32_t)round(loc.x);
    uint32_t y = (uint32_t)round(loc.y);
    pixel color = {0, 0, 0};

    if(x < img->info.width && y < img->info.height) {
        color = img->array[y][x];
    }

    return color;
}

int comparePixels(pixel a, pixel b) {
    if(a.r == b.r && a.g == b.g && a.b == b.b)
        return 1;
    else
        return 0;
}

imageError drawPxLine(image* img, vec a, vec b, pixel color) {
    double k;

    if((b.x - a.x) != 0)
        k = fabs((b.y - a.y) / (b.x - a.x));

    uint32_t i;
    uint32_t iadd;
    uint32_t idest;
    double j;
    double ptj;
    double jadd;
    int mode;
    vec pt = {floor(a.x), floor(a.y)};
    paintPixel(img, pt, color);

    if((b.x - a.x) == 0) {

        if(b.y - a.y < 0)
            return drawPxLine(img, b, a, color);
        pt.x = a.x;
        double p = a.y;
        while(1) {
            pt.y = p;
            paintPixel(img, pt, color);
            p++;
            if(p == b.y)
                break;
        }
        return 0;
    }

    if(b.x - a.x < 0) {
        return drawPxLine(img, b, a, color);
    }

    iadd = 1;
    jadd = 1;

    if(k - 1 > 0.0000001) {
        mode = 1;
        i = (uint32_t)a.y;
        idest = (uint32_t)b.y;
        j = a.x;
        ptj = a.x;
        k = fabs((b.x - a.x) / (b.y - a.y));
        if((b.y - a.y) < 0) {
            iadd = -1;
        }
    }
    else {
        mode = 0;
        i = (uint32_t)a.x;
        idest = (uint32_t)b.x;
        j = a.y;
        ptj = a.y;
        if((b.y - a.y) < 0) {
            jadd = -1;
            k = -1 * k;
        }
    }

    i += iadd;

    while(1) {
        j += k;
        while(fabs(ptj - j) - 0.5 >= 0.0000001) {
            ptj += jadd;
        }
        if(mode) {
            pt.x = ptj;
            pt.y = (double)i;
            paintPixel(img, pt, color);
        }
        else {
            pt.x = (double)i;
            pt.y = ptj;
            paintPixel(img, pt, color);
        }
        if(i == idest) {
            break;
        }
        i += iadd;
    }

    return IERR_NONE;
}

imageError drawLine(image* img, vec a, vec b, pixel color, uint32_t thick) {

    if(b.x - a.x == 0 && b.y - a.y == 0) {
        paintPixel(img, a, color);
        return IERR_NONE;
    }

    if(thick == 0)
        return IERR_INVALID_THICK;

    if(b.x - a.x < 0) {
        return drawLine(img, b, a, color, thick);
    }

    double k;

    if((b.x - a.x) != 0)
        k = fabs((b.y - a.y) / (b.x - a.x));
    else
        k = 2;

    vec ipt;
    vec jpt;

    for(uint32_t i = 0; i < thick; i++) {
        if(i % 2 == 0) {
            if(k - 1 > 0.0000001) {
                ipt.x = a.x - (i / 2);
                ipt.y = a.y;
                jpt.x = b.x - (i / 2);
                jpt.y = b.y;
            }
            else {
                ipt.x = a.x;
                ipt.y = a.y + (i / 2);
                jpt.x = b.x;
                jpt.y = b.y + (i / 2);
            }
            drawPxLine(img, ipt, jpt, color);
        }
        else {
            if(k - 1 > 0.0000001) {
                ipt.x = a.x + ((i + 1) / 2);
                ipt.y = a.y;
                jpt.x = b.x + ((i + 1) / 2);
                jpt.y = b.y;
            }
            else {
                ipt.x = a.x;
                ipt.y = a.y - ((i + 1) / 2);
                jpt.x = b.x;
                jpt.y = b.y - ((i + 1) / 2);
            }
            drawPxLine(img, ipt, jpt, color);
        }
    }

    return IERR_NONE;
}

int drawPxCircle(image *img, vec center, double r, pixel color) {
    vec v = {0, round(r)};
    vec z = {0, 0};
    vec* array = malloc(ARRADD * sizeof(vec));
    size_t arrLen = ARRADD;
    size_t i = 1;
    size_t repeatLen;
    array[0] = v;
    vec a, b, c;
    double aerr, berr, cerr;

    while(1) {
        // printf("v(%f, %f)\n", v.x, v.y);
        a.x = v.x + 1;
        a.y = v.y;
        b.x = v.x;
        b.y = v.y - 1;
        c.x = v.x + 1;
        c.y = v.y - 1;
        aerr = fabs(findDist(a, z) - r);
        berr = fabs(findDist(b, z) - r);
        cerr = fabs(findDist(c, z) - r);

        if((aerr - berr) < 0.0000001 && (aerr - cerr) < 0.0000001) {
            v.x = a.x;
            v.y = a.y;
        } else if((berr - aerr) < 0.0000001 && (berr - cerr) < 0.0000001) {
            v.x = b.x;
            v.y = b.y;
        } else if((cerr - aerr) < 0.0000001 && (cerr - berr) < 0.0000001) {
            v.x = c.x;
            v.y = c.y;
        }

        array[i] = v;
        i++;

        if(i == arrLen - 1) {
            expandArray((void**)&array, sizeof(vec), &arrLen);
        }

        if(v.y == 0)
            break;
    }

    repeatLen = i;
    size_t j = i;
    vec mirror;

    for(i = 0; i < repeatLen; i++) {
        mirror.x = array[i].x;
        mirror.y = -1 * array[i].y;
        array[j] = mirror;
        j++;
        if(j == arrLen - 1) {
            expandArray((void**)&array, sizeof(vec), &arrLen);
        }
    }

    repeatLen = j;

    for(i = 0; i < repeatLen; i++) {
        mirror.x = -1 * array[i].x;
        mirror.y = array[i].y;
        array[j] = mirror;
        j++;
        if(j == arrLen - 1) {
            expandArray((void**)&array, sizeof(vec), &arrLen);
        }
    }

    repeatLen = j;

    for(i = 0; i < repeatLen; i++) {
        paintPixel(img, addVec(center, array[i]), color);
    }

    free(array);
    return 0;
}

imageError drawCircle(image *img, vec center, double r, double w, pixel color, pixel flood, int fcond) {

    if(r <= 0)
        return IERR_INVALID_RADIUS;
    if(w <= 0)
        return IERR_INVALID_THICK;

    w -= 1;
    drawPxCircle(img, center, r, color);
    drawPxCircle(img, center, r - w, color);
    double i = 0;

    if(fcond != 0) {
        while (i < r) {
            drawPxCircle(img, center, r - i, flood);
            i += 0.5;
        }
        i = 0;
    }

    while(i <= w && r - i > 0) {
        drawPxCircle(img, center, r - i, color);
        i += 0.5;
    }

    return IERR_NONE;
}

imageError setComponent(image* img, char comp, size_t val) {
    for(size_t i = 0; i < img->info.height; i++) {
        for(size_t j = 0; j < img->info.width; j++) {
            switch(comp) {
                case 'r':
                    img->array[i][j].r = val;
                    break;
                case 'g':
                    img->array[i][j].g = val;
                    break;

                case 'b':
                    img->array[i][j].b = val;
                    break;
                default:
                    return IERR_INVALID_COMPONENT;
            }
        }
    }
    return IERR_NONE;
}

int drawPxRectangle(image* img, vec a, vec b, pixel color) {
    vec c = {a.x, b.y};
    vec d = {b.x, a.y};
    drawLine(img, a, c, color, 1);
    drawLine(img, b, c, color, 1);
    drawLine(img, b, d, color, 1);
    drawLine(img, a, d, color, 1);
    return IERR_NONE;
}

imageError drawRectangle(image* img, vec a, vec b, pixel color, uint32_t thick) {

    if(thick == 0)
        return IERR_INVALID_THICK;

    vec c, d;

    if(a.x > b.x && a.y > b.y) {
        return drawRectangle(img, b, a, color, thick);
    }

    if(a.y > b.y && b.x > a.x) {
        c.x = a.x;
        c.y = b.y;
        d.x = b.x;
        d.y = a.y;
        return drawRectangle(img, c, d, color, thick);
    }

    if(a.x > b.x && b.y > a.y) {
        return drawRectangle(img, b, a, color, thick);
    }

    size_t i = 0;

    while(a.x <= b.x && a.y <= b.y && i < thick) {
        drawPxRectangle(img, a, b, color);
        a.x++;
        a.y++;
        b.x--;
        b.y--;
        i++;
    }

    return IERR_NONE;
}
int checkRectangle(image* img, vec a, vec* b, pixel color) {
    vec i = {a.x, a.y};
    vec check;

    while(comparePixels(getPixel(img, i), color) && i.x < (double) img->info.width) {
    i.x++;
    }

    i.x--;

    while(comparePixels(getPixel(img, i), color) && i.y < (double) img->info.height) {
        i.y++;
    }

    i.y--;
    *b = i;

    check.x = a.x;
    check.y = a.y + 1;

    while (check.y <= i.y) {
        while(check.x < i.x) {
            if(!comparePixels(getPixel(img, check), color)) {
                return 0;
            }
            check.x++;
        }
        check.x = a.x;
        check.y++;
    }

    check.x = a.x;
    check.y = a.y - 1;

    if(check.x >= 0) {
        while(check.x <= i.x) {
            if(comparePixels(getPixel(img, check), color) && check.y >= 0)
                return 0;
            check.x++;
        }
    }

    check.y = a.y;
    check.x = i.x + 1;

    if(check.x < (double) img->info.width) {
        while(check.y <= i.y) {
            if(comparePixels(getPixel(img, check), color))
                return 0;
            check.y++;
        }
    }

    check.y = a.y;
    check.x = a.x - 1;

    if(check.x >= 0) {
        while(check.y <= i.y) {
            if(comparePixels(getPixel(img, check), color))
                return 0;
            check.y++;
        }
    }

    check.x = a.x;
    check.y = i.y + 1;

    if(check.y < (double) img->info.height) {
        while(check.x <= i.x) {
            if(comparePixels(getPixel(img, check), color))
                return 0;
            check.x++;
        }
    }

    return 1;
}

imageError outlineRectangles(image *img, pixel rectColor, pixel outline, uint32_t thick) {
    if(thick == 0)
        return IERR_INVALID_THICK;
    size_t height, width;
    height = img->info.height;
    width = img->info.width;
    uint8_t** array = malloc(sizeof(uint8_t*)*height);

    for(size_t i = 0; i < height; i++) {
        array[i] = calloc(width, sizeof(uint8_t));
    }

    vec iter = {0, 0};
    vec corner;
    vec one;

    while(iter.y < (double) height) {
        while(iter.x < (double) width) {
            if(comparePixels(getPixel(img, iter), rectColor) &&
            array[(uint32_t) floor(iter.y)][(uint32_t) floor(iter.x)] != 1) {
                if(checkRectangle(img, iter, &corner, rectColor)) {
                    drawRectangle(img, iter, corner, outline, thick);
                    one.x = iter.x;
                    one.y = iter.y;
                    while(one.y <= corner.y) {
                        while(one.x <= corner.x) {
                            array[(uint32_t)one.y][(uint32_t)one.x] = 1;
                            one.x++;
                        }
                        one.x = iter.x;
                        one.y++;
                    }
                }
            }
            iter.x++;
        }
        iter.x = 0;
        iter.y++;
    }

    return IERR_NONE;
}

imageError splitImage(image* img, uint32_t n, uint32_t m, uint32_t thick, pixel color) {
    if(n == 0 || m == 0)
        return IERR_INVALID_DIMENSION;
    double width = img->info.width;
    double height = img->info.height;
    uint32_t y_size = (size_t) (floor((height - (n - 1) * thick) / (double) n));
    uint32_t x_size = (size_t) (floor((width - (m - 1) * thick) / (double) m));
    vec a = {(double) x_size, 0};
    vec b = {(double) (x_size + thick - 1), (double) img->info.height - 1};

    for(size_t i = 1; i < m; i++) {
        drawRectangle(img, a, b, color, thick);
        a.x += x_size + thick;
        b.x += x_size + thick;
    }

    a.x = 0;
    a.y = (double) img->info.height - 1 - (double) y_size;
    b.x = (double) img->info.width - 1;
    b.y = (double) img->info.height - y_size - thick;

    for(size_t i = 1; i < n; i++) {
        drawRectangle(img, a, b, color, thick);
        a.y -= y_size + thick;
        b.y -= y_size + thick;
    }

    // printf("%zu %zu\n", x_size, y_size);
    return IERR_NONE;
}

image generateImage(uint32_t width, uint32_t height, pixel background) {
    size_t lineLen;
    if(((sizeof(pixel)*(width)) % 4 != 0))
        lineLen = sizeof(pixel) * width + 4 - ((sizeof(pixel) * width) % 4);
    else
        lineLen = sizeof(pixel) * width;
    uint32_t size = 54 + height * lineLen;

    imageHeader header = {
            19778,
            size,
            0,
            0,
            54,
    };

    imageInfo info = {
            40,
            width,
            height,
            1,
            24,
            0,
            size - 54,
            11811,
            11811,
            0,
            0,
    };

    char* fullHeader = malloc(sizeof(char));
    fullHeader[0] = 0;
    pixel** array = malloc(height * sizeof(pixel*));
    pixel* line;

    for(size_t i = 0; i < height; i++) {
        line = malloc(lineLen);
        for(size_t j = 0; j < width; j++) {
            line[j] = background;
        }
        array[i] = line;
    }

    image img = {header, info, fullHeader, array};

    return img;
}

imageError rectCircle(image *img, vec a, vec b, double w, pixel color, pixel flood, int fcond) {
    if(fabs(b.x - a.x) - fabs(b.y - a.y) > 0.0000001)
        return IERR_INVALID_RECTANGLE;
    vec cen = {(a.x + b.x) / 2, (a.y + b.y) / 2};
    double r = fabs(b.x - cen.x);
    drawCircle(img, cen, r, w, color, flood, fcond);
    return IERR_NONE;
}

int checkNearby(image* img, vec pos) {
    vec array[] = {
            {pos.x - 1, pos.y},
            {pos.x + 1, pos.y},
            {pos.x, pos.y - 1},
            {pos.x, pos.y + 1},
    };
    vec cur;
    pixel white = {255, 255, 255};
    for(size_t i = 0; i < 4; i++) {
        cur = array[i];
        if(cur.x >= 0 && cur.y >= 0 && cur.x < img->info.width && cur.y < img->info.height) {
            if(comparePixels(getPixel(img, cur), white))
                return 1;
        }
    }
    return 0;
}

imageError outlineWhite(image* img, pixel outline) {
    vec pos = {0, 0};
    pixel white = {255, 255, 255};
    while(pos.y < img->info.height) {
        while(pos.x < img->info.width) {
            if(checkNearby(img, pos) && !comparePixels(getPixel(img, pos), white))
                paintPixel(img, pos, outline);
            pos.x++;
        }
        pos.x = 0;
        pos.y++;
    }
    return IERR_NONE;
}