#include <VGA.h>
#include"win_help.h"
#include"win_data.h"
#include"win_options.h"
#include"win_operation.h"
#include"win_train.h"
#include"win_result.h"
#include"win_mess.h"
#include"longtime.h"
#include "Adafruit_GFX.h"
#include "SEPS525_OLED.h"

#include <SPI.h>
#include <SD.h>
//#include"data_manager.h"


// Require keyboard control library
#include <KeyboardController.h>
//
extern char cp1251_en_to_ru[];

// Initialize USB Controller
USBHost usb;

// Attach keyboard controller to USB
KeyboardController keyboard(usb);

Keyboard_Handler* pw;

Temporary_Data Temp_D;

Data_Manager Data_Mngr(Temp_D);

CLongTimer TMR1(1000);
int count = 1;

SEPS525_OLED tft;

// This function intercepts key press
void keyPressed()
{
	Process();
}

// This function intercepts key release
void keyReleased()
{

}
void change_wind(void);

void Process() 
{

	// getModifiers() returns a bits field with the modifiers-keys
	int mod = keyboard.getModifiers();

	if ((mod & LeftCtrl) || (mod & RightCtrl))
	{
		switch (keyboard.getKey())
		{
		case	49:	//"1"
			(* pw).action_button1();
			break;

		case	50:	//"2"
			(* pw).action_button2();
			break;

		case	51:	//"3"
			(* pw).action_button3();
			break;

		case	52:	//"4"
			(* pw).action_button4();
			break;

		case	53:	//"5"
			(* pw).action_button5();
			break;

		case	54:	//"6"
			(* pw).action_button6();
			break;

		case	55:	//"7"
			(* pw).action_button7();
			break;

		case	56:	//"8"
			(* pw).action_button8();
			break;

		case	57:	//"9"
			(* pw).action_button9();
			break;

		default:
			break;
		}
	}
	else
	{
		switch (keyboard.getOemKey())
		{
		case	58:	//"F1"
			(* pw).action_buttonF1();
			break;

		case	59:	//"F2"
			(* pw).action_buttonF2();
			break;

		case	60:	//"F3"
			(* pw).action_buttonF3();
			break;

		case	61:	//"F4"
			(* pw).action_buttonF4();
			break;

		case	62:	//"F5"
			(* pw).action_buttonF5();
			break;

		case	79:	//"Right point"
			(* pw).action_button_rigth_point();
			break;

		case	80:	//"Left point"
			(* pw).action_button_left_point();
			break;

		default:
			(* pw).set_word(cp1251_en_to_ru[keyboard.getKey()]);
			break;
		}

	}

	change_wind();
	change_wind();//����� �������� ����� ����������, � �� ����� ��� ������ ������� �� ������

	// getKey() returns the ASCII translation of OEM key
	// combined with modifiers.cp1251_en_to_ru[keyboard.getKey()]);
}

void send_win_end(char *XstrX)
{
	XstrX[0] = 208; //�
	XstrX[1] = 229; //�
	XstrX[2] = 231; //�
	XstrX[3] = 243; //�
	XstrX[4] = 235; //�
	XstrX[5] = 252; //�
	XstrX[6] = 242; //�
	XstrX[7] = 224; //�
	XstrX[8] = 242; //�
	XstrX[9] = 32; // 
	XstrX[10] = 241; //�
	XstrX[11] = 238; //�
	XstrX[12] = 245; //�
	XstrX[13] = 240; //�
	XstrX[14] = 224; //�
	XstrX[15] = 237; //�
	XstrX[16] = 229; //�
	XstrX[17] = 237; //�
	XstrX[18] = 0; // New str
}
void send_wind_error_init_SD(char *XstrX)
{
	XstrX[0] = 202; //�
	XstrX[1] = 224; //�
	XstrX[2] = 240; //�
	XstrX[3] = 242; //�
	XstrX[4] = 224; //�
	XstrX[5] = 32; // 
	XstrX[6] = 239; //�
	XstrX[7] = 224; //�
	XstrX[8] = 236; //�
	XstrX[9] = 255; //�
	XstrX[10] = 242; //�
	XstrX[11] = 232; //�
	XstrX[12] = 32; // 
	XstrX[13] = 237; //�
	XstrX[14] = 229; //�
	XstrX[15] = 32; // 
	XstrX[16] = 243; //�
	XstrX[17] = 241; //�
	XstrX[18] = 242; //�
	XstrX[19] = 224; //�
	XstrX[20] = 237; //�
	XstrX[21] = 238; //�
	XstrX[22] = 226; //�
	XstrX[23] = 235; //�
	XstrX[24] = 229; //�
	XstrX[25] = 237; //�
	XstrX[26] = 224; //�
	XstrX[27] = 32; // 
	XstrX[28] = 232; //�
	XstrX[29] = 235; //�
	XstrX[30] = 232; //�
	XstrX[31] = 32; // 
	XstrX[32] = 237; //�
	XstrX[33] = 229; //�
	XstrX[34] = 232; //�
	XstrX[35] = 241; //�
	XstrX[36] = 239; //�
	XstrX[37] = 240; //�
	XstrX[38] = 224; //�
	XstrX[39] = 226; //�
	XstrX[40] = 237; //�
	XstrX[41] = 224; //�
	XstrX[42] = 0; // New str
}

void change_wind()
{
	static id_wind handler_num = id_wind_data;
	char _str[256];
	if ((*pw).get_jump())
	{
		id_wind pre_handler_num = (*pw).get_handler();
		(*pw).finit();
		switch ((*pw).get_handler())
		{
		case id_wind_help:
			Serial.println("Go to help");
			delete pw;
			pw = new Win_Help_Handler(&Temp_D, Data_Mngr);
			(*pw).set_back_handler(handler_num);
			break;
		case id_wind_data:
			Serial.println("Go to data");
			delete pw;
			pw = new Win_Data_Handler(&Temp_D, Data_Mngr);
			break;
		case id_wind_option:
			Serial.println("Go to option");
			delete pw;
			pw = new Win_Option_Handler(&Temp_D, Data_Mngr);
			break;
		case id_wind_oper:
			Serial.println("Go to oper");
			delete pw;
			pw = new Win_Oper_Handler(&Temp_D, Data_Mngr, tft);
			break;
		case id_wind_train:
			Serial.println("Go to train");
			delete pw;
			pw = new Win_Train_Handler(&Temp_D, Data_Mngr);
			break;
		case id_wind_result:
			Serial.println("Go to result");
			delete pw;
			pw = new Win_Result_Handler(&Temp_D, Data_Mngr);;
			break;

		case id_wind_end:
			Serial.println("Go to mess of the end");
			delete pw;
			pw = new Win_Mess_Handler(&Temp_D, Data_Mngr);
			(*pw).set_back_handler(id_wind_data);

			send_win_end(_str);

			(*pw).push_message(_str);

			break;
		case id_wind_error_init_SD_wind:
			Serial.println("Go to mess of the end");
			delete pw;
			pw = new Win_Mess_Handler(&Temp_D, Data_Mngr);
			(*pw).set_back_handler(handler_num);

			send_wind_error_init_SD(_str);

			(*pw).push_message(_str);

			break;
			/*
		case id_wind_error_verif_data_mngr:
			Serial.println("Go to mess of the end");
			if (false)
			{
				(*pw).set_back_handler(handler_num);
			}

			char _str_error_verif_data_mngr[] = { 208, 229, 231, 243, 235, 252, 242,
				224, 242, 32, 241, 238, 245, 240, 224,
				237, 229, 237, 0 };//��������� ��������
			(*pw).push_message(_str_error_verif_data_mngr);
			break;*/
		default:
			Serial.println("Ooops!"); 
			break;
		}
		(*pw).init();
		handler_num = pre_handler_num;
	}
}

void refresh_timer()
{
	(*pw).refresh_timer();
}
void setup()
{
	Serial.begin(9600);
	VGA.begin(320, 240, VGA_COLOUR);

	TMR1.event(refresh_timer);
	TMR1.start();
	tft.begin();
	pw = new Win_Data_Handler(&Temp_D, Data_Mngr);//This window is first
	(*pw).init();

}
const int deley_t = 100;

int i_Loop = 0;
void loop()
{
	TMR1.tick();

	// Process USB tasks
	usb.Task();



	//count++;
	//Serial.println(count);
	////delay(100);
	//(*pw).finit();
	//delete pw;
	//pw = new Win_Help_Handler(&Temp_D, Data_Mngr);
	//(*pw).init();
	//(*pw).finit();
	//delete pw;
	//pw = new Win_Data_Handler(&Temp_D, Data_Mngr);
	//(*pw).init();
	//(*pw).finit();
	//delete pw;
	//pw = new Win_Option_Handler(&Temp_D, Data_Mngr);
	//(*pw).init();
	//(*pw).finit();
	//delete pw;
	//pw = new Win_Oper_Handler(&Temp_D, Data_Mngr);
	//(*pw).init();
	//(*pw).finit();
	//delete pw;
	//pw = new Win_Train_Handler(&Temp_D, Data_Mngr);
	//(*pw).init();
	//(*pw).finit();
	//delete pw;
	//pw = new Win_Result_Handler(&Temp_D, Data_Mngr);
	//(*pw).init();
	//(*pw).finit();
	//delete pw;
	//pw = new Win_Mess_Handler(&Temp_D, Data_Mngr);

	//(*pw).init();
}
