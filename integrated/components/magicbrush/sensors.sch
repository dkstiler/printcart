EESchema Schematic File Version 4
LIBS:magicbrush-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L jeroens_components:VL53L0X U6
U 1 1 5BBC954E
P 9100 1700
F 0 "U6" H 8800 2300 50  0000 L CNN
F 1 "VL53L0X" H 8750 2150 50  0000 L CNN
F 2 "JeroensFootprints:VL53LxX" H 8700 2150 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/vl53l0x.pdf" H 9000 600 50  0001 C CNN
	1    9100 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C16
U 1 1 5BBC963B
P 9700 1700
F 0 "C16" H 9815 1746 50  0000 L CNN
F 1 "100n" H 9815 1655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9738 1550 50  0001 C CNN
F 3 "~" H 9700 1700 50  0001 C CNN
	1    9700 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C17
U 1 1 5BBC9686
P 10150 1700
F 0 "C17" H 10265 1746 50  0000 L CNN
F 1 "4u7" H 10265 1655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 10188 1550 50  0001 C CNN
F 3 "~" H 10150 1700 50  0001 C CNN
	1    10150 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 1000 10150 1550
Wire Wire Line
	9700 1550 9700 1000
Connection ~ 9700 1000
Wire Wire Line
	9700 1000 10150 1000
Wire Wire Line
	10150 2350 10150 1850
Wire Wire Line
	9700 1850 9700 2350
Connection ~ 9700 2350
Wire Wire Line
	9700 2350 10150 2350
NoConn ~ 8500 1500
$Comp
L power:VCC #PWR0122
U 1 1 5BBC98DE
P 9700 1000
F 0 "#PWR0122" H 9700 850 50  0001 C CNN
F 1 "VCC" H 9717 1173 50  0000 C CNN
F 2 "" H 9700 1000 50  0001 C CNN
F 3 "" H 9700 1000 50  0001 C CNN
	1    9700 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5BBC9948
P 9700 2350
F 0 "#PWR0123" H 9700 2100 50  0001 C CNN
F 1 "GND" H 9705 2177 50  0000 C CNN
F 2 "" H 9700 2350 50  0001 C CNN
F 3 "" H 9700 2350 50  0001 C CNN
	1    9700 2350
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:VL53L0X U9
U 1 1 5BBC9AF9
P 9100 3600
F 0 "U9" H 8800 4200 50  0000 L CNN
F 1 "VL53L0X" H 8750 4050 50  0000 L CNN
F 2 "JeroensFootprints:VL53LxX" H 8700 4050 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/vl53l0x.pdf" H 9000 2500 50  0001 C CNN
	1    9100 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C20
U 1 1 5BBC9B00
P 9700 3600
F 0 "C20" H 9815 3646 50  0000 L CNN
F 1 "100n" H 9815 3555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9738 3450 50  0001 C CNN
F 3 "~" H 9700 3600 50  0001 C CNN
	1    9700 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C21
U 1 1 5BBC9B07
P 10150 3600
F 0 "C21" H 10265 3646 50  0000 L CNN
F 1 "4u7" H 10265 3555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 10188 3450 50  0001 C CNN
F 3 "~" H 10150 3600 50  0001 C CNN
	1    10150 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 2900 9450 2900
Wire Wire Line
	10150 2900 10150 3450
Connection ~ 9450 2900
Wire Wire Line
	9450 2900 9700 2900
Wire Wire Line
	9700 3450 9700 2900
Connection ~ 9700 2900
Wire Wire Line
	9700 2900 10150 2900
Wire Wire Line
	8850 4250 9000 4250
Wire Wire Line
	10150 4250 10150 3750
Connection ~ 9000 4250
Wire Wire Line
	9000 4250 9150 4250
Connection ~ 9150 4250
Wire Wire Line
	9150 4250 9300 4250
Connection ~ 9300 4250
Wire Wire Line
	9300 4250 9450 4250
Connection ~ 9450 4250
Wire Wire Line
	9450 4250 9700 4250
Wire Wire Line
	9700 3750 9700 4250
Connection ~ 9700 4250
Wire Wire Line
	9700 4250 10150 4250
NoConn ~ 8500 3400
$Comp
L power:VCC #PWR0124
U 1 1 5BBC9B23
P 9700 2900
F 0 "#PWR0124" H 9700 2750 50  0001 C CNN
F 1 "VCC" H 9717 3073 50  0000 C CNN
F 2 "" H 9700 2900 50  0001 C CNN
F 3 "" H 9700 2900 50  0001 C CNN
	1    9700 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0125
U 1 1 5BBC9B29
P 9700 4250
F 0 "#PWR0125" H 9700 4000 50  0001 C CNN
F 1 "GND" H 9705 4077 50  0000 C CNN
F 2 "" H 9700 4250 50  0001 C CNN
F 3 "" H 9700 4250 50  0001 C CNN
	1    9700 4250
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:VL53L0X U11
U 1 1 5BBC9E6B
P 9100 5450
F 0 "U11" H 8800 6050 50  0000 L CNN
F 1 "VL53L0X" H 8750 5900 50  0000 L CNN
F 2 "JeroensFootprints:VL53LxX" H 8700 5900 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/vl53l0x.pdf" H 9000 4350 50  0001 C CNN
	1    9100 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C25
U 1 1 5BBC9E72
P 9700 5450
F 0 "C25" H 9815 5496 50  0000 L CNN
F 1 "100n" H 9815 5405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9738 5300 50  0001 C CNN
F 3 "~" H 9700 5450 50  0001 C CNN
	1    9700 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C26
U 1 1 5BBC9E79
P 10150 5450
F 0 "C26" H 10265 5496 50  0000 L CNN
F 1 "4u7" H 10265 5405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 10188 5300 50  0001 C CNN
F 3 "~" H 10150 5450 50  0001 C CNN
	1    10150 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 4750 9450 4750
Wire Wire Line
	10150 4750 10150 5300
Connection ~ 9450 4750
Wire Wire Line
	9450 4750 9700 4750
Wire Wire Line
	9700 5300 9700 4750
Connection ~ 9700 4750
Wire Wire Line
	9700 4750 10150 4750
Wire Wire Line
	8850 6100 9000 6100
Wire Wire Line
	10150 6100 10150 5600
Connection ~ 9000 6100
Wire Wire Line
	9000 6100 9150 6100
Connection ~ 9150 6100
Wire Wire Line
	9150 6100 9300 6100
Connection ~ 9300 6100
Wire Wire Line
	9300 6100 9450 6100
Connection ~ 9450 6100
Wire Wire Line
	9450 6100 9700 6100
Wire Wire Line
	9700 5600 9700 6100
Connection ~ 9700 6100
Wire Wire Line
	9700 6100 10150 6100
NoConn ~ 8500 5250
$Comp
L power:VCC #PWR0126
U 1 1 5BBC9E95
P 9700 4750
F 0 "#PWR0126" H 9700 4600 50  0001 C CNN
F 1 "VCC" H 9717 4923 50  0000 C CNN
F 2 "" H 9700 4750 50  0001 C CNN
F 3 "" H 9700 4750 50  0001 C CNN
	1    9700 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0127
U 1 1 5BBC9E9B
P 9700 6100
F 0 "#PWR0127" H 9700 5850 50  0001 C CNN
F 1 "GND" H 9705 5927 50  0000 C CNN
F 2 "" H 9700 6100 50  0001 C CNN
F 3 "" H 9700 6100 50  0001 C CNN
	1    9700 6100
	1    0    0    -1  
$EndComp
$Comp
L Interface_Expansion:TCA9534 U8
U 1 1 5BBCB093
P 6350 2900
F 0 "U8" H 6550 3400 50  0000 C CNN
F 1 "TCA9534" H 6300 3600 50  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 7300 2350 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tca9534.pdf" H 6450 2800 50  0001 C CNN
	1    6350 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 2300 5750 2300
Wire Wire Line
	5750 3300 5950 3300
Wire Wire Line
	5950 3200 5750 3200
Connection ~ 5750 3200
Wire Wire Line
	5750 3200 5750 3300
Wire Wire Line
	5750 3100 5950 3100
Wire Wire Line
	5750 3100 5750 3200
NoConn ~ 5950 2800
$Comp
L jeroens_components:TCS3472 U7
U 1 1 5BBD1BEC
P 4750 2750
F 0 "U7" H 4850 3000 50  0000 L CNN
F 1 "TCS3472" H 4350 3000 50  0000 L CNN
F 2 "JeroensFootprints:DFNL_FN_TCS3472" H 4450 2900 50  0001 C CNN
F 3 "https://ams.com/documents/20143/36005/TCS3472_DS000390_2-00.pdf" H 5100 1950 50  0001 C CNN
	1    4750 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C19
U 1 1 5BBD1E45
P 5500 2500
F 0 "C19" H 5600 2650 50  0000 L CNN
F 1 "100n" H 5650 2550 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5538 2350 50  0001 C CNN
F 3 "~" H 5500 2500 50  0001 C CNN
	1    5500 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C18
U 1 1 5BBE2A17
P 5200 2500
F 0 "C18" H 5250 2600 50  0000 L CNN
F 1 "100n" H 5250 2400 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5238 2350 50  0001 C CNN
F 3 "~" H 5200 2500 50  0001 C CNN
	1    5200 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2350 4750 2300
Wire Wire Line
	4750 2300 5200 2300
Connection ~ 5750 2300
$Comp
L power:VCC #PWR0128
U 1 1 5BBE4DEE
P 5750 2150
F 0 "#PWR0128" H 5750 2000 50  0001 C CNN
F 1 "VCC" H 5767 2323 50  0000 C CNN
F 2 "" H 5750 2150 50  0001 C CNN
F 3 "" H 5750 2150 50  0001 C CNN
	1    5750 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2350 5200 2300
Wire Wire Line
	5500 2350 5500 2300
Wire Wire Line
	5500 2300 5750 2300
$Comp
L power:GND #PWR0129
U 1 1 5BBF1523
P 5200 2800
F 0 "#PWR0129" H 5200 2550 50  0001 C CNN
F 1 "GND" H 5205 2627 50  0000 C CNN
F 2 "" H 5200 2800 50  0001 C CNN
F 3 "" H 5200 2800 50  0001 C CNN
	1    5200 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0130
U 1 1 5BBF156C
P 5500 2800
F 0 "#PWR0130" H 5500 2550 50  0001 C CNN
F 1 "GND" H 5505 2627 50  0000 C CNN
F 2 "" H 5500 2800 50  0001 C CNN
F 3 "" H 5500 2800 50  0001 C CNN
	1    5500 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0131
U 1 1 5BBF15B5
P 4750 3200
F 0 "#PWR0131" H 4750 2950 50  0001 C CNN
F 1 "GND" H 4755 3027 50  0000 C CNN
F 2 "" H 4750 3200 50  0001 C CNN
F 3 "" H 4750 3200 50  0001 C CNN
	1    4750 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2650 5200 2800
Wire Wire Line
	5500 2800 5500 2650
NoConn ~ 4300 2850
Text GLabel 2950 1150 0    50   Input ~ 0
I2C_SCL
Text GLabel 2950 1050 0    50   Input ~ 0
I2C_SDA
$Comp
L jeroens_components:AO3400 Q1
U 1 1 5BC0393B
P 6450 5250
F 0 "Q1" H 6555 5296 50  0000 L CNN
F 1 "AO3400" H 6555 5205 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6550 4700 50  0001 C CNN
F 3 "http://www.aosmd.com/pdfs/datasheet/ao3400.pdf" H 6550 4800 50  0001 C CNN
	1    6450 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R16
U 1 1 5BC03A1E
P 5900 5400
F 0 "R16" H 5970 5446 50  0000 L CNN
F 1 "10K" H 5970 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5830 5400 50  0001 C CNN
F 3 "~" H 5900 5400 50  0001 C CNN
	1    5900 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 5BC03A8B
P 6450 4850
F 0 "R14" H 6520 4896 50  0000 L CNN
F 1 "68R" H 6520 4805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6380 4850 50  0001 C CNN
F 3 "~" H 6450 4850 50  0001 C CNN
	1    6450 4850
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D5
U 1 1 5BC03BAA
P 6450 4450
F 0 "D5" V 6488 4332 50  0000 R CNN
F 1 "LED WH" V 6397 4332 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6450 4450 50  0001 C CNN
F 3 "~" H 6450 4450 50  0001 C CNN
	1    6450 4450
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0132
U 1 1 5BC03CD6
P 5900 5550
F 0 "#PWR0132" H 5900 5300 50  0001 C CNN
F 1 "GND" H 5905 5377 50  0000 C CNN
F 2 "" H 5900 5550 50  0001 C CNN
F 3 "" H 5900 5550 50  0001 C CNN
	1    5900 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0133
U 1 1 5BC03D3C
P 6450 5550
F 0 "#PWR0133" H 6450 5300 50  0001 C CNN
F 1 "GND" H 6455 5377 50  0000 C CNN
F 2 "" H 6450 5550 50  0001 C CNN
F 3 "" H 6450 5550 50  0001 C CNN
	1    6450 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 5450 6450 5550
Wire Wire Line
	5900 5250 6150 5250
Wire Wire Line
	6450 5050 6450 5000
Wire Wire Line
	6450 4700 6450 4600
Text GLabel 6450 4300 1    50   Input ~ 0
Vbat
Wire Wire Line
	6150 3950 6150 5250
Connection ~ 6150 5250
$Comp
L Sensor_Motion:MPU-9250 U10
U 1 1 5BC19B90
P 4050 4800
F 0 "U10" H 3600 5550 50  0000 C CNN
F 1 "MPU-9250" H 4400 5550 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24_3x3mm_P0.4mm" H 4050 3800 50  0001 C CNN
F 3 "https://store.invensense.com/datasheets/invensense/MPU9250REV1.0.pdf" H 4050 4650 50  0001 C CNN
	1    4050 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C23
U 1 1 5BC19D7D
P 2950 3800
F 0 "C23" H 3065 3846 50  0000 L CNN
F 1 "1u" H 3065 3755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2988 3650 50  0001 C CNN
F 3 "~" H 2950 3800 50  0001 C CNN
	1    2950 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C22
U 1 1 5BC19E01
P 2600 3800
F 0 "C22" H 2715 3846 50  0000 L CNN
F 1 "100n" H 2715 3755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2638 3650 50  0001 C CNN
F 3 "~" H 2600 3800 50  0001 C CNN
	1    2600 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3650 2600 3600
Wire Wire Line
	2600 3600 2950 3600
Wire Wire Line
	4150 3600 4150 3800
Wire Wire Line
	3950 3900 3950 3600
Wire Wire Line
	3950 3600 4150 3600
Wire Wire Line
	2950 3650 2950 3600
Connection ~ 2950 3600
$Comp
L power:GND #PWR0134
U 1 1 5BC2496A
P 2600 3950
F 0 "#PWR0134" H 2600 3700 50  0001 C CNN
F 1 "GND" H 2605 3777 50  0000 C CNN
F 2 "" H 2600 3950 50  0001 C CNN
F 3 "" H 2600 3950 50  0001 C CNN
	1    2600 3950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0135
U 1 1 5BC249C8
P 2950 3950
F 0 "#PWR0135" H 2950 3700 50  0001 C CNN
F 1 "GND" H 2955 3777 50  0000 C CNN
F 2 "" H 2950 3950 50  0001 C CNN
F 3 "" H 2950 3950 50  0001 C CNN
	1    2950 3950
	1    0    0    -1  
$EndComp
Connection ~ 3950 3600
$Comp
L power:VCC #PWR0136
U 1 1 5BC2C5B9
P 3950 3450
F 0 "#PWR0136" H 3950 3300 50  0001 C CNN
F 1 "VCC" H 3967 3623 50  0000 C CNN
F 2 "" H 3950 3450 50  0001 C CNN
F 3 "" H 3950 3450 50  0001 C CNN
	1    3950 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3450 3950 3600
$Comp
L power:GND #PWR0137
U 1 1 5BC3042C
P 4050 5700
F 0 "#PWR0137" H 4050 5450 50  0001 C CNN
F 1 "GND" H 4055 5527 50  0000 C CNN
F 2 "" H 4050 5700 50  0001 C CNN
F 3 "" H 4050 5700 50  0001 C CNN
	1    4050 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 1050 3300 1050
Connection ~ 3300 1050
Wire Wire Line
	3300 1050 3700 1050
Wire Wire Line
	3200 1150 3200 2550
Connection ~ 3200 1150
Wire Wire Line
	3200 1150 2950 1150
Wire Wire Line
	3350 4800 3100 4800
Wire Wire Line
	3350 4600 3100 4600
Connection ~ 3100 4600
Wire Wire Line
	3100 4600 3100 4450
NoConn ~ 4750 4700
NoConn ~ 4750 4800
NoConn ~ 4750 4500
$Comp
L power:GND #PWR0138
U 1 1 5BC50272
P 4750 5700
F 0 "#PWR0138" H 4750 5450 50  0001 C CNN
F 1 "GND" H 4755 5527 50  0000 C CNN
F 2 "" H 4750 5700 50  0001 C CNN
F 3 "" H 4750 5700 50  0001 C CNN
	1    4750 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 5200 4750 5700
$Comp
L Device:C C24
U 1 1 5BC54FEC
P 5000 5350
F 0 "C24" H 5115 5396 50  0000 L CNN
F 1 "100n" H 5115 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5038 5200 50  0001 C CNN
F 3 "~" H 5000 5350 50  0001 C CNN
	1    5000 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0139
U 1 1 5BC550A0
P 5000 5750
F 0 "#PWR0139" H 5000 5500 50  0001 C CNN
F 1 "GND" H 5005 5577 50  0000 C CNN
F 2 "" H 5000 5750 50  0001 C CNN
F 3 "" H 5000 5750 50  0001 C CNN
	1    5000 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5500 5000 5750
Wire Wire Line
	4750 5000 5000 5000
Wire Wire Line
	5000 5000 5000 5200
Wire Wire Line
	4750 5100 4850 5100
Wire Wire Line
	4850 5100 4850 3800
Wire Wire Line
	4850 3800 4150 3800
Connection ~ 4150 3800
Wire Wire Line
	4150 3800 4150 3900
$Comp
L power:GND #PWR0140
U 1 1 5BC644ED
P 3300 5700
F 0 "#PWR0140" H 3300 5450 50  0001 C CNN
F 1 "GND" H 3305 5527 50  0000 C CNN
F 2 "" H 3300 5700 50  0001 C CNN
F 3 "" H 3300 5700 50  0001 C CNN
	1    3300 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 5700 3300 5000
Wire Wire Line
	3300 5000 3350 5000
$Comp
L power:GND #PWR0165
U 1 1 5BCC91EB
P 6350 3600
F 0 "#PWR0165" H 6350 3350 50  0001 C CNN
F 1 "GND" H 6355 3427 50  0000 C CNN
F 2 "" H 6350 3600 50  0001 C CNN
F 3 "" H 6350 3600 50  0001 C CNN
	1    6350 3600
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0166
U 1 1 5BCC9325
P 5200 2150
F 0 "#PWR0166" H 5200 2000 50  0001 C CNN
F 1 "VCC" H 5217 2323 50  0000 C CNN
F 2 "" H 5200 2150 50  0001 C CNN
F 3 "" H 5200 2150 50  0001 C CNN
	1    5200 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2150 5200 2300
Connection ~ 5200 2300
Wire Wire Line
	5750 2150 5750 2300
Text GLabel 7450 2800 2    50   Input ~ 0
BOOST_EN
$Comp
L Device:R R13
U 1 1 5BCEF31D
P 7350 3050
F 0 "R13" H 7420 3096 50  0000 L CNN
F 1 "10K" H 7420 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7280 3050 50  0001 C CNN
F 3 "~" H 7350 3050 50  0001 C CNN
	1    7350 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0167
U 1 1 5BCEF3D2
P 7350 3200
F 0 "#PWR0167" H 7350 2950 50  0001 C CNN
F 1 "GND" H 7350 3050 50  0000 C CNN
F 2 "" H 7350 3200 50  0001 C CNN
F 3 "" H 7350 3200 50  0001 C CNN
	1    7350 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 2900 7350 2800
Wire Wire Line
	7350 2800 7450 2800
Text GLabel 6750 3000 2    50   Input ~ 0
LCD_RST
$Comp
L jeroens_components:AO3400 Q2
U 1 1 5BD1A353
P 7500 5250
F 0 "Q2" H 7605 5296 50  0000 L CNN
F 1 "AO3400" H 7605 5205 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7600 4700 50  0001 C CNN
F 3 "http://www.aosmd.com/pdfs/datasheet/ao3400.pdf" H 7600 4800 50  0001 C CNN
	1    7500 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R17
U 1 1 5BD1A35A
P 6950 5400
F 0 "R17" H 7020 5446 50  0000 L CNN
F 1 "10K" H 7020 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6880 5400 50  0001 C CNN
F 3 "~" H 6950 5400 50  0001 C CNN
	1    6950 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 5BD1A361
P 7500 4850
F 0 "R15" H 7570 4896 50  0000 L CNN
F 1 "68R" H 7570 4805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7430 4850 50  0001 C CNN
F 3 "~" H 7500 4850 50  0001 C CNN
	1    7500 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0168
U 1 1 5BD1A36F
P 6950 5550
F 0 "#PWR0168" H 6950 5300 50  0001 C CNN
F 1 "GND" H 6955 5377 50  0000 C CNN
F 2 "" H 6950 5550 50  0001 C CNN
F 3 "" H 6950 5550 50  0001 C CNN
	1    6950 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0169
U 1 1 5BD1A375
P 7500 5550
F 0 "#PWR0169" H 7500 5300 50  0001 C CNN
F 1 "GND" H 7505 5377 50  0000 C CNN
F 2 "" H 7500 5550 50  0001 C CNN
F 3 "" H 7500 5550 50  0001 C CNN
	1    7500 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 5450 7500 5550
Wire Wire Line
	6950 5250 7200 5250
Wire Wire Line
	7500 5050 7500 5000
Wire Wire Line
	7500 4700 7500 4600
Text GLabel 7500 4600 1    50   Input ~ 0
LCD_BL_K
$Comp
L Device:Net-Tie_2 NT1
U 1 1 5BD5C567
P 3100 4350
F 0 "NT1" V 3100 3250 50  0000 L CNN
F 1 "Net-Tie_2" V 3200 3050 50  0000 L CNN
F 2 "NetTie:NetTie-2_THT_Pad0.3mm" H 3100 4350 50  0001 C CNN
F 3 "~" H 3100 4350 50  0001 C CNN
	1    3100 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 4250 3100 3600
Wire Wire Line
	2950 3600 3100 3600
Connection ~ 3100 3600
Wire Wire Line
	3100 3600 3950 3600
$Comp
L Connector:TestPoint TP4
U 1 1 5BD751BF
P 3700 1050
F 0 "TP4" H 3758 1170 50  0000 L CNN
F 1 "SDA" H 3758 1079 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3900 1050 50  0001 C CNN
F 3 "~" H 3900 1050 50  0001 C CNN
	1    3700 1050
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP5
U 1 1 5BD75244
P 3700 1150
F 0 "TP5" H 3642 1177 50  0000 R CNN
F 1 "SCL" H 3642 1268 50  0000 R CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3900 1150 50  0001 C CNN
F 3 "~" H 3900 1150 50  0001 C CNN
	1    3700 1150
	-1   0    0    1   
$EndComp
Wire Wire Line
	3700 1150 3200 1150
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDEA28E
P 1650 5050
AR Path="/5BBC906D/5BDEA28E" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BDEA28E" Ref="X33"  Part="1" 
F 0 "X33" V 1604 5327 50  0000 L CNN
F 1 "wire_pad" V 1695 5327 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 5050 50  0001 C CNN
F 3 "" H 1650 5050 50  0001 C CNN
	1    1650 5050
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDEA295
P 1650 5150
AR Path="/5BBC906D/5BDEA295" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BDEA295" Ref="X34"  Part="1" 
F 0 "X34" V 1604 5427 50  0000 L CNN
F 1 "wire_pad" V 1695 5427 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 5150 50  0001 C CNN
F 3 "" H 1650 5150 50  0001 C CNN
	1    1650 5150
	0    1    1    0   
$EndComp
Text GLabel 1650 5150 0    50   Input ~ 0
I2C_SDA
Text GLabel 1650 5050 0    50   Input ~ 0
I2C_SCL
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDF2748
P 1650 5300
AR Path="/5BBC906D/5BDF2748" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BDF2748" Ref="X35"  Part="1" 
F 0 "X35" V 1604 5577 50  0000 L CNN
F 1 "wire_pad" V 1695 5577 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 5300 50  0001 C CNN
F 3 "" H 1650 5300 50  0001 C CNN
	1    1650 5300
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDF274F
P 1650 5400
AR Path="/5BBC906D/5BDF274F" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BDF274F" Ref="X36"  Part="1" 
F 0 "X36" V 1604 5677 50  0000 L CNN
F 1 "wire_pad" V 1695 5677 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 5400 50  0001 C CNN
F 3 "" H 1650 5400 50  0001 C CNN
	1    1650 5400
	0    1    1    0   
$EndComp
Text GLabel 1650 5400 0    50   Input ~ 0
I2C_SDA
Text GLabel 1650 5300 0    50   Input ~ 0
I2C_SCL
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDF8799
P 1650 5550
AR Path="/5BBC906D/5BDF8799" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BDF8799" Ref="X37"  Part="1" 
F 0 "X37" V 1604 5827 50  0000 L CNN
F 1 "wire_pad" V 1695 5827 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 5550 50  0001 C CNN
F 3 "" H 1650 5550 50  0001 C CNN
	1    1650 5550
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDF87A0
P 1650 5650
AR Path="/5BBC906D/5BDF87A0" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BDF87A0" Ref="X38"  Part="1" 
F 0 "X38" V 1604 5927 50  0000 L CNN
F 1 "wire_pad" V 1695 5927 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 5650 50  0001 C CNN
F 3 "" H 1650 5650 50  0001 C CNN
	1    1650 5650
	0    1    1    0   
$EndComp
Text GLabel 1650 5650 0    50   Input ~ 0
I2C_SDA
Text GLabel 1650 5550 0    50   Input ~ 0
I2C_SCL
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDFE7DA
P 1650 5800
AR Path="/5BBC906D/5BDFE7DA" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BDFE7DA" Ref="X39"  Part="1" 
F 0 "X39" V 1604 6077 50  0000 L CNN
F 1 "wire_pad" V 1695 6077 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 5800 50  0001 C CNN
F 3 "" H 1650 5800 50  0001 C CNN
	1    1650 5800
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDFE7E1
P 1650 5900
AR Path="/5BBC906D/5BDFE7E1" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BDFE7E1" Ref="X40"  Part="1" 
F 0 "X40" V 1604 6177 50  0000 L CNN
F 1 "wire_pad" V 1695 6177 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 5900 50  0001 C CNN
F 3 "" H 1650 5900 50  0001 C CNN
	1    1650 5900
	0    1    1    0   
$EndComp
Text GLabel 1650 5900 0    50   Input ~ 0
I2C_SDA
Text GLabel 1650 5800 0    50   Input ~ 0
I2C_SCL
Text Label 8200 3250 0    50   ~ 0
XSHUT1
Text Label 8250 1350 0    50   ~ 0
XSHUT2
Text Label 8250 5100 0    50   ~ 0
XSHUT3
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE04E3A
P 1650 6550
AR Path="/5BBC906D/5BE04E3A" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE04E3A" Ref="X41"  Part="1" 
F 0 "X41" V 1604 6827 50  0000 L CNN
F 1 "wire_pad" V 1695 6827 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 6550 50  0001 C CNN
F 3 "" H 1650 6550 50  0001 C CNN
	1    1650 6550
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE04E41
P 1650 6650
AR Path="/5BBC906D/5BE04E41" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE04E41" Ref="X42"  Part="1" 
F 0 "X42" V 1696 6927 50  0000 L CNN
F 1 "wire_pad" V 1605 6927 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 6650 50  0001 C CNN
F 3 "" H 1650 6650 50  0001 C CNN
	1    1650 6650
	0    1    -1   0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE04E48
P 1650 6800
AR Path="/5BBC906D/5BE04E48" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE04E48" Ref="X43"  Part="1" 
F 0 "X43" V 1604 7077 50  0000 L CNN
F 1 "wire_pad" V 1695 7077 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 6800 50  0001 C CNN
F 3 "" H 1650 6800 50  0001 C CNN
	1    1650 6800
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE04E4F
P 1650 6900
AR Path="/5BBC906D/5BE04E4F" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE04E4F" Ref="X44"  Part="1" 
F 0 "X44" V 1604 7177 50  0000 L CNN
F 1 "wire_pad" V 1695 7177 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 6900 50  0001 C CNN
F 3 "" H 1650 6900 50  0001 C CNN
	1    1650 6900
	0    1    1    0   
$EndComp
Text Label 1500 6550 0    50   ~ 0
XSHUT1A
Text Label 1500 6650 0    50   ~ 0
XSHUT1A
Text Label 1500 6800 0    50   ~ 0
XSHUT2
Text Label 1500 6900 0    50   ~ 0
XSHUT2
Wire Wire Line
	1500 6550 1650 6550
Wire Wire Line
	1650 6650 1500 6650
Wire Wire Line
	1500 6800 1650 6800
Wire Wire Line
	1650 6900 1500 6900
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE2A497
P 1650 7050
AR Path="/5BBC906D/5BE2A497" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE2A497" Ref="X45"  Part="1" 
F 0 "X45" V 1604 7327 50  0000 L CNN
F 1 "wire_pad" V 1695 7327 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 7050 50  0001 C CNN
F 3 "" H 1650 7050 50  0001 C CNN
	1    1650 7050
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE2A49E
P 1650 7150
AR Path="/5BBC906D/5BE2A49E" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE2A49E" Ref="X46"  Part="1" 
F 0 "X46" V 1604 7427 50  0000 L CNN
F 1 "wire_pad" V 1695 7427 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 7150 50  0001 C CNN
F 3 "" H 1650 7150 50  0001 C CNN
	1    1650 7150
	0    1    1    0   
$EndComp
Text Label 1500 7050 0    50   ~ 0
XSHUT3
Text Label 1500 7150 0    50   ~ 0
XSHUT3
Wire Wire Line
	1500 7050 1650 7050
Wire Wire Line
	1650 7150 1500 7150
$Comp
L power:GND #PWR0173
U 1 1 5BE350B8
P 5750 3600
F 0 "#PWR0173" H 5750 3350 50  0001 C CNN
F 1 "GND" H 5755 3427 50  0000 C CNN
F 2 "" H 5750 3600 50  0001 C CNN
F 3 "" H 5750 3600 50  0001 C CNN
	1    5750 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 3300 5750 3600
Connection ~ 5750 3300
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE3C08B
P 1650 6050
AR Path="/5BBC906D/5BE3C08B" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE3C08B" Ref="X49"  Part="1" 
F 0 "X49" V 1604 6327 50  0000 L CNN
F 1 "wire_pad" V 1695 6327 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 6050 50  0001 C CNN
F 3 "" H 1650 6050 50  0001 C CNN
	1    1650 6050
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE3C092
P 1650 6150
AR Path="/5BBC906D/5BE3C092" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE3C092" Ref="X50"  Part="1" 
F 0 "X50" V 1604 6427 50  0000 L CNN
F 1 "wire_pad" V 1695 6427 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 1650 6150 50  0001 C CNN
F 3 "" H 1650 6150 50  0001 C CNN
	1    1650 6150
	0    1    1    0   
$EndComp
Text GLabel 1650 6150 0    50   Input ~ 0
I2C_SDA
Text GLabel 1650 6050 0    50   Input ~ 0
I2C_SCL
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE42FFD
P 3050 7100
AR Path="/5BBC906D/5BE42FFD" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE42FFD" Ref="X52"  Part="1" 
F 0 "X52" V 3096 7377 50  0000 L CNN
F 1 "wire_pad" V 3005 7377 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 3050 7100 50  0001 C CNN
F 3 "" H 3050 7100 50  0001 C CNN
	1    3050 7100
	0    1    -1   0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE42FF6
P 3050 7000
AR Path="/5BBC906D/5BE42FF6" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE42FF6" Ref="X51"  Part="1" 
F 0 "X51" V 3004 7277 50  0000 L CNN
F 1 "wire_pad" V 3095 7277 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 3050 7000 50  0001 C CNN
F 3 "" H 3050 7000 50  0001 C CNN
	1    3050 7000
	0    1    1    0   
$EndComp
Text GLabel 3050 7000 0    50   Input ~ 0
BOOST_EN
Text GLabel 3050 7100 0    50   Input ~ 0
BOOST_EN
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE5158C
P 3050 6700
AR Path="/5BBC906D/5BE5158C" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE5158C" Ref="X55"  Part="1" 
F 0 "X55" V 3096 6977 50  0000 L CNN
F 1 "wire_pad" V 3005 6977 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 3050 6700 50  0001 C CNN
F 3 "" H 3050 6700 50  0001 C CNN
	1    3050 6700
	0    1    -1   0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE51593
P 3050 6600
AR Path="/5BBC906D/5BE51593" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE51593" Ref="X53"  Part="1" 
F 0 "X53" V 3004 6877 50  0000 L CNN
F 1 "wire_pad" V 3095 6877 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 3050 6600 50  0001 C CNN
F 3 "" H 3050 6600 50  0001 C CNN
	1    3050 6600
	0    1    1    0   
$EndComp
Text GLabel 3050 6600 0    50   Input ~ 0
LCD_RST
Text GLabel 3050 6700 0    50   Input ~ 0
LCD_RST
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE5878A
P 4500 6700
AR Path="/5BBC906D/5BE5878A" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE5878A" Ref="X56"  Part="1" 
F 0 "X56" V 4546 6977 50  0000 L CNN
F 1 "wire_pad" V 4455 6977 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 4500 6700 50  0001 C CNN
F 3 "" H 4500 6700 50  0001 C CNN
	1    4500 6700
	0    1    -1   0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE58791
P 4500 6600
AR Path="/5BBC906D/5BE58791" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE58791" Ref="X54"  Part="1" 
F 0 "X54" V 4454 6877 50  0000 L CNN
F 1 "wire_pad" V 4545 6877 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 4500 6600 50  0001 C CNN
F 3 "" H 4500 6600 50  0001 C CNN
	1    4500 6600
	0    1    1    0   
$EndComp
Text GLabel 4500 6600 0    50   Input ~ 0
Vbat
Text GLabel 4500 6700 0    50   Input ~ 0
Vbat
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE5F428
P 4500 7000
AR Path="/5BBC906D/5BE5F428" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE5F428" Ref="X60"  Part="1" 
F 0 "X60" V 4546 7277 50  0000 L CNN
F 1 "wire_pad" V 4455 7277 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 4500 7000 50  0001 C CNN
F 3 "" H 4500 7000 50  0001 C CNN
	1    4500 7000
	0    1    -1   0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BE5F42F
P 4500 6900
AR Path="/5BBC906D/5BE5F42F" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BE5F42F" Ref="X59"  Part="1" 
F 0 "X59" V 4454 7177 50  0000 L CNN
F 1 "wire_pad" V 4545 7177 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 4500 6900 50  0001 C CNN
F 3 "" H 4500 6900 50  0001 C CNN
	1    4500 6900
	0    1    1    0   
$EndComp
Text GLabel 4500 6900 0    50   Input ~ 0
Vbat
Text GLabel 4500 7000 0    50   Input ~ 0
Vbat
$Comp
L power:VCC #PWR07
U 1 1 5BE663A1
P 5850 6700
F 0 "#PWR07" H 5850 6550 50  0001 C CNN
F 1 "VCC" H 5867 6873 50  0000 C CNN
F 2 "" H 5850 6700 50  0001 C CNN
F 3 "" H 5850 6700 50  0001 C CNN
	1    5850 6700
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X57
U 1 1 5BE6651B
P 5850 6700
F 0 "X57" H 5772 6772 50  0000 R CNN
F 1 "wire_pad" H 5772 6863 50  0000 R CNN
F 2 "JeroensFootprints:wire_pad_1mm" H 5850 6700 50  0001 C CNN
F 3 "" H 5850 6700 50  0001 C CNN
	1    5850 6700
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR08
U 1 1 5BE66688
P 6250 6700
F 0 "#PWR08" H 6250 6550 50  0001 C CNN
F 1 "VCC" H 6267 6873 50  0000 C CNN
F 2 "" H 6250 6700 50  0001 C CNN
F 3 "" H 6250 6700 50  0001 C CNN
	1    6250 6700
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X58
U 1 1 5BE6668E
P 6250 6700
F 0 "X58" H 6172 6772 50  0000 R CNN
F 1 "wire_pad" H 6172 6863 50  0000 R CNN
F 2 "JeroensFootprints:wire_pad_1mm" H 6250 6700 50  0001 C CNN
F 3 "" H 6250 6700 50  0001 C CNN
	1    6250 6700
	-1   0    0    1   
$EndComp
Wire Wire Line
	9300 1000 9450 1000
Wire Wire Line
	9450 1000 9700 1000
Connection ~ 9450 1000
Wire Wire Line
	8850 2350 9000 2350
Connection ~ 9000 2350
Connection ~ 9150 2350
Wire Wire Line
	9000 2350 9150 2350
Wire Wire Line
	9150 2350 9300 2350
Connection ~ 9300 2350
Connection ~ 9450 2350
Wire Wire Line
	9300 2350 9450 2350
Wire Wire Line
	9450 2350 9700 2350
Wire Wire Line
	8500 1350 8250 1350
Wire Wire Line
	8200 3250 8500 3250
Wire Wire Line
	8250 5100 8500 5100
Connection ~ 7350 2800
Wire Wire Line
	6750 2800 7350 2800
Text Label 6750 2600 0    50   ~ 0
XSHUT3
Text Label 6750 2700 0    50   ~ 0
XSHUT1A
Text Label 6750 3300 0    50   ~ 0
XSHUT2
Wire Wire Line
	7150 3950 7150 2900
Wire Wire Line
	7150 2900 6750 2900
Wire Wire Line
	6150 3950 7150 3950
Wire Wire Line
	7250 4250 6950 4250
Wire Wire Line
	6950 4250 6950 5250
Connection ~ 6950 5250
Wire Wire Line
	6750 3100 7250 3100
Wire Wire Line
	7250 3100 7250 4250
Text Label 6950 5150 1    50   ~ 0
LCD_BL_EN
Text Label 3850 6150 0    50   ~ 0
LCD_BL_EN
$Comp
L jeroens_components:wire_pad X61
U 1 1 5BEE9332
P 4100 6150
F 0 "X61" V 4054 6427 50  0000 L CNN
F 1 "wire_pad" V 4145 6427 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 4100 6150 50  0001 C CNN
F 3 "" H 4100 6150 50  0001 C CNN
	1    4100 6150
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 6150 3850 6150
Text Label 3850 6300 0    50   ~ 0
LCD_BL_EN
$Comp
L jeroens_components:wire_pad X62
U 1 1 5BEEF521
P 4100 6300
F 0 "X62" V 4054 6577 50  0000 L CNN
F 1 "wire_pad" V 4145 6577 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 4100 6300 50  0001 C CNN
F 3 "" H 4100 6300 50  0001 C CNN
	1    4100 6300
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 6300 3850 6300
NoConn ~ 6750 3200
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BF13200
P 800 6550
AR Path="/5BBC906D/5BF13200" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BF13200" Ref="X67"  Part="1" 
F 0 "X67" V 754 6827 50  0000 L CNN
F 1 "wire_pad" V 845 6827 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 800 6550 50  0001 C CNN
F 3 "" H 800 6550 50  0001 C CNN
	1    800  6550
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BF13207
P 800 6650
AR Path="/5BBC906D/5BF13207" Ref="X?"  Part="1" 
AR Path="/5BBC9070/5BF13207" Ref="X68"  Part="1" 
F 0 "X68" V 846 6927 50  0000 L CNN
F 1 "wire_pad" V 755 6927 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 800 6650 50  0001 C CNN
F 3 "" H 800 6650 50  0001 C CNN
	1    800  6650
	0    1    -1   0   
$EndComp
Text Label 650  6550 0    50   ~ 0
XSHUT1
Text Label 650  6650 0    50   ~ 0
XSHUT1A
Wire Wire Line
	650  6550 800  6550
Wire Wire Line
	800  6650 650  6650
$Comp
L Device:R R28
U 1 1 5BF297D9
P 2650 2300
F 0 "R28" H 2720 2346 50  0000 L CNN
F 1 "3K3" H 2720 2255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2580 2300 50  0001 C CNN
F 3 "~" H 2650 2300 50  0001 C CNN
	1    2650 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R29
U 1 1 5BF29900
P 2850 2300
F 0 "R29" H 2920 2346 50  0000 L CNN
F 1 "3K3" H 2920 2255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2780 2300 50  0001 C CNN
F 3 "~" H 2850 2300 50  0001 C CNN
	1    2850 2300
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0163
U 1 1 5BF29A1F
P 2750 2000
F 0 "#PWR0163" H 2750 1850 50  0001 C CNN
F 1 "VCC" H 2767 2173 50  0000 C CNN
F 2 "" H 2750 2000 50  0001 C CNN
F 3 "" H 2750 2000 50  0001 C CNN
	1    2750 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 2150 2650 2000
Wire Wire Line
	2650 2000 2750 2000
Wire Wire Line
	2750 2000 2850 2000
Wire Wire Line
	2850 2000 2850 2150
Connection ~ 2750 2000
Wire Wire Line
	2850 2450 2850 2550
Wire Wire Line
	2850 2550 3200 2550
Wire Wire Line
	3300 2650 2650 2650
Wire Wire Line
	2650 2650 2650 2450
Wire Wire Line
	3300 2650 3300 1050
Text GLabel 4300 2650 0    50   Input ~ 0
I2C_SCL
Text GLabel 4300 2750 0    50   Input ~ 0
I2C_SDA
Text GLabel 5950 2600 0    50   Input ~ 0
I2C_SCL
Text GLabel 5950 2700 0    50   Input ~ 0
I2C_SDA
Text GLabel 3000 4700 0    50   Input ~ 0
I2C_SCL
Text GLabel 3000 4500 0    50   Input ~ 0
I2C_SDA
Wire Wire Line
	3100 4600 3100 4800
Wire Wire Line
	3000 4700 3350 4700
Wire Wire Line
	3000 4500 3350 4500
Text GLabel 8500 1900 0    50   Input ~ 0
I2C_SCL
Text GLabel 8500 1800 0    50   Input ~ 0
I2C_SDA
Text GLabel 8500 3800 0    50   Input ~ 0
I2C_SCL
Text GLabel 8500 3700 0    50   Input ~ 0
I2C_SDA
Text GLabel 8500 5650 0    50   Input ~ 0
I2C_SCL
Text GLabel 8500 5550 0    50   Input ~ 0
I2C_SDA
$EndSCHEMATC
