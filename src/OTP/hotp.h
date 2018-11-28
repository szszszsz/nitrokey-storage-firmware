/*
 * Author: Copyright (C) Andrzej Surowiec 2012
 *
 *
 * This file is part of Nitrokey.
 *
 * Nitrokey is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Nitrokey is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nitrokey. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file contains modifications done by Rudolf Boeddeker
 * For the modifications applies:
 *
 * Author: Copyright (C) Rudolf Boeddeker  Date: 2013-08-16
 *
 * Nitrokey  is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Nitrokey is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nitrokey. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HOTP_H
#define HOTP_H

typedef enum
{
    FLASH_BUSY = 1,
    FLASH_ERROR_PG,
    FLASH_ERROR_WRP,
    FLASH_COMPLETE,
    FLASH_TIMEOUT
} FLASH_Status;

/*
   #define NUMBER_OF_HOTP_SLOTS 2 #define NUMBER_OF_TOTP_SLOTS 4 */

#define NUMBER_OF_HOTP_SLOTS 3
#define NUMBER_OF_TOTP_SLOTS 15



// Flash memory pages:
// 0x801E800
// 0x801EC00 <- slots
// 0x801F000 <- slot 1 counter
// 0x801F400 <- slot 2 counter
// 0x801F800
// 0x801FC00 <- backup page


/*
   slot structure: 1b 0x01 if slot is used (programmed) 15b slot name 20b secret 1b configuration flags: MSB [x|x|x|x|x|send token id|send enter
   after code?|no. of digits 6/8] LSB 12b token id 1b keyboard layout 2b TOTP interval value

 */

#define SLOT_CONFIG_DIGITS 0
#define SLOT_CONFIG_ENTER 1
#define SLOT_CONFIG_TOKENID 2
#define SLOT_CONFIG_PASSWORD_USED   3
#define SLOT_CONFIG_PASSWORD_TIMED  4

/*
   global config slot:

   1b slot sent after numlock 1b slot sent after caps lock 1b slot sent after scroll lock

 */

#define FLASH_PAGE_SIZE 512 // AVR

#define SLOT_PAGE_SIZE  500 // less than actual page, so we can copy it to backup page with additional info

#define FLASH_START            0x80000000
#define OTP_FLASH_START_PAGE   496
#define SLOTS_ADDRESS         (FLASH_START + OTP_FLASH_START_PAGE * FLASH_PAGE_SIZE + (FLASH_PAGE_SIZE*0))   // 0x8003e800
#define SLOT1_COUNTER_ADDRESS (FLASH_START + OTP_FLASH_START_PAGE * FLASH_PAGE_SIZE + (FLASH_PAGE_SIZE*5))   // 0x8003
#define SLOT2_COUNTER_ADDRESS (FLASH_START + OTP_FLASH_START_PAGE * FLASH_PAGE_SIZE + (FLASH_PAGE_SIZE*6))   // 0x8003
#define SLOT3_COUNTER_ADDRESS (FLASH_START + OTP_FLASH_START_PAGE * FLASH_PAGE_SIZE + (FLASH_PAGE_SIZE*7))   // 0x8003
#define BACKUP_PAGE_ADDRESS   (FLASH_START + OTP_FLASH_START_PAGE * FLASH_PAGE_SIZE + (FLASH_PAGE_SIZE*8))   // (5 Pages) 0x8003
#define TIME_ADDRESS          (FLASH_START + OTP_FLASH_START_PAGE * FLASH_PAGE_SIZE + (FLASH_PAGE_SIZE*13))   //

// OTP block = 3 flash blocks = 3*512 byte
#define BACKUP_ADDRESS_OFFSET ((FLASH_PAGE_SIZE * 3) - 12)  // 500 - no flash block addr
#define BACKUP_LENGTH_OFFSET  ((FLASH_PAGE_SIZE * 3) -  8)  // 504 - no flash block addr
#define BACKUP_OK_OFFSET      ((FLASH_PAGE_SIZE * 3) -  6)  // 506 - no flash block addr

#define OTP_SLOT_SIZE         128

#define GLOBAL_CONFIG_OFFSET  (OTP_SLOT_SIZE * 0)

#define HOTP_SLOT1_OFFSET     (OTP_SLOT_SIZE * 1)
#define HOTP_SLOT2_OFFSET     (OTP_SLOT_SIZE * 2)
#define HOTP_SLOT3_OFFSET     (OTP_SLOT_SIZE * 3)

#define TOTP_SLOT1_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  0))
#define TOTP_SLOT2_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  1))
#define TOTP_SLOT3_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  2))
#define TOTP_SLOT4_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  3))
#define TOTP_SLOT5_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  4))
#define TOTP_SLOT6_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  5))
#define TOTP_SLOT7_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  6))
#define TOTP_SLOT8_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  7))
#define TOTP_SLOT9_OFFSET     ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  8))
#define TOTP_SLOT10_OFFSET    ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE *  9))
#define TOTP_SLOT11_OFFSET    ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE * 10))
#define TOTP_SLOT12_OFFSET    ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE * 11))
#define TOTP_SLOT13_OFFSET    ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE * 12))
#define TOTP_SLOT14_OFFSET    ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE * 13))
#define TOTP_SLOT15_OFFSET    ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE * 14))
#define TOTP_SLOT16_OFFSET    ((OTP_SLOT_SIZE * 4) + (OTP_SLOT_SIZE * 15))

// End of data = ((128 * 4) + (128 * 16)) = 2560 >= 3 flash page


#define SLOT_TYPE_OFFSET     0
#define SLOT_NAME_OFFSET     1
#define SECRET_OFFSET       16
#define CONFIG_OFFSET       56
#define TOKEN_ID_OFFSET     57
#define INTERVAL_OFFSET     70

#define TIME_OFFSET          4
#define SECRET_LEN          40


#define TOKEN_PER_FLASHPAGE        (FLASH_PAGE_SIZE-8)


extern u32 hotp_slots[NUMBER_OF_HOTP_SLOTS];
extern u32 totp_slots[NUMBER_OF_TOTP_SLOTS + 1];
extern u32 hotp_slot_counters[NUMBER_OF_HOTP_SLOTS];
extern u32 hotp_slot_offsets[NUMBER_OF_HOTP_SLOTS];
extern u32 totp_slot_offsets[NUMBER_OF_TOTP_SLOTS + 1];

extern u8 page_buffer[FLASH_PAGE_SIZE * 3];

extern u64 current_time;

u16 getu16 (u8 * array);
u32 getu32 (u8 * array);
u64 getu64 (u8 * array);

u64 endian_swap (u64 x);

u32 dynamic_truncate (u8 * hmac_result);
void erase_counter (u8 slot);

void write_data_to_flash (u8 * data, u16 len, u32 addr);
u32 get_hotp_value (u64 counter, u8 * secret, u8 secret_length, u8 len);
u64 get_counter_value (u32 addr);
u32 get_flash_time_value (void);
u32 get_time_value (void);
u8 set_time_value (u32 time);
u8 set_counter_value (u32 addr, u64 counter);
u32 get_code_from_hotp_slot (u8 slot);
u8 increment_counter_page (u32 addr);
void write_to_slot (u8 * data, u16 offset, u16 len);
void backup_data (u8 * data, u16 len, u32 addr);
u8 check_backups (void);
u8 get_hotp_slot_config (u8 slot_number);
u8 get_totp_slot_config (u8 slot_number);
u32 get_code_from_totp_slot (u8 slot, u64 challenge);


u8* get_hotp_slot_addr (u8 slot_number);
u8* get_totp_slot_addr (u8 slot_number);

#endif // HOTP_H
