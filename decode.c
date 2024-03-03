//decode.c

#include<stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include "common.h"
#include<stdlib.h>

/* Function Definitions */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	 /* argv[2] -> .bmp file
            yes-> store argv[2] into src_image_fname
            no-> return e_failure
                    no->e_failure */
    if(strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
         decInfo->src_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
	/*argv[3] -->is passed or not
            passed->store argv[3] into secret_fname */
	if(argv[3] != NULL)
	{
		decInfo->secret_fname = argv[3];
	}
	
	return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
	if( open_decode_files(decInfo) == e_success )
	{
		printf("Open files successfully done\n");
		if(decode_magic_string(decInfo, MAGIC_STRING) == e_success)
		{
			printf("Decoding Magic string is done\n");
			if( decode_secret_file_extn_size(decInfo) == e_success )
			{
				printf("Decoding secret file extention is done\n");
				if( decode_secret_file_extn(decInfo -> extn_secret_file, decInfo) == e_success )
				{
					printf("Decoding secret file extention is done\n");
					if(open_secret_file(decInfo) == e_success)
					{
						printf("Open secret file successfully done\n");
						if( decode_secret_file_size(decInfo) == e_success )
						{
							printf("Decoding secret file size successfully done\n");
							if( decode_secret_file_data(decInfo) == e_success )
							{
								printf("Decoding secret file data successfully done\n");
							}
							else
							{
								printf("decode_secret_file_data is failure\n");
								return e_failure;
							}
						}
						else
						{
							printf("decode_secret_file_size is failure\n");
							return e_failure;
						}
					}
					else
					{
						printf("open_secret_file is failure\n");
						return e_failure;
					}
				}
				else
				{
					printf("decode_secret_file_extn is failure\n");
					return e_failure;
				}
			}
			else
			{
				printf("decoding_secret_file_extn_size is failure\n");
				return e_failure;
			}
		
		}
		else
		{
			printf("decode_magic_string is failure\n");
			return e_failure;
		}
	}
	else
	{
		printf("Open file function is failure\n");
        return e_failure;
	}
	return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
	char str[8];
	char *data = malloc(8 * sizeof(char));
	fseek(decInfo->fptr_secret, 0, SEEK_SET);
	
	for(int i=0 ; i < (decInfo->size_secret_file) ; i++)
	{
		fread(str, 8, 1, decInfo -> fptr_src_image);
		data[0] = decode_lsb_to_byte(str);
		fwrite(&data[0], 1, 1, decInfo->fptr_secret);
	}
	return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
	char str[32];
    fread(str, 32,1, decInfo -> fptr_src_image);
    decInfo->size_secret_file = encode_lsb_to_size(str);
	return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo, char *magic_string)
{
	fseek(decInfo->fptr_src_image, 54, SEEK_SET);
	char *message;
	message = decode_image_to_data(strlen(magic_string), decInfo->fptr_src_image);
	//printf("%s  %s \n",message,magic_string);
	if(strcmp(message,magic_string) == 0)
	{
		printf("magic string matched\n");
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

char *decode_image_to_data(int size, FILE *fptr_src_image)
{
	char str[8];
	char *data = (char *)malloc(size);
    for(int i = 0 ; i < size ; i++)
    {
        fread(str, 8, 1, fptr_src_image);
        data[i] = decode_lsb_to_byte(str);
    }
	return data;

}

char decode_lsb_to_byte(char *image_buffer)
{
	char data = 0;
	for(int i=0 ; i < 8 ; i++)
	{
		data = data | ((image_buffer[i] & 1) << (7-i));
	}
	return data;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
	char str[32];
    fread(str, 32, 1, decInfo -> fptr_src_image);
    decInfo->secret_file_extn_size = encode_lsb_to_size(str);
	//printf("%d\n",decInfo->secret_file_extn_size);
    return e_success;
}

int encode_lsb_to_size(char *buffer)
{
	int size = 0;
	for(int i=0 ; i< 32; i++)
	{
		size = size | ((buffer[i] & 1) << (31-i));
	}
	return size;
}

Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
	decInfo->extn_secret_file = decode_image_to_data(decInfo->secret_file_extn_size ,decInfo -> fptr_src_image);
    //printf("%s\n",decInfo->extn_secret_file);
	return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

        return e_failure;
    }
	
    // No failure return e_success
    return e_success;
}

Status open_secret_file(DecodeInfo *decInfo)
{
	switch(decInfo->secret_file_extn_size)
	{
		case 2:
			decInfo->secret_fname = "output.c";
			break;
		case 3:
			decInfo->secret_fname = "output.sh";
			break;
		case 4:
			decInfo->secret_fname = "output.txt";
			break;
	}
	//printf("%s\n",decInfo->secret_fname);
	
	//opening output file
	decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
	// Do Error handling
    if (decInfo->secret_fname == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

        return e_failure;
    }
	return e_success;
}
