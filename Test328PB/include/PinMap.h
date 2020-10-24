/*
 * PinMap.h
 *
 * Created: 4/12/2020 10:35:10 PM
 *  Author: MQUEZADA
 */ 


#ifndef PINMAP_H_
#define PINMAP_H_

// IC LEG    FUNC               Function
/*
01           Charger status OUT    O
02           Charger status IN     I
03           SW Music            PCINT24
04           VCC
05           GND
06           NC
07           XTAL1
08           XTAL2
09           PI PWR FET            O
10           BUZZER                O
11           SW Forward          PCINT 23
12           SW Play             PCINT 0
13           SW REWIND           PCINT 1
14           SW C                PCINT 2
15           SW B                PCINT 3/PMOSI
16           SW A                PCINT 4/PMISO
17           SW VOL DN           PCINT 5/PSCK
18           AVCC
19           SW VOL UP           PCINT 26
20           AREF
21           GND
22           SW MUTE             PCINT 27
23           SW Enter            PCINT 8
24           VBATT                ADC
25           SW Zones            PCINT 10
26           SW Back             PCINT 11
27           SDA                   IO
28           SCL                   CLK
29           RESET
30           PI RESET TOGGLE        O
31           SCROLL INT          PCINT 17
32           TOUCH INTERRUPT     PCINT 18 / INT0

*/

// INTERRUPTS
#define SW_PLAYp B
#define SW_PLAYb PCINT0
#define SW_PLAYbm (1 << SW_PLAYb)
#define SW_PLAY_DIR_OUT (DDR#SW_PLAYp |= SW_PLAYbm)
#define SW_PLAY_DIR_IN (DDR#SW_PLAYp &= ~SW_PLAYbm)
#define SW_PLAY_HIGH (PORT#SW_PLAYp |= SW_PLAYbm)
#define SW_PLAY_LOW (PORT#SW_PLAYp &= ~SW_PLAYbm)
#define SW_PLAY_IS_HIGH (PINB & SW_PLAYbm)
#define SW_PLAY_IS_LOW !(PINB & SW_PLAYbm)

#define SW_REWp B
#define SW_REWb PCINT1
#define SW_REWbm (1 << SW_REWb)
#define SW_REW_DIR_OUT (DDR#SW_REWp |= SW_REWbm)
#define SW_REW_DIR_IN (DDR#SW_REWp &= ~SW_REWbm)
#define SW_REW_HIGH (PORT#SW_REWp |= SW_REWbm)
#define SW_REW_LOW (PORT#SW_REWp &= ~SW_REWbm)
#define SW_REW_IS_HIGH (PINB & SW_REWbm)
#define SW_REW_IS_LOW !(PINB & SW_REWbm)

#define SW_Cp B
#define SW_Cb PCINT2
#define SW_Cbm (1 << SW_Cb)
#define SW_C_DIR_OUT (DDR#SW_Cp |= SW_Cbm)
#define SW_C_DIR_IN (DDR#SW_Cp &= ~SW_Cbm)
#define SW_C_HIGH (PORT#SW_Cp |= SW_Cbm)
#define SW_C_LOW (PORT#SW_Cp &= ~SW_Cbm)
#define SW_C_IS_HIGH (PINB & SW_Cbm)
#define SW_C_IS_LOW !(PINB & SW_Cbm)

#define SW_Bp B
#define SW_Bb PCINT3
#define SW_Bbm (1 << SW_Bb)
#define SW_B_DIR_OUT (DDR#SW_Bp |= SW_Bbm)
#define SW_B_DIR_IN (DDR#SW_Bp &= ~SW_Bbm)
#define SW_B_HIGH (PORT#SW_Bp |= SW_Bbm)
#define SW_B_LOW (PORT#SW_Bp &= ~SW_Bbm)
#define SW_B_IS_HIGH (PINB & SW_Bbm)
#define SW_B_IS_LOW !(PINB & SW_Bbm)

#define SW_Ap B
#define SW_Ab PCINT4
#define SW_Abm (1 << SW_Ab)
#define SW_A_DIR_OUT (DDR#SW_Ap |= SW_Abm)
#define SW_A_DIR_IN (DDR#SW_Ap &= ~SW_Abm)
#define SW_A_HIGH (PORT#SW_Ap |= SW_Abm)
#define SW_A_LOW (PORT#SW_Ap &= ~SW_Abm)
#define SW_A_IS_HIGH (PINB & SW_Abm)
#define SW_A_IS_LOW !(PINB & SW_Abm)

#define SW_VOL_DNp B
#define SW_VOL_DNb PCINT5
#define SW_VOL_DNbm (1 << SW_VOL_DNb)
#define SW_VOL_DN_DIR_OUT (DDR#SW_VOL_DNp |= SW_VOL_DNbm)
#define SW_VOL_DN_DIR_IN (DDR#SW_VOL_DNp &= ~SW_VOL_DNbm)
#define SW_VOL_DN_HIGH (PORT#SW_VOL_DNp |= SW_VOL_DNbm)
#define SW_VOL_DN_LOW (PORT#SW_VOL_DNp &= ~SW_VOL_DNbm)
#define SW_VOL_DN_IS_HIGH (PINB & SW_VOL_DNbm)
#define SW_VOL_DN_IS_LOW !(PINB & SW_VOL_DNbm)

#define SW_ENTERp C
#define SW_ENTERb PCINT8
#define SW_ENTERbm (1 << SW_ENTERb)
#define SW_ENTER_DIR_OUT (DDR#SW_ENTERp |= SW_ENTERbm)
#define SW_ENTER_DIR_IN (DDR#SW_ENTERp &= ~SW_ENTERbm)
#define SW_ENTER_HIGH (PORT#SW_ENTERp |= SW_ENTERbm)
#define SW_ENTER_LOW (PORT#SW_ENTERp &= ~SW_ENTERbm)
#define SW_ENTER_IS_HIGH (PINC & SW_ENTERbm)
#define SW_ENTER_IS_LOW !(PINC & SW_ENTERbm)

#define SW_ZONESp C
#define SW_ZONESb PCINT10
#define SW_ZONESbm (1 << SW_ZONESb)
#define SW_ZONES_DIR_OUT (DDR#SW_ZONESp |= SW_ZONESbm)
#define SW_ZONES_DIR_IN (DDR#SW_ZONESp &= ~SW_ZONESbm)
#define SW_ZONES_HIGH (PORT#SW_ZONESp |= SW_ZONESbm)
#define SW_ZONES_LOW (PORT#SW_ZONESp &= ~SW_ZONESbm)
#define SW_ZONES_IS_HIGH (PINC & SW_ZONESbm)
#define SW_ZONES_IS_LOW !(PINC & SW_ZONESbm)

#define SW_RETURNp C
#define SW_RETURNb PCINT11
#define SW_RETURNbm (1 << SW_RETURNb)
#define SW_RETURN_DIR_OUT (DDR#SW_RETURNp |= SW_RETURNbm)
#define SW_RETURN_DIR_IN (DDR#SW_RETURNp &= ~SW_RETURNbm)
#define SW_RETURN_HIGH (PORT#SW_RETURNp |= SW_RETURNbm)
#define SW_RETURN_LOW (PORT#SW_RETURNp &= ~SW_RETURNbm)
#define SW_RETURN_IS_HIGH (PINC & SW_RETURNbm)
#define SW_RETURN_IS_LOW !(PINC & SW_RETURNbm)

#define SCROLL_INTp D
#define SCROLL_INTb PCINT17
#define SCROLL_INTbm (1 << SCROLL_INTb)
#define SCROLL_INT_DIR_OUT (DDR#SCROLL_INTp |= SCROLL_INTbm)
#define SCROLL_INT_DIR_IN (DDR#SCROLL_INTp &= ~SCROLL_INTbm)
#define SCROLL_INT_HIGH (PORT#SCROLL_INTp |= SCROLL_INTbm)
#define SCROLL_INT_LOW (PORT#SCROLL_INTp &= ~SCROLL_INTbm)
#define SCROLL_INT_IS_HIGH (PIND & SCROLL_INTbm)
#define SCROLL_INT_IS_LOW !(PIND & SCROLL_INTbm)

#define SW_FORWARDp D
#define SW_FORWARDb PCINT23
#define SW_FORWARDbm (1 << SW_FORWARDb)
#define SW_FORWARD_DIR_OUT (DDR#SW_FORWARDp |= SW_FORWARDbm)
#define SW_FORWARD_DIR_IN (DDR#SW_FORWARDp &= ~SW_FORWARDbm)
#define SW_FORWARD_HIGH (PORT#SW_FORWARDp |= SW_FORWARDbm)
#define SW_FORWARD_LOW (PORT#SW_FORWARDp &= ~SW_FORWARDbm)
#define SW_FORWARD_IS_HIGH (PIND & SW_FORWARDbm)
#define SW_FORWARD_IS_LOW !(PIND & SW_FORWARDbm)

#define SW_MUSICp E
#define SW_MUSICb PCINT24
#define SW_MUSICbm (1 << SW_MUSICb)
#define SW_MUSIC_DIR_OUT (DDR#SW_MUSICp |= SW_MUSICbm)
#define SW_MUSIC_DIR_IN (DDR#SW_MUSICp &= ~SW_MUSICbm)
#define SW_MUSIC_HIGH (PORT#SW_MUSICp |= SW_MUSICbm)
#define SW_MUSIC_LOW (PORT#SW_MUSICp &= ~SW_MUSICbm)
#define SW_MUSIC_IS_HIGH (PINE & SW_MUSICbm)
#define SW_MUSIC_IS_LOW !(PINE & SW_MUSICbm)

#define SW_VOL_UPp E
#define SW_VOL_UPb PCINT26
#define SW_VOL_UPbm (1 << SW_VOL_UPb)
#define SW_VOL_UP_DIR_OUT (DDR#SW_VOL_UPp |= SW_VOL_UPbm)
#define SW_VOL_UP_DIR_IN (DDR#SW_VOL_UPp &= ~SW_VOL_UPbm)
#define SW_VOL_UP_HIGH (PORT#SW_VOL_UPp |= SW_VOL_UPbm)
#define SW_VOL_UP_LOW (PORT#SW_VOL_UPp &= ~SW_VOL_UPbm)
#define SW_VOL_UP_IS_HIGH (PINE & SW_VOL_UPbm)
#define SW_VOL_UP_IS_LOW !(PINE & SW_VOL_UPbm)

#define SW_MUTEp E
#define SW_MUTEb PCINT27
#define SW_MUTEbm (1 << SW_MUTEb)
#define SW_MUTE_DIR_OUT (DDR#SW_MUTEp |= SW_MUTEbm)
#define SW_MUTE_DIR_IN (DDR#SW_MUTEp &= ~SW_MUTEbm)
#define SW_MUTE_HIGH (PORT#SW_MUTEp |= SW_MUTEbm)
#define SW_MUTE_LOW (PORT#SW_MUTEp &= ~SW_MUTEbm)
#define SW_MUTE_IS_HIGH (PINE & SW_MUTEbm)
#define SW_MUTE_IS_LOW !(PINE & SW_MUTEbm)

// ADC
#define BATT_Vp C
#define BATT_Vb 1
#define BATT_Vbm (1 << BATT_Vb)
//#define BATT_V_DIR_OUT (DDR#BATT_Vp |= BATT_Vbm)
#define BATT_V_DIR_IN (DDR#BATT_Vp &= ~BATT_Vbm)
//#define BATT_V_HIGH (PORT#BATT_Vp |= BATT_Vbm)
//#define BATT_V_LOW (PORT#BATT_Vp &= ~BATT_Vbm)

// OUTPUTS
#define PI_RES_TOGp D
#define PI_RES_TOGb 0
#define PI_RES_TOGbm (1 << PI_RES_TOGb)
#define PI_RES_TOG_DIR_OUT (DDR # PI_RES_TOGp |= PI_RES_TOGbm)
#define PI_RES_TOG_DIR_IN (DDRD &= ~PI_RES_TOGbm)
#define PI_RES_TOG_HIGH (PORT#PI_RES_TOGp |= PI_RES_TOGbm)
#define PI_RES_TOG_LOW (PORT#PI_RES_TOGp &= ~PI_RES_TOGbm)

#define CH_STAT_OUTp D
#define CH_STAT_OUTb 3
#define CH_STAT_OUTbm (1 << CH_STAT_OUTb)
#define CH_STAT_OUT_DIR_OUT (DDRD |= CH_STAT_OUTbm)
#define CH_STAT_OUT_DIR_IN (DDRD &= ~CH_STAT_OUTbm)
#define CH_STAT_OUT_HIGH (PORTD |= CH_STAT_OUTbm)
#define CH_STAT_OUT_LOW (PORTD &= ~CH_STAT_OUTbm)
#define CH_STAT_OUT_IS_HIGH (PIND & CH_STAT_OUTbm)
#define CH_STAT_OUT_IS_LOW !(PIND & CH_STAT_OUTbm)

#define CH_STAT_INp D
#define CH_STAT_INb 4
#define CH_STAT_INbm (1 << CH_STAT_INb)
#define CH_STAT_IN_DIR_OUT (DDRD |= CH_STAT_INbm)
#define CH_STAT_IN_DIR_IN (DDRD &= ~CH_STAT_INbm)
#define CH_STAT_IN_HIGH (PORTD |= CH_STAT_INbm)
#define CH_STAT_IN_LOW (PORTD &= ~CH_STAT_INbm)
#define CH_STAT_IN_IS_HIGH (PIND & CH_STAT_INbm)
#define CH_STAT_IN_IS_LOW !(PIND & CH_STAT_INbm)

#define PI_PWR_SWp D
#define PI_PWR_SWb 5
#define PI_PWR_SWbm (1 << PI_PWR_SWb)
#define PI_PWR_SW_DIR_OUT (DDRD |= PI_PWR_SWbm)
#define PI_PWR_SW_DIR_IN (DDRD &= ~PI_PWR_SWbm)
#define PI_PWR_SW_HIGH (PORTD |= PI_PWR_SWbm)
#define PI_PWR_SW_LOW (PORTD &= ~PI_PWR_SWbm)
#define PI_PWR_SW_IS_HIGH (PIND & PI_PWR_SWbm)
#define PI_PWR_SW_IS_LOW !(PIND & PI_PWR_SWbm)

#define BUZZERp D
#define BUZZERb 6
#define BUZZERbm (1 << BUZZERb)
#define BUZZER_DIR_OUT (DDRD |= BUZZERbm)
#define BUZZER_DIR_IN (DDRD &= ~BUZZERbm)
#define BUZZER_HIGH (PORTD |= BUZZERbm)
#define BUZZER_LOW (PORTD &= ~BUZZERbm)

#endif /* PINMAP_H_ */