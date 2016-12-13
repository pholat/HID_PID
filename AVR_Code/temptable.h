/*
 * temptable.h
 *
 *  Created on: 03-08-2014
 *      Author: pholat
 */

#ifndef TEMPTABLE_H_
#define TEMPTABLE_H_

uint16_t tempTable[]= {
    1021,1020,1019,1018,1016,1014,1011,1008,1005,1001,996,990,983,976,967,957,945,
    932,918,902,885,866,846,824,800,776,750,723,695,667,638,609,580,551,523,495,467,
    441,415,390,367,344,323,302,283,265,248,232,217,203,190,178,166,156,146,137,128,
    120,113,106,100
};
uint8_t deltTable[]= {
    0,1,1,1,2,2,2,3,4,4,5,6,7,8,9,10,11,13,14,16,17,19,20,22,23,25,26,27,28,28,29,29,
    29,29,28,28,27,27,26,25,24,23,21,20,19,18,17,16,15,14,13,12,11,11,10,9,9,8,7,7,6
};

uint16_t find_temp(uint16_t ADC)
{
    uint16_t temperature=300;

    return temperature;
}

#endif /* TEMPTABLE_H_ */
