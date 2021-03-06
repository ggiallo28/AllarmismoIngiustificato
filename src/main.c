#include "main.h"



int
main(int argc, char* argv[])
{

	/************** VARIABLE DECLARATION */
	KeyStruct keyPadState;
	AlarmStruct state;
	EdgeStruct sense;
	char code[CODE_DIM+1];
	DEFAULT_CODE(code);
	bool isEquals;

	/************* INIT ***************/
	keypad_init(&keyPadState);
	timer_start();
	sense_init(&sense);
	alarm_init(&state);
	lcd_init();
	buzzer_init();

	/************** INFINITE LOOP *****/
	HD44780_ClrScr();
	HD44780_GotoXY(0,0);
	HD44780_PutStr("SALVE PRF FOGGIA");
	HD44780_GotoXY(0,1);
	HD44780_PutStr("Alarm System 1.0");
	keypad_flush(&keyPadState);

	while(1){
		state.eventsArray[MAGN1] = magn_one_read(&sense);
		state.eventsArray[MAGN2] = magn_two_read(&sense);
		state.eventsArray[MOVE1] = move_one_read(&sense);
		state.eventsArray[MOVE2] = move_two_read(&sense);
		state.eventsArray[MOVE3] = move_three_read(&sense);
		state.eventsArray[MOVE4] = move_four_read(&sense);
		state.eventsArray[KEYPAD] = get_code(&keyPadState);

		/******** Update State ***********/
		isEquals = true;

		if(state.eventsArray[KEYPAD]){
			pin_sound();
			int i=0;
			HD44780_ClrScr();
			HD44780_GotoXY(0,0);
			HD44780_PutStr("PIN:");
			HD44780_GotoXY(0,1);
			alarm_off();

			while(keyPadState.code[i] != '\0' && code[i] != '\0'){
				HD44780_PutChar(keyPadState.code[i]);
				(isEquals & (keyPadState.code[i] == code[i])) ? (isEquals = true) : (isEquals = false);
				i++;
			}

			if(isEquals){
				state.isActive = !state.isActive;
				keypad_flush(&keyPadState);

				if(state.isActive){
					HD44780_ClrScr();
					HD44780_GotoXY(0,0);
					HD44780_PutStr("ALLARME");
					HD44780_GotoXY(0,1);
					HD44780_PutStr("ATTIVATO");
				}else{
					HD44780_ClrScr();
					HD44780_GotoXY(0,0);
					HD44780_PutStr("ALLARME");
					HD44780_GotoXY(0,1);
					HD44780_PutStr("DISATTIVATO");
				}
			}
			if(!isEquals && keyPadState.index == CODE_DIM){
				HD44780_GotoXY(0,1);
				HD44780_PutStr("ERRATO");
				keypad_flush(&keyPadState);
			}
		}
		/******** Generate Outputs *******/
		if(state.eventsArray[MAGN1] && state.isActive){
			state.isRinging = true;
			HD44780_ClrScr();
			HD44780_GotoXY(0,0);
			HD44780_PutStr("VIOLAZIONE");
			HD44780_GotoXY(0,1);
			HD44780_PutStr("PORTA 1");
		}
		if(state.eventsArray[MAGN2] && state.isActive){
			state.isRinging = true;
			HD44780_ClrScr();
			HD44780_GotoXY(0,0);
			HD44780_PutStr("VIOLAZIONE");
			HD44780_GotoXY(0,1);
			HD44780_PutStr("PORTA 2");
		}
		if(state.eventsArray[MOVE1] && state.isActive){
			state.isRinging = true;
			HD44780_ClrScr();
			HD44780_GotoXY(0,0);
			HD44780_PutStr("VIOLAZIONE");
			HD44780_GotoXY(0,1);
			HD44780_PutStr("STANZA 1");
		}
		if(state.eventsArray[MOVE2] && state.isActive){
			state.isRinging = true;
			HD44780_ClrScr();
			HD44780_GotoXY(0,0);
			HD44780_PutStr("VIOLAZIONE");
			HD44780_GotoXY(0,1);
			HD44780_PutStr("STANZA 2");
		}
		if(state.eventsArray[MOVE3] && state.isActive){
			state.isRinging = true;
			HD44780_ClrScr();
			HD44780_GotoXY(0,0);
			HD44780_PutStr("VIOLAZIONE");
			HD44780_GotoXY(0,1);
			HD44780_PutStr("STANZA 3");
		}
		if(state.eventsArray[MOVE4] && state.isActive){
			state.isRinging = true;
			HD44780_ClrScr();
			HD44780_GotoXY(0,0);
			HD44780_PutStr("VIOLAZIONE");
			HD44780_GotoXY(0,1);
			HD44780_PutStr("STANZA 4");
		}

		if(!state.isActive){
			alarm_off();
			state.isRinging = false;
		}

		if(state.isRinging)
			alarm_on();

		if(keyPadState.prevChar == ENTER_CHAR)
			change_pin(code, &keyPadState);

		timer_sleep(1);
	}
}

// ----------------------------------------------------------------------------

