__kernel void hello_kernel(__global int* buffer, int n) {
       if (get_global_id(0) < n) {
            const uint id = get_global_id(0);
            buffer[id] = buffer[n-1-id];
       }
    }