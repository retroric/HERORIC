
;
; Assembly routines
;


#define HIRES_ADDRESS $A000


_cli
; call CLI to CLear Interrupts -- AUTORISER les interruptions
	cli
	rts
	
_sei
; call SEI to SEt Interrupts - INTERDIRE les interruptions
	sei
	rts
	

