#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "file.h"
#include "common.h"
#include "parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s -n -f <database file>\n", argv[0]);
    printf("\t-n   - Create new database file\n");
    printf("\t-f   - Path to database file\n");
}

int main(int argc, char *argv[]) {
    int c = 0;
    char *filepath = NULL;
    char *addstring = NULL;
    bool newfile = false;
    int database_file_descriptor = -1;
    struct dbheader_t *database_header = NULL;
    struct employee_t *employees = NULL;

    while ((c = getopt(argc, argv, "nf:a:")) != -1) {
	switch (c) {
        case 'a':
            addstring = optarg;
            break;
        case 'n':
            newfile = true;
            break;
        case 'f':
            filepath = optarg;
            break;
        case '?':
            printf("Unknown option -%c\n", c);
            break;
        default:
            return STATUS_ERROR;
        }
    }

    if (filepath == NULL) {
	printf("Filepath is a required argument\n");
        print_usage(argv);

        return 0;
    }

    if (newfile) {
	database_file_descriptor = create_db_file(filepath);
	
        if (database_file_descriptor == STATUS_ERROR) {
            printf("Unable to create database file\n");
            return STATUS_ERROR;
        }

        if (create_db_header(database_file_descriptor, &database_header) == STATUS_ERROR) {
            printf("Failed to create database header\n");
            return STATUS_ERROR;
        }
        
    } else {
	database_file_descriptor = open_db_file(filepath);
        if (database_file_descriptor == STATUS_ERROR) {
	    printf("Unable to open database file\n");
	    return STATUS_ERROR;
        }
        
        if (validate_db_header(database_file_descriptor, &database_header) == STATUS_ERROR) {
            printf("Unable to validate database header\n");
            return STATUS_ERROR;
        }
    }

    if (read_employees(database_file_descriptor, database_header, &employees) == STATUS_ERROR) {
	printf("Failed to read employees\n");
	return STATUS_ERROR;
    }

    if (addstring) {
	database_header->count++;
	employees = realloc(employees, database_header->count * sizeof(struct employee_t));
	
	add_employee(database_header, employees, addstring);
    }
    
    output_file(database_file_descriptor, database_header, employees);

    return 0;
}
