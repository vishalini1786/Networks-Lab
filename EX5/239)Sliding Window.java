import java.util.*;

class Solution {
    public int[] maxSlidingWindow(int[] nums, int k) {

        int n = nums.length;
        int[] result = new int[n - k + 1];

        Deque<Integer> deque = new ArrayDeque<>();

        int j = 0;

        for (int i = 0; i < n; i++) {

            // Remove elements outside the window
            while (!deque.isEmpty() && deque.peekFirst() <= i - k) {
                deque.pollFirst();
            }

            // Remove smaller elements
            while (!deque.isEmpty() &&
                   nums[deque.peekLast()] <= nums[i]) {
                deque.pollLast();
            }

            // Add current index
            deque.offerLast(i);

            // Store maximum
            if (i >= k - 1) {
                result[j] = nums[deque.peekFirst()];
                j++;
            }
        }

        return result;
    }
}
