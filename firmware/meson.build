# SPDX-License-Identifier: BSD-3-Clause

firmwareSrc = [
	'startup.cxx', 'fuses.cxx', 'MXKeyboard.cxx', 'uart.cxx',
	'system.cxx', 'led.cxx', 'timer.cxx', 'dma.cxx',
	'memory.cxx', 'ps2.cxx', 'usb.cxx', 'usbRequests.cxx'
]

firmwareArgs = []
if targetCXX.has_argument('-Wno-misspelled-isr')
	firmwareArgs += ['-Wno-misspelled-isr']
endif

firmware = executable(
	'MXKeyboard',
	firmwareSrc,
	include_directories: include_directories('include', 'include/c++'),
	cpp_args: firmwareArgs,
	link_args: ['-T', '@0@/atxmega256a3u.ld'.format(meson.current_source_dir())],
	gnu_symbol_visibility: 'inlineshidden',
	override_options: ['b_lto=true'],
	name_suffix: 'elf',
	build_by_default: true,
	install: false
)

avrdude = find_program('avrdude')
run_target(
	'program',
	command: [
		avrdude,
		'-p', 'ATxmega256A3U',
		'-c', 'jtag3',
		'-U', 'flash:w:@0@:e'.format(firmware.full_path())
	],
	depends: firmware
)

objdump = find_program('objdump')
run_target(
	'disasm',
	command: [objdump, '-dC', firmware]
)

size = find_program('size')
run_target(
	'size',
	command: [size, firmware]
)
