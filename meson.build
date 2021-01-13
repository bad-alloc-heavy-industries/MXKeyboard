# SPDX-License-Identifier: BSD-3-Clause

project(
	'MXKeyboard',
	'cpp',
	default_options: [
		'cpp_std=c++17',
		'warning_level=3',
		'buildtype=release',
		'strip=true',
		'b_ndebug=if-release',
		'b_lto=true'
	],
	version: '0.1.0',
	meson_version: '>= 0.53',
	subproject_dir: 'deps'
)

if not meson.is_cross_build()
	error('MXKeyboard must be cross-compiled to the target microcontroller and cannot be built for the host machine')
endif

targetCXX = meson.get_compiler('cpp', native: false)
hostCXX = meson.get_compiler('cpp', native: true)

subdir('bootloader')
subdir('firmware')
