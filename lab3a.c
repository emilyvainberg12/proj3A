//parse and output data in superblock

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
char block_bitmap[1024]; 
int fd = 0; 
unsigned int inodes_count = 0; 
unsigned int blocks_count = 0; 
unsigned int block_size = 0; 
unsigned int directory_count = 0; 
unsigned int free_inodes_count = 0; 
unsigned int free_blocks_count = 0; 
unsigned long bitmap_block_address = 0; 

int main() {

    fd = open("EXT2_test.img", O_RDONLY);

    // skipping the boot block with the 1024 
    pread(fd, &super, sizeof(super), 1024);

    inodes_count = super.s_inodes_count; 
    blocks_count = super.s_blocks_count; 
    //Do the rest 


    block_size = 1024 << super.s_log_block_size; 
    //wrong order- make sure to print the correct order 
    printf("%s","%d", "%d", 'SUPERBLOCK', inodes_count, blocks_count, block_size); 

    pread(fd, &group_descriptor, sizeof(group_descriptor), 2048);

    bitmap_block = group_descriptor.bg_block_bitmap;		/* Blocks bitmap block */
	//group_descriptor.bg_inode_bitmap;		/* Inodes bitmap block */
	//group_descriptor.bg_inode_table;		/* Inodes table block */
	free_blocks_count = group_descriptor.bg_free_blocks_count;	/* Free blocks count */
	free_inodes_count = group_descriptor.bg_free_inodes_count;	/* Free inodes count */
	directory_count = group_descriptor.bg_used_dirs_count;	/* Directories count */


     printf("%s","%d", "%d", "GROUP", bitmap_block, free_blocks_count, free_inodes_count); 

    // free block entries
    pread(fd, &block_bitmap, sizeof(block_bitmap), bitmap_block_address);
    //  1 -> used, 0 -> free 

    for (int i = 0; i < 1024{
        char x = block_bitmap[i]; 
        for (int j = 0; j < 8; j++){
            if (((x << j) & 128) != 1){
                printf("%s", "%d", 'BFREE', i*8 +j); 
            }
        }
        
    }
}



