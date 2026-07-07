# This file is part of Telegram Desktop,
# the official desktop application for the Telegram messaging service.
#
# For license and copyright information please follow this link:
# https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL

function(generate_numbers target_name numbers_file)
    set(gen_dst ${CMAKE_CURRENT_BINARY_DIR}/gen)
    file(MAKE_DIRECTORY ${gen_dst})

    set(gen_timestamp ${gen_dst}/numbers.timestamp)
    set(gen_files
        ${gen_dst}/numbers.cpp
        ${gen_dst}/numbers.h
    )

    desktop_app_prepare_codegen_command(_codegen_command
        codegen_numbers
        -o${gen_dst}
        ${numbers_file}
    )
    add_custom_command(
    OUTPUT
        ${gen_timestamp}
    BYPRODUCTS
        ${gen_files}
    COMMAND
        ${_codegen_command}
    COMMENT "Generating numbers (${target_name})"
    DEPENDS
        codegen_numbers
        ${numbers_file}
    )
    generate_target(${target_name} numbers ${gen_timestamp} "${gen_files}" ${gen_dst})
endfunction()
