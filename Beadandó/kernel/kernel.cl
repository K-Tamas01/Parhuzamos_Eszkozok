__kernel void compression(__global char* characters, __global int* unichar_count, __global int* huffman_code, int array_size) {
    int id = get_global_id(0);

    if(id >= array_size){
        return;
    }

    char temp;
    for (int i = 0; i < array_size; i++) {
        if (unichar_count[id] > unichar_count[i]) {
            temp = characters[id];
            characters[id] = characters[i];
            characters[i] = temp;
            unichar_count[i] = unichar_count[id] + unichar_count[i];
            unichar_count[id] = unichar_count[i] - unichar_count[id];
            unichar_count[i] = unichar_count[i] - unichar_count[id];
        }
    }
}