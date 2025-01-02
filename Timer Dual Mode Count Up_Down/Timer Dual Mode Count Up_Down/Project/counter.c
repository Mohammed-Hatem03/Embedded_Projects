//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
//
//unsigned char mode = 1, mode_flag = 0;
//unsigned char hrs_increment_flag = 0, hrs_decrement_flag = 0;
//unsigned char min_increment_flag = 0, min_decrement_flag = 0;
//unsigned char sec_increment_flag = 0, sec_decrement_flag = 0;
//unsigned char secUnits = 0, secTens = 0;
//unsigned char minUnits = 0, minTens = 0;
//unsigned char hrsUnits = 0, hrsTens = 0;
//
//ISR (TIMER1_COMPA_vect) {
//	if (mode == 1) {
//		PORTD |= (1 << PD4);
//		PORTD &= (~(1 << PD5));
//		PORTD &= (~(1 << PD0));
//		if (secUnits >= 9) {
//			secUnits = 0;
//			secTens++;
//			if (secTens >= 6) {
//				secTens = 0;
//				minUnits++;
//				if (minUnits >= 9) {
//					minUnits = 0;
//					minTens++;
//					if (minTens >= 6) {
//						minTens = 0;
//						hrsUnits++;
//						if (hrsUnits >= 9) {
//							hrsUnits = 0;
//							hrsTens++;
//							if (hrsTens >= 6) {
//								hrsTens = 5;
//								hrsUnits = 9;
//								minTens = 5;
//								minUnits = 9;
//								secTens = 5;
//								secUnits = 9;
//							}
//						}
//					}
//				}
//			}
//		} else {
//			secUnits++;
//		}
//	} else if (mode == 0) {
//		PORTD &= ~(1 << PD4);
//		PORTD |= ((1 << PD5));
//		PORTD &= (~(1 << PD0));
//		if (secUnits == 0) {
//			secUnits = 9;
//			if (secTens == 0) {
//				secTens = 5;
//				if (minUnits == 0) {
//					minUnits = 9;
//					if (minTens == 0) {
//						minTens = 5;
//						if (hrsUnits == 0) {
//							hrsUnits = 9;
//							if (hrsTens == 0) {
//								hrsTens = 0;
//								hrsUnits = 0;
//								minTens = 0;
//								minUnits = 0;
//								secTens = 0;
//								secUnits = 0;
//								PORTD |= (1 << PD0);
//							} else {
//								hrsTens--;
//							}
//						} else {
//							hrsUnits--;
//						}
//					} else {
//						minTens--;
//					}
//				} else {
//					minUnits--;
//				}
//			} else {
//				secTens--;
//			}
//		} else {
//			secUnits--;
//		}
//	}
//}
//ISR(INT0_vect) {
//	TCNT1 = 0;
//	hrsTens = 0;
//	hrsUnits = 0;
//	minTens = 0;
//	minUnits = 0;
//	secTens = 0;
//	secUnits = 0;
//	PORTD &= ~(1 << PD0);
//}
//
//ISR(INT1_vect) {
//	TCCR1B &= ~(1 << CS12);
//	TCCR1B &= ~(1 << CS11);
//	TCCR1B &= ~(1 << CS10);
//}
//
//ISR(INT2_vect) {
//	TCCR1B |= (1 << CS12) | (1 << CS10);
//}
//
//void TIMER1_CTC(void) {
//	TCNT1 = 0;
//	OCR1A = 15624;
//	TIMSK = (1 << OCIE1A);
//	TCCR1A = (1 << FOC1A) | (1 << FOC1B);
//	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
//}
//
//void INT0_Init(void) {
//	GICR |= (1 << INT0); // ENABLE INT0
//	MCUCR |= (1 << ISC01); // FALLING EDGE
//}
//
//void INT1_Init(void) {
//	MCUCR |= (1 << ISC11) | (1 << ISC10); // RISING EDGE
//	GICR |= (1 << INT1); //ENABLE INT1
//}
//
//void INT2_Init(void) {
//	GICR |= (1 << INT2); //ENABLE INT2
//	MCUCSR &= (~(1 << ISC2)); // FALLING EDGE
//}
//void Sev_Segement_Disply(void) {
//	PORTA = 0x20;
//	PORTC = (PORTC & 0xF0) | (secUnits & 0x0F);
//	_delay_ms(2);
//
//	PORTA = 0x10;
//	PORTC = (PORTC & 0xF0) | (secTens & 0x0F);
//	_delay_ms(2);
//
//	PORTA = 0x08;
//	PORTC = (PORTC & 0xF0) | (minUnits & 0x0F);
//	_delay_ms(2);
//
//	PORTA = 0x04;
//	PORTC = (PORTC & 0xF0) | (minTens & 0x0F);
//	_delay_ms(2);
//
//	PORTA = 0x02;
//	PORTC = (PORTC & 0xF0) | (hrsUnits & 0x0F);
//	_delay_ms(2);
//
//	PORTA = 0x01;
//	PORTC = (PORTC & 0xF0) | (hrsTens & 0x0F);
//	_delay_ms(2);
//}
//
//void COUNT_MODE(void) {
//	if (!(PINB & (1 << PB7))) {
//		_delay_ms(30);
//		if (!(PINB & (1 << PB7))) {
//			if (mode_flag == 0) {
//				mode = !mode;
//				mode_flag = 1;
//			}
//		}
//	} else {
//		mode_flag = 0;
//	}
//	if (mode == 1) {
//		PORTD |= (1 << PD4);
//		PORTD &= (~(1 << PD5));
//	} else if (mode == 0) {
//		PORTD &= (~(1 << PD4));
//		PORTD |= (1 << PD5);
//	}
//}
//
//void TIME_UP(void) {
//	//SECONDS INC.
//	if (!(PINB & (1 << PB6))) {
//		_delay_ms(30);
//		if (!(PINB & (1 << PB6))) {
//			if (sec_increment_flag == 0) {
//				if (secUnits == 9) {
//					secUnits = 0;
//					if (secTens == 5) {
//						secTens = 5;
//						secUnits = 9;
//					} else {
//						secTens++;
//					}
//				} else {
//					secUnits++;
//				}
//				sec_increment_flag = 1;
//			}
//		}
//	} else {
//		sec_increment_flag = 0;
//	}
//	//MINUTES INC.
//	if (!(PINB & (1 << PB4))) {
//		_delay_ms(30);
//		if (!(PINB & (1 << PB4))) {
//			if (min_increment_flag == 0) {
//				if (minUnits == 9) {
//					minUnits = 0;
//					if (minTens == 5) {
//						minTens = 5;
//						minUnits = 9;
//					} else {
//						minTens++;
//					}
//				} else {
//					minUnits++;
//				}
//				min_increment_flag = 1;
//			}
//		}
//	} else {
//		min_increment_flag = 0;
//	}
//	//HOURS INC.
//	if (!(PINB & (1 << PB1))) {
//		_delay_ms(30);
//		if (!(PINB & (1 << PB1))) {
//			if (hrs_increment_flag == 0) {
//				if (hrsUnits == 9) {
//					hrsUnits = 0;
//					if (hrsTens == 5) {
//						hrsTens = 5;
//						hrsUnits = 9;
//					} else {
//						hrsTens++;
//					}
//				} else {
//					hrsUnits++;
//				}
//				hrs_increment_flag = 1;
//			}
//		}
//	} else {
//		hrs_increment_flag = 0;
//	}
//}
//void TIME_DOWN(void) {
//
//	//SECONDS DEC.
//	if (!(PINB & (1 << PB5))) {
//		_delay_ms(30);
//		if (!(PINB & (1 << PB5))) {
//			if (sec_decrement_flag == 0) {
//				if (secUnits == 0) {
//					secUnits = 9;
//					if (secTens == 0) {
//						secTens = 0;
//						secUnits = 0;
//					} else {
//						secTens--;
//					}
//				} else {
//					secUnits--;
//				}
//				sec_decrement_flag = 1;
//			}
//		}
//	} else {
//		sec_decrement_flag = 0;
//	}
//	//MINUTES DEC.
//	if (!(PINB & (1 << PB3))) {
//		_delay_ms(30);
//		if (!(PINB & (1 << PB3))) {
//			if (min_decrement_flag == 0) {
//				if (minUnits == 0) {
//					minUnits = 9;
//					if (minTens == 0) {
//						minTens = 0;
//						minUnits = 0;
//					} else {
//						minTens--;
//					}
//				} else {
//					minUnits--;
//				}
//				min_decrement_flag = 1;
//			}
//		}
//	} else {
//		min_decrement_flag = 0;
//	}
//	//HOURS DEC.
//	if (!(PINB & (1 << PB0))) {
//		_delay_ms(30);
//		if (!(PINB & (1 << PB0))) {
//			if (hrs_decrement_flag == 0) {
//				if (hrsUnits == 0) {
//					hrsUnits = 9;
//					if (hrsTens == 0) {
//						hrsTens = 0;
//						hrsUnits = 0;
//					} else {
//						hrsTens--;
//					}
//				} else {
//					hrsUnits--;
//				}
//				hrs_decrement_flag = 1;
//			}
//		}
//	} else {
//		hrs_decrement_flag = 0;
//	}
//}
//
//int main(void) {
//	SREG |= (1 << 7); /* Enable I-BIT */
//
//	/*INPUT PINS*/
//	//PORT D
//	DDRD &= (~(1 << PD2));
//	DDRD &= (~(1 << PD3)); // PD2,PD3 INPUT PINS
//	PORTD |= (1 << PD2); // ENABLE INTERNAL PULL-UP RES.
//
//	//PORT B
//	DDRB &= 0x00;
//	PORTB |= 0xFF;
//	//ENABLE INTERANL PULL-UP RES.
//
//	//PORT A
//	DDRA &= (~(1 << PA0)) & (~(1 << PA1)) & (~(1 << PA2));
//	DDRA &= (~(1 << PA3)) & (~(1 << PA4)) & (~(1 << PA5));
//
//	/*OUTPUT PINS*/
//	//PORT D
//	DDRD |= (1 << PD0) | (1 << PD4) | (1 << PD5);
//	PORTD &= (~(1 << PD0)) & (~(1 << PD4)) & (~(1 << PD5)); // OFF
//
//	//PORT C
//	DDRC |= 0x0F;
//	PORTC &= 0xF0;
//
//	TIMER1_CTC();
//
//	INT0_Init();
//	INT1_Init();
//	INT2_Init();
//
//	while (1) {
//		Sev_Segement_Disply();
//		COUNT_MODE();
//		TIME_UP();
//		TIME_DOWN();
//	}
//}
//
