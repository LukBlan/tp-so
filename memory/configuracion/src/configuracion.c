/*
 ============================================================================
 Name        : configuracon.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "config.h"
#include "string.h"

t_config *config_create(char *path) {
	FILE* file = fopen(path, "r");

	if (file == NULL) {
		return NULL;
	}

	struct stat stat_file;
	stat(path, &stat_file);

	t_config *config = malloc(sizeof(t_config));

	config->path = strdup(path);
	config->properties = dictionary_create();

	char* buffer = calloc(1, stat_file.st_size + 1);
	fread(buffer, stat_file.st_size, 1, file);

	if (strstr(buffer, "\r\n")) {
		printf("\n\nconfig_create - WARNING: the file %s contains a \\r\\n sequence "
		 "- the Windows new line sequence. The \\r characters will remain as part "
		 "of the value, as Unix newlines consist of a single \\n. You can install "
		 "and use `dos2unix` program to convert your files to Unix style.\n\n", path);
	}

	char** lines = string_split(buffer, "\n");

	void add_cofiguration(char *line) {
		if (!string_is_empty(line) && !string_starts_with(line, "#")) {
			char** keyAndValue = string_n_split(line, 2, "=");
			dictionary_put(config->properties, keyAndValue[0], keyAndValue[1]);
			free(keyAndValue[0]);
			free(keyAndValue);
		}
	}
	string_iterate_lines(lines, add_cofiguration);

	string_array_destroy(lines);
	free(buffer);
	fclose(file);

	return config;
}
