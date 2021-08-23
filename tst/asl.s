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
  lda #1
  asl
  asl
  lda #5
  asl
  asl
  ; zeropage
  lda #1
  sta $00
  asl $00
  asl $00
  lda #5
  sta $01
  asl $01
  asl $01
  ; zeropage,X
  lda #1
  sta $00
  ldx #0
  asl $00, x
  lda #5
  sta $01
  ldx #1
  asl $00, x
  ; absolute
  lda #1
  sta $0500
  asl $0500
  asl $0500
  lda #5
  sta $0501
  asl $0501
  asl $0501
  ; absolute,X
  lda #1
  sta $0500
  ldx #0
  asl $0500, x
  asl $0500, x
  lda #5
  sta $0501
  ldx #1
  asl $0500, x
  asl $0500, x
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