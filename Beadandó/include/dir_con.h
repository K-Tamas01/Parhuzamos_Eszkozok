#ifndef DIR_CON_H
#define DIR_CON_H

/**
 * Load the OpenCL kernel source code from a file.
 * 
 * path: Path of the source file
 * error_code: 0 on successful file loading
 * 
 * Returns by a dynamically allocated string
 */
int dir_read(const char* const path, char** files, int* count, const int MAX_FILES);

#endif