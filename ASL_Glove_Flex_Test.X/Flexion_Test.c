/*
 * File:   Flexion_Test.c
 * Author: tikraeme
 *
 * Created on May 22, 2023, 3:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timers.h"
#include "BOARD.h"
#include "AD.h"
#include "serial.h"
#include "Oled.h"
#include "Node.h"
#include <xc.h>

// LITERALLY THE MOST CURSED WAY TO HAVE TIME DELAYS THIS IS DISGUSTING
//
// #define DELAY(x)    {int wait; for (wait = 0; wait <= x; wait++)
// {asm("nop");}} #define A_BIT       18300 #define A_BIT_LESS  50000 #define
// A_LOT       183000

/*
 *
 */

#define WINDOW_SIZE 50
#define CALIBRATION_SIZE 20
#define CALIBRATION_SEP 500
#define CALIBRATION_TIME 5000

unsigned int moving_avg_calc(unsigned int arr[WINDOW_SIZE]) {
    unsigned int total = 0;
    for (int i = 0; i < WINDOW_SIZE; i++) {
        total += arr[i];
    }
    return total / WINDOW_SIZE;
}

void ReadFingers(unsigned int readings[5]) {
    readings[4] = 1023 - AD_ReadADPin(1 << 1);
    readings[0] = 1023 - AD_ReadADPin(1 << 5);
    for (int i = 3; i > 0; i--) {
        readings[i] = AD_ReadADPin(1 << (5 - i));
    }
}

// Just here in case we decide to use it later

/**
 * Calculates a linear regression for the input data, return a and b s.t. y=ax+b
 */
void LinearRegression(unsigned int x[CALIBRATION_SIZE],
                      unsigned int y[CALIBRATION_SIZE], float *a, float *b) {
    int sum_x, sum_y, sum_x2, sum_xy = 0;
    for (int i = 0; i < CALIBRATION_SIZE; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_x2 += x[i] * x[i];
        sum_xy += x[i] * y[i];
    }
    float denom = CALIBRATION_SIZE * sum_x2 - sum_x * sum_x;
    *a = (sum_y * sum_x2 - sum_x * sum_xy) / denom;

    *b = (CALIBRATION_SIZE * sum_xy - sum_x * sum_y) / denom;
}

int main() {
    BOARD_Init();
    TIMERS_Init();
    AD_Init();
    OledInit();
    AD_AddPins(AD_A1 | AD_A2 | AD_A3 | AD_A4 | AD_A5);
    unsigned int raw_readings[5] = {0};  // Raw input
    unsigned int straight_val[5] = {0};  // Calibration measurement
    unsigned int flexed_thresh[5] = {0}; // Flexion threshold
    TRISDSET = 1 << 10;
    unsigned int moving_average[5][WINDOW_SIZE];
    int moving_avg_ptr = 0;   // Tracks current moving average index
    unsigned int adj_data[5]; // Averaged data
    int calibrated = 0;       // Has the glove been "calibrated"
    int calNum = 0;

    // Timing for taking inputs
    unsigned int curTime;
    unsigned int prevTime;

    uint8_t avg = 0;    // Have WINDOW_SIZE readings been taken
    uint8_t button = 0; // Button input

    Flexion_t fingers[5] = {0};
    //Node_t dummy = DefaultInit();
    
    //--------------Tree Creation------------------------------
    Node_t dummy = DefaultInit();
    Node_t t_f = DefaultInit();
    Node_t t_s = DefaultInit();
    AddChild(&dummy, &t_f);
    AddChild(&dummy, &t_s);
    SetFlex(&t_f, FLEXED);
    SetFlex(&t_s, STRAIGHT);

    Node_t i_ff = DefaultInit();
    Node_t i_fs = DefaultInit();
    AddChild(&t_f, &i_ff);
    AddChild(&t_f, &i_fs);
    SetFlex(&i_ff, FLEXED);
    SetFlex(&i_fs, STRAIGHT);

    Node_t i_sf = DefaultInit();
    Node_t i_ss = DefaultInit();
    AddChild(&t_s, &i_sf);
    AddChild(&t_s, &i_ss);
    SetValue(&i_sf, "10");
    SetFlex(&i_sf, FLEXED);
    SetFlex(&i_ss, STRAIGHT);

    Node_t m_fsf = DefaultInit();
    Node_t m_fss = DefaultInit();
    Node_t m_ssf = DefaultInit();
    Node_t m_sff = DefaultInit();
    Node_t m_ffs = DefaultInit();
    AddChild(&i_fs, &m_fsf);
    AddChild(&i_fs, &m_fss);
    AddChild(&i_ss, &m_ssf);
    AddChild(&i_sf, &m_sff);
    AddChild(&i_ff,&m_ffs);
    SetFlex(&m_fsf, FLEXED);
    SetFlex(&m_fss, STRAIGHT);
    SetFlex(&m_ssf,FLEXED);
    SetFlex(&m_sff,FLEXED);
    SetFlex(&m_ffs,STRAIGHT);
    SetLetter(&m_ffs, "F");
    SetValue(&m_ffs, "9");
    SetLetter(&m_sff, "Y");
    SetLetter(&m_ssf, "G");

    Node_t m_sss = DefaultInit();
    AddChild(&i_ss, &m_sss);
    SetFlex(&m_sss, STRAIGHT);

    Node_t m_fff = DefaultInit();
    AddChild(&i_ff,&m_fff);
    SetFlex(&m_fff,FLEXED);

    Node_t r_fsff = DefaultInit();
    Node_t r_ffff = DefaultInit();
    Node_t r_fsfs = DefaultInit();
    AddChild(&m_fsf, &r_fsff);
    AddChild(&m_fsf, &r_fsfs);
    AddChild(&m_fff,&r_ffff);
    SetFlex(&r_fsff, FLEXED);
    SetFlex(&r_fsfs, STRAIGHT);
    SetFlex(&r_ffff, FLEXED);
    SetValue(&r_fsff, "1");
    SetLetter(&r_fsff, "D");
    SetValue(&r_fsfs, "8");

    Node_t r_fssf = DefaultInit();
    Node_t r_fsss = DefaultInit();
    AddChild(&m_fss, &r_fssf);
    AddChild(&m_fss, &r_fsss);
    SetFlex(&r_fssf, FLEXED);
    SetFlex(&r_fsss, STRAIGHT);

    Node_t r_sssf = DefaultInit();
    Node_t r_ssss = DefaultInit();
    AddChild(&m_sss, &r_sssf);
    AddChild(&m_sss, &r_ssss);
    SetFlex(&r_sssf, FLEXED);
    SetFlex(&r_ssss, STRAIGHT);
    SetValue(&r_sssf, "3");
    SetLetter(&r_sssf, "H");
    SetValue(&r_ssss, "5");

    Node_t p_fssff = DefaultInit();
    Node_t p_fssfs = DefaultInit();
    AddChild(&r_fssf, &p_fssff);
    AddChild(&r_fssf, &p_fssfs);
    SetFlex(&p_fssff, FLEXED);
    SetFlex(&p_fssfs, STRAIGHT);
    SetValue(&p_fssff, "2");
    SetLetter(&p_fssff,"R");
    SetValue(&p_fssfs, "7");

    Node_t p_fsssf = DefaultInit();
    Node_t p_fssss = DefaultInit();
    Node_t p_fffff = DefaultInit();
    Node_t p_ffffs = DefaultInit();
    AddChild(&r_fsss, &p_fsssf);
    AddChild(&r_fsss, &p_fssss);
    AddChild(&r_ffff,&p_fffff);
    AddChild(&r_ffff,&p_ffffs);
    SetFlex(&p_fsssf, FLEXED);
    SetFlex(&p_fssss, STRAIGHT);
    SetFlex(&p_fffff,FLEXED);
    SetFlex(&p_ffffs,STRAIGHT);
    SetLetter(&p_fffff, "O");
    SetValue(&p_fffff, "0");
    SetValue(&p_fsssf, "6");
    SetLetter(&p_fsssf, "W");
    SetValue(&p_fssss, "4");
    SetLetter(&p_fssss,"B");
    SetLetter(&p_ffffs, "I");
    //---------------------------------------------------------

    //state driven variables
    char* newVal = "12\0";
    char* oldVal = "12\0";  
    
    while (1) {
        if (calibrated) {
            if (TIMERS_GetMilliSeconds() % 20 == 0) {
                if (AD_IsNewDataReady()) {
                    ReadFingers(raw_readings);
                    button = (PORTD >> 10) & 1;
                    if (!avg && moving_avg_ptr % (WINDOW_SIZE - 1) == 0)
                        avg = 1;
                    for (int i = 0; i < 5; i++) {
                        moving_average[i][moving_avg_ptr] = raw_readings[i];
                        if (avg)
                            adj_data[i] = moving_avg_calc(moving_average[i]);
                        else
                            adj_data[i] = raw_readings[i];
                        if (adj_data[i] > flexed_thresh[i] &&
                            fingers[i] != STRAIGHT) {
                      //      printf("%d: STRAIGHT\r\n", i);
                            fingers[i] = STRAIGHT;
                        } else if (adj_data[i] < flexed_thresh[i] &&
                                   fingers[i] != FLEXED) {
                   ///         printf("%i: FLEXED\r\n", i);
                            fingers[i] = FLEXED;
                        }
                    }
                    // Will it work? Who knows
                    //test[] = {1, 2, 1, 1, 1};
                    
                    newVal = GetVal(dummy, fingers, button);
                    
                    //printf("%s\t", newVal);
                    //printf("[%d,%d,%d,%d,%d]\r\n",fingers[0],fingers[1],fingers[2],fingers[3],fingers[4]);
                    OledClear(0);
                    OledDrawString(newVal);
                    OledUpdate();

                    moving_avg_ptr = (moving_avg_ptr + 1) % WINDOW_SIZE;
                    
                    oldVal = newVal;
                }
            }
        } else {
            // "Calibrate" each finger to determine individual straight and
            // flexed thresholds (might not need a straight threshold? Can be
            // used for other levels of flex)
            OledClear(0);
            OledDrawString("Now calibrating: Hold fingers straight");
            OledUpdate();
            // Read a raw value from the ADC every few milliseconds to get
            // an average value at straight flex

            // TODO: Test if new process for data collection works properly
            // (collects CALIBRATION_SIZE number of points rather than going for
            // a certain amount of time, I think the ADC was having issues?)
            curTime = TIMERS_GetMilliSeconds();
            prevTime = 0;
            while (calNum < CALIBRATION_SIZE) {
                if ((curTime - prevTime) > CALIBRATION_SEP &&
                    AD_IsNewDataReady()) {
                    ReadFingers(raw_readings);
                    for (int i = 0; i < 5; i++) {
                        straight_val[i] += raw_readings[i];
                        //printf("%u\r\n", raw_readings[i]);
                    }
                    calNum++;
                    prevTime = curTime;
                }
                curTime = TIMERS_GetMilliSeconds();
            }

            // Same process for flexed
            OledClear(0);
            OledDrawString("Now calibrating: Hold fingers flexed");
            OledUpdate();
            curTime = TIMERS_GetMilliSeconds();
            calNum = 0;
            prevTime = 0;
            while (calNum < CALIBRATION_SIZE) {
                if ((curTime - prevTime) > CALIBRATION_SEP &&
                    AD_IsNewDataReady()) {
                    ReadFingers(raw_readings);
                    for (int i = 0; i < 5; i++) {
                        flexed_thresh[i] += raw_readings[i];
                        printf("%u\r\n", raw_readings[i]);
                    }
                    calNum++;
                    prevTime = curTime;
                }
                curTime = TIMERS_GetMilliSeconds();
            }
            OledClear(0);
            OledDrawString("Calibration done");
            OledUpdate();
            for (int i = 0; i < 5; i++) {
                straight_val[i] = straight_val[i] / CALIBRATION_SIZE;
                flexed_thresh[i] = flexed_thresh[i] / CALIBRATION_SIZE;
                // This should get us a midpoint threshold
                flexed_thresh[i] = (straight_val[i] + flexed_thresh[i]) / 2;
                printf("Finger %d: Straight - %d Flexed - %d\r\n", i,
                       straight_val[i], flexed_thresh[i]);
            }
            calibrated = 1;
        }
    }
}
