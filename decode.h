//decode.h

#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;  //beautiful.bmp
    FILE *fptr_src_image;   //address of beautiful.bmp file
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;     //secret file name
    FILE *fptr_secret;      //address of secret file
    char secret_data[MAX_SECRET_BUF_SIZE];
	int secret_file_extn_size;  //to store size of extension
	char *extn_secret_file;  //store secret file extn
	long size_secret_file;  //store secret file size

} DecodeInfo;

/* Decoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Validate Magic String */
Status decode_magic_string(DecodeInfo *decInfo, char *magic_string);

/* Decode function, which does the real decoding */
char *decode_image_to_data(int size, FILE *fptr_src_image);

/* Decode a LSB into image of image data array */
char decode_lsb_to_byte(char *image_buffer);

/* Decode secret file extenstion size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode lsb into size of image data array */
int encode_lsb_to_size(char *buffer);

/* Decode secret file extenstion */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo);

/* Open secret file */
Status open_secret_file(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);
 
/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
