#include <stdlib.h>

int* countBits(int n, int* returnSize) {
    // 1. Allocate the memory array
    int* ans = (int*)malloc((n + 1) * sizeof(int));
    *returnSize = n + 1;
    
    // 2. Loop through every number from 0 to n
    for (int i = 0; i <= n; i++) {
        int temp = i;
        int count = 0;
        
        // 3. Use your division-by-2 logic to count 1s
        while (temp > 0) {
            int lastBit = temp % 2; 
            if (lastBit == 1) {
                count++;
            }
            temp /= 2; // Move to the next bit
        }
        
        ans[i] = count; // Store the total count for this number
    }
    
    return ans;
}
