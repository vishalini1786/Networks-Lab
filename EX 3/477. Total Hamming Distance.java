class Solution {
    public int totalHammingDistance(int[] nums) {
        int n = nums.length;
        int answer = 0;

        for (int bit = 0; bit < 31; bit++) {

            int ones = 0;

            for (int i = 0; i < n; i++) {

                if ((nums[i] & (1 << bit)) != 0) {
                    ones++;
                }
            }

            int zeros = n - ones;

            answer = answer + (ones * zeros);
        }

        return answer;
    }
}
