//Perform calculations on samples to determine characteristics
#include "math.h"
#include "Cpu.h"
#include "types.h"
#include "sample.h"

uint16_t TripTime[3][1898] = { //TripTime[sample->Characteristic][iRMSposition]
  { //INVERSE
      236.75,178.41,143.40,120.06,103.39,90.89,81.16,73.37,67.01,61.70,57.20,53.35,50.02,47.09,44.51,42.22,40.17,38.32,36.65,35.13,33.74,32.47,31.30,30.22,29.22,28.29,27.42,26.61,25.85,25.14,24.48,23.85,23.26,22.70,22.17,21.66,21.19,20.73,20.30,19.89,19.50,19.13,18.77,18.43,18.10,17.79,17.48,17.19,16.92,16.65,16.39,16.14,15.90,15.67,15.45,15.23,15.02,14.82,14.63,14.44,14.26,14.08,13.91,13.74,13.58,13.42,13.27,13.12,12.98,12.84,12.70,12.57,12.44,12.31,12.19,12.07,11.95,11.84,11.73,11.62,11.51,11.41,11.31,11.21,11.11,11.02,10.93,10.84,10.75,10.66,10.58,10.49,10.41,10.33,10.25,10.18,10.10,10.03,9.96,9.89,9.82,9.75,9.68,9.62,9.55,9.49,9.43,9.36,9.30,9.25,9.19,9.13,9.07,9.02,8.97,8.91,8.86,8.81,8.76,8.71,8.66,8.61,8.56,8.52,8.47,8.42,8.38,8.33,8.29,8.25,8.21,8.16,8.12,8.08,8.04,8.00,7.96,7.93,7.89,7.85,7.81,7.78,7.74,7.71,7.67,7.64,7.60,7.57,7.54,7.50,7.47,7.44,7.41,7.38,7.35,7.32,7.29,7.26,7.23,7.20,7.17,7.14,7.11,7.09,7.06,7.03,7.00,6.98,6.95,6.93,6.90,6.87,6.85,6.83,6.80,6.78,6.75,6.73,6.71,6.68,6.66,6.64,6.61,6.59,6.57,6.55,6.53,6.50,6.48,6.46,6.44,6.42,6.40,6.38,6.36,6.34,6.32,6.30,6.28,6.26,6.24,6.23,6.21,6.19,6.17,6.15,6.13,6.12,6.10,6.08,6.07,6.05,6.03,6.01,6.00,5.98,5.96,5.95,5.93,5.92,5.90,5.88,5.87,5.85,5.84,5.82,5.81,5.79,5.78,5.76,5.75,5.73,5.72,5.71,5.69,5.68,5.66,5.65,5.64,5.62,5.61,5.60,5.58,5.57,5.56,5.54,5.53,5.52,5.51,5.49,5.48,5.47,5.46,5.44,5.43,5.42,5.41,5.40,5.38,5.37,5.36,5.35,5.34,5.33,5.31,5.30,5.29,5.28,5.27,5.26,5.25,5.24,5.23,5.22,5.21,5.19,5.18,5.17,5.16,5.15,5.14,5.13,5.12,5.11,5.10,5.09,5.08,5.07,5.06,5.05,5.04,5.04,5.03,5.02,5.01,5.00,4.99,4.98,4.97,4.96,4.95,4.94,4.93,4.93,4.92,4.91,4.90,4.89,4.88,4.87,4.87,4.86,4.85,4.84,4.83,4.82,4.82,4.81,4.80,4.79,4.78,4.78,4.77,4.76,4.75,4.74,4.74,4.73,4.72,4.71,4.71,4.70,4.69,4.68,4.68,4.67,4.66,4.65,4.65,4.64,4.63,4.63,4.62,4.61,4.61,4.60,4.59,4.58,4.58,4.57,4.56,4.56,4.55,4.54,4.54,4.53,4.52,4.52,4.51,4.50,4.50,4.49,4.49,4.48,4.47,4.47,4.46,4.45,4.45,4.44,4.44,4.43,4.42,4.42,4.41,4.40,4.40,4.39,4.39,4.38,4.38,4.37,4.36,4.36,4.35,4.35,4.34,4.34,4.33,4.32,4.32,4.31,4.31,4.30,4.30,4.29,4.29,4.28,4.27,4.27,4.26,4.26,4.25,4.25,4.24,4.24,4.23,4.23,4.22,4.22,4.21,4.21,4.20,4.20,4.19,4.19,4.18,4.18,4.17,4.17,4.16,4.16,4.15,4.15,4.14,4.14,4.13,4.13,4.12,4.12,4.11,4.11,4.10,4.10,4.10,4.09,4.09,4.08,4.08,4.07,4.07,4.06,4.06,4.05,4.05,4.05,4.04,4.04,4.03,4.03,4.02,4.02,4.01,4.01,4.01,4.00,4.00,3.99,3.99,3.99,3.98,3.98,3.97,3.97,3.96,3.96,3.96,3.95,3.95,3.94,3.94,3.94,3.93,3.93,3.92,3.92,3.92,3.91,3.91,3.90,3.90,3.90,3.89,3.89,3.89,3.88,3.88,3.87,3.87,3.87,3.86,3.86,3.86,3.85,3.85,3.84,3.84,3.84,3.83,3.83,3.83,3.82,3.82,3.82,3.81,3.81,3.81,3.80,3.80,3.79,3.79,3.79,3.78,3.78,3.78,3.77,3.77,3.77,3.76,3.76,3.76,3.75,3.75,3.75,3.74,3.74,3.74,3.73,3.73,3.73,3.72,3.72,3.72,3.71,3.71,3.71,3.70,3.70,3.70,3.70,3.69,3.69,3.69,3.68,3.68,3.68,3.67,3.67,3.67,3.66,3.66,3.66,3.65,3.65,3.65,3.65,3.64,3.64,3.64,3.63,3.63,3.63,3.63,3.62,3.62,3.62,3.61,3.61,3.61,3.60,3.60,3.60,3.60,3.59,3.59,3.59,3.58,3.58,3.58,3.58,3.57,3.57,3.57,3.57,3.56,3.56,3.56,3.55,3.55,3.55,3.55,3.54,3.54,3.54,3.54,3.53,3.53,3.53,3.53,3.52,3.52,3.52,3.51,3.51,3.51,3.51,3.50,3.50,3.50,3.50,3.49,3.49,3.49,3.49,3.48,3.48,3.48,3.48,3.47,3.47,3.47,3.47,3.46,3.46,3.46,3.46,3.45,3.45,3.45,3.45,3.44,3.44,3.44,3.44,3.43,3.43,3.43,3.43,3.43,3.42,3.42,3.42,3.42,3.41,3.41,3.41,3.41,3.40,3.40,3.40,3.40,3.40,3.39,3.39,3.39,3.39,3.38,3.38,3.38,3.38,3.38,3.37,3.37,3.37,3.37,3.36,3.36,3.36,3.36,3.36,3.35,3.35,3.35,3.35,3.34,3.34,3.34,3.34,3.34,3.33,3.33,3.33,3.33,3.33,3.32,3.32,3.32,3.32,3.32,3.31,3.31,3.31,3.31,3.30,3.30,3.30,3.30,3.30,3.29,3.29,3.29,3.29,3.29,3.28,3.28,3.28,3.28,3.28,3.27,3.27,3.27,3.27,3.27,3.27,3.26,3.26,3.26,3.26,3.26,3.25,3.25,3.25,3.25,3.25,3.24,3.24,3.24,3.24,3.24,3.23,3.23,3.23,3.23,3.23,3.23,3.22,3.22,3.22,3.22,3.22,3.21,3.21,3.21,3.21,3.21,3.21,3.20,3.20,3.20,3.20,3.20,3.19,3.19,3.19,3.19,3.19,3.19,3.18,3.18,3.18,3.18,3.18,3.17,3.17,3.17,3.17,3.17,3.17,3.16,3.16,3.16,3.16,3.16,3.16,3.15,3.15,3.15,3.15,3.15,3.15,3.14,3.14,3.14,3.14,3.14,3.14,3.13,3.13,3.13,3.13,3.13,3.13,3.12,3.12,3.12,3.12,3.12,3.12,3.11,3.11,3.11,3.11,3.11,3.11,3.11,3.10,3.10,3.10,3.10,3.10,3.10,3.09,3.09,3.09,3.09,3.09,3.09,3.08,3.08,3.08,3.08,3.08,3.08,3.08,3.07,3.07,3.07,3.07,3.07,3.07,3.06,3.06,3.06,3.06,3.06,3.06,3.06,3.05,3.05,3.05,3.05,3.05,3.05,3.05,3.04,3.04,3.04,3.04,3.04,3.04,3.04,3.03,3.03,3.03,3.03,3.03,3.03,3.03,3.02,3.02,3.02,3.02,3.02,3.02,3.02,3.01,3.01,3.01,3.01,3.01,3.01,3.01,3.00,3.00,3.00,3.00,3.00,3.00,3.00,2.99,2.99,2.99,2.99,2.99,2.99,2.99,2.99,2.98,2.98,2.98,2.98,2.98,2.98,2.98,2.97,2.97,2.97,2.97,2.97,2.97,2.97,2.97,2.96,2.96,2.96,2.96,2.96,2.96,2.96,2.95,2.95,2.95,2.95,2.95,2.95,2.95,2.95,2.94,2.94,2.94,2.94,2.94,2.94,2.94,2.94,2.93,2.93,2.93,2.93,2.93,2.93,2.93,2.93,2.92,2.92,2.92,2.92,2.92,2.92,2.92,2.92,2.91,2.91,2.91,2.91,2.91,2.91,2.91,2.91,2.91,2.90,2.90,2.90,2.90,2.90,2.90,2.90,2.90,2.89,2.89,2.89,2.89,2.89,2.89,2.89,2.89,2.89,2.88,2.88,2.88,2.88,2.88,2.88,2.88,2.88,2.87,2.87,2.87,2.87,2.87,2.87,2.87,2.87,2.87,2.86,2.86,2.86,2.86,2.86,2.86,2.86,2.86,2.86,2.85,2.85,2.85,2.85,2.85,2.85,2.85,2.85,2.85,2.84,2.84,2.84,2.84,2.84,2.84,2.84,2.84,2.84,2.83,2.83,2.83,2.83,2.83,2.83,2.83,2.83,2.83,2.82,2.82,2.82,2.82,2.82,2.82,2.82,2.82,2.82,2.82,2.81,2.81,2.81,2.81,2.81,2.81,2.81,2.81,2.81,2.80,2.80,2.80,2.80,2.80,2.80,2.80,2.80,2.80,2.80,2.79,2.79,2.79,2.79,2.79,2.79,2.79,2.79,2.79,2.79,2.78,2.78,2.78,2.78,2.78,2.78,2.78,2.78,2.78,2.78,2.77,2.77,2.77,2.77,2.77,2.77,2.77,2.77,2.77,2.77,2.76,2.76,2.76,2.76,2.76,2.76,2.76,2.76,2.76,2.76,2.76,2.75,2.75,2.75,2.75,2.75,2.75,2.75,2.75,2.75,2.75,2.74,2.74,2.74,2.74,2.74,2.74,2.74,2.74,2.74,2.74,2.74,2.73,2.73,2.73,2.73,2.73,2.73,2.73,2.73,2.73,2.73,2.73,2.72,2.72,2.72,2.72,2.72,2.72,2.72,2.72,2.72,2.72,2.72,2.71,2.71,2.71,2.71,2.71,2.71,2.71,2.71,2.71,2.71,2.71,2.70,2.70,2.70,2.70,2.70,2.70,2.70,2.70,2.70,2.70,2.70,2.70,2.69,2.69,2.69,2.69,2.69,2.69,2.69,2.69,2.69,2.69,2.69,2.68,2.68,2.68,2.68,2.68,2.68,2.68,2.68,2.68,2.68,2.68,2.68,2.67,2.67,2.67,2.67,2.67,2.67,2.67,2.67,2.67,2.67,2.67,2.67,2.66,2.66,2.66,2.66,2.66,2.66,2.66,2.66,2.66,2.66,2.66,2.66,2.65,2.65,2.65,2.65,2.65,2.65,2.65,2.65,2.65,2.65,2.65,2.65,2.65,2.64,2.64,2.64,2.64,2.64,2.64,2.64,2.64,2.64,2.64,2.64,2.64,2.63,2.63,2.63,2.63,2.63,2.63,2.63,2.63,2.63,2.63,2.63,2.63,2.63,2.62,2.62,2.62,2.62,2.62,2.62,2.62,2.62,2.62,2.62,2.62,2.62,2.62,2.61,2.61,2.61,2.61,2.61,2.61,2.61,2.61,2.61,2.61,2.61,2.61,2.61,2.61,2.60,2.60,2.60,2.60,2.60,2.60,2.60,2.60,2.60,2.60,2.60,2.60,2.60,2.59,2.59,2.59,2.59,2.59,2.59,2.59,2.59,2.59,2.59,2.59,2.59,2.59,2.59,2.58,2.58,2.58,2.58,2.58,2.58,2.58,2.58,2.58,2.58,2.58,2.58,2.58,2.58,2.57,2.57,2.57,2.57,2.57,2.57,2.57,2.57,2.57,2.57,2.57,2.57,2.57,2.57,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.56,2.55,2.55,2.55,2.55,2.55,2.55,2.55,2.55,2.55,2.55,2.55,2.55,2.55,2.55,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.54,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.53,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.52,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.51,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.49,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.48,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.47,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.46,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.45,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.44,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.43,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.42,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.41,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.40,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.39,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.38,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.37,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.36,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.35,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.34,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.33,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.32,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.31,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.30,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.29,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.28,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27,2.27
  },
  { //VERY_INVERSE
      450.00,337.50,270.00,225.00,192.86,168.75,150.00,135.00,122.73,112.50,103.85,96.43,90.00,84.38,79.41,75.00,71.05,67.50,64.29,61.36,58.70,56.25,54.00,51.92,50.00,48.21,46.55,45.00,43.55,42.19,40.91,39.71,38.57,37.50,36.49,35.53,34.62,33.75,32.93,32.14,31.40,30.68,30.00,29.35,28.72,28.13,27.55,27.00,26.47,25.96,25.47,25.00,24.55,24.11,23.68,23.28,22.88,22.50,22.13,21.77,21.43,21.09,20.77,20.45,20.15,19.85,19.57,19.29,19.01,18.75,18.49,18.24,18.00,17.76,17.53,17.31,17.09,16.88,16.67,16.46,16.27,16.07,15.88,15.70,15.52,15.34,15.17,15.00,14.84,14.67,14.52,14.36,14.21,14.06,13.92,13.78,13.64,13.50,13.37,13.24,13.11,12.98,12.86,12.74,12.62,12.50,12.39,12.27,12.16,12.05,11.95,11.84,11.74,11.64,11.54,11.44,11.34,11.25,11.16,11.07,10.98,10.89,10.80,10.71,10.63,10.55,10.47,10.38,10.31,10.23,10.15,10.07,10.00,9.93,9.85,9.78,9.71,9.64,9.57,9.51,9.44,9.38,9.31,9.25,9.18,9.12,9.06,9.00,8.94,8.88,8.82,8.77,8.71,8.65,8.60,8.54,8.49,8.44,8.39,8.33,8.28,8.23,8.18,8.13,8.08,8.04,7.99,7.94,7.89,7.85,7.80,7.76,7.71,7.67,7.63,7.58,7.54,7.50,7.46,7.42,7.38,7.34,7.30,7.26,7.22,7.18,7.14,7.11,7.07,7.03,6.99,6.96,6.92,6.89,6.85,6.82,6.78,6.75,6.72,6.68,6.65,6.62,6.59,6.55,6.52,6.49,6.46,6.43,6.40,6.37,6.34,6.31,6.28,6.25,6.22,6.19,6.16,6.14,6.11,6.08,6.05,6.03,6.00,5.97,5.95,5.92,5.90,5.87,5.84,5.82,5.79,5.77,5.74,5.72,5.70,5.67,5.65,5.63,5.60,5.58,5.56,5.53,5.51,5.49,5.47,5.44,5.42,5.40,5.38,5.36,5.34,5.31,5.29,5.27,5.25,5.23,5.21,5.19,5.17,5.15,5.13,5.11,5.09,5.08,5.06,5.04,5.02,5.00,4.98,4.96,4.95,4.93,4.91,4.89,4.87,4.86,4.84,4.82,4.80,4.79,4.77,4.75,4.74,4.72,4.70,4.69,4.67,4.66,4.64,4.62,4.61,4.59,4.58,4.56,4.55,4.53,4.52,4.50,4.49,4.47,4.46,4.44,4.43,4.41,4.40,4.38,4.37,4.35,4.34,4.33,4.31,4.30,4.29,4.27,4.26,4.25,4.23,4.22,4.21,4.19,4.18,4.17,4.15,4.14,4.13,4.12,4.10,4.09,4.08,4.07,4.05,4.04,4.03,4.02,4.01,3.99,3.98,3.97,3.96,3.95,3.94,3.92,3.91,3.90,3.89,3.88,3.87,3.86,3.85,3.84,3.82,3.81,3.80,3.79,3.78,3.77,3.76,3.75,3.74,3.73,3.72,3.71,3.70,3.69,3.68,3.67,3.66,3.65,3.64,3.63,3.62,3.61,3.60,3.59,3.58,3.57,3.56,3.55,3.54,3.53,3.52,3.52,3.51,3.50,3.49,3.48,3.47,3.46,3.45,3.44,3.44,3.43,3.42,3.41,3.40,3.39,3.38,3.38,3.37,3.36,3.35,3.34,3.33,3.33,3.32,3.31,3.30,3.29,3.28,3.28,3.27,3.26,3.25,3.25,3.24,3.23,3.22,3.21,3.21,3.20,3.19,3.18,3.18,3.17,3.16,3.15,3.15,3.14,3.13,3.13,3.12,3.11,3.10,3.10,3.09,3.08,3.08,3.07,3.06,3.05,3.05,3.04,3.03,3.03,3.02,3.01,3.01,3.00,2.99,2.99,2.98,2.97,2.97,2.96,2.95,2.95,2.94,2.93,2.93,2.92,2.92,2.91,2.90,2.90,2.89,2.88,2.88,2.87,2.87,2.86,2.85,2.85,2.84,2.84,2.83,2.82,2.82,2.81,2.81,2.80,2.80,2.79,2.78,2.78,2.77,2.77,2.76,2.76,2.75,2.74,2.74,2.73,2.73,2.72,2.72,2.71,2.71,2.70,2.69,2.69,2.68,2.68,2.67,2.67,2.66,2.66,2.65,2.65,2.64,2.64,2.63,2.63,2.62,2.62,2.61,2.61,2.60,2.60,2.59,2.59,2.58,2.58,2.57,2.57,2.56,2.56,2.55,2.55,2.54,2.54,2.53,2.53,2.52,2.52,2.51,2.51,2.50,2.50,2.50,2.49,2.49,2.48,2.48,2.47,2.47,2.46,2.46,2.45,2.45,2.45,2.44,2.44,2.43,2.43,2.42,2.42,2.42,2.41,2.41,2.40,2.40,2.39,2.39,2.39,2.38,2.38,2.37,2.37,2.36,2.36,2.36,2.35,2.35,2.34,2.34,2.34,2.33,2.33,2.32,2.32,2.32,2.31,2.31,2.30,2.30,2.30,2.29,2.29,2.28,2.28,2.28,2.27,2.27,2.27,2.26,2.26,2.25,2.25,2.25,2.24,2.24,2.24,2.23,2.23,2.22,2.22,2.22,2.21,2.21,2.21,2.20,2.20,2.20,2.19,2.19,2.18,2.18,2.18,2.17,2.17,2.17,2.16,2.16,2.16,2.15,2.15,2.15,2.14,2.14,2.14,2.13,2.13,2.13,2.12,2.12,2.12,2.11,2.11,2.11,2.10,2.10,2.10,2.09,2.09,2.09,2.08,2.08,2.08,2.07,2.07,2.07,2.06,2.06,2.06,2.05,2.05,2.05,2.05,2.04,2.04,2.04,2.03,2.03,2.03,2.02,2.02,2.02,2.01,2.01,2.01,2.01,2.00,2.00,2.00,1.99,1.99,1.99,1.99,1.98,1.98,1.98,1.97,1.97,1.97,1.97,1.96,1.96,1.96,1.95,1.95,1.95,1.95,1.94,1.94,1.94,1.93,1.93,1.93,1.93,1.92,1.92,1.92,1.91,1.91,1.91,1.91,1.90,1.90,1.90,1.90,1.89,1.89,1.89,1.89,1.88,1.88,1.88,1.88,1.87,1.87,1.87,1.86,1.86,1.86,1.86,1.85,1.85,1.85,1.85,1.84,1.84,1.84,1.84,1.83,1.83,1.83,1.83,1.82,1.82,1.82,1.82,1.81,1.81,1.81,1.81,1.80,1.80,1.80,1.80,1.80,1.79,1.79,1.79,1.79,1.78,1.78,1.78,1.78,1.77,1.77,1.77,1.77,1.76,1.76,1.76,1.76,1.76,1.75,1.75,1.75,1.75,1.74,1.74,1.74,1.74,1.74,1.73,1.73,1.73,1.73,1.72,1.72,1.72,1.72,1.72,1.71,1.71,1.71,1.71,1.70,1.70,1.70,1.70,1.70,1.69,1.69,1.69,1.69,1.69,1.68,1.68,1.68,1.68,1.67,1.67,1.67,1.67,1.67,1.66,1.66,1.66,1.66,1.66,1.65,1.65,1.65,1.65,1.65,1.64,1.64,1.64,1.64,1.64,1.63,1.63,1.63,1.63,1.63,1.62,1.62,1.62,1.62,1.62,1.61,1.61,1.61,1.61,1.61,1.61,1.60,1.60,1.60,1.60,1.60,1.59,1.59,1.59,1.59,1.59,1.58,1.58,1.58,1.58,1.58,1.58,1.57,1.57,1.57,1.57,1.57,1.56,1.56,1.56,1.56,1.56,1.56,1.55,1.55,1.55,1.55,1.55,1.54,1.54,1.54,1.54,1.54,1.54,1.53,1.53,1.53,1.53,1.53,1.53,1.52,1.52,1.52,1.52,1.52,1.52,1.51,1.51,1.51,1.51,1.51,1.51,1.50,1.50,1.50,1.50,1.50,1.50,1.49,1.49,1.49,1.49,1.49,1.49,1.48,1.48,1.48,1.48,1.48,1.48,1.47,1.47,1.47,1.47,1.47,1.47,1.46,1.46,1.46,1.46,1.46,1.46,1.45,1.45,1.45,1.45,1.45,1.45,1.45,1.44,1.44,1.44,1.44,1.44,1.44,1.43,1.43,1.43,1.43,1.43,1.43,1.43,1.42,1.42,1.42,1.42,1.42,1.42,1.42,1.41,1.41,1.41,1.41,1.41,1.41,1.40,1.40,1.40,1.40,1.40,1.40,1.40,1.39,1.39,1.39,1.39,1.39,1.39,1.39,1.38,1.38,1.38,1.38,1.38,1.38,1.38,1.37,1.37,1.37,1.37,1.37,1.37,1.37,1.37,1.36,1.36,1.36,1.36,1.36,1.36,1.36,1.35,1.35,1.35,1.35,1.35,1.35,1.35,1.34,1.34,1.34,1.34,1.34,1.34,1.34,1.34,1.33,1.33,1.33,1.33,1.33,1.33,1.33,1.32,1.32,1.32,1.32,1.32,1.32,1.32,1.32,1.31,1.31,1.31,1.31,1.31,1.31,1.31,1.31,1.30,1.30,1.30,1.30,1.30,1.30,1.30,1.30,1.29,1.29,1.29,1.29,1.29,1.29,1.29,1.29,1.28,1.28,1.28,1.28,1.28,1.28,1.28,1.28,1.27,1.27,1.27,1.27,1.27,1.27,1.27,1.27,1.27,1.26,1.26,1.26,1.26,1.26,1.26,1.26,1.26,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.24,1.24,1.24,1.24,1.24,1.24,1.24,1.24,1.24,1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.23,1.22,1.22,1.22,1.22,1.22,1.22,1.22,1.22,1.22,1.21,1.21,1.21,1.21,1.21,1.21,1.21,1.21,1.21,1.20,1.20,1.20,1.20,1.20,1.20,1.20,1.20,1.20,1.19,1.19,1.19,1.19,1.19,1.19,1.19,1.19,1.19,1.19,1.18,1.18,1.18,1.18,1.18,1.18,1.18,1.18,1.18,1.17,1.17,1.17,1.17,1.17,1.17,1.17,1.17,1.17,1.17,1.16,1.16,1.16,1.16,1.16,1.16,1.16,1.16,1.16,1.16,1.15,1.15,1.15,1.15,1.15,1.15,1.15,1.15,1.15,1.15,1.15,1.14,1.14,1.14,1.14,1.14,1.14,1.14,1.14,1.14,1.14,1.13,1.13,1.13,1.13,1.13,1.13,1.13,1.13,1.13,1.13,1.13,1.12,1.12,1.12,1.12,1.12,1.12,1.12,1.12,1.12,1.12,1.11,1.11,1.11,1.11,1.11,1.11,1.11,1.11,1.11,1.11,1.11,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.09,1.09,1.09,1.09,1.09,1.09,1.09,1.09,1.09,1.09,1.09,1.09,1.08,1.08,1.08,1.08,1.08,1.08,1.08,1.08,1.08,1.08,1.08,1.07,1.07,1.07,1.07,1.07,1.07,1.07,1.07,1.07,1.07,1.07,1.07,1.06,1.06,1.06,1.06,1.06,1.06,1.06,1.06,1.06,1.06,1.06,1.06,1.05,1.05,1.05,1.05,1.05,1.05,1.05,1.05,1.05,1.05,1.05,1.05,1.04,1.04,1.04,1.04,1.04,1.04,1.04,1.04,1.04,1.04,1.04,1.04,1.04,1.03,1.03,1.03,1.03,1.03,1.03,1.03,1.03,1.03,1.03,1.03,1.03,1.03,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.01,1.01,1.01,1.01,1.01,1.01,1.01,1.01,1.01,1.01,1.01,1.01,1.01,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,0.99,0.99,0.99,0.99,0.99,0.99,0.99,0.99,0.99,0.99,0.99,0.99,0.99,0.99,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.98,0.97,0.97,0.97,0.97,0.97,0.97,0.97,0.97,0.97,0.97,0.97,0.97,0.97,0.97,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.96,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.94,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.92,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.91,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.90,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.89,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.88,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.87,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.86,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.84,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.83,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.82,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.81,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.79,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.78,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.71,0.71,0.71,0.71,0.71,0.71,0.71,0.71,0.71,0.71,0.71,0.71
  },
  { //EXTREMELY_INVERSE
      1313.63,980.39,780.49,647.25,552.10,480.77,425.31,380.95,344.68,314.47,288.91,267.02,248.06,231.48,216.86,203.87,192.26,181.82,172.38,163.80,155.98,148.81,142.22,136.15,130.53,125.31,120.46,115.94,111.72,107.76,104.04,100.55,97.26,94.16,91.23,88.46,85.83,83.33,80.96,78.71,76.56,74.52,72.56,70.70,68.91,67.20,65.57,64.00,62.50,61.05,59.66,58.33,57.04,55.80,54.61,53.46,52.35,51.28,50.25,49.25,48.28,47.35,46.44,45.57,44.72,43.90,43.10,42.33,41.58,40.85,40.14,39.46,38.79,38.14,37.51,36.89,36.30,35.71,35.15,34.60,34.06,33.53,33.02,32.53,32.04,31.57,31.10,30.65,30.21,29.78,29.36,28.95,28.55,28.15,27.77,27.39,27.03,26.67,26.31,25.97,25.63,25.30,24.98,24.66,24.35,24.05,23.75,23.46,23.17,22.89,22.62,22.35,22.08,21.82,21.57,21.32,21.07,20.83,20.60,20.36,20.14,19.91,19.69,19.48,19.26,19.05,18.85,18.65,18.45,18.25,18.06,17.87,17.69,17.51,17.33,17.15,16.98,16.81,16.64,16.47,16.31,16.15,15.99,15.84,15.68,15.53,15.38,15.24,15.09,14.95,14.81,14.67,14.54,14.41,14.27,14.14,14.02,13.89,13.76,13.64,13.52,13.40,13.28,13.17,13.05,12.94,12.83,12.72,12.61,12.50,12.40,12.29,12.19,12.09,11.99,11.89,11.79,11.70,11.60,11.51,11.41,11.32,11.23,11.14,11.05,10.97,10.88,10.80,10.71,10.63,10.55,10.47,10.39,10.31,10.23,10.15,10.08,10.00,9.93,9.85,9.78,9.71,9.64,9.57,9.50,9.43,9.36,9.29,9.22,9.16,9.09,9.03,8.97,8.90,8.84,8.78,8.72,8.66,8.60,8.54,8.48,8.42,8.37,8.31,8.25,8.20,8.14,8.09,8.04,7.98,7.93,7.88,7.83,7.77,7.72,7.67,7.62,7.58,7.53,7.48,7.43,7.38,7.34,7.29,7.25,7.20,7.16,7.11,7.07,7.02,6.98,6.94,6.90,6.85,6.81,6.77,6.73,6.69,6.65,6.61,6.57,6.53,6.49,6.45,6.42,6.38,6.34,6.30,6.27,6.23,6.20,6.16,6.12,6.09,6.05,6.02,5.99,5.95,5.92,5.89,5.85,5.82,5.79,5.76,5.72,5.69,5.66,5.63,5.60,5.57,5.54,5.51,5.48,5.45,5.42,5.39,5.36,5.33,5.31,5.28,5.25,5.22,5.19,5.17,5.14,5.11,5.09,5.06,5.03,5.01,4.98,4.96,4.93,4.91,4.88,4.86,4.83,4.81,4.78,4.76,4.74,4.71,4.69,4.67,4.64,4.62,4.60,4.57,4.55,4.53,4.51,4.49,4.46,4.44,4.42,4.40,4.38,4.36,4.34,4.32,4.30,4.27,4.25,4.23,4.21,4.19,4.18,4.16,4.14,4.12,4.10,4.08,4.06,4.04,4.02,4.00,3.99,3.97,3.95,3.93,3.91,3.90,3.88,3.86,3.84,3.83,3.81,3.79,3.78,3.76,3.74,3.73,3.71,3.69,3.68,3.66,3.65,3.63,3.61,3.60,3.58,3.57,3.55,3.54,3.52,3.51,3.49,3.48,3.46,3.45,3.43,3.42,3.40,3.39,3.38,3.36,3.35,3.33,3.32,3.31,3.29,3.28,3.26,3.25,3.24,3.22,3.21,3.20,3.19,3.17,3.16,3.15,3.13,3.12,3.11,3.10,3.08,3.07,3.06,3.05,3.04,3.02,3.01,3.00,2.99,2.98,2.96,2.95,2.94,2.93,2.92,2.91,2.90,2.89,2.87,2.86,2.85,2.84,2.83,2.82,2.81,2.80,2.79,2.78,2.77,2.76,2.75,2.74,2.72,2.71,2.70,2.69,2.68,2.67,2.66,2.65,2.64,2.64,2.63,2.62,2.61,2.60,2.59,2.58,2.57,2.56,2.55,2.54,2.53,2.52,2.51,2.50,2.50,2.49,2.48,2.47,2.46,2.45,2.44,2.43,2.43,2.42,2.41,2.40,2.39,2.38,2.37,2.37,2.36,2.35,2.34,2.33,2.33,2.32,2.31,2.30,2.29,2.29,2.28,2.27,2.26,2.25,2.25,2.24,2.23,2.22,2.22,2.21,2.20,2.19,2.19,2.18,2.17,2.17,2.16,2.15,2.14,2.14,2.13,2.12,2.12,2.11,2.10,2.09,2.09,2.08,2.07,2.07,2.06,2.05,2.05,2.04,2.03,2.03,2.02,2.01,2.01,2.00,2.00,1.99,1.98,1.98,1.97,1.96,1.96,1.95,1.95,1.94,1.93,1.93,1.92,1.92,1.91,1.90,1.90,1.89,1.89,1.88,1.87,1.87,1.86,1.86,1.85,1.85,1.84,1.83,1.83,1.82,1.82,1.81,1.81,1.80,1.80,1.79,1.78,1.78,1.77,1.77,1.76,1.76,1.75,1.75,1.74,1.74,1.73,1.73,1.72,1.72,1.71,1.71,1.70,1.70,1.69,1.69,1.68,1.68,1.67,1.67,1.66,1.66,1.65,1.65,1.64,1.64,1.63,1.63,1.62,1.62,1.61,1.61,1.61,1.60,1.60,1.59,1.59,1.58,1.58,1.57,1.57,1.56,1.56,1.56,1.55,1.55,1.54,1.54,1.53,1.53,1.53,1.52,1.52,1.51,1.51,1.50,1.50,1.50,1.49,1.49,1.48,1.48,1.48,1.47,1.47,1.46,1.46,1.46,1.45,1.45,1.44,1.44,1.44,1.43,1.43,1.42,1.42,1.42,1.41,1.41,1.41,1.40,1.40,1.39,1.39,1.39,1.38,1.38,1.38,1.37,1.37,1.37,1.36,1.36,1.35,1.35,1.35,1.34,1.34,1.34,1.33,1.33,1.33,1.32,1.32,1.32,1.31,1.31,1.31,1.30,1.30,1.30,1.29,1.29,1.29,1.28,1.28,1.28,1.27,1.27,1.27,1.26,1.26,1.26,1.25,1.25,1.25,1.24,1.24,1.24,1.24,1.23,1.23,1.23,1.22,1.22,1.22,1.21,1.21,1.21,1.20,1.20,1.20,1.20,1.19,1.19,1.19,1.18,1.18,1.18,1.18,1.17,1.17,1.17,1.16,1.16,1.16,1.16,1.15,1.15,1.15,1.14,1.14,1.14,1.14,1.13,1.13,1.13,1.13,1.12,1.12,1.12,1.11,1.11,1.11,1.11,1.10,1.10,1.10,1.10,1.09,1.09,1.09,1.09,1.08,1.08,1.08,1.08,1.07,1.07,1.07,1.07,1.06,1.06,1.06,1.06,1.05,1.05,1.05,1.05,1.04,1.04,1.04,1.04,1.03,1.03,1.03,1.03,1.03,1.02,1.02,1.02,1.02,1.01,1.01,1.01,1.01,1.00,1.00,1.00,1.00,1.00,0.99,0.99,0.99,0.99,0.98,0.98,0.98,0.98,0.98,0.97,0.97,0.97,0.97,0.96,0.96,0.96,0.96,0.96,0.95,0.95,0.95,0.95,0.95,0.94,0.94,0.94,0.94,0.94,0.93,0.93,0.93,0.93,0.93,0.92,0.92,0.92,0.92,0.92,0.91,0.91,0.91,0.91,0.91,0.90,0.90,0.90,0.90,0.90,0.89,0.89,0.89,0.89,0.89,0.89,0.88,0.88,0.88,0.88,0.88,0.87,0.87,0.87,0.87,0.87,0.86,0.86,0.86,0.86,0.86,0.86,0.85,0.85,0.85,0.85,0.85,0.85,0.84,0.84,0.84,0.84,0.84,0.83,0.83,0.83,0.83,0.83,0.83,0.82,0.82,0.82,0.82,0.82,0.82,0.81,0.81,0.81,0.81,0.81,0.81,0.80,0.80,0.80,0.80,0.80,0.80,0.80,0.79,0.79,0.79,0.79,0.79,0.79,0.78,0.78,0.78,0.78,0.78,0.78,0.77,0.77,0.77,0.77,0.77,0.77,0.77,0.76,0.76,0.76,0.76,0.76,0.76,0.76,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.74,0.74,0.74,0.74,0.74,0.74,0.74,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.72,0.72,0.72,0.72,0.72,0.72,0.72,0.71,0.71,0.71,0.71,0.71,0.71,0.71,0.70,0.70,0.70,0.70,0.70,0.70,0.70,0.70,0.69,0.69,0.69,0.69,0.69,0.69,0.69,0.69,0.68,0.68,0.68,0.68,0.68,0.68,0.68,0.68,0.67,0.67,0.67,0.67,0.67,0.67,0.67,0.67,0.66,0.66,0.66,0.66,0.66,0.66,0.66,0.66,0.65,0.65,0.65,0.65,0.65,0.65,0.65,0.65,0.65,0.64,0.64,0.64,0.64,0.64,0.64,0.64,0.64,0.63,0.63,0.63,0.63,0.63,0.63,0.63,0.63,0.63,0.62,0.62,0.62,0.62,0.62,0.62,0.62,0.62,0.62,0.61,0.61,0.61,0.61,0.61,0.61,0.61,0.61,0.61,0.61,0.60,0.60,0.60,0.60,0.60,0.60,0.60,0.60,0.60,0.59,0.59,0.59,0.59,0.59,0.59,0.59,0.59,0.59,0.59,0.58,0.58,0.58,0.58,0.58,0.58,0.58,0.58,0.58,0.58,0.57,0.57,0.57,0.57,0.57,0.57,0.57,0.57,0.57,0.57,0.57,0.56,0.56,0.56,0.56,0.56,0.56,0.56,0.56,0.56,0.56,0.55,0.55,0.55,0.55,0.55,0.55,0.55,0.55,0.55,0.55,0.55,0.54,0.54,0.54,0.54,0.54,0.54,0.54,0.54,0.54,0.54,0.54,0.53,0.53,0.53,0.53,0.53,0.53,0.53,0.53,0.53,0.53,0.53,0.53,0.52,0.52,0.52,0.52,0.52,0.52,0.52,0.52,0.52,0.52,0.52,0.52,0.51,0.51,0.51,0.51,0.51,0.51,0.51,0.51,0.51,0.51,0.51,0.51,0.50,0.50,0.50,0.50,0.50,0.50,0.50,0.50,0.50,0.50,0.50,0.50,0.50,0.49,0.49,0.49,0.49,0.49,0.49,0.49,0.49,0.49,0.49,0.49,0.49,0.49,0.48,0.48,0.48,0.48,0.48,0.48,0.48,0.48,0.48,0.48,0.48,0.48,0.48,0.47,0.47,0.47,0.47,0.47,0.47,0.47,0.47,0.47,0.47,0.47,0.47,0.47,0.47,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.45,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.44,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.42,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.39,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.38,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.37,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.36,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.35,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.34,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.33,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.32,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.31,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.30,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.29,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.28,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.26,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.22,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.21,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20
  }
};

bool Sample_Init(void) {
  //set LED high?
  return true;
}

float Voltage_RMS(const TSample* const sample) {
  float vRMS;
  uint8_t i = 0;
  uint16_t data = 0;
  for(i=0; i<16;i++) {
    data = data + sample->VoltageSamples[i];
  }
  return vRMS;
  // raw value from analog get
  //convert to voltage
 // VRMS from 16 samples of voltages
}

float Current_RMS(const TSample* const sample) {
  float tempiRMS;
  tempiRMS = Sample->vRMS/0.350;
  return tempiRMS;
}

void TripTimeCalculation(float iRMS) {
  uint16_t index;
  uint16_t time;
  index = (((iRMS)*100)-103);
  //time = TripTime[sample->IDMTCharacteristic][index];
  //Time to Trip = k/(iRMS^a-1)
  //if iRMS < 1.03 -> time = infinite
  //PIT to output 5v after a certain amount of time
}

/*A true RMS meter works by taking the square of the instantaneous value of the input voltage or current,
*  averaging this value over time, and then displaying the square root of this average.
*  https://www.tester.co.uk/blog/electrical/multimeter-jargon-busters-true-rms*/

//One PIT firing  16 times per 50Hz Cycle to store current instanteous voltage at that moment
//Second PIT firing for the trip after dedicated time period
//3 channel - 3 structs referencing each channel and its values?
//counter++ =20ms -> trip time =
