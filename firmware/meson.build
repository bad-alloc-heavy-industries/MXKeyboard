# SPDX-License-Identifier: BSD-3-Clause

keyboardSrc = [
	'startup.cxx', 'fuses.cxx', 'MXKeyboard.cxx', 'uart.cxx',
	'system.cxx'
]

executable(
	'MXKeyboard',
	keyboardSrc,
	include_directories: include_directories('include', 'include/c++'),
	link_args: ['-T', '@0@/atxmega256a3u.ld'.format(meson.current_source_dir())],
	gnu_symbol_visibility: 'inlineshidden',
	override_options: ['b_lto=true'],
	name_suffix: 'elf',
	build_by_default: true,
	install: false
)
