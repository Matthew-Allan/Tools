#include "cols.h"

#include <ctype.h>
#include <stdio.h>

#include <bitprint.h>
#include <colours.h>

#define CTL_DEL(type, chr) (chr == DEL ? del_ ## type : ctl_ ## type ##s[chr])

static const char ctl_names[][3] = {
    "NUL", "SOH", "SOT", "ETX", "EOT", "ENQ", "ACK", "BEL",
    "BS",  "TB",  "LF",  "VT",  "FF",  "CR",  "SO",  "SI",
    "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
    "CAN", "EM",  "SUB", "ESC", "FS",  "GS",  "RS",  "US"
};
static const char del_name[3] = "DEL";

static const char *ctl_descs[] = {
    "Null", "Start of heading", "Start of text", "End of text", "End of transmission",
    "Enquiry", "Acknowledge", "Bell", "Backspace", "Horizontal tab", "Line feed",
    "Vertical tab", "Form feed", "Carriage return", "Shift out", "Shift in",
    "Data link escape", "Device control 1", "Device control 2", "Device control 3",
    "Device control 4", "Negative acnknowledge", "Synchronous idle", 
    "End of transmission block", "Cancel", "End of medium", "Subsitute", "Escape",
    "File seperator", "Group seperator", "Record seperator", "Unit seperator"
};
static const char del_desc[] = "Delete";

static const char ctl_carrets[32] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";
static const char del_carret = '?';

static void displayChar(int chr) {
    if (isprint(chr)) {
        printf(GREEN "[ " CYAN "%c " GREEN "]" RESET , chr);
    } else if (chr < 128 && chr >= 0) {
        printf(CYAN " %-3.3s " RESET, CTL_DEL(name, chr));
    } else {
        printf("     ");
    }
}

static void displayHex(int chr) {
    printf(BLUE "%02x " RESET, chr);
}

static void displayDec(int chr) {
    printf(PURPLE "%-3d" RESET, chr);
}

static void displayBinary(int chr) {
    printf(INT8_STR_CLR, INT8_ARG_CLR(chr, RED_INT, RESET));
}

static void displayCaret(int chr) {
    if (isprint(chr) || chr > 127 || chr < 0) {
        printf("   ");
    } else {
        printf(GREEN "^%c " RESET, CTL_DEL(carret, chr));
    }
}

static void displayDescription(int chr) {
    if (isprint(chr) || chr > 127 || chr < 0) {
        printf("                         ");
    } else {
        printf(YELLOW "%-25.25s" RESET, CTL_DEL(desc, chr));
    }
}

const column chr_col = {.name = "Char", .len = 5, .displayCell = displayChar};
const column hex_col = {.name = "Hex", .len = 3, .displayCell = displayHex};
const column dec_col = {.name = "Dec", .len = 3, .displayCell = displayDec};
const column bin_col = {.name = "Binary", .len = 8, .displayCell = displayBinary};
const column crt_col = {.name = "Crt", .len = 3, .displayCell = displayCaret};
const column dsc_col = {.name = "Description", .len = 25, .displayCell = displayDescription};