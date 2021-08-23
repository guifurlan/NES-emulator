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

   ;NOTE: you can also split the variable declarations into individual pages, like thi

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
  ; accumulator
  lda #$AA
  rol
  rol
  lda #$55
  rol
  rol
  ; zeropage
  lda #$AA
  sta $00
  rol $00
  rol $00
  lda #$55
  sta $01
  rol $01
  rol $01
  ; zeropage,X
  lda #$AA
  sta $00
  ldx #0
  rol $00, x
  lda #$55
  sta $01
  ldx #1
  rol $00, x
  ; absolute
  lda #$AA
  sta $0500
  rol $0500
  rol $0500
  lda #$55
  sta $0501
  rol $0501
  rol $0501
  ; absolute,X
  lda #$AA
  sta $0500
  ldx #0
  rol $0500, x
  rol $0500, x
  lda #$55
  sta $0501
  ldx #1
  rol $0500, x
  rol $0500, x
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