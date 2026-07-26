class Solution {
    public boolean hasAlternatingBits(int n) {

        int previousBit = n % 2;
        n = n / 2;

        while (n > 0) {

            int currentBit = n % 2;

            if (currentBit == previousBit) {
                return false;
            }

            previousBit = currentBit;
            n = n / 2;
        }

        return true;
    }
}
