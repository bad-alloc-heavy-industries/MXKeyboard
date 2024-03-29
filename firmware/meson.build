# SPDX-License-Identifier: BSD-3-Clause

substrate = subproject(
	'substrate',
	required: true,
	version: '>=0.0.1',
	default_options: [
		'build_tests=false',
		'build_library=false'
	]
).get_variable(
	'substrate_dep'
).partial_dependency(
	compile_args: true,
	includes: true
)

dragonAVR = subproject(
	'dragonAVR',
	required: true,
	version: '>=0.0.1',
	default_options: [
		'chip=atxmega256a3u'
	]
).get_variable(
	'dragonAVR_dep'
)

dragonUSB = subproject(
	'dragonUSB',
	required: true,
	version: '>=0.0.1',
	default_options: [
		'chip=atxmega256a3u',
		'interfaces=1',
		'endpoints=1',
		'epBufferSize=64',
		'configDescriptors=1',
		'ifaceDescriptors=1',
		'endpointDescriptors=1',
		'strings=4',
		#'drivers=dfu'
	]
).get_variable(
	'dragonUSB_dep'
)

firmwareSrc = [
	'startup.cxx', 'fuses.cxx', 'MXKeyboard.cxx', 'uart.cxx',
	'system.cxx', 'led.cxx', 'timer.cxx', 'dma.cxx',
	'memory.cxx', 'ps2.cxx', 'keyMatrix.cxx', 'profile.cxx',
	'usb/descriptors.cxx', 'usb/hid.cxx'
]

firmwareArgs = targetCXX.get_supported_arguments(
	'-Wno-misspelled-isr',
	'-Wvla',
	'-Wimplicit-fallthrough',
	'-Wstack-usage=2048',
	'-fno-jump-tables'
)

if debug and optimisation == '0'
	add_project_arguments('-Og', language: 'cpp')
	add_project_link_arguments('-Og', language: 'cpp')
elif optimisation == '0'
	add_project_arguments('-O1', language: 'cpp')
	add_project_link_arguments('-O1', language: 'cpp')
endif

firmware = executable(
	'MXKeyboard',
	firmwareSrc,
	include_directories: include_directories('include'),
	dependencies: [substrate, dragonAVR, dragonUSB],
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

size = find_program('get_fw_size.py', dirs: '@0@/../scripts'.format(meson.current_source_dir()))
run_target(
	'size',
	command: [
		size,
		'--flash-size=262144', # 256KiB of Flash
		'--ram-size=16384', # 16KiB of RAM
		'--size-prog=@0@'.format(find_program('size').path()),
		firmware
	]
)
