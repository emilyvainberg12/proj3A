//parse and output data in superblock


// TODO: CHECK ABOUT BLOCKS_PER_GROUP FOR: "GROUP, 0, 8192"
#include <getopt.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include "ext2_fs.h"

struct ext2_super_block super; 
struct ext2_group_desc group_descriptor; 
int fd = 0; 
unsigned int inodes_count = 0; 
unsigned int blocks_count = 0; 
unsigned int blocks_per_group = 0; 
unsigned int inodes_per_group = 0; 
unsigned int first_free_inode = 0; 
unsigned int inode_size = 0; 
unsigned int block_size = 0; 
unsigned int directory_count = 0; 
unsigned int free_inodes_count = 0; 
unsigned int free_blocks_count = 0; 
unsigned long bitmap_block_address = 0; 

void read_group(int group, int total_groups); 
void b_free(int group, unsigned int block_bitmap); 
void i_free(int group, int block_bitmap, int inode_table); 
unsigned long offset_block(unsigned int block);
void inodes_summary(unsigned int inode_table_id, unsigned int num_inode, unsigned int index);

int main(int argc, char *argv[]) {
    //fd = open(argv[1], O_RDONLY);
    fd = open("trivial.img", O_RDONLY);

   /* SUPERBLOCK SUMMARY BEGIN */
    pread(fd, &super, sizeof(super), 1024);
    blocks_count = super.s_blocks_count;
    inodes_count = super.s_inodes_count;
    block_size = 1024 << super.s_log_block_size;
    inode_size = super.s_inode_size;
    blocks_per_group = super.s_blocks_per_group;
    inodes_per_group = super.s_inodes_per_group;
    first_free_inode = super.s_first_ino;
    fprintf(stdout, "SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n", blocks_count, inodes_count, block_size, inode_size, blocks_per_group,
    inodes_per_group, first_free_inode); 
    /* SUPER BLOCK SUMMARY END */

    int num_groups = super.s_blocks_count / super.s_blocks_per_group;
    if ((double) num_groups < (double) super.s_blocks_count / super.s_blocks_per_group) {
		num_groups++;
	}
    if (num_groups <= 0){
        num_groups = 1; 
    }
     
    for (int i = 0; i < num_groups; i++){
        read_group(i, num_groups); 
    }
    return 0; 
}

void read_group(int group, int total_groups){

    unsigned int tot_blocks_in_group = super.s_blocks_per_group;
    unsigned int num_inodes_in_group = super.s_inodes_per_group;

	if (group == total_groups - 1) {
		tot_blocks_in_group = blocks_count - (blocks_per_group * (total_groups - 1));
	}

	if (group == total_groups - 1) {
		num_inodes_in_group = inodes_count - (inodes_per_group * (total_groups - 1));
	}

    int group_d_offset = 0; 
    if(block_size == 1024){ //If block size = 1024 -> start at block 2
        group_d_offset = block_size * 2;
    }
    else if(block_size > 1024){ //If block size > 1024 -> start at block 1
        group_d_offset = block_size;
    }
    else {   //If block size < 1024 -> Not possible
        exit(2);
    }

    pread(fd, &group_descriptor, sizeof(group_descriptor), group_d_offset + (group*sizeof(group_descriptor)));
    fprintf(stdout, "GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n", group, tot_blocks_in_group, num_inodes_in_group,
    group_descriptor.bg_free_blocks_count, group_descriptor.bg_free_inodes_count, group_descriptor.bg_block_bitmap,
    group_descriptor.bg_inode_bitmap, group_descriptor.bg_inode_table);

    unsigned int block_bitmap = group_descriptor.bg_block_bitmap;
    unsigned int inode_table = group_descriptor.bg_inode_table;
    unsigned int inode_bitmap = group_descriptor.bg_inode_bitmap;
    
    b_free(group, block_bitmap); 
    i_free(group, inode_bitmap, inode_table); 
}

 /* FREE BLOCK ENTRIES BEGIN */
void b_free(int group, unsigned int block_bitmap){

    char* byte = (char*) malloc(block_size);
    unsigned int current = super.s_first_data_block + group * super.s_blocks_per_group;
    pread(fd, byte, block_size, offset_block(block_bitmap));

	for (unsigned int i = 0; i < block_size; i++) {
		char x = byte[i];
		for (unsigned int j = 0; j < 8; j++) {
			if (!(1 & x )) {
				fprintf(stdout, "BFREE,%d\n", current);
			}
            current++;
			x >>= 1;
		}
	}
}
/* FREE BLOCK ENTRIES END */

/* FREE I-NODES ENTRIES BEGIN */
void i_free(int group, int block_bitmap, int inode_table){

    char* byte = (char*) malloc(block_size);
    unsigned int current = 1 + (group * super.s_inodes_per_group);
    int start_index = current; 
    
    pread(fd, byte,(super.s_inodes_per_group/8), offset_block(block_bitmap));

	for (unsigned int i = 0; i < super.s_inodes_per_group/8; i++) {
		char x = byte[i];
		for (unsigned int j = 0; j < 8; j++) {
			if (!(1 & x )) {
				fprintf(stdout, "IFREE,%d\n", current);
			}
            else {
                inodes_summary(inode_table, current, current - start_index);
            }
            current++;
			x >>= 1;
		}
	}

}

void inodes_summary(unsigned int inode_table_id, unsigned int num_inode, unsigned int index){
    return; 
   // printf("hello"); 
}

unsigned long offset_block(unsigned int block) {
	return 1024+ (block - 1) * block_size;
}




/* DIRECTORY BEGIN */
/* DIRECTORY END */

/* INDIRECTBLOCK BEGIN */
/* INDIRECTBLOCK END */





