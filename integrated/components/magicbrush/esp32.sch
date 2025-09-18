EESchema Schematic File Version 4
LIBS:magicbrush-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 6450 3500 2    50   Input ~ 0
CART_D3
Text GLabel 6450 3300 2    50   Input ~ 0
CART_CSYNC
Text GLabel 6450 3100 2    50   Input ~ 0
CART_D1
Text GLabel 6450 3200 2    50   Input ~ 0
CART_D2
Text GLabel 6450 4500 2    50   Input ~ 0
CART_S5
Text GLabel 6450 4600 2    50   Input ~ 0
CART_S1
Text GLabel 6450 4700 2    50   Input ~ 0
CART_S4
Text GLabel 6450 3900 2    50   Input ~ 0
CART_S2
Text GLabel 6450 4300 2    50   Input ~ 0
CART_D3
Text GLabel 6450 4400 2    50   Input ~ 0
CART_DCLK
Text GLabel 6450 3400 2    50   Input ~ 0
CART_F3
Text GLabel 6450 4200 2    50   Input ~ 0
CART_F5
Text GLabel 6450 4000 2    50   Input ~ 0
I2C_SDA
Text GLabel 6450 4100 2    50   Input ~ 0
I2C_SCL
$Comp
L power:VCC #PWR0141
U 1 1 5BCD963D
P 5400 2000
F 0 "#PWR0141" H 5400 1850 50  0001 C CNN
F 1 "VCC" H 5417 2173 50  0000 C CNN
F 2 "" H 5400 2000 50  0001 C CNN
F 3 "" H 5400 2000 50  0001 C CNN
	1    5400 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C12
U 1 1 5BCD9677
P 5800 2250
F 0 "C12" H 5915 2296 50  0000 L CNN
F 1 "100n" H 5850 2150 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5838 2100 50  0001 C CNN
F 3 "~" H 5800 2250 50  0001 C CNN
	1    5800 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0142
U 1 1 5BCD96FE
P 5800 2400
F 0 "#PWR0142" H 5800 2150 50  0001 C CNN
F 1 "GND" H 5805 2227 50  0000 C CNN
F 2 "" H 5800 2400 50  0001 C CNN
F 3 "" H 5800 2400 50  0001 C CNN
	1    5800 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 2000 5400 2100
Wire Wire Line
	5800 2100 5500 2100
Connection ~ 5500 2100
Wire Wire Line
	5500 2100 5500 2650
$Comp
L Device:C C13
U 1 1 5BCD9747
P 6150 2250
F 0 "C13" H 6265 2296 50  0000 L CNN
F 1 "22u" H 6200 2150 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6188 2100 50  0001 C CNN
F 3 "~" H 6150 2250 50  0001 C CNN
	1    6150 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0154
U 1 1 5BCD97CF
P 6150 2400
F 0 "#PWR0154" H 6150 2150 50  0001 C CNN
F 1 "GND" H 6155 2227 50  0000 C CNN
F 2 "" H 6150 2400 50  0001 C CNN
F 3 "" H 6150 2400 50  0001 C CNN
	1    6150 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 2100 5800 2100
Connection ~ 5800 2100
$Comp
L Device:R R10
U 1 1 5BCD9837
P 4900 2100
F 0 "R10" V 4693 2100 50  0000 C CNN
F 1 "10K" V 4784 2100 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4830 2100 50  0001 C CNN
F 3 "~" H 4900 2100 50  0001 C CNN
	1    4900 2100
	0    1    1    0   
$EndComp
$Comp
L Device:C C14
U 1 1 5BCD988E
P 4450 2350
F 0 "C14" H 4300 2450 50  0000 L CNN
F 1 "100n" H 4200 2250 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4488 2200 50  0001 C CNN
F 3 "~" H 4450 2350 50  0001 C CNN
	1    4450 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0155
U 1 1 5BCD99A7
P 4450 2500
F 0 "#PWR0155" H 4450 2250 50  0001 C CNN
F 1 "GND" H 4455 2327 50  0000 C CNN
F 2 "" H 4450 2500 50  0001 C CNN
F 3 "" H 4450 2500 50  0001 C CNN
	1    4450 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2200 4450 2100
Wire Wire Line
	4450 2100 4700 2100
Wire Wire Line
	5050 2100 5400 2100
Wire Wire Line
	4800 2850 4700 2850
Wire Wire Line
	4700 2850 4700 2800
Connection ~ 4700 2100
Wire Wire Line
	4700 2100 4750 2100
Wire Wire Line
	5350 5100 5350 5200
Wire Wire Line
	5350 5200 5500 5200
Wire Wire Line
	5650 5200 5650 5100
Wire Wire Line
	5500 5100 5500 5200
Connection ~ 5500 5200
Wire Wire Line
	5500 5200 5650 5200
Wire Wire Line
	5500 5300 5500 5200
$Comp
L power:GND #PWR0156
U 1 1 5BCD9F0B
P 5500 5300
F 0 "#PWR0156" H 5500 5050 50  0001 C CNN
F 1 "GND" H 5505 5127 50  0000 C CNN
F 2 "" H 5500 5300 50  0001 C CNN
F 3 "" H 5500 5300 50  0001 C CNN
	1    5500 5300
	1    0    0    -1  
$EndComp
NoConn ~ 4800 3950
NoConn ~ 4800 4050
NoConn ~ 4800 4150
NoConn ~ 4800 4250
NoConn ~ 4800 4350
NoConn ~ 4800 4450
Wire Wire Line
	6250 3100 6450 3100
Wire Wire Line
	6250 3200 6450 3200
Wire Wire Line
	6250 3300 6450 3300
Wire Wire Line
	6250 3400 6450 3400
Wire Wire Line
	6250 3500 6450 3500
Wire Wire Line
	6250 3600 6450 3600
Wire Wire Line
	6250 3700 6450 3700
Wire Wire Line
	6450 3800 6250 3800
Wire Wire Line
	6250 3900 6450 3900
Wire Wire Line
	6450 4000 6250 4000
Wire Wire Line
	6250 4100 6450 4100
Wire Wire Line
	6450 4200 6250 4200
Wire Wire Line
	6450 4600 6250 4600
Wire Wire Line
	6250 4700 6450 4700
$Comp
L Switch:SW_Push SW?
U 1 1 5BCE1F8E
P 3550 4550
AR Path="/5BBC9070/5BCE1F8E" Ref="SW?"  Part="1" 
AR Path="/5BBC906D/5BCE1F8E" Ref="SW2"  Part="1" 
F 0 "SW2" V 3504 4698 50  0000 L CNN
F 1 "SW_Push" V 3595 4698 50  0000 L CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 3550 4750 50  0001 C CNN
F 3 "" H 3550 4750 50  0001 C CNN
	1    3550 4550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BCE1F95
P 3550 4750
AR Path="/5BBC9070/5BCE1F95" Ref="#PWR?"  Part="1" 
AR Path="/5BBC906D/5BCE1F95" Ref="#PWR0157"  Part="1" 
F 0 "#PWR0157" H 3550 4500 50  0001 C CNN
F 1 "GND" H 3555 4577 50  0000 C CNN
F 2 "" H 3550 4750 50  0001 C CNN
F 3 "" H 3550 4750 50  0001 C CNN
	1    3550 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5BCE1F9B
P 3550 3950
AR Path="/5BBC9070/5BCE1F9B" Ref="R?"  Part="1" 
AR Path="/5BBC906D/5BCE1F9B" Ref="R12"  Part="1" 
F 0 "R12" H 3620 3996 50  0000 L CNN
F 1 "10K" H 3620 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3480 3950 50  0001 C CNN
F 3 "~" H 3550 3950 50  0001 C CNN
	1    3550 3950
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 5BCE1FA8
P 3550 3800
AR Path="/5BBC9070/5BCE1FA8" Ref="#PWR?"  Part="1" 
AR Path="/5BBC906D/5BCE1FA8" Ref="#PWR0158"  Part="1" 
F 0 "#PWR0158" H 3550 3650 50  0001 C CNN
F 1 "VCC" H 3567 3973 50  0000 C CNN
F 2 "" H 3550 3800 50  0001 C CNN
F 3 "" H 3550 3800 50  0001 C CNN
	1    3550 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3750 4100 3750
$Comp
L Connector:Conn_01x05_Male J3
U 1 1 5BD26790
P 3450 3000
F 0 "J3" H 3556 3378 50  0000 C CNN
F 1 "Conn_01x05_Male" H 3556 3287 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 3450 3000 50  0001 C CNN
F 3 "~" H 3450 3000 50  0001 C CNN
	1    3450 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2800 4700 2800
Connection ~ 4700 2800
Wire Wire Line
	4700 2800 4700 2100
Wire Wire Line
	4800 3250 4000 3250
Wire Wire Line
	4000 3250 4000 2900
Wire Wire Line
	4000 2900 3650 2900
Wire Wire Line
	3650 3000 4100 3000
Wire Wire Line
	4100 3000 4100 3150
Wire Wire Line
	4100 3150 4800 3150
Wire Wire Line
	4800 3050 4600 3050
Wire Wire Line
	3800 3050 3800 3100
Wire Wire Line
	3800 3100 3650 3100
$Comp
L power:GND #PWR0159
U 1 1 5BD287C6
P 3750 3200
F 0 "#PWR0159" H 3750 2950 50  0001 C CNN
F 1 "GND" H 3755 3027 50  0000 C CNN
F 2 "" H 3750 3200 50  0001 C CNN
F 3 "" H 3750 3200 50  0001 C CNN
	1    3750 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 3200 3750 3200
Wire Wire Line
	3550 4100 3550 4200
$Comp
L Switch:SW_Push SW?
U 1 1 5BD2A840
P 3050 4550
AR Path="/5BBC9070/5BD2A840" Ref="SW?"  Part="1" 
AR Path="/5BBC906D/5BD2A840" Ref="SW1"  Part="1" 
F 0 "SW1" V 3004 4698 50  0000 L CNN
F 1 "SW_Push" V 3095 4698 50  0000 L CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 3050 4750 50  0001 C CNN
F 3 "" H 3050 4750 50  0001 C CNN
	1    3050 4550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BD2A847
P 3050 4750
AR Path="/5BBC9070/5BD2A847" Ref="#PWR?"  Part="1" 
AR Path="/5BBC906D/5BD2A847" Ref="#PWR0160"  Part="1" 
F 0 "#PWR0160" H 3050 4500 50  0001 C CNN
F 1 "GND" H 3055 4577 50  0000 C CNN
F 2 "" H 3050 4750 50  0001 C CNN
F 3 "" H 3050 4750 50  0001 C CNN
	1    3050 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5BD2A84D
P 3050 3950
AR Path="/5BBC9070/5BD2A84D" Ref="R?"  Part="1" 
AR Path="/5BBC906D/5BD2A84D" Ref="R11"  Part="1" 
F 0 "R11" H 3120 3996 50  0000 L CNN
F 1 "10K" H 3120 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2980 3950 50  0001 C CNN
F 3 "~" H 3050 3950 50  0001 C CNN
	1    3050 3950
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 5BD2A854
P 3050 3800
AR Path="/5BBC9070/5BD2A854" Ref="#PWR?"  Part="1" 
AR Path="/5BBC906D/5BD2A854" Ref="#PWR0161"  Part="1" 
F 0 "#PWR0161" H 3050 3650 50  0001 C CNN
F 1 "VCC" H 3067 3973 50  0000 C CNN
F 2 "" H 3050 3800 50  0001 C CNN
F 3 "" H 3050 3800 50  0001 C CNN
	1    3050 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 4100 3050 4300
Wire Wire Line
	4100 4300 3050 4300
Wire Wire Line
	4100 3750 4100 4300
Connection ~ 3050 4300
Wire Wire Line
	3050 4300 3050 4350
Wire Wire Line
	3550 4200 4000 4200
Wire Wire Line
	4000 4200 4000 3650
Wire Wire Line
	4000 3650 4800 3650
Connection ~ 3550 4200
Wire Wire Line
	3550 4200 3550 4350
NoConn ~ 4800 3450
NoConn ~ 4800 3550
Text GLabel 6450 3800 2    50   Input ~ 0
LCD_SDA
Text GLabel 6450 3700 2    50   Input ~ 0
LCD_SCK
Text GLabel 6450 3600 2    50   Input ~ 0
LCD_CS
Text GLabel 4550 2950 0    50   Input ~ 0
LCD_RS
Wire Wire Line
	4600 3050 4600 2950
Wire Wire Line
	4600 2950 4550 2950
Connection ~ 4600 3050
Wire Wire Line
	4600 3050 3800 3050
Wire Wire Line
	6250 4300 6450 4300
Wire Wire Line
	6450 4400 6250 4400
Wire Wire Line
	6250 4500 6450 4500
$Comp
L jeroens_components:LCD_LH096T DISP1
U 1 1 5BD34236
P 9750 3700
F 0 "DISP1" H 10177 3621 50  0000 L CNN
F 1 "LCD_LH096T" H 10177 3530 50  0000 L CNN
F 2 "JeroensFootprints:LCD_LH086T" H 9750 3700 50  0001 C CNN
F 3 "http://www.jmdlcd.com/downloadRepository/f743f83e-f761-4d72-b1f0-3fcd0ae34dc2.pdf" H 9750 3700 50  0001 C CNN
	1    9750 3700
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0162
U 1 1 5BD342FA
P 9750 2600
F 0 "#PWR0162" H 9750 2450 50  0001 C CNN
F 1 "VCC" H 9767 2773 50  0000 C CNN
F 2 "" H 9750 2600 50  0001 C CNN
F 3 "" H 9750 2600 50  0001 C CNN
	1    9750 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0164
U 1 1 5BD366AF
P 9700 4600
F 0 "#PWR0164" H 9700 4350 50  0001 C CNN
F 1 "GND" H 9705 4427 50  0000 C CNN
F 2 "" H 9700 4600 50  0001 C CNN
F 3 "" H 9700 4600 50  0001 C CNN
	1    9700 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 4600 9800 4600
Connection ~ 9700 4600
Text GLabel 9100 3650 0    50   Input ~ 0
LCD_SDA
Text GLabel 9100 3750 0    50   Input ~ 0
LCD_SCK
Text GLabel 9100 3850 0    50   Input ~ 0
LCD_RS
Text GLabel 9100 3950 0    50   Input ~ 0
LCD_RST
Text GLabel 9100 4050 0    50   Input ~ 0
LCD_CS
Wire Wire Line
	9100 3650 9250 3650
Wire Wire Line
	9250 3750 9100 3750
Wire Wire Line
	9100 3850 9250 3850
Wire Wire Line
	9250 3950 9100 3950
Wire Wire Line
	9100 4050 9250 4050
Text GLabel 9050 4250 0    50   Input ~ 0
Vbat
Wire Wire Line
	9050 4250 9250 4250
Text GLabel 9050 4350 0    50   Input ~ 0
LCD_BL_K
Wire Wire Line
	9050 4350 9250 4350
Connection ~ 5400 2100
Wire Wire Line
	5400 2100 5500 2100
$Comp
L jeroens_components:ESP-WROVER32 U5
U 1 1 5BD4644F
P 5500 3800
F 0 "U5" H 5525 5128 50  0000 C CNN
F 1 "ESP-WROVER32" H 5525 5037 50  0000 C CNN
F 2 "JeroensFootprints:ESP-WROVER-MOD" H 5450 5800 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wrover-b_datasheet_en.pdf" H 5450 5800 50  0001 C CNN
	1    5500 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 5100 5850 5200
Wire Wire Line
	5850 5200 5650 5200
Connection ~ 5650 5200
Text GLabel 8250 5100 1    50   Input ~ 0
LCD_BL_K
$Comp
L power:PWR_FLAG #FLG0105
U 1 1 5BD6480C
P 8250 5100
F 0 "#FLG0105" H 8250 5175 50  0001 C CNN
F 1 "PWR_FLAG" H 8250 5273 50  0000 C CNN
F 2 "" H 8250 5100 50  0001 C CNN
F 3 "~" H 8250 5100 50  0001 C CNN
	1    8250 5100
	-1   0    0    1   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD1B6
P 2200 6600
AR Path="/5BC75E0E/5BDCD1B6" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD1B6" Ref="X22"  Part="1" 
F 0 "X22" V 2154 6877 50  0000 L CNN
F 1 "wire_pad" V 2245 6877 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 6600 50  0001 C CNN
F 3 "" H 2200 6600 50  0001 C CNN
	1    2200 6600
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD1BD
P 2200 6500
AR Path="/5BC75E0E/5BDCD1BD" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD1BD" Ref="X21"  Part="1" 
F 0 "X21" V 2154 6777 50  0000 L CNN
F 1 "wire_pad" V 2245 6777 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 6500 50  0001 C CNN
F 3 "" H 2200 6500 50  0001 C CNN
	1    2200 6500
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD1C4
P 2200 6400
AR Path="/5BC75E0E/5BDCD1C4" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD1C4" Ref="X20"  Part="1" 
F 0 "X20" V 2154 6677 50  0000 L CNN
F 1 "wire_pad" V 2245 6677 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 6400 50  0001 C CNN
F 3 "" H 2200 6400 50  0001 C CNN
	1    2200 6400
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD1CB
P 2200 6300
AR Path="/5BC75E0E/5BDCD1CB" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD1CB" Ref="X19"  Part="1" 
F 0 "X19" V 2154 6577 50  0000 L CNN
F 1 "wire_pad" V 2245 6577 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 6300 50  0001 C CNN
F 3 "" H 2200 6300 50  0001 C CNN
	1    2200 6300
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD1D2
P 2200 6200
AR Path="/5BC75E0E/5BDCD1D2" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD1D2" Ref="X18"  Part="1" 
F 0 "X18" V 2154 6477 50  0000 L CNN
F 1 "wire_pad" V 2245 6477 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 6200 50  0001 C CNN
F 3 "" H 2200 6200 50  0001 C CNN
	1    2200 6200
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD1D9
P 2200 6100
AR Path="/5BC75E0E/5BDCD1D9" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD1D9" Ref="X17"  Part="1" 
F 0 "X17" V 2154 6377 50  0000 L CNN
F 1 "wire_pad" V 2245 6377 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 6100 50  0001 C CNN
F 3 "" H 2200 6100 50  0001 C CNN
	1    2200 6100
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD1E0
P 2200 6000
AR Path="/5BC75E0E/5BDCD1E0" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD1E0" Ref="X16"  Part="1" 
F 0 "X16" V 2154 6277 50  0000 L CNN
F 1 "wire_pad" V 2245 6277 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 6000 50  0001 C CNN
F 3 "" H 2200 6000 50  0001 C CNN
	1    2200 6000
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD1E7
P 2200 5900
AR Path="/5BC75E0E/5BDCD1E7" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD1E7" Ref="X15"  Part="1" 
F 0 "X15" V 2154 6177 50  0000 L CNN
F 1 "wire_pad" V 2245 6177 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 5900 50  0001 C CNN
F 3 "" H 2200 5900 50  0001 C CNN
	1    2200 5900
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 6600 2200 6600
Text GLabel 2100 6600 0    50   Input ~ 0
CART_DCLK
Wire Wire Line
	2100 6500 2200 6500
Text GLabel 2100 6500 0    50   Input ~ 0
CART_S3
Wire Wire Line
	2100 6400 2200 6400
Text GLabel 2100 6400 0    50   Input ~ 0
CART_S2
Wire Wire Line
	2100 6300 2200 6300
Text GLabel 2100 6300 0    50   Input ~ 0
CART_S4
Wire Wire Line
	2100 6200 2200 6200
Text GLabel 2100 6200 0    50   Input ~ 0
CART_S1
Wire Wire Line
	2100 6100 2200 6100
Text GLabel 2100 6100 0    50   Input ~ 0
CART_S5
Wire Wire Line
	2100 6000 2200 6000
Wire Wire Line
	2100 5900 2200 5900
Text GLabel 2100 5800 0    50   Input ~ 0
CART_D2
Wire Wire Line
	2100 5800 2200 5800
Text GLabel 2100 5700 0    50   Input ~ 0
CART_D1
Wire Wire Line
	2100 5700 2200 5700
Text GLabel 2100 5900 0    50   Input ~ 0
CART_CSYNC
Text GLabel 2100 6000 0    50   Input ~ 0
CART_D3
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD202
P 2200 5800
AR Path="/5BC75E0E/5BDCD202" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD202" Ref="X14"  Part="1" 
F 0 "X14" V 2154 6077 50  0000 L CNN
F 1 "wire_pad" V 2245 6077 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 5800 50  0001 C CNN
F 3 "" H 2200 5800 50  0001 C CNN
	1    2200 5800
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BDCD209
P 2200 5700
AR Path="/5BC75E0E/5BDCD209" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BDCD209" Ref="X13"  Part="1" 
F 0 "X13" V 2154 5977 50  0000 L CNN
F 1 "wire_pad" V 2245 5977 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 5700 50  0001 C CNN
F 3 "" H 2200 5700 50  0001 C CNN
	1    2200 5700
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR01
U 1 1 5BDCF80D
P 1850 7150
F 0 "#PWR01" H 1850 7000 50  0001 C CNN
F 1 "VCC" H 1867 7323 50  0000 C CNN
F 2 "" H 1850 7150 50  0001 C CNN
F 3 "" H 1850 7150 50  0001 C CNN
	1    1850 7150
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X23
U 1 1 5BDCF8C5
P 1850 7150
F 0 "X23" H 1772 7222 50  0000 R CNN
F 1 "wire_pad" H 1772 7313 50  0000 R CNN
F 2 "JeroensFootprints:wire_pad_1mm" H 1850 7150 50  0001 C CNN
F 3 "" H 1850 7150 50  0001 C CNN
	1    1850 7150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5BDCFBF9
P 2550 7300
F 0 "#PWR02" H 2550 7050 50  0001 C CNN
F 1 "GND" H 2555 7127 50  0000 C CNN
F 2 "" H 2550 7300 50  0001 C CNN
F 3 "" H 2550 7300 50  0001 C CNN
	1    2550 7300
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X24
U 1 1 5BDCFCB4
P 2550 7300
F 0 "X24" H 2628 7465 50  0000 L CNN
F 1 "wire_pad" H 2628 7374 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_1mm" H 2550 7300 50  0001 C CNN
F 3 "" H 2550 7300 50  0001 C CNN
	1    2550 7300
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X31
U 1 1 5BDD2CEE
P 5400 6000
F 0 "X31" V 5354 6277 50  0000 L CNN
F 1 "wire_pad" V 5445 6277 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 5400 6000 50  0001 C CNN
F 3 "" H 5400 6000 50  0001 C CNN
	1    5400 6000
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X32
U 1 1 5BDD2E32
P 5400 6100
F 0 "X32" V 5354 6377 50  0000 L CNN
F 1 "wire_pad" V 5445 6377 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 5400 6100 50  0001 C CNN
F 3 "" H 5400 6100 50  0001 C CNN
	1    5400 6100
	0    1    1    0   
$EndComp
Text GLabel 5400 6100 0    50   Input ~ 0
I2C_SDA
Text GLabel 5400 6000 0    50   Input ~ 0
I2C_SCL
$Comp
L power:VCC #PWR0171
U 1 1 5BE31859
P 2050 7150
F 0 "#PWR0171" H 2050 7000 50  0001 C CNN
F 1 "VCC" H 2067 7323 50  0000 C CNN
F 2 "" H 2050 7150 50  0001 C CNN
F 3 "" H 2050 7150 50  0001 C CNN
	1    2050 7150
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X47
U 1 1 5BE3185F
P 2050 7150
F 0 "X47" H 1972 7222 50  0000 R CNN
F 1 "wire_pad" H 1972 7313 50  0000 R CNN
F 2 "JeroensFootprints:wire_pad_1mm" H 2050 7150 50  0001 C CNN
F 3 "" H 2050 7150 50  0001 C CNN
	1    2050 7150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0172
U 1 1 5BE31866
P 2750 7300
F 0 "#PWR0172" H 2750 7050 50  0001 C CNN
F 1 "GND" H 2755 7127 50  0000 C CNN
F 2 "" H 2750 7300 50  0001 C CNN
F 3 "" H 2750 7300 50  0001 C CNN
	1    2750 7300
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X48
U 1 1 5BE3186C
P 2750 7300
F 0 "X48" H 2828 7465 50  0000 L CNN
F 1 "wire_pad" H 2828 7374 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_1mm" H 2750 7300 50  0001 C CNN
F 3 "" H 2750 7300 50  0001 C CNN
	1    2750 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 2600 9750 3050
Text GLabel 2200 5400 0    50   Input ~ 0
CART_F3
Text GLabel 2200 5500 0    50   Input ~ 0
CART_F5
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BEFC3F2
P 2200 5400
AR Path="/5BC75E0E/5BEFC3F2" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BEFC3F2" Ref="X63"  Part="1" 
F 0 "X63" V 2154 5677 50  0000 L CNN
F 1 "wire_pad" V 2245 5677 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 5400 50  0001 C CNN
F 3 "" H 2200 5400 50  0001 C CNN
	1    2200 5400
	0    1    1    0   
$EndComp
$Comp
L jeroens_components:wire_pad X?
U 1 1 5BEFC3F9
P 2200 5500
AR Path="/5BC75E0E/5BEFC3F9" Ref="X?"  Part="1" 
AR Path="/5BBC906D/5BEFC3F9" Ref="X64"  Part="1" 
F 0 "X64" V 2154 5777 50  0000 L CNN
F 1 "wire_pad" V 2245 5777 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_0.5mm" H 2200 5500 50  0001 C CNN
F 3 "" H 2200 5500 50  0001 C CNN
	1    2200 5500
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0174
U 1 1 5C0BCE32
P 3300 7300
F 0 "#PWR0174" H 3300 7050 50  0001 C CNN
F 1 "GND" H 3305 7127 50  0000 C CNN
F 2 "" H 3300 7300 50  0001 C CNN
F 3 "" H 3300 7300 50  0001 C CNN
	1    3300 7300
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X69
U 1 1 5C0BCE38
P 3300 7300
F 0 "X69" H 3378 7465 50  0000 L CNN
F 1 "wire_pad" H 3378 7374 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_1mm" H 3300 7300 50  0001 C CNN
F 3 "" H 3300 7300 50  0001 C CNN
	1    3300 7300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0175
U 1 1 5C0BCE3F
P 3500 7300
F 0 "#PWR0175" H 3500 7050 50  0001 C CNN
F 1 "GND" H 3505 7127 50  0000 C CNN
F 2 "" H 3500 7300 50  0001 C CNN
F 3 "" H 3500 7300 50  0001 C CNN
	1    3500 7300
	1    0    0    -1  
$EndComp
$Comp
L jeroens_components:wire_pad X70
U 1 1 5C0BCE45
P 3500 7300
F 0 "X70" H 3578 7465 50  0000 L CNN
F 1 "wire_pad" H 3578 7374 50  0000 L CNN
F 2 "JeroensFootprints:wire_pad_1mm" H 3500 7300 50  0001 C CNN
F 3 "" H 3500 7300 50  0001 C CNN
	1    3500 7300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
