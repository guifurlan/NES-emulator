; Author: tokumaru
; http://forums.nesdev.com/viewtopic.php?%20p=58138#58138
;----------------------------------------------------------------
; constants
;----------------------------------------------------------------
PRG_COUNT = 1 ;1 = 16KB, 2 = 32KB
MIRRORING = %0001 ;%0000 = horizontal, %0001 = vertical, %1000 = four-screen

;----------------------------------------------------------------
; variables
;----------------------------------------------------------------

   .enum $0000

   ;NOTE: declare variables using the DSB and DSW directives, like this:

   ;MyVariable0 .dsb 1
   ;MyVariable1 .dsb 3

   .ende

   ;NOTE: you can also split the variable declarations into individual pages, like this:

   ;.enum $0100
   ;.ende

   ;.enum $0200
   ;.ende

;----------------------------------------------------------------
; iNES header
;----------------------------------------------------------------

   .db "NES", $1a ;identification of the iNES header
   .db PRG_COUNT ;number of 16KB PRG-ROM pages
   .db $01 ;number of 8KB CHR-ROM pages
   .db $00|MIRRORING ;mapper 0 and mirroring
   .dsb 9, $00 ;clear the remaining bytes

;----------------------------------------------------------------
; program bank(s)
;----------------------------------------------------------------

   .base $10000-(PRG_COUNT*$4000)

Reset:
    lda #$ff
   ; immediate
   sbc #1
   ; immediate with carry
   sec
   sbc #1

   ; zeropage
   lda #$ff
   sta $00
   lda #1
   sbc $00

   ; absolute
   lda #$ff
   sta $0500
   sbc $0500
    
   ; absolute, x
   ldx #1
   sta $0501
   sbc $0500, x

   ; absolute, y
   ldy #1
   lda #$ff
   sta $0501
   sbc $0500, y

   ; zeropage, x
   ldx #1
   lda #$ff
   sta $01
   sbc $00, x

   ; (indirect, x)
   ; (indirect), y

   brk ; Abort execution

NMI:

   ;NOTE: NMI code goes here

IRQ:

   ;NOTE: IRQ code goes here

;----------------------------------------------------------------
; interrupt vectors
;----------------------------------------------------------------

   .org $fffa

   .dw NMI
   .dw Reset
   .dw IRQ

   .dsb $2000