#include "main.h"

const int armthing[10] = {1,2,3,4,5,6,7,8,9,10}, cbendpos = 100, cbstartpos = 0, cb_pwm = 127, arm_const = 127, leftfront = 1,
lefttback = 3, rightfront = 10, rightback = 7, rightarm = 2, leftarm = 9, mobilegoalright = 4, mobilegoalleft = 8, clawleft = 5, clawright = 6,
stop = 0, maxmenus = 6, bound = 100, larmpot = 5, rarmpot = 2, cbpot = 3, clawpot = 1, rightshaft = 1, leftshaft = 3;
int menu = 2;
bool overrided = true;
struct motor{int m1, m2, m3, m4, m5, m6, m7, m8, m9, m10;}motor;
struct dgtl{bool d1, d3;}dgtl;
struct last{bool arm, override, su, sd, co, cc, mo, mc; int lj;}last;
void lcdmenu(){
	int lcdjoystick = joystickGetAnalog(1, 4), test = millis();;
	if(bound <= abs(lcdjoystick) && bound > last.lj){menu = (menu + maxmenus + lcdjoystick/abs(lcdjoystick) % maxmenus);}
	last.lj = abs(lcdjoystick);
	switch(menu){
		case 1:lcdPrint(uart1, 1, "main bat %d", powerLevelMain());lcdPrint(uart1, 2, "back-up bat %d", powerLevelMain());
		case 2:lcdPrint(uart1, 1, "L Arm Pot %d", analogRead(larmpot));lcdPrint(uart1, 2, "R Arm Pot %d", analogRead(rarmpot));
		case 3:lcdPrint(uart1, 1, "CLAW POT %d", analogRead(clawpot));lcdPrint(uart1, 2, "run time %d", test);
		//case 4:lcdPrint(uart1, 1, "Stack %d", stack); lcdPrint(uart1, 2, "                ");
		case 5:lcdPrint(uart1, 1, "   r8 my code   ");lcdPrint(uart1, 2, "  954-849-1442  ");
		case 6:lcdPrint(uart1, 1, "Steve Coded This");lcdPrint(uart1, 2, "       :)       ");
		case 7:lcdPrint(uart1, 1, "  L ENCODER %d ", analogRead(leftshaft));lcdPrint(uart1, 2, " R ENCODER %d", analogRead(rightshaft));
		default:lcdPrint(uart1, 1,"      Error     ");lcdPrint(uart1, 2, "    No Screen   ");}}

void drive(int left, int right){motor.m1 = -left; motor.m3 = -left; motor.m10 = -right; motor.m7 = right;}
void arm(int pwm){motor.m2 = -pwm; motor.m9 = pwm;}
//void cb(int pwm){motor.m8 = pwm; motor.m5 = pwm;}
void MOGO(int pwm){motor.m4 = pwm; motor.m8 = -pwm;}
void claw(int pwm){motor.m5 = -pwm; motor.m6 = pwm;}
void update(){
	motorSet(1, motor.m1); motorSet(2, motor.m2); motorSet(3, motor.m3); motorSet(4, motor.m4); motorSet(5, motor.m5);
	motorSet(6, motor.m6); motorSet(7, motor.m7); motorSet(8, motor.m8); motorSet(9, motor.m9); motorSet(10, motor.m10);}

void potentarm(int pwm, int potent, int potentval){
	int nop = (potent - potentval)/abs(potent - potentval);
	while(potentval != potent){arm(pwm * nop);}	arm(stop);}


/*
void potentcb(int pwm, int potent, int potentval){
	int nop = (potent - potentval)/abs(potent - potentval);
	while(potentval != potent){cb(pwm * nop);} cb(stop);}
*?

void scoredatcone(int x){
	claw(true);
	potentarm(arm_const, armthing[x], analogRead(rarmpot));
	potentcb(cb_pwm, cbendpos, analogRead(cbpot));
	claw(false);
	potentcb(cb_pwm, cbstartpos, analogRead(cbpot));
	potentarm(arm_const, armthing[0], analogRead(rarmpot));} */


int previous;
int pid(int Kp, int Kd, int val, int currentval)
{
	int error = val - currentval;
	int d = error - previous;
	int x = Kp * error + Kd * d;
	return x;
	previous = error;
}


void controller(){
	int LeftJoyStick = joystickGetAnalog(1, 2), RightJoyStick = joystickGetAnalog(1, 3);
	bool ClawOpen = joystickGetDigital(1, 5, JOY_UP), armopen = joystickGetDigital(1, 6, JOY_UP), armclose = joystickGetDigital(1, 6, JOY_DOWN),
	mogoopen = joystickGetDigital(1, 8, JOY_UP), mogoclose = joystickGetDigital(1, 8, JOY_DOWN);
	//stackup = joystickGetDigital(2, 6, JOY_UP), stackdown = joystickGetDigital(2, 6, JOY_DOWN),
	//cbup = joystickGetDigital(1, 5, JOY_UP), cbdown = joystickGetDigital(1, 5, JOY_DOWN),
	//override = joystickGetDigital(2, 5, JOY_UP);
	if(ClawOpen)
	{
		if(analogRead(clawpot) > 1400)
		{
			claw(30);
		}
		claw(127);}
	else if(analogRead(clawpot) < 1401){claw(-20);}
	else{claw(0);}
	if(armopen){arm(127);} else if(armclose){arm(-127);} else{arm(0);};
	if(mogoopen){MOGO(-127);} else if(mogoclose){MOGO(127);} else{MOGO(0);};

	drive(pid( 1, 1, LeftJoyStick, encoderGet(LeftEnc)),pid( 1, 1, RightJoyStick, encoderGet(RightEnc)));}

void recordcode(){
	digitalWrite(5, LOW);
	for(int t = 0; t < 153; t++){
		controller(); update();
		auton[t].m1 = motor.m1; auton[t].m2 = motor.m2; auton[t].m3 = motor.m3;	auton[t].m4 = motor.m4; auton[t].m5 =	motor.m5;
		auton[t].m6 = motor.m6;	auton[t].m7 = motor.m7;	auton[t].m8 = motor.m8;	auton[t].m9 = motor.m9;	auton[t].m10 = motor.m10;
			wait(100);}
		digitalWrite(5, HIGH);
	}

void reruncode(){ // totally not a stall code ;) hopefully they dont read this again
	for(int i = 0; i < 153; i++){
		motor.m1 = auton[i].m1;	motor.m2 = auton[i].m2;	motor.m3 = auton[i].m3;	motor.m4 = auton[i].m4;	motor.m5 = auton[i].m5;
		motor.m6 = auton[i].m6;	motor.m7 = auton[i].m7;	motor.m8 = auton[i].m8;	motor.m9 = auton[i].m9;	motor.m10 = auton[i].m10;
		update(); wait(10);}}

void savecode(){
	printf("\nint motor1[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m1);}printf("};");wait(1000);
	printf("\nint motor2[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m2);}printf("};");wait(1000);
	printf("\nint motor3[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m3);}printf("};");wait(1000);
	printf("\nint motor4[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m4);}printf("};");wait(1000);
	printf("\nint motor5[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m5);}printf("};");wait(1000);
	printf("\nint motor6[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m6);}printf("};");wait(1000);
	printf("\nint motor7[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m7);}printf("};");wait(1000);
	printf("\nint motor8[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m8);}printf("};");wait(1000);
	printf("\nint motor9[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m9);}printf("};");wait(1000);
	printf("\nint motor10[buffer] = {");for(int n = 0; n < buffer; n++){printf("%d,", auton[n].m10);}printf("};");wait(1000);}

void operatorControl(){

	TaskHandle UPDATE = taskRunLoop(update, 20);
	TaskHandle CONTROLLER = taskRunLoop(controller, 20);

	while (1){
		lcdmenu();
		bool record = joystickGetDigital(1, 7, JOY_UP), rerun = joystickGetDigital(1, 7, JOY_DOWN), save = joystickGetDigital(1, 7, JOY_LEFT);
		if(record){taskSuspend(CONTROLLER);recordcode();taskResume(CONTROLLER);}
		else if(rerun){taskSuspend(CONTROLLER);reruncode();taskResume(CONTROLLER);}
		else if(save){taskSuspend(CONTROLLER);savecode();taskResume(CONTROLLER);}
		delay(20);}
	taskDelete(CONTROLLER);
	taskDelete(UPDATE);}
