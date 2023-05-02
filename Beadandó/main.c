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
        char* characters = (char*) malloc(sizeof(char) * (size + 1));
        if(characters == NULL){
            printf("[ERROR] Failed to allocate memory.\n");
            return -1;
        }
        if (fread(characters, sizeof(char), size, src_file) != size) {
            printf("[ERROR] Failed to read text/binary file.\n");
            return -1;
        }

        characters[size] = '\0';
        fclose(src_file);

        char* uniChars = (char*) malloc(sizeof(char) * 256);
        int* uniCharsCount = (int*) malloc(sizeof(int) * 256);

        //Host buffers
        cl_mem chars = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(char), NULL, NULL);
        cl_mem uniCharCount = clCreateBuffer(context, CL_MEM_READ_WRITE, 256 * sizeof(int), NULL, NULL);
        cl_mem uniCharsBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, 256 * sizeof(int), NULL, NULL);

        // Set kernel arguments
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &chars);
        clSetKernelArg(kernel, 1, sizeof(int), &size);
        clSetKernelArg(kernel, 2, sizeof(cl_mem), &uniCharCount);
        clSetKernelArg(kernel, 3, sizeof(cl_mem), &uniCharsBuffer);

        // Create the command queue
        cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &err);

        clEnqueueWriteBuffer(
            command_queue,
            chars,
            CL_FALSE,
            0,
            size * sizeof(char),
            characters,
            0,
            NULL,
            NULL
        );

        clEnqueueWriteBuffer(
            command_queue,
            uniCharCount,
            CL_FALSE,
            0,
            255 * sizeof(int),
            uniCharsCount,
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
            uniCharsBuffer,
            CL_TRUE,
            0,
            256 * sizeof(int),
            uniChars,
            0,
            NULL,
            NULL
        );

        clEnqueueReadBuffer(
            command_queue,
            uniCharCount,
            CL_TRUE,
            0,
            256 * sizeof(int),
            uniCharsCount,
            0,
            NULL,
            NULL
        );

        printf("%c", "b");

        // huffman fa építés

        // for(int i = 0; i < strlen(uniChars) - 1; i++){
        //     char temp;
        //     for(int j = i + 1; j < strlen(uniChars)){
        //         if(uniCharCount[i] > uniCharCount[j]){
        //             temp = uniChars[i];
        //             uniChars[i] = uniChars[j];
        //             uniChars[j] = temp;

        //             uniCharCount[i] += uniCharCount[j];
        //             uniCharCount[j] = uniCharCount[i] - uniCharCount[j];
        //             uniCharCount[i] = uniCharCount[i] - uniCharCount[j];
        //         }
        //     }
        // }

        clReleaseMemObject(uniCharsBuffer);
        clReleaseMemObject(uniCharCount);
        clReleaseMemObject(chars);
        free(uniChars);
        free(characters);
    }

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(files);
    free(source_code);

    return 0;
}