// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"


struct pixelData
{
    int red;
    int green;
    int blue;
};

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    //get the factor it increase the image size by as an integer
    int factor = atoi(argv[1]);

    // pointers to command line args
    char *infile = argv[2];
    char *outfile = argv[3];


    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);


    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

     struct pixelData pixel[bi.biWidth];

    //change appropriate File header values

//    bf.bfSize = (bi.biSizeImage * (pow(factor,2)) + 54);

BITMAPFILEHEADER bf2;
BITMAPINFOHEADER bi2;

    bf2 = bf;
    bi2 = bi;

    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //change appropriate info header values
    bi2.biWidth = bi.biWidth * factor;
    bi2.biHeight = bi.biHeight * factor;

    int outPadding = (4- (bi2.biWidth * sizeof(RGBTRIPLE)) % 4 ) % 4;

    bi2.biSizeImage = ((sizeof(RGBTRIPLE) * bi2.biWidth) + outPadding) * abs(bi2.biHeight);

    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    //bi2.biSizeImage = bi.biSizeImage * (pow(factor,2));

    bf2.bfSize = bi2.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf2, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi2, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines


    // iterate over infile's scanlines

        for (int i = 0, biHeight = (abs(bi.biHeight)); i < biHeight; i++)
        {

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                pixel[j].red = triple.rgbtRed;
                pixel[j].green = triple.rgbtGreen;
                pixel[j].blue = triple.rgbtBlue;
                //Add rgb triple to the linked list


            }

            fseek(inptr, padding, SEEK_CUR);

            for (int m = 0; m < factor; m++)
            {
                for (int l = 0; l < bi.biWidth; l++)
                {
                    for (int n = 0; n < factor; n++)
                    {
                        RGBTRIPLE triple;

                        triple.rgbtRed = pixel[l].red;
                        triple.rgbtGreen =pixel[l].green;
                        triple.rgbtBlue = pixel[l].blue;

                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }

                for (int k = 0; k < outPadding; k++)
                {
                    fputc(0x00, outptr);
                }

            }

            // skip over padding, if any
            //fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)

        }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}


