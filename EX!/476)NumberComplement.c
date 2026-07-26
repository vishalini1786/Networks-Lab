int findComplement(int num) {
    if (num == 0) {
        return 1;
    }

    int complementSum = 0;
    long placeValue = 1; 

    while (num > 0) {
        int lastBit = num % 2; 
        int flippedBit = (lastBit == 0) ? 1 : 0;

        complementSum += flippedBit * placeValue;

        placeValue *= 2; 
        num /= 2;        
    }

    return complementSum;
}
