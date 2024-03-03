#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>

int main(int argc, char *argv[])
{
	int res = check_operation_type(argv);
	if( res == e_encode )
	{
		printf("Selected encoding\n");
		if(argc == 4 || argc == 5)
		{
			EncodeInfo encInfo;  //structure declaration
			if(read_and_validate_encode_args(argv, &encInfo) == e_success)
			{
				printf("read and validate is successfully done\n");
				if( do_encoding(&encInfo) == e_success)
				{
					printf("Encoding is successfully done\n");
				}
				else
				{
					printf("Encoding is failure\n");
				}
			}
			else
			{
				printf("read and validate is failre\n");
			}
		}
		else
		{
			printf("ERROR: pass args for \nencoding ./a.out -e beautiful.bmp secret.txt <>\ndecoding ./a.out -d output.bmp <>\n");
		}
	}
	else if( res == e_decode )
	{
		printf("Selected decoding\n");
		if(argc >= 3)
		{
			DecodeInfo decInfo;  //structure declaration
			if(read_and_validate_decode_args(argv, &decInfo) == e_success)
			{
				printf("read and validate is successfully done\n");
				if( do_decoding(&decInfo) == e_success)
				{
					printf("Decoding is successfully done\n");
				}
				else
				{
					printf("Decoding is failure\n");
				}
			}
			else
			{
				printf("read and validate is failre\n");
			}
		}
		else
		{
			printf("ERROR: pass args for \ndecoding ./a.out -e beautiful.bmp secret.txt <>\ndecoding ./a.out -d output.bmp <>\n");
		}
	}
	else
	{
		printf("ERROR: Pass valid arguments \nEg: encoding ./a.out -e beautiful.bmp secret.txt <>\n    decoding ./a.out -d output.bmp <>\n");
	}
}

OperationType check_operation_type(char *argv[])
{
	/*argv[1] if -e or not
	  	yes-> return e_encode
		no-> argv[1] is -d or not
			  yes->return e_decode
			  no-> return e_unsupported */
	if(strcmp(argv[1], "-e") == 0)
	{
		return e_encode;
	}
	else if(strcmp(argv[1], "-d") == 0)
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}
