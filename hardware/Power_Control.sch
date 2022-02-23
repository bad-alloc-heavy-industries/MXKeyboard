EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 6
Title "Power Control"
Date ""
Rev "1"
Comp "Rachel Mant"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	8300 1900 8100 1900
Wire Wire Line
	8300 1900 8300 1800
Wire Wire Line
	8300 1900 8300 2500
Connection ~ 8300 1900
Wire Wire Line
	4300 1800 4300 1900
Wire Wire Line
	4300 1900 4300 2600
Wire Wire Line
	4200 1900 4300 1900
Connection ~ 4300 1900
Wire Wire Line
	7700 3200 7700 3300
Wire Wire Line
	8300 2700 8300 3200
Wire Wire Line
	8300 3200 7700 3200
Wire Wire Line
	7100 2700 7100 3200
Wire Wire Line
	7100 3200 7700 3200
Wire Wire Line
	6800 2700 6800 3200
Wire Wire Line
	6800 3200 7100 3200
Connection ~ 7700 3200
Connection ~ 7100 3200
Wire Wire Line
	3900 4200 4200 4200
Wire Wire Line
	4200 4200 4200 4300
Wire Wire Line
	4200 4100 4200 4200
Wire Wire Line
	4200 4200 4700 4200
Wire Wire Line
	4700 4200 4700 4100
Connection ~ 4200 4200
Wire Wire Line
	7100 1900 6800 1900
Wire Wire Line
	6800 1900 6700 1900
Wire Wire Line
	7100 1900 7100 2500
Wire Wire Line
	7100 1900 7300 1900
Wire Wire Line
	6800 2500 6800 1900
Connection ~ 7100 1900
Connection ~ 6800 1900
Wire Wire Line
	3500 3100 3300 3100
Wire Wire Line
	3300 3100 3200 3100
Wire Wire Line
	3500 3100 3500 3300
Wire Wire Line
	3300 3100 3300 1900
Wire Wire Line
	3300 3100 3300 4200
Wire Wire Line
	3300 4200 3700 4200
Wire Wire Line
	3300 1900 3400 1900
Connection ~ 3300 3100
Wire Wire Line
	7800 2700 7800 2800
Wire Wire Line
	4700 3900 4700 3700
Wire Wire Line
	3500 3500 3500 3700
Wire Wire Line
	3500 3700 3700 3700
Wire Wire Line
	3900 2800 3900 3000
Wire Wire Line
	3900 3000 4200 3000
Wire Wire Line
	4200 3000 4300 3000
Wire Wire Line
	4300 3000 4300 2800
Wire Wire Line
	4200 3000 4200 3200
Connection ~ 4200 3000
Wire Wire Line
	3900 2400 3900 2600
Wire Wire Line
	7800 2400 7800 2500
$Comp
L Device:R_Small R20
U 1 1 A5EEF4AE
P 4300 2700
AR Path="/A5EEF4AE" Ref="R20"  Part="1" 
AR Path="/5F5D6721/A5EEF4AE" Ref="R20"  Part="1" 
F 0 "R20" V 4350 2700 59  0000 C TNN
F 1 "10k" V 4250 2700 59  0000 C BNN
F 2 "rhais_rcl:R0603" H 4300 2700 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2860681.pdf" H 4300 2700 50  0001 C CNN
F 4 "Multicomp" H 4300 2700 59  0001 L BNN "MFR"
F 5 "MCWR06X1002FTL" H 4300 2700 59  0001 L BNN "MPN"
F 6 "2446547" H 4300 2700 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mcwr06x1002ftl/res-10k-1-0-1w-0603-thick-film/dp/2446547" H 4300 2700 50  0001 C CNN "URL_FARNELL"
	1    4300 2700
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_Small R21
U 1 1 9DD4332C
P 3900 2700
AR Path="/9DD4332C" Ref="R21"  Part="1" 
AR Path="/5F5D6721/9DD4332C" Ref="R21"  Part="1" 
F 0 "R21" V 3850 2700 59  0000 C BNN
F 1 "10k" V 3950 2700 59  0000 C TNN
F 2 "rhais_rcl:R0603" H 3900 2700 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2860681.pdf" H 3900 2700 50  0001 C CNN
F 4 "Multicomp" H 3900 2700 59  0001 L BNN "MFR"
F 5 "MCWR06X1002FTL" H 3900 2700 59  0001 L BNN "MPN"
F 6 "2446547" H 3900 2700 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mcwr06x1002ftl/res-10k-1-0-1w-0603-thick-film/dp/2446547" H 3900 2700 50  0001 C CNN "URL_FARNELL"
	1    3900 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C10
U 1 1 00242585
P 8300 2600
AR Path="/00242585" Ref="C10"  Part="1" 
AR Path="/5F5D6721/00242585" Ref="C10"  Part="1" 
F 0 "C10" H 8350 2700 59  0000 L CNN
F 1 "47uF" H 8350 2500 59  0000 L CNN
F 2 "rhais_rcl:C0805" H 8300 2600 50  0001 C CNN
F 3 "https://www.murata.com/en-eu/api/pdfdownloadapi?cate=luCeramicCapacitorsSMD&partno=GRM21BR61A476ME15%23" H 8300 2600 50  0001 C CNN
F 4 "muRata" H 8300 2600 59  0001 L BNN "MFR"
F 5 "GRM21BR61A476ME15L" H 8300 2600 59  0001 L BNN "MPN"
F 6 "2611939" H 8300 2600 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/murata/grm21br61a476me15l/cap-47-f-10v-20-x5r-0805/dp/2611939" H 8300 2600 50  0001 C CNN "URL_FARNELL"
	1    8300 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R23
U 1 1 18259785
P 3500 3400
AR Path="/18259785" Ref="R23"  Part="1" 
AR Path="/5F5D6721/18259785" Ref="R23"  Part="1" 
F 0 "R23" V 3450 3400 59  0000 C BNN
F 1 "10k" V 3550 3400 59  0000 C TNN
F 2 "rhais_rcl:R0603" H 3500 3400 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2860681.pdf" H 3500 3400 50  0001 C CNN
F 4 "Multicomp" H 3500 3400 59  0001 L BNN "MFR"
F 5 "MCWR06X1002FTL" H 3500 3400 59  0001 L BNN "MPN"
F 6 "2446547" H 3500 3400 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mcwr06x1002ftl/res-10k-1-0-1w-0603-thick-film/dp/2446547" H 3500 3400 50  0001 C CNN "URL_FARNELL"
	1    3500 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R24
U 1 1 AE0241E3
P 7800 2600
AR Path="/AE0241E3" Ref="R24"  Part="1" 
AR Path="/5F5D6721/AE0241E3" Ref="R24"  Part="1" 
F 0 "R24" V 7750 2600 59  0000 C BNN
F 1 "10k" V 7850 2600 59  0000 C TNN
F 2 "rhais_rcl:R0603" H 7800 2600 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2860681.pdf" H 7800 2600 50  0001 C CNN
F 4 "Multicomp" H 7800 2600 59  0001 L BNN "MFR"
F 5 "MCWR06X1002FTL" H 7800 2600 59  0001 L BNN "MPN"
F 6 "2446547" H 7800 2600 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mcwr06x1002ftl/res-10k-1-0-1w-0603-thick-film/dp/2446547" H 7800 2600 50  0001 C CNN "URL_FARNELL"
	1    7800 2600
	1    0    0    -1  
$EndComp
$Comp
L MXKeyboard-eagle-import:DMN65D8L Q1
U 1 1 7A211019
P 4100 3600
AR Path="/7A211019" Ref="Q1"  Part="1" 
AR Path="/5F5D6721/7A211019" Ref="Q1"  Part="1" 
F 0 "Q1" H 3800 3200 59  0000 L BNN
F 1 "DMN65D8L" V 4450 3400 59  0000 L BNN
F 2 "rhais_package-smd:SOT-23" H 4100 3600 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMN65D8L.pdf" H 4100 3600 50  0001 C CNN
F 4 "Diodes Inc" H 4100 3600 50  0001 C CNN "MFR"
F 5 "DMN65D8L" H 4100 3600 50  0001 C CNN "MPN"
F 6 "2543546" H 4100 3600 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/diodes-inc/dmn65d8l-7/mosfet-n-ch-20v-sot-23-3/dp/2543546" H 4100 3600 50  0001 C CNN "URL_FARNELL"
	1    4100 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R22
U 1 1 4920326A
P 3800 4200
AR Path="/4920326A" Ref="R22"  Part="1" 
AR Path="/5F5D6721/4920326A" Ref="R22"  Part="1" 
F 0 "R22" V 3850 4200 59  0000 C TNN
F 1 "10k" V 3750 4200 59  0000 C BNN
F 2 "rhais_rcl:R0603" H 3800 4200 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2860681.pdf" H 3800 4200 50  0001 C CNN
F 4 "Multicomp" H 3800 4200 59  0001 L BNN "MFR"
F 5 "MCWR06X1002FTL" H 3800 4200 59  0001 L BNN "MPN"
F 6 "2446547" H 3800 4200 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mcwr06x1002ftl/res-10k-1-0-1w-0603-thick-film/dp/2446547" H 3800 4200 50  0001 C CNN "URL_FARNELL"
	1    3800 4200
	0    -1   -1   0   
$EndComp
$Comp
L MXKeyboard-eagle-import:DMP2240UDM Q2
U 1 1 DA191B3C
P 3800 2000
AR Path="/DA191B3C" Ref="Q2"  Part="1" 
AR Path="/5F5D6721/DA191B3C" Ref="Q2"  Part="1" 
F 0 "Q2" H 3800 1800 59  0000 R TNN
F 1 "DMP2240UDM" V 4100 2050 59  0000 C TNN
F 2 "rhais_package-smd:SOT-26" H 3800 2000 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds31197.pdf" H 3800 2000 50  0001 C CNN
F 4 "Diodes Inc" H 3800 2000 50  0001 C CNN "MFR"
F 5 "DMP2240UDM" H 3800 2000 50  0001 C CNN "MPN"
F 6 "2543520" H 3800 2000 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/diodes-inc/dmp2240udm-7/mosfet-aec-q101-dual-p-ch-20v/dp/2543520" H 3800 2000 50  0001 C CNN "URL_FARNELL"
	1    3800 2000
	0    -1   -1   0   
$EndComp
$Comp
L MXKeyboard-eagle-import:DMP2240UDM Q2
U 2 1 DA191B30
P 7700 2000
AR Path="/DA191B30" Ref="Q2"  Part="2" 
AR Path="/5F5D6721/DA191B30" Ref="Q2"  Part="2" 
F 0 "Q2" H 7700 1800 59  0000 R TNN
F 1 "DMP2240UDM" V 8000 2050 59  0000 C TNN
F 2 "rhais_package-smd:SOT-26" H 7700 2000 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds31197.pdf" H 7700 2000 50  0001 C CNN
F 4 "Diodes Inc" H 7700 2000 50  0001 C CNN "MFR"
F 5 "DMP2240UDM" H 7700 2000 50  0001 C CNN "MPN"
F 6 "2543520" H 7700 2000 50  0001 C CNN "OC_FARNELL"
F 7 "https://uk.farnell.com/diodes-inc/dmp2240udm-7/mosfet-aec-q101-dual-p-ch-20v/dp/2543520" H 7700 2000 50  0001 C CNN "URL_FARNELL"
	2    7700 2000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R36
U 1 1 1A263169
P 7100 2600
AR Path="/1A263169" Ref="R36"  Part="1" 
AR Path="/5F5D6721/1A263169" Ref="R36"  Part="1" 
F 0 "R36" V 7050 2600 59  0000 C BNN
F 1 "10k" V 7150 2600 59  0000 C TNN
F 2 "rhais_rcl:R0603" H 7100 2600 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2860681.pdf" H 7100 2600 50  0001 C CNN
F 4 "Multicomp" H 7100 2600 59  0001 L BNN "MFR"
F 5 "MCWR06X1002FTL" H 7100 2600 59  0001 L BNN "MPN"
F 6 "2446547" H 7100 2600 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/multicomp/mcwr06x1002ftl/res-10k-1-0-1w-0603-thick-film/dp/2446547" H 7100 2600 50  0001 C CNN "URL_FARNELL"
	1    7100 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C33
U 1 1 1A1C7E04
P 6800 2600
AR Path="/1A1C7E04" Ref="C33"  Part="1" 
AR Path="/5F5D6721/1A1C7E04" Ref="C33"  Part="1" 
F 0 "C33" H 6850 2700 59  0000 L CNN
F 1 "47uF" H 6850 2500 59  0000 L CNN
F 2 "rhais_rcl:C0805" H 6800 2600 50  0001 C CNN
F 3 "https://www.murata.com/en-eu/api/pdfdownloadapi?cate=luCeramicCapacitorsSMD&partno=GRM21BR61A476ME15%23" H 6800 2600 50  0001 C CNN
F 4 "muRata" H 6800 2600 59  0001 L BNN "MFR"
F 5 "GRM21BR61A476ME15L" H 6800 2600 59  0001 L BNN "MPN"
F 6 "2611939" H 6800 2600 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/murata/grm21br61a476me15l/cap-47-f-10v-20-x5r-0805/dp/2611939" H 6800 2600 50  0001 C CNN "URL_FARNELL"
	1    6800 2600
	-1   0    0    -1  
$EndComp
Text HLabel 7800 2800 3    50   Input ~ 0
5V_USB
Text HLabel 6700 1900 0    50   Input ~ 0
5V_PS2
Text HLabel 4700 3700 1    50   Input ~ 0
5V_USB
Text HLabel 3200 3100 0    50   Input ~ 0
5V_USB
$Comp
L power:+5V #PWR061
U 1 1 71DF1DD9
P 4300 1800
F 0 "#PWR061" H 4300 1650 50  0001 C CNN
F 1 "+5V" H 4315 1973 50  0000 C CNN
F 2 "" H 4300 1800 50  0001 C CNN
F 3 "" H 4300 1800 50  0001 C CNN
	1    4300 1800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR063
U 1 1 71DF221A
P 8300 1800
F 0 "#PWR063" H 8300 1650 50  0001 C CNN
F 1 "+5V" H 8315 1973 50  0000 C CNN
F 2 "" H 8300 1800 50  0001 C CNN
F 3 "" H 8300 1800 50  0001 C CNN
	1    8300 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR062
U 1 1 71DF2914
P 7700 3300
F 0 "#PWR062" H 7700 3050 50  0001 C CNN
F 1 "GND" H 7705 3127 50  0000 C CNN
F 2 "" H 7700 3300 50  0001 C CNN
F 3 "" H 7700 3300 50  0001 C CNN
	1    7700 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR060
U 1 1 71DF3A94
P 4200 4300
F 0 "#PWR060" H 4200 4050 50  0001 C CNN
F 1 "GND" H 4205 4127 50  0000 C CNN
F 2 "" H 4200 4300 50  0001 C CNN
F 3 "" H 4200 4300 50  0001 C CNN
	1    4200 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C32
U 1 1 0908C5D9
P 4700 4000
AR Path="/0908C5D9" Ref="C32"  Part="1" 
AR Path="/5F5D6721/0908C5D9" Ref="C32"  Part="1" 
F 0 "C32" H 4750 4100 59  0000 L CNN
F 1 "47uF" H 4750 3900 59  0000 L CNN
F 2 "rhais_rcl:C0805" H 4700 4000 50  0001 C CNN
F 3 "https://www.murata.com/en-eu/api/pdfdownloadapi?cate=luCeramicCapacitorsSMD&partno=GRM21BR61A476ME15%23" H 4700 4000 50  0001 C CNN
F 4 "muRata" H 4700 4000 59  0001 L BNN "MFR"
F 5 "GRM21BR61A476ME15L" H 4700 4000 59  0001 L BNN "MPN"
F 6 "2611939" H 4700 4000 59  0001 L BNN "OC_FARNELL"
F 7 "https://uk.farnell.com/murata/grm21br61a476me15l/cap-47-f-10v-20-x5r-0805/dp/2611939" H 4700 4000 50  0001 C CNN "URL_FARNELL"
	1    4700 4000
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Small TP1
U 1 1 5F6FCA50
P 5500 3000
F 0 "TP1" H 5548 3046 50  0000 L CNN
F 1 "5V USB" H 5500 2950 39  0000 C TNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 5700 3000 50  0001 C CNN
F 3 "" H 5700 3000 50  0001 C CNN
	1    5500 3000
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Small TP2
U 1 1 5F6FE1CF
P 5750 3000
F 0 "TP2" H 5798 3046 50  0000 L CNN
F 1 "+5V" H 5750 2950 39  0000 C TNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 5950 3000 50  0001 C CNN
F 3 "" H 5950 3000 50  0001 C CNN
	1    5750 3000
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Small TP3
U 1 1 5F6FEBD4
P 6000 3000
F 0 "TP3" H 6048 3046 50  0000 L CNN
F 1 "5V PS2" H 6000 2950 39  0000 C TNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6200 3000 50  0001 C CNN
F 3 "" H 6200 3000 50  0001 C CNN
	1    6000 3000
	1    0    0    -1  
$EndComp
Text HLabel 6000 2900 1    50   Input ~ 0
5V_PS2
$Comp
L power:+5V #PWR064
U 1 1 5F700442
P 5750 2900
F 0 "#PWR064" H 5750 2750 50  0001 C CNN
F 1 "+5V" H 5765 3073 50  0000 C CNN
F 2 "" H 5750 2900 50  0001 C CNN
F 3 "" H 5750 2900 50  0001 C CNN
	1    5750 2900
	1    0    0    -1  
$EndComp
Text HLabel 5500 2900 1    50   Input ~ 0
5V_USB
Wire Wire Line
	5500 2900 5500 3000
Wire Wire Line
	5750 2900 5750 3000
Wire Wire Line
	6000 2900 6000 3000
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5FB49264
P 8400 1900
F 0 "#FLG0101" H 8400 1975 50  0001 C CNN
F 1 "PWR_FLAG" V 8400 2028 50  0000 L CNN
F 2 "" H 8400 1900 50  0001 C CNN
F 3 "~" H 8400 1900 50  0001 C CNN
	1    8400 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	8400 1900 8300 1900
$EndSCHEMATC
