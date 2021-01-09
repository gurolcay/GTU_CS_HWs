#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define HEADER_SIZE     (8)
#define INTEL           (18761) // 4949 H
#define MOTOROLA        (19789) // 4D4D H
#define BYTE            (1) // 8-bit unsigned integer
#define ASCII           (2) // 8-bit > 7-bit ASCII code; last byte binary zero <
#define SHORT           (3) // 16-bit (2-byte) unsigned integer
#define LONG            (4) // 32-bit (4-byte) unsigned integer
#define RATIONAL        (5) // two longs; firs represents the numerator of a fraction; the second, the denominator
typedef enum {FALSE, TRUE} boolean;

struct Header {
		uint16_t type_order;
		uint16_t constant_value;
		uint32_t idf_offset;
};

struct TIF_Tag {
		uint16_t tag_id;
		uint16_t data_type;
		uint32_t data_count;
		uint32_t data_offset;
};

struct TIF_Info {
		int height;
		int width;
		int bitsPerSample;
		int samplesPerPixel;
		int stripByteCounts;
		char type_order[10];
		int whiteColor;
		uint32_t stripOffset;
		int rowsPerStrip;
		uint32_t idf_offset;
};

uint16_t tags_number;

void read_tif_file_hex(const char *file_name);
void match_tags(struct TIF_Tag *tag, struct TIF_Info *info);
void print_tif_image(int fd, struct TIF_Info tif_info);
void print_tif_info(struct TIF_Info tif_info);
void convert_byte_to_bit(uint8_t value, int* column, int* row, struct TIF_Info tif_info);

int main(int argc, char const *argv[]) {

		if (argc != 2) {
				printf("Usage: '%s test.tif'\n",argv[0]);
				return 0;
		}

		read_tif_file_hex(argv[1]);

		return 0;
}


void read_tif_file_hex(const char *file_name) {
		struct Header header_block;
		struct TIF_Info tif_info;
		struct TIF_Tag tif_tag;
		int fd = -1, bytes_read = -1, i = 0;

		fd = open(file_name, O_RDONLY);

		if (fd == -1) {
				fprintf(stderr, "Open failed: %s\n",file_name );
				exit(EXIT_FAILURE);
		}

		bytes_read = read(fd, &header_block, sizeof(header_block));
		if (bytes_read == -1) {
				fprintf(stderr, "Failed to read tiff file: %s\n", file_name);
				exit(EXIT_FAILURE);
		}

		fprintf(stderr, "type_order: %d \n",(int)header_block.type_order );
		fprintf(stderr, "constant_value: %d \n",(int)header_block.constant_value );
		fprintf(stderr, "idf_offset: %d \n",(int)header_block.idf_offset );
		tif_info.idf_offset = header_block.idf_offset;

		if ((int)header_block.type_order == INTEL) {
				strcpy(tif_info.type_order, "INTEL\0");
		} else if ((int)header_block.type_order == MOTOROLA) {
				strcpy(tif_info.type_order, "MOTOROLA\0");
		} else {
				fprintf(stderr, "This isn't tiff file: %s\n",file_name);
				exit(EXIT_FAILURE);
		}

		lseek(fd, header_block.idf_offset, SEEK_SET);

		bytes_read = read(fd, &tags_number, sizeof(tags_number));
		if (bytes_read == -1) {
				fprintf(stderr, "Failed to read tiff file: %s\n",file_name);
				exit(EXIT_FAILURE);
		}
		// printf("tags_number: %d\n",(int)tags_number );
		for (i = 0; i < (int)tags_number; i++) {
				bytes_read = read(fd, &tif_tag, sizeof(tif_tag));
				match_tags(&tif_tag, &tif_info);
		}

		print_tif_info(tif_info);
		print_tif_image(fd, tif_info);

		close(fd);

}

void match_tags(struct TIF_Tag *tag, struct TIF_Info *info) {
		// printf("---------------------------------------------------------------------\n");
		// fprintf(stderr, "tag_id: %d\n",tag->tag_id );
		// fprintf(stderr, "data_type: %04x\n",tag->data_type );
		// fprintf(stderr, "data_count: %08x\n",tag->data_count );
		// fprintf(stderr, "data_offset: %08x\n",tag->data_offset );

		switch ((int)tag->tag_id) {
		case 256: //ImageWidth
				// fprintf(stderr, "Image Width: %d\n",(int)tag->data_offset );
				info->width = (int)tag->data_offset;
				break;
		case 257: //ImageLength
				// fprintf(stderr, "Image Length: %d\n",(int)tag->data_offset );
				info->height = (int)tag->data_offset;
				break;
		case 258: //BitsPerSample
				// fprintf(stderr, "Bits Per Sample: %d\n",(int)tag->data_offset );
				info->bitsPerSample = (int)tag->data_offset;
				break;
		case 259: //compression
				if ((int)tag->data_offset != 1) {
						fprintf(stderr, "TIF file has compression. Because of this, this ");
						fprintf(stderr, "program will terminated.\n");
						exit(EXIT_FAILURE);
				}
				break;
		case 262: // Color
				info->whiteColor = (int)tag->data_offset;
				break;
		case 273:
				info->stripOffset = (int)tag->data_offset;
				break;
		case 277: //SamplesPerPixel
				// fprintf(stderr, "Samples Per Pixel: %d\n",(int)tag->data_offset );
				info->samplesPerPixel = (int)tag->data_offset;
				break;
		case 279: //StripByteCounts
				info->stripByteCounts = (int)tag->data_offset;
				break;
				// case 283:
				//   info->XResolution = (int)tag->data_offset;
				//   break;
				// case 284:
				//   info->YResolution = (int)tag->data_offset;
				//   break;
		}
}
void print_tif_info(struct TIF_Info tif_info) {
		fprintf(stdout, "Width: %d pixels\n",tif_info.width );
		fprintf(stdout, "Height: %d pixels\n",tif_info.height );
		fprintf(stdout, "Byte order: %s\n",tif_info.type_order );
		fprintf(stdout, "Strip Offset: %04x\n",tif_info.stripOffset );
		fprintf(stdout, "White Color: %d\n",tif_info.whiteColor );
		// fprintf(stdout, "XResolution: %d\n",tif_info.XResolution );
		// fprintf(stdout, "YResolution: %d\n",tif_info.YResolution );
}

void print_tif_image(int fd, struct TIF_Info tif_info) {
		int bitsPerPixel; //bir pixel basina dusen bit sayisi
		int offset = 0, bytes_read;
		int column = 0, row = 0, temp = 0, mod = 0;
		uint8_t byte;
		bitsPerPixel = (int)tif_info.bitsPerSample * (int)tif_info.samplesPerPixel;
		fprintf(stdout, "bitsPerPixel: %d\n",bitsPerPixel );

		lseek(fd, tif_info.stripOffset, SEEK_SET);
		offset += HEADER_SIZE;
		// byte = (unsigned char *)calloc(, sizeof(unsigned char))
		mod = (bitsPerPixel * tif_info.width) / 8;
		do {
				bytes_read = read(fd, &byte, sizeof(byte));
				// printf("%d ", byte);
				if (bitsPerPixel == 1) {
						++temp;
						convert_byte_to_bit(byte, &column, &row, tif_info);
						// printf("\ntemp: %d\n",temp );
						if (row == tif_info.height)
								break;
						// printf("current_column: %d\n",current_column );
				}else if (bitsPerPixel == 8) {
						if (byte == 255) {
								fprintf(stdout, "1");
						}else if (byte == 0) {
								fprintf(stdout, "0");
						}
				}
				offset += bytes_read;

		} while (offset != (int)tif_info.idf_offset);

		printf("\noffset: %d\n",offset);

}

void convert_byte_to_bit(uint8_t byte, int* column, int* row, struct TIF_Info tif_info) {
		int byte_number = 8;
		unsigned int bit;
		unsigned int bits[byte_number];
		int i = 0;
		int k = byte_number;
		//printf("\nbyte: %02x\n",byte);
		for (i = 0; i < 8; ++i) {
				bit = ((byte>>i) & 1);
				if(tif_info.whiteColor == 0) {
						if (bit == 0)
						{
								bits[i] = 1;
								//printf("1");
						}
						else if(bit == 1)
						{
								bits[i] = 0;
								//printf("0");
						}
				}
				else if(tif_info.whiteColor == 1) {
						if (bit == 1)
						{
								bits[i] = 1;
								//printf("1");
						}
						else if(bit == 0)
						{
								bits[i] = 0;
								//printf("0");
						}
				}
				k--;

		}
		for (i = 7; i>= 0; --i)
		{
				printf("%d",bits[i]);
				(*column)++;

				if (*column == tif_info.width) {
						printf("\n");
						*column = 0;
						(*row)++;
						break;
				}
		}

}
