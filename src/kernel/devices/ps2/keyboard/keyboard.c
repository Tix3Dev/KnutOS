/*
	This file is part of an x86_64 hobbyist operating system called KnutOS
	Everything is openly developed on GitHub: https://github.com/Tix3Dev/KnutOS/

	Copyright (C) 2021-2022  Yves Vollmeier <https://github.com/Tix3Dev>
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// The standard_keycodes array + the KEY enum is from this tutorial:
// http://www.brokenthorn.com/Resources/OSDev19.html

#include <stdint.h>

#include <boot/stivale2.h>
#include <devices/pic/pic.h>
#include <devices/ps2/keyboard/keyboard.h>
#include <libk/io/io.h>
#include <libk/log/log.h>

static uint8_t is_keyboard_active = 0;	// boolean whether keyboard IRQ should get processed

static void (*final_handler)(KEY_INFO_t);

static uint32_t standard_keycodes[] =
{
//  key			scancode
    KEY_UNKNOWN,	//0x0
    KEY_ESCAPE,		//0x1
    KEY_1,		//0x2
    KEY_2,		//0x3
    KEY_3,		//0x4
    KEY_4,		//0x5
    KEY_5,		//0x6
    KEY_6,		//0x7
    KEY_7,		//0x8
    KEY_8,		//0x9
    KEY_9,		//0xa
    KEY_0,		//0xb
    KEY_MINUS,		//0xc
    KEY_EQUAL,		//0xd
    KEY_BACKSPACE,	//0xe
    KEY_TAB,		//0xf
    KEY_Q,		//0x10
    KEY_W,		//0x11
    KEY_E,		//0x12
    KEY_R,		//0x13
    KEY_T,		//0x14
    KEY_Y,		//0x15
    KEY_U,		//0x16
    KEY_I,		//0x17
    KEY_O,		//0x18
    KEY_P,		//0x19
    KEY_LEFTBRACKET,	//0x1a
    KEY_RIGHTBRACKET,	//0x1b
    KEY_RETURN,		//0x1c
    KEY_LCTRL,		//0x1d
    KEY_A,		//0x1e
    KEY_S,		//0x1f
    KEY_D,		//0x20
    KEY_F,		//0x21
    KEY_G,		//0x22
    KEY_H,		//0x23
    KEY_J,		//0x24
    KEY_K,		//0x25
    KEY_L,		//0x26
    KEY_SEMICOLON,	//0x27
    KEY_QUOTE,		//0x28
    KEY_GRAVE,		//0x29
    KEY_LSHIFT,		//0x2a
    KEY_BACKSLASH,	//0x2b
    KEY_Z,		//0x2c
    KEY_X,		//0x2d
    KEY_C,		//0x2e
    KEY_V,		//0x2f
    KEY_B,		//0x30
    KEY_N,		//0x31
    KEY_M,		//0x32
    KEY_COMMA,		//0x33
    KEY_DOT,		//0x34
    KEY_SLASH,		//0x35
    KEY_RSHIFT,		//0x36
    KEY_KP_ASTERISK,	//0x37
    KEY_RALT,		//0x38
    KEY_SPACE,		//0x39
    KEY_CAPSLOCK,	//0x3a
    KEY_F1,		//0x3b
    KEY_F2,		//0x3c
    KEY_F3,		//0x3d
    KEY_F4,		//0x3e
    KEY_F5,		//0x3f
    KEY_F6,		//0x40
    KEY_F7,		//0x41
    KEY_F8,		//0x42
    KEY_F9,		//0x43
    KEY_F10,		//0x44
    KEY_KP_NUMLOCK,	//0x45
    KEY_SCROLLLOCK,	//0x46
    KEY_HOME,		//0x47
    KEY_KP_8,		//0x48
    KEY_PAGEUP,		//0x49
    KEY_KP_2,		//0x50
    KEY_KP_3,		//0x51
    KEY_KP_0,		//0x52
    KEY_KP_DECIMAL,	//0x53
    KEY_UNKNOWN,	//0x54
    KEY_UNKNOWN,	//0x55
    KEY_UNKNOWN,	//0x56
    KEY_F11,		//0x57
    KEY_F12		//0x58
};

static uint8_t ctrl	    = 0;
static uint8_t alt	    = 0;
static uint8_t shift	    = 0;

static uint8_t capslock	    = 0;
static uint8_t numlock	    = 0;
static uint8_t scrolllock   = 0;

// initialize the keyboard
void keyboard_init(void)
{
    // clear the set mask for the keyboard IRQ
    pic_clear_mask(1);

    // clear the data register of the keyboard controller
    while (io_inb(KB_CONTROLLER_COMMAND) & 0x1)
        io_inb(KB_CONTROLLER_DATA);

    // enable scanning (so that the keyboard will send scan codes)
    keyboard_send_command(0xF4);

    serial_log(INFO, "Keyboard driver initialized\n");
    kernel_log(INFO, "Keyboard driver initialized\n");
}

// send a command byte to the keyboard controller
void keyboard_send_command(uint8_t command_byte)
{
    // wait until keyboard is ready
    while (io_inb(KB_CONTROLLER_COMMAND) & 0x2)
        io_outb(KB_CONTROLLER_DATA, command_byte);
}

// gets called whenever a keyboard IRQ occurrs
void keyboard_irq_handler(void)
{
    // read from keyboard controller so that in any case it will be able to send more IRQs
    uint8_t scancode = io_inb(KB_CONTROLLER_DATA);

    // check if keyboard IRQ should get processed
    if (!is_keyboard_active)
        return;

    KEY_INFO_t key_info;

    key_info.keycode		= KEY_UNKNOWN;
    key_info.ascii_character	= '\0';

    // check for breakcode
    if (scancode & 0x80)
    {
        scancode -= 0x80;

        uint32_t key = standard_keycodes[scancode];

        if (key == KEY_LCTRL || key == KEY_RCTRL)
            ctrl = 0;
        else if (key == KEY_LALT || key == KEY_RALT)
            alt = 0;
        else if (key == KEY_LSHIFT || key == KEY_RSHIFT)
            shift = 0;
    }
    else
    {
        uint32_t key = standard_keycodes[scancode];

        key_info.keycode = key;

        if (key == KEY_LCTRL || key == KEY_RCTRL)
            ctrl = 1;
        else if (key == KEY_LALT || key == KEY_RALT)
            alt = 1;
        else if (key == KEY_LSHIFT || key == KEY_RSHIFT)
            shift = 1;
        else if (key == KEY_CAPSLOCK)
            capslock = capslock ? 0 : 1;
        else if (key == KEY_KP_NUMLOCK)
            numlock = numlock ? 0 : 1;
        else if (key == KEY_SCROLLLOCK)
            scrolllock = scrolllock ? 0 : 1;
        // else if (key == KEY_RETURN)
        // 	key_info.ascii_character = '\n';
        else if (key <= 0x7F)
            key_info.ascii_character = keycode_to_ascii(key);
    }

    final_handler(key_info);

    /*
        if ()
        {
    	serial_set_color(TERM_RED);
    	debug("\n──────────────────────────────\n");
    	debug("⚠ UNKNOWN SCANCODE RECEIVED! ⚠\n\n");
    	debug("⤷ Scancode: 0x%x | Set: %d\n", scancode, scancode_set);
    	serial_set_color(TERM_COLOR_RESET);
        }
    */
}

// type change and conversion for upper- or lowercase if shift or capslock is pressed
char keycode_to_ascii(KEYCODE_t keycode)
{
    uint8_t character = keycode;

    if (shift && capslock)
    {
        if (character == '0')
            character = KEY_RIGHTPARENTHESIS;
        else if (character == '1')
            character = KEY_EXCLAMATION;
        else if (character == '2')
            character = KEY_AT;
        else if (character == '3')
            character = KEY_HASH;
        else if (character == '4')
            character = KEY_DOLLAR;
        else if (character == '5')
            character = KEY_PERCENT;
        else if (character == '6')
            character = KEY_CARRET;
        else if (character == '7')
            character = KEY_AMPERSAND;
        else if (character == '8')
            character = KEY_ASTERISK;
        else if (character == '9')
            character = KEY_LEFTPARENTHESIS;
        else if (character == KEY_COMMA)
            character = KEY_LESS;
        else if (character == KEY_DOT)
            character = KEY_GREATER;
        else if (character == KEY_SLASH)
            character = KEY_QUESTION;
        else if (character == KEY_SEMICOLON)
            character = KEY_COLON;
        else if (character == KEY_QUOTE)
            character = KEY_QUOTEDOUBLE;
        else if (character == KEY_LEFTBRACKET)
            character = KEY_LEFTCURL;
        else if (character == KEY_RIGHTBRACKET)
            character = KEY_RIGHTCURL;
        else if (character == KEY_GRAVE)
            character = KEY_TILDE;
        else if (character == KEY_MINUS)
            character = KEY_UNDERSCORE;
        else if (character == KEY_EQUAL)
            character = KEY_PLUS;
        else if (character == KEY_BACKSLASH)
            character = KEY_BAR;
    }
    else if (shift && !capslock)
    {
        if (character >= 'a' && character <= 'z')
            character -= 32;

        if (character == '0')
            character = KEY_RIGHTPARENTHESIS;
        else if (character == '1')
            character = KEY_EXCLAMATION;
        else if (character == '2')
            character = KEY_AT;
        else if (character == '3')
            character = KEY_HASH;
        else if (character == '4')
            character = KEY_DOLLAR;
        else if (character == '5')
            character = KEY_PERCENT;
        else if (character == '6')
            character = KEY_CARRET;
        else if (character == '7')
            character = KEY_AMPERSAND;
        else if (character == '8')
            character = KEY_ASTERISK;
        else if (character == '9')
            character = KEY_LEFTPARENTHESIS;
        else if (character == KEY_COMMA)
            character = KEY_LESS;
        else if (character == KEY_DOT)
            character = KEY_GREATER;
        else if (character == KEY_SLASH)
            character = KEY_QUESTION;
        else if (character == KEY_SEMICOLON)
            character = KEY_COLON;
        else if (character == KEY_QUOTE)
            character = KEY_QUOTEDOUBLE;
        else if (character == KEY_LEFTBRACKET)
            character = KEY_LEFTCURL;
        else if (character == KEY_RIGHTBRACKET)
            character = KEY_RIGHTCURL;
        else if (character == KEY_GRAVE)
            character = KEY_TILDE;
        else if (character == KEY_MINUS)
            character = KEY_UNDERSCORE;
        else if (character == KEY_EQUAL)
            character = KEY_PLUS;
        else if (character == KEY_BACKSLASH)
            character = KEY_BAR;

    }
    else if (!shift && capslock)
    {
        if (character >= 'a' && character <= 'z')
            character -= 32;
    }

    return character;
}

// KEY_INFO_t struct will be passed to handler so it must take it as argument
// the x, y coordinates are optional, if not used pass NULL otherwise it will set a writable area
// -> set a barrier
void activate_keyboard_processing(void *handler)
{
    final_handler = handler;

    is_keyboard_active = 1;
}

// to undo activate_keyboard_processing (otherwise no change because 0 is default state)
void disable_keyboard_processing(void)
{
    is_keyboard_active = 0;
}
