#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dir_con.h"
#include "compression.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#define MAX_FILES 1000000
#define GLOBAL_WORKERS 10000

int main(void)
{
    char **files = (char **) malloc(sizeof(char *) * MAX_FILES);
    int count = 0;
    dir_read("Adatok", files, &count, MAX_FILES);
    printf("Found %d files:\n", count);
    for(int i = 0; i < count; i++){
        printf("%s\n", files[i]);
    }


    //read files
    char** file_contain = (char**) malloc(sizeof(char *) * count);
    int* sizeOfFile = (int*) malloc(sizeof(int *) * count);
    for(int i = 0; i < count; i++){
        FILE* src_file;
        int size;

        src_file = fopen(files[i], "rb");
        if(src_file == NULL){
            printf("[ERROR] Falied to open file\n");
            return -1;
        }
        fseek(src_file, 0 , SEEK_END);
        size = ftell(src_file);
        rewind(src_file);
        file_contain[i] = (char*) malloc(sizeof(char)* (size + 1));
        if(file_contain[i] == NULL){
            printf("[ERROR] Failed to allocate memory.\n");
            return -1;
        }
        if (fread(file_contain[i], sizeof(char), size, src_file) != size) {
            printf("[ERROR] Failed to read text/binary file.\n");
            return -1;
        }
        file_contain[i][size] = '\0';
        sizeOfFile[i] = size;
        fclose(src_file);
    }

    for(int i = 0; i < count; i++){
        printf("\n%s\n", files[i]);
        fwrite(file_contain[i], 1, sizeOfFile[i], stdout);
    }

    // characters count
    char** characters = (char**) malloc(sizeof(char*) * count);
    int** charCount = (int**) malloc(sizeof(int*) * count);
    int* unique_countOfFiles = (int*) malloc(sizeof(int) * count);
    for(int i = 0; i < count; i++){
        int unique_count = 0;
        char visited[255] = {0};
        for(int j = 0; j < sizeOfFile[i]; j++){
            if(visited[(int) characters[i][j]] == 0){
                visited[(int) characters[i][j]] = 1;
                unique_count++;
            }
        }
        unique_countOfFiles[i] = unique_count;
        characters[i] = (char*) malloc(sizeof(char*) * unique_countOfFiles[i]);
        charCount[i] = (int*) malloc(sizeof(int) * unique_countOfFiles[i]);
        int index = 0;
        for(int j = 0; j < sizeOfFile[i]; j++){
            int found = 0;
            for(int k = 0; k < unique_countOfFiles[i] && found == 0; k++){
                if(characters[i][k] == file_contain[i][j]){
                    charCount[i][k]++;
                    found = 1;
                }
            }
            if(found == 0){
                characters[i][index] = file_contain[i][j];
                charCount[i][index] = 1;
                index++;
            }
        }
        printf("\n");
    }

    int i;
    cl_int err;

    // Get platform
    cl_uint n_platforms;
	cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
		return 0;
	}

    // Get device
	cl_device_id device_id;
	cl_uint n_devices;
	err = clGetDeviceIDs(
		platform_id,
		CL_DEVICE_TYPE_GPU,
		1,
		&device_id,
		&n_devices
	);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
		return 0;
	}

    //kernel code read
    FILE* source_file;
    char* source_code;
    int file_size;

    source_file = fopen("./kernel/kernel.cl", "rb");
    if (source_file == NULL) {
        printf("[ERROR] Failed to open kernel file.\n");
        return -1;
    }

    fseek(source_file, 0, SEEK_END);
    file_size = ftell(source_file);
    rewind(source_file);
    source_code = (char*)malloc(file_size + 1);
    if (fread(source_code, sizeof(char), file_size, source_file) != file_size) {
        printf("[ERROR] Failed to read kernel file.\n");
        return -1;
    }
    source_code[file_size] = '\0';

    fclose(source_file);


    // Create OpenCL context
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    // Build the program
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&source_code, NULL, NULL);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", err);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "compression", NULL);

    // Host buffer
    int** huffmanTree = (int**)malloc(sizeof(int*) * count);

    // Create the device buffer
    cl_mem huffman_tree = clCreateBuffer(context, CL_MEM_READ_WRITE, count * sizeof(int), NULL, NULL);
    cl_mem charactersInFile = clCreateBuffer(context, CL_MEM_READ_WRITE, count * sizeof(char), NULL, NULL);
    cl_mem characters_count = clCreateBuffer(context, CL_MEM_READ_WRITE, count * sizeof(int), NULL, NULL);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&charactersInFile);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&characters_count);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&huffman_tree);
    for(int i = 0; i < sizeof(characters); i++){
        int n = sizeof(characters[i]);
        clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&n);
    }

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &err);

    for(int i = 0; i < count; i++){
        clEnqueueWriteBuffer(
            command_queue,
            charactersInFile,
            CL_FALSE,
            0,
            sizeOfFile[i] * sizeof(int),
            characters[i],
            0,
            NULL,
            NULL
        );

        clEnqueueWriteBuffer(
            command_queue,
            characters_count,
            CL_FALSE,
            0,
            sizeOfFile[i] * sizeof(int),
            charCount[i],
            0,
            NULL,
            NULL
        );

        // Size specification
        size_t local_work_size = 256;
        size_t n_work_groups = (GLOBAL_WORKERS + local_work_size + 1) / local_work_size;
        size_t global_work_size = n_work_groups * local_work_size;

        // Apply the kernel on the range
        clEnqueueNDRangeKernel(
            command_queue,
            kernel,
            1,
            NULL,
            &global_work_size,
            &local_work_size,
            0,
            NULL,
            NULL
        );

        // Host buffer <- Device buffer
        clEnqueueReadBuffer(
            command_queue,
            huffman_tree,
            CL_TRUE,
            0,
            count * sizeof(int),
            huffmanTree[i],
            0,
            NULL,
            NULL
        );


        clEnqueueReadBuffer(
            command_queue,
            charactersInFile,
            CL_TRUE,
            0,
            count * sizeof(int),
            characters[i],
            0,
            NULL,
            NULL
        );
        clEnqueueReadBuffer(
            command_queue,
            characters_count,
            CL_TRUE,
            0,
            count * sizeof(int),
            charCount[i],
            0,
            NULL,
            NULL
        );

    }

    for(int i = 0; i < count; i++){
        for(int j = 0; j < 12; j++){
            printf("%c: %d\n", characters[i][j], charCount[i][j]);
        }
    }

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(characters);
    free(charCount);
    free(files);
    free(file_contain);
    free(sizeOfFile);
    free(source_code);
    free(huffman_tree);

    return 0;
}