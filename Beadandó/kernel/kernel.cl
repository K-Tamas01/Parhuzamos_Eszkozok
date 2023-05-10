__kernel void count_chars(__global const unsigned char* chars, const int size, __global int* uniCharCount) {
    int id = get_global_id(0);

    if(id >= size) return;

    int uniCode = chars[id];
    
    if(uniCode < 0){
        atomic_inc(&uniCharCount[uniCode + 256]);
    }
    else {
        atomic_inc(&uniCharCount[uniCode]);
    }
}