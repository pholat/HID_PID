#ifndef TEMPTABLE_H
#define TEMPTABLE_H
#include <QVector>

/*
 * Standard values shall be calculated bu the begining of program
 * but as I calculated it before it doesn't matter
 * if you want more precise values than you shall change it.
 * Also ... you shall use external ADC module, fine 0.1% 1kR resistor
 * filter on input, and finally 5V Voltage refs (which in fact isn't
 * very important as voltage on refs is same as supply, and as such
 * ADC won't waver.
 * */

// Each number means +5 temp val
// So tempTable[0] is 0*C
// tempTable [1] is 5*C
// till 300*C
// It's from epcos 100k NTC table
u_int16_t tempTable[]= {
    1021,1020,1019,1018,1016,1014,1011,1008,1005,1001,996,990,983,976,967,957,945,
    932,918,902,885,866,846,824,800,776,750,723,695,667,638,609,580,551,523,495,467,
    441,415,390,367,344,323,302,283,265,248,232,217,203,190,178,166,156,146,137,128,
    120,113,106,100
};
u_int8_t deltTable[]= {
    0,1,1,1,2,2,2,3,4,4,5,6,7,8,9,10,11,13,14,16,17,19,20,22,23,25,26,27,28,28,29,29,
    29,29,28,28,27,27,26,25,24,23,21,20,19,18,17,16,15,14,13,12,11,11,10,9,9,8,7,7,6
};

double temp_find(uint16_t ADC)
{
    double temperature=0;
    int i=0;
    while(tempTable[i]>ADC)
        i++;
    //for(i=0;tempTable[i]>ADC && i<60 ;i++);
    // Temperatures are 0,5,10... ADC measurements resolution variates which is included with
    // deltTable. i*5 is alvays too big, and here comes
    // temperature = (actual ADC - standard values)*(5*C )/ (per actual ADC per 5*C resolution)
    // If else is to remember not to divide by 0...
    double multiply;
    if(deltTable[i]) {
        multiply = 5/(double)(deltTable[i]);
    } else {
        multiply = 0;
    };
    temperature= i*5 + (ADC - tempTable[i])*multiply;//*5/deltTable[i];// - (double)((ADC-tempTable[i])*5)/deltTable[i];
    return temperature;
}

u_int16_t temp_to_send(double temp)
{
    // Temperature to set is being calculated like that:
    // First we take integral position in tempTable
    // Than we take only post coma part of tempset
    // To calculate right value to set we take standard ADC value
    // and than we add what would be from deltTable scaling
    // Temp set is actually value betveen 0 - 1024 (roughly...)
    int integral = (uint)(temp)/5;
    double hundreth = temp - (uint)temp;
    u_int16_t temp_to_Set = tempTable[integral] + deltTable[integral]*hundreth/5;
    return temp_to_Set;
}

#endif // TEMPTABLE_H
