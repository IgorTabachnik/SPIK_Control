#ifndef MODBUSREGS_H
#define MODBUSREGS_H
//Copy past form UsingModBus.h project BK for STM32
//ModBus map of registers
////////////////////////////_______________WORK SPACE_____________///////////////////////////////////////////

/*-------------------------------Holding Registers---------------------------------------------------------*/
//Configuration of MKM
#define RegNameDevice 0//2 byte = type Device BK/OP, 3 byte = Slave ID

/*Bit/MKM	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
 * 			x	x	x	x	x	x	x	y	y	y	y	y	y	y	y	y
 * y=1 if MKM was installed else 0
 * */


//OpenShortTest feture measure val out
#define MV_Out_enable 4

#define RegConfigOpenShortSave 99
//AD5522
/*		VcmpH	VcmpL	VclampH	VclampL	Ios
 * IO	2reg	2reg	2reg	2reg	2reg total 10reg
 * RB
 * ENI
 * ENO
 * Vref
 * Vpp											total 60 regs
 * */
#define RegOpenShortTabelBegin 100 //from 100 to 159 reg

#define RegControlAD5522 160 //launch openshort test

/*Number AD5522			15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
 * Connect/Disconnect	x	x	x	y13	y12	y11	y10	y9	y8	y7	y6	y5	y4	y3	y2	y1
 * x-not use
 * y1 to y13 corresponds to AD5522_N where N from 1 to 13
 * yx = 1 if AD5522 is connect else AD5522 disconnect
 * */
#define RegStatusAD5522 161

/*
 * Table of results of tests consist of 7 regs in format:
 * -2-1-		+0 in begin reg
 * -4-3-		+1
 * -6-5-		+2
 * -8-7-		+3
 * -10-9-		+4
 * -12-11-		+5
 * -13-notuse-	+6
 * total of regs is 7
 * -2-1- this is register, which corresponds to AD5522_2 and AD5522_1. format of -2-1- is
 * ----------------------2------------------------	|	----------------------1------------------------
 * CPOL3 CPOH3 CPOL2 CPOH2 CPOL1 CPOH1 CPOL0 CPOH0	|	CPOL3 CPOH3 CPOL2 CPOH2 CPOL1 CPOH1 CPOL0 CPOH0
 * */
#define RegCompareAD5522Begin 162 //size of table is 7

/*
 * Table of alarms. Consist of 13 regs for each AD5522_N
 * Format of Register is:
 * LTMPALM TMPALM LC3 C3 LC2 C2 LC1 C1 LC0 C0
 * */
#define RegAlarmAD5522Begin 169

#define RegStatisticAD5522Begin 170

//LTM4675
#define RegFreePMBUS 171

//____________UpdateFPGAFirmware
#define FPGAFlashUpdateControl 400
#define FPGAFlashStartAddr 401
#define FPGAFlashSizeByte 449
#define FPGAUpdataFifo 450 //256byte = 128 reg 450-577


//Func table
#define RegInitFuncTest 550
#define RegUpdateFuncTable 580
#define RegSizeFuncTable 585

#define RegStartFuncTest 560
#define RegFinishFuncTest 570

/*
 * uint16_t cmd
 * uint16_t Vcc_L
 * uint16_t Vcc_H
 * uint16_t VccQ_L
 * uint16_t VccQ_H
 * uint16_t Temp_L
 * uint16_t Temp_H
 * uitn16_t pattern
 * 1cmd = 8 regs*/
#define RegFuncTestTable 590//50 commands in one table

#define RegUpdateFPGAOpt 990
#define RegSelectMKM 995
#define RegFPGAOptStruct 1000

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------Debug space-----------------------------------------------*/

///////////////////////////////////___________HOLDING REGISTER___________/////////////////////////////////
//_________________FPGA
#define RegOperation 0
#define Addr0 1
#define Addr1 2
#define ValueWr 3
#define CountWrRd 4
#define PWR_FPGA_ONN_OFF 5//2-ON 1-OFF 0-Nothing to do
#define Prog_FPGA 6
#define Reset_FPGA 7
#define ArrayWr 8 //from 8 to 130

//_________________LTM4675
#define DebugLTM_wVcc 900
#define DebugLTM_rVcc 902

#define DebugLTM_Ch0Err 904
#define DebugLTM_Ch1Err 905

#define DebugLTM_wVccQ 910
#define DebugLTM_rVccQ 912

#define DebugLTM_Wr 920
#define DebugLTM_Rd 921
#define DebugLTM_Number 922

//_________________AD5522
#define PWR_ON_AD5522 201
#define UpdateAD5522 202
#define MVoutAD5522 203
#define RdCmpStatus 204
#define RdAlmStatus 205
#define ClearLtchBits 206
#define selPMU 207
#define selChannels 208
#define selMKM 209
#define changeMKM 210

/*
 * 		Force	Compare		OutVal		CLL		CLH		CMP_L	CMP_H
 * DUT0 1 reg	1 reg		2reg		2reg	2reg	2reg	2reg   12regs
 * DUT1
 * DUT2
 * DUT3
 * total 48 reg
 * */
#define DebugRegOpenShortTabelBegin 211 //- 258
#define SelRsenseAD5522 259

//__________________FAN
#define RegFan 172
#define RegPWM_F 173
#define RegPWM_D 175

//_________________MUXandRelays
#define RegManualControlMuxAndRelays 260

/*--------------------------------------------WORK SPACE------------------------------------------------------*/

///////////////////////////////////___________INPUT REGISTER___________/////////////////////////////////////
#define OpenShortTestResultBegin 500//Array of connects field Malfunction size 13*8=104 registers
#define OpenShortTestStatus 604
#define OSTestMeasureVout 605 //13*4 - 3 = 49 values in 98 registers 605-702

#define RegTabelDeviceDS18B20 704// 704-735 32reg
#define RegTempArrDS18B20 736// 736-751  16 reg

#define LTM4675StatusAll 752// 606-FPGA_LTM 607-LTM1..615-LTM8

#define FPGAUpdateState 500//0-Update disable, 1-Update enable, 2-Ready to receive next

#define LTMFPGATemp 800

//FuncTest
#define RegCountMKM 197
#define RegFuncTestStatus 198
#define RegFuncTestCurrentInst  199
#define RegFuncTestResult 200//8 Regs

/*--------------------------------------------DEBUG SPACE-----------------------------------------------------*/

///////////////////////////////////___________INPUT REGISTER___________/////////////////////////////////////

//_________LTM4675
#define ValidRd 0
#define SuccessWr 1
#define ValueRd 2

//LTM_FPGA
#define VccVal 3//consist of 2 words
#define VccQVal 5//consist of 2 words
#define IccVal 7//consist of 2 words
#define IccQVal 9//consist of 2 words
#define ErrorVcc 11
#define ErrorVccQ 12

#define MKM8VccVal 13//consist of 2 words
#define MKM8VccQVal 15//consist of 2 words
#define MKM8IccVal 17//consist of 2 words
#define MKM8IccQVal 19//consist of 2 words
#define MKM8ErrorVcc 21
#define MKM8ErrorVccQ 22
#define ArrayRd 100

//STATE FPGA
#define INIT_B_pin 23
#define DONE_pin 24
#define Error_pin 25
#define Ready_pin 26

//AD5522
#define AD5522CmpStatus 200
#define AD5522AlmStatus 201
#define MVOutDuts 202 //2regs
#define AD5522InitStatus 220
#define SPIAD5522Status 221

//FAN
#define RegTahFreq 170
#define RegFANRPM  172

/*

/*--------------------------------------------DEBUG SPACE-----------------------------------------------------*/

////////////////////////////////////////------Coils Bits-----///////////////////////////////////////////////////
#define CoilMKMPWR1 0
#define CoilMKMPWR2 1
#define CoilMKMPWR3 2
#define CoilMKMPWR4 3
#define CoilMKMPWR5 4
#define CoilMKMPWR6 5
#define CoilMKMPWR7 6
#define CoilMKMPWR8 7

#define CoilVrefRel1 8
#define CoilVrefRel2 9
#define CoilVrefRel3 10
#define CoilVrefRel4 11
#define CoilVrefRel5 12
#define CoilVrefRel6 13
#define CoilVrefRel7 14
#define CoilVrefRel8 15

#define CoilMUXPWR1 16
#define CoilMUXPWR2 17
#define CoilMUXPWR3 18
#define CoilMUXPWR4 19
#define CoilMUXPWR5 20
#define CoilMUXPWR6 21
#define CoilMUXPWR7 22
#define CoilMUXPWR8 23

#define CoilMUXSel1 24
#define CoilMUXSel2 25
#define CoilMUXSel3 26
#define CoilMUXSel4 27
#define CoilMUXSel5 28
#define CoilMUXSel6 29
#define CoilMUXSel7 30
#define CoilMUXSel8 31

////////////////////////////////////////------Discret Bits-----///////////////////////////////////////////////////
#define DiscPortH 0//0-15
#define DiscPortC 16//16-31
#define DiscPortE 32//32-47
//Copy end
#endif // MODBUSREGS_H
