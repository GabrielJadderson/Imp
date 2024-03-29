#include "file_io.h"
#include <stdio.h>
#include "memory.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

char* file_get_name(char* filepath)
{
	if (filepath)
	{
		char* current = NULL;
		char* previous = NULL;

		current = strchr(filepath, '/');
		if (current == NULL)
			return filepath;
		while (current != NULL)
		{
			previous = current;
			current = strchr(current, '/');
		}
		return previous;
	}
	return NULL;
}

falcon_file_handle* file_write(char* filepath, string* data)
{
	if (filepath && data)
	{

	}
	return NULL;
}

void file_write_linked_list(char* filepath, linked_list** list)
{
	if (!filepath) return;

	//string* path = file_check_and_sanitize_in_working_directory(filepath);
	//if (path == NULL) return;

	FILE * file = NULL;
	if ((file = fopen(filepath, "w")) != NULL)
	{
		linked_list_node** referencePointer = (*list)->root;
		while (*referencePointer)
		{
			linked_list_node* ptr = *referencePointer;
			referencePointer = &(*referencePointer)->next; //continue down the list until null
			fprintf(file, "%s", (char*)ptr->data);
		}
		fflush(file);
		fclose(file);
	}
	else log_internal_errno(__FILE__, __func__, __LINE__);
}


void file_write_string(char* filepath, char* string)
{
	if (!filepath) return;
	//string* path = file_check_and_sanitize_in_working_directory(filepath);
	//if (path == NULL) return;



	FILE * file = NULL;

	if ((file = fopen(filepath, "w")) != NULL)
	{
		fprintf(file, "%s", string);
		fclose(file);
	}
	else log_internal_errno(__FILE__, __func__, __LINE__);
}

/*
falcon_file_handle* file_open(char* filepath)
{
	if (filepath)
	{

		FILE * file = NULL;
		if ((file = fopen(path->Text, "r")) != NULL)
		{
			char *buffer = NULL;
			size_t size = 0;

			//getting the size of the file, by going to the end.
			if (fseek(file, 0, SEEK_END) != 0) //failed?
			{
				log_internal_errno(__FILE__, __func__, __LINE__);
				return NULL;
			}

			size = ftell(file); //store the size
			rewind(file); //go back to start.
			buffer = Malloc((size + 1) * sizeof(char));
			if (buffer)
			{
				fread(buffer, size, 1, file);
				buffer[size] = '\0';
				//fclose(file);
			}
			falcon_file_handle* fh = Malloc(sizeof(falcon_file_handle));
			fh->file_path = path;
			fh->file_handle = file;
			fh->file_size = size;
			fh->file_data = string_heap_new(buffer);
			fh->file_is_opened = true;
			return fh;
			//fclose(file);

		}
		else log_internal_errno(__FILE__, __func__, __LINE__);
	}
	else log_internal_error(__FILE__, __func__, __LINE__, "The input filepath was null.");
	return NULL;
}
*/

bool file_close(falcon_file_handle * fh)
{
	if (fh)
	{
		if (fh->file_is_opened)
		{
			int result = fclose(fh->file_handle);
			if (result == 0)
			{
				fh->file_is_opened = false;
				return true;
			}
		}
	}
	return false;
}

//this is expensive. only use this at the end.
bool file_close_and_clean(falcon_file_handle * fh)
{
	if (fh)
	{
		if (fh->file_is_opened)
		{
			if (file_close(fh))
			{
				string_heap_delete(fh->file_data);
				string_heap_delete(fh->file_path);
				free(fh);
				fh = NULL;
				return true;
			}
		}
	}
	return false;
}
