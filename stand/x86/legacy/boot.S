.globl _legacy_entry

.section .text
.code16

_legacy_entry:
	# Disable interrupts
	cli

	# Enable A20 Line
	#
	# Most likely this will already be on, but just setting it explicitly
	# to be sure. Otherwise, *bad* things will happen :)
	movw $0x2401, %ax
	int $0x15
	jc _die			# CF set on failure


	# TODO load the stage1 loader and grab memory map
_die:
	hlt
	jmp _die

# Disk Signature
# Partition Table
.space 440-(.-_legacy_entry)
.space 510-(.-_legacy_entry)

# Boot Signature
.word 0xaa55
