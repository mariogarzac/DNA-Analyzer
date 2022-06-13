#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE 1024
#define num 256
#define MAX_LINE_LENGTH 100000

/*
int charCount(FILE *fp, char *fileName)
{
    int arr[27] = {0};
    int index = 0;
    int total = 0;
    int xCount = 0;

    int c;

    fp = fopen(fileName, "r");
    if (fp == NULL)
    {

        exit(1);
    }

    while ((c = fgetc(fp)) != EOF)
    {
        if ((c - 65) == 23)
        {
            xCount++;
        }
        index = c - 65;
        arr[index] += 1;
    }

    for (int i = 0; i < 27; i++)
    {
        total += arr[i];
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("%d\n", xCount);
    fclose(fp);
    return total;
}

void strReplace()
{
    FILE *dna;
    FILE *ref;

    FILE *res;

    char data[SIZE];
    char refData[SIZE];
    char tmp[SIZE];

    dna = fopen("files/ref.txt", "r");
    ref = fopen("files/seq.txt", "r");

    if (dna == NULL || ref == NULL)
    {
        perror("Kaput");
        exit(1);
    }

    int position = 0;
    int substringLength = 0;

    fgets(data, SIZE, dna);

    for (int i = 0; i < SIZE; i++)
    {
        tmp[i] = data[i];
    }

    while (fscanf(ref, "%s", refData) == 1)
    {
        if (strcmp(refData, "\0") != 0)
        {
            char *ret;
            ret = strstr(data, refData);
            if (ret != NULL)
            {
                position = ret - data;
                int n = position + strlen(refData);

                for (int i = position; i < n; i++)
                {
                    tmp[i] = 'X';
                }
            }
            else
            {
                printf("no se encontró %s\n");
            }
        }
    }

    res = fopen("files/RESULTS.txt", "w");
    fprintf(res, "%s", tmp);
    fclose(dna);
    fclose(ref);
}
*/
void processFile()
{
    char *path;
    char *refS;
    FILE *file, *file2;
    char line[MAX_LINE_LENGTH];
    unsigned int line_count = 0;
    long fileSize;
    char cActual, cNumber;
    int iActual, iCounterSearch, iCounterChar, iCounter;
    char cScan;
    char *subS[1000];
    int countEOL = 0;

    int iIndex;
    int iCounterOfSubStrings = 0;
    float iPer;

    iActual = 0;
    // iCounter = 0;
    iCounterSearch = 0;

    file = fopen("files/seq.txt", "r");

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        rewind(file);

        refS = (char *)malloc(sizeof(char) * fileSize);

        fread(refS, 1, fileSize, file);

        while (strlen(refS) > fileSize)
        {
            memmove(&refS[strlen(refS) - 1], &refS[strlen(refS)], strlen(refS) - (strlen(refS) - 1));
        }
    }
    else
    {
        printf("Error al leer. \n");
        exit(1);
    }

    char *temp = (char *)malloc(sizeof(char) * 100000);
    for (int i = 0; i <= fileSize; i += 100000)
    {
        strncpy(temp, refS + i, 100000);
    }
    fclose(file);

    // puts(refS);

    file2 = fopen("files/ref.txt", "r");

    if (!file)
    {
        perror("s_cerevisia_2022_01.seq");
        exit(1);
    }

    int iSize = strlen(refS);
    char *p;
    char *result = strstr(refS, line);

    int f, nf, ant, tot, act, si, no;
    si = 0;
    no = 0;
    ant = 0;
    f = 0;
    nf = 0;
    tot = 0;
    act = 0;

    FILE *fptr;
    fptr = fopen("files/RESULTS.txt", "w");
#pragma omp parallel for
    for (int y = 0; y < 1000; y++)
    {
        #pragma omp critical
        while (fscanf(file2, "%s", line) == 1)
        {
            //  \0  "PARA SALTO DE LINEA EN ARCHIVO"///
            if (strcmp(line, "\0") != 0)
            {
                countEOL++;
                // puts(line);
                char *result = strstr(refS, line);

                if (result)
                {
                    int position = result - refS;
                    act = strlen(line);
                    int end = position + act;

                    // printf("A partir del indice: %d\n", position);
                    fprintf(fptr, "A partir del indice: %d\n", position);

                    tot += act;
                    si++;
                }
                else
                {
                    fprintf(fptr, "Not Found\n");
                    no++;
                }
            }
        }
    }

    int lengthf = strlen(refS);
    int numXs = 0;
    for (int x = 0; x < lengthf; x++)
    {
        if (refS[x] == 'X')
        {
            numXs++;
        }
    }

    // int porce = ((float)(numXs * 100) / lengthf);
    // printf("Porcentaje: %.2d", porce);

    fprintf(fptr, "%d", countEOL);
    fprintf(fptr, "Total cubierto %d / %d\n", tot, iSize);

    iPer = (iSize * 1.0) / (tot * 1.0);
    iPer *= 100;

    fprintf(fptr, "Porcentaje %f\n", iPer);
    fprintf(fptr, "Mapeadas %d\n", si);
    fprintf(fptr, "No Mapeadas %d\n", no);

    free(refS);
    fclose(fptr);

    if (fclose(file))
    {
        exit(1);
        perror(path);
    }
}