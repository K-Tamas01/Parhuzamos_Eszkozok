#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dir_con.h"
#include "compression.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#define MAX_FILES 1000000
#define GLOBAL_WORKERS 10000

int main(void)
{
    clock_t start = clock();
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
    double allWriteTime[count];
    double allReadTime[count];
    double allKernelTime[count];

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
    cl_kernel kernel = clCreateKernel(program, "count_chars", NULL);

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
        printf("%d\n", size);
        char characters[size];
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

        int uniCharsCount[256];

        //Host buffers
        cl_mem chars = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(char), NULL, NULL);
        cl_mem uniCharCount = clCreateBuffer(context, CL_MEM_READ_WRITE, 256 * sizeof(int), NULL, NULL);

        // Set kernel arguments
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &chars);
        clSetKernelArg(kernel, 1, sizeof(int), &size);
        clSetKernelArg(kernel, 2, sizeof(cl_mem), &uniCharCount);

        // Create the command queue
        cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &err);

        clock_t writeStart = clock();

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

        clock_t writeEnd = clock();
        allWriteTime[i] = (double)((writeEnd - writeStart) / CLOCKS_PER_SEC);

        // Size specification
        size_t local_work_size = 256;
        size_t n_work_groups = (GLOBAL_WORKERS + local_work_size + 1) / local_work_size;
        size_t global_work_size = n_work_groups * local_work_size;

        clock_t kernelStart = clock();

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

        clFinish(command_queue);

        clock_t kernelEnd = clock();
        allKernelTime[i] = (double)((writeEnd - writeStart) / CLOCKS_PER_SEC);

        // Host buffer <- Device buffer

        clock_t readStart = clock();

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

        clock_t readEnd = clock();
        allReadTime[i] = (double)((writeEnd - writeStart) / CLOCKS_PER_SEC);

        clReleaseMemObject(chars);
        clReleaseMemObject(uniCharCount);

        compreession(files[i], uniCharsCount);
    }

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(files);
    free(source_code);

    clock_t end = clock();
    double runTime = (double)(end - start)/ CLOCKS_PER_SEC;

    double sumReadTime = 0.0;
    double sumWriteTime = 0.0;
    double sumKernelTime = 0.0;

    for(int i = 0; i < count; i++){
        sumReadTime += (double)allReadTime[i];
        sumWriteTime += (double)allWriteTime[i];
        sumKernelTime += (double)allKernelTime[i];
    }


    printf("Readtime: %f sec\n", sumReadTime);
    for(int i = 0; i < count; i++){
        printf("\t%d Read the contents of the buffer from a kernel: %f sec\n", i+1, allReadTime[i]);
    }
    printf("\n");
    printf("Writetime: %f sec\n", sumWriteTime);
    for(int i = 0; i < count; i++){
        printf("\t%d Write the contents of the file to a buffer: %f sec\n", i+1, allReadTime[i]);
    }
    printf("\n");
    printf("Kerneltime: %f sec\n", sumKernelTime);
    for(int i = 0; i < count; i++){
        printf("\t%d kernel runtime: %f sec\n", i+1, allReadTime[i]);
    }
    printf("\n");
    printf("Runtime: %f sec\n", runTime);

    return 0;
}