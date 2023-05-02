__kernel void compression(__global char* chars, int size, __global int* uniCharCount, __global int* uniChars) {
    int id = get_global_id(0);
    if(id >= size) return;

    int ascii = (int)chars[id];
    atomic_add(&uniCharCount[ascii], 1);

    if (uniCharCount[ascii] == 1) {
        atomic_add(&uniChars[0], 1);
    }
}