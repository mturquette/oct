/*
 * OMAP Clock Tool
 *
 * Copyright (C) 2010 Texas Instruments, Incorporated
 * Written by Mike Turquette <mturquette@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
//#include "clktree.h"
#include <sys/types.h>

/* XXX
#include <xmlparser.h>
#include <graphiz.h>
*/

/* XXX handle opts and args later */

int path_concat(char *dest, const char *src1, const char *src2) {
	char *tmp = dest;

	tmp = stpcpy(tmp, src1);
	tmp = stpcpy(tmp, "/");
	strcpy(tmp, src2);

	return 0;
}

/*
 * this can be generalised to work even for the first step
 * first pass in path to debugfs mount, then pass in string 'clock'
 * clk_tree_search(debugfs_mnt, "clock");
 *
 * better yet, char *path should be both the starting point (debugfs_mnt) and
 * also the final path to the clock we care about.  This final path is needed
 * is for printing the rate, parent traversals, etc.
 */
char *clk_tree_search(char *path, char *clk_name)
{
	struct dirent *dir_content;
	DIR *current_dir = opendir(path);
	char *new_path;

	new_path = malloc(sizeof(char) * 1000);

	/* dir_content represents all items inside current_dir */
	while((dir_content=readdir(current_dir)) != NULL) {
		path_concat(new_path, path, dir_content->d_name);

		if (dir_content->d_type == DT_DIR &&
				(strcmp(dir_content->d_name, ".") &&
				 strcmp(dir_content->d_name, ".."))) {
			if (!strcmp(dir_content->d_name, clk_name)) {
				closedir(current_dir);
				return new_path;
			} else {
				return clk_tree_search(new_path, clk_name);
			}
		}
	}

	/*
	 * XXX this does not make sense if we thread instances of
	 * clk_tree_search in the future
	 */
	closedir(current_dir);
	return NULL;
}

int main(char *argv, int argc)
{
	/* fastest DPLLs are sub-10GHz.  12 chars gives padding of 1 digit */
	char buf[12];
	unsigned long rate;
	int fd;

	/*
	 * for now assume /home/mturquette/debugfs/clock exists
	 * assume clock we're interested in is 'pck'
	 * tree is very simple.  add branches to it later
	 */
	//char *path = "/home/mturquette/debugfs";

	char *path = "/debug/clock/virt_26m_ck/osc_sys_ck/sys_ck/dpll4_ck";
	path = clk_tree_search(path, "pck");

	//printf("final path to pck is %s\n", path);

	path_concat(path, path, "rate");
	printf("path to rate is %s\n", path);

	size_t nbytes;
	ssize_t bytes_read;

	nbytes = sizeof(buf);

	fd = open(path, O_RDONLY);
	bytes_read = read(fd, buf, nbytes);
	rate = strtoul(buf, NULL, 0);


	printf("buf is %s\n", buf);
	printf("rate is %lu\n", rate);

	close(fd);
	free(path);

	return 0;
}
