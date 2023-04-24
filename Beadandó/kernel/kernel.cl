__kernel void compression(__global int* buffer, int n) {
    if (get_global_id(0) < n) {
        buffer[get_global_id(0)] = 11;
    }
}