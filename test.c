#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

char *path_cat (const char *str1, char *str2);
int path_concat(char *, const char*, const char*);

int main () {
	struct dirent *dp;

	// enter existing path to directory below
	const char *dir_path="/home/mturquette/debugfs/clock";
	DIR *dir = opendir(dir_path);
	while ((dp=readdir(dir)) != NULL) {
		char *tmp;
		tmp = malloc(sizeof(char) * 1000);
		//tmp = path_cat(dir_path, dp->d_name);
		path_concat(tmp, dir_path, dp->d_name);
		printf("%s\n", tmp);
		free(tmp);
		tmp=NULL;
	}
	closedir(dir);
	return 0;
}

int path_concat(char *dest, const char *src1, const char *src2) {
	/*size_t str1_len = strlen(str1);
	size_t str2_len = strlen(str2);
	result = malloc((str1_len+str2_len+1)*sizeof *result);*/

	char *tmp = dest;

	tmp = stpcpy(tmp, src1);
	strcpy(tmp, src2);
	//tmp = stpcpy(tmp, src2);

	return 0;
}

char *path_cat (const char *str1, char *str2) {
	size_t str1_len = strlen(str1);
	size_t str2_len = strlen(str2);
	char *result;
	result = malloc((str1_len+str2_len+1)*sizeof *result);
	strcpy (result,str1);
	int i,j;
	for(i=str1_len, j=0; ((i<(str1_len+str2_len)) && (j<str2_len));i++, j++) {
		result[i]=str2[j];
	}
	result[str1_len+str2_len]='\0';
	return result;
}

