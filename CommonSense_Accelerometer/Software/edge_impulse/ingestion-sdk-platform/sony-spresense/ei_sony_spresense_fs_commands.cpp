
/* Include ----------------------------------------------------------------- */
#include "ei_sony_spresense_fs_commands.h"
#include "ei_device_sony_spresense.h"

#define SERIAL_FLASH 0
#define MICRO_SD     1
#define RAM          2

#define SAMPLE_MEMORY MICRO_SD

#define SIZE_RAM_BUFFER 0x10000//(0x20000)
#define RAM_BLOCK_SIZE	1024
#define RAM_N_BLOCKS    (SIZE_RAM_BUFFER / RAM_BLOCK_SIZE)

#define FILE_NAME_CONFIG    "config.bin"
#define FILE_NAME_SAMPLE    "sample.bin"
#define FILE_MAX_SIZE       0x200000
#define FILE_BLOCK_SIZE     1024
#define FILE_N_BLOCKS       (FILE_MAX_SIZE / FILE_BLOCK_SIZE)

/* Private function prototypes --------------------------------------------- */
#if (SAMPLE_MEMORY == SERIAL_FLASH)
static uint32_t flash_write(uint32_t address, const uint8_t *buffer, uint32_t bufferSize);
static uint32_t flash_erase_sectors(uint32_t startAddress, uint32_t nSectors);
static uint32_t flash_wait_while_busy(void);
static void flash_write_enable(void);
static uint8_t flash_status_register(void);
static void flash_erase_sector(uint32_t byteAddress);
static void flash_erase_block(uint32_t byteAddress);
static void flash_program_page(uint32_t byteAddress, uint8_t *page, uint32_t pageBytes);
static uint32_t flash_read_data(uint32_t byteAddress, uint8_t *buffer, uint32_t readBytes);
#endif

extern "C" bool spresense_openFile(const char *name, bool write);
extern "C" bool spresense_closeFile(const char *name);
extern "C" bool spresense_writeToFile(const char *name, const uint8_t *buf, uint32_t length);
extern "C" uint32_t spresense_readFromFile(const char *name, uint8_t *buf, uint32_t length);

/* Private variables ------------------------------------------------------- */
static bool sd_card_inserted = true;

#if (SAMPLE_MEMORY == RAM)
static uint8_t ram_memory[SIZE_RAM_BUFFER];
#endif

/** 32-bit align write buffer size */
#define WORD_ALIGN(a) ((a & 0x3) ? (a & ~0x3) + 0x4 : a)

/**
 * @brief      Copy configuration data to config pointer
 *
 * @param      config       Destination pointer for config
 * @param[in]  config_size  Size of configuration in bytes
 *
 * @return     ei_sony_spresense_ret_t enum
 */
int ei_sony_spresense_fs_load_config(uint32_t *config, uint32_t config_size)
{
    int retVal = SONY_SPRESENSE_FS_CMD_OK;

    if (config == NULL) {
        return SONY_SPRESENSE_FS_CMD_NULL_POINTER;
    }

#if (SAMPLE_MEMORY == RAM)

    return retVal;

#elif (SAMPLE_MEMORY == SERIAL_FLASH)

    if (flash_wait_while_busy() == 0) {
        retVal = SONY_SPRESENSE_FS_CMD_READ_ERROR;
    }
    else {
        if (flash_read_data(0, (uint8_t *)config, config_size) != 0) {
            retVal = SONY_SPRESENSE_FS_CMD_READ_ERROR;
        }
    }

    return retVal;

#elif (SAMPLE_MEMORY == MICRO_SD)

    if(spresense_openFile((const char *)FILE_NAME_CONFIG, false) == false) {

        /* Try to create and write to the config file */
        if(spresense_openFile((const char *)FILE_NAME_CONFIG, true) == false) {
            /* Missing SD card, return OK so default config can be loaded */
            ei_printf("File cannot open %s is the SD card inserted?\r\n", FILE_NAME_CONFIG);
            sd_card_inserted = false;
        }
        else {
            spresense_writeToFile((const char *)FILE_NAME_CONFIG, (const uint8_t *)config, config_size);
            spresense_closeFile((const char *)FILE_NAME_CONFIG);
        }

        return SONY_SPRESENSE_FS_CMD_OK;
    }

    if(spresense_readFromFile(FILE_NAME_CONFIG, (uint8_t *)config, config_size) < 0) {
        retVal = SONY_SPRESENSE_FS_CMD_READ_ERROR;
    }
    else if(spresense_closeFile((const char *)FILE_NAME_CONFIG) == false) {
        retVal = SONY_SPRESENSE_FS_CMD_FILE_ERROR;
    }
    return retVal;
#endif
}

/**
 * @brief      Write config to Flash
 *
 * @param[in]  config       Pointer to configuration data
 * @param[in]  config_size  Size of configuration in bytes
 *
 * @return     ei_sony_spresense_ret_t enum
 */
int ei_sony_spresense_fs_save_config(const uint32_t *config, uint32_t config_size)
{
    int retVal = SONY_SPRESENSE_FS_CMD_OK;

    if (config == NULL) {
        return SONY_SPRESENSE_FS_CMD_NULL_POINTER;
    }

#if (SAMPLE_MEMORY == RAM)

    return retVal;

#elif (SAMPLE_MEMORY == SERIAL_FLASH)

    retVal = flash_erase_sectors(0, 1);

    return (retVal == SONY_SPRESENSE_FS_CMD_OK) ? flash_write(0, (const uint8_t *)config, config_size)
                                     : retVal;

#elif (SAMPLE_MEMORY == MICRO_SD)

    if(sd_card_inserted == false) {
        retVal = SONY_SPRESENSE_FS_CMD_OK;
    }
    else if(spresense_openFile((const char *)FILE_NAME_CONFIG, true) == false) {

        retVal = SONY_SPRESENSE_FS_CMD_FILE_ERROR;
    }
    else if(spresense_writeToFile((const char *)FILE_NAME_CONFIG, (const uint8_t *)config, config_size) == false) {
        retVal = SONY_SPRESENSE_FS_CMD_WRITE_ERROR;
    }

    else if(spresense_closeFile((const char *)FILE_NAME_CONFIG) == false) {
        retVal = SONY_SPRESENSE_FS_CMD_FILE_ERROR;
    }

    return retVal;
#endif
}

/**
 * @brief      Erase blocks in sample data space
 *
 * @param[in]  start_block  The start block
 * @param[in]  end_address  The end address
 *
 * @return     ei_sony_spresense_ret_t
 */
int ei_sony_spresense_fs_erase_sampledata(uint32_t start_block, uint32_t end_address)
{
#if (SAMPLE_MEMORY == RAM)
    return SONY_SPRESENSE_FS_CMD_OK;
#elif (SAMPLE_MEMORY == SERIAL_FLASH)
    return flash_erase_sectors(MX25R_BLOCK64_SIZE, end_address / MX25R_SECTOR_SIZE);
#elif (SAMPLE_MEMORY == MICRO_SD)
    return (int)!spresense_openFile((const char *)FILE_NAME_SAMPLE, true);
#endif
}

/**
 * @brief      Write sample data
 *
 * @param[in]  sample_buffer   The sample buffer
 * @param[in]  address_offset  The address offset
 * @param[in]  n_samples       The n samples
 *
 * @return     ei_sony_spresense_ret_t
 */
int ei_sony_spresense_fs_write_samples(const void *sample_buffer, uint32_t address_offset, uint32_t n_samples)
{

#if (SAMPLE_MEMORY == RAM)
    uint32_t n_word_samples = WORD_ALIGN(n_samples);

    if ((address_offset + n_word_samples) > SIZE_RAM_BUFFER) {
        return SONY_SPRESENSE_FS_CMD_WRITE_ERROR;
    }
    else if (sample_buffer == 0) {
        return SONY_SPRESENSE_FS_CMD_NULL_POINTER;
    }

    for (int i = 0; i < n_word_samples; i++) {
        ram_memory[address_offset + i] = *((char *)sample_buffer + i);
    }
    return SONY_SPRESENSE_FS_CMD_OK;

#elif (SAMPLE_MEMORY == SERIAL_FLASH)
    uint32_t n_word_samples = WORD_ALIGN(n_samples);

    return flash_write(
        MX25R_BLOCK64_SIZE + address_offset,
        (const uint8_t *)sample_buffer,
        n_word_samples);

#elif (SAMPLE_MEMORY == MICRO_SD)

    if(spresense_writeToFile((const char *)FILE_NAME_CONFIG, (const uint8_t *)sample_buffer, n_samples) == true) {
        return SONY_SPRESENSE_FS_CMD_OK;
    }
    else {
        return SONY_SPRESENSE_FS_CMD_WRITE_ERROR;
    }
#endif
}

/**
 * @brief      Read sample data
 *
 * @param      sample_buffer   The sample buffer
 * @param[in]  address_offset  The address offset
 * @param[in]  n_read_bytes    The n read bytes
 *
 * @return     ei_sony_spresense_ret_t
 */
int ei_sony_spresense_fs_read_sample_data(void *sample_buffer, uint32_t address_offset, uint32_t n_read_bytes)
{
#if (SAMPLE_MEMORY == RAM)
    if ((address_offset + n_read_bytes) > SIZE_RAM_BUFFER) {
        return SONY_SPRESENSE_FS_CMD_READ_ERROR;
    }
    else if (sample_buffer == 0) {
        return SONY_SPRESENSE_FS_CMD_NULL_POINTER;
    }

    for (int i = 0; i < n_read_bytes; i++) {
        *((char *)sample_buffer + i) = ram_memory[address_offset + i];
    }
    return SONY_SPRESENSE_FS_CMD_OK;

#elif (SAMPLE_MEMORY == SERIAL_FLASH)

    int retVal = SONY_SPRESENSE_FS_CMD_OK;

    if (flash_wait_while_busy() == 0) {
        retVal = SONY_SPRESENSE_FS_CMD_READ_ERROR;
    }
    else {
        if (flash_read_data(
                MX25R_BLOCK64_SIZE + address_offset,
                (uint8_t *)sample_buffer,
                n_read_bytes) != 0) {
            retVal = SONY_SPRESENSE_FS_CMD_READ_ERROR;
        }
    }

    return retVal;

#elif (SAMPLE_MEMORY == MICRO_SD)

    int retVal = SONY_SPRESENSE_FS_CMD_OK;

    if(address_offset == 0) {
        if(spresense_openFile((const char *)FILE_NAME_SAMPLE, true) == false) {
            retVal = SONY_SPRESENSE_FS_CMD_FILE_ERROR;
        }
    }

    if(spresense_readFromFile(FILE_NAME_SAMPLE, (uint8_t *)sample_buffer, n_read_bytes) < 0) {
        retVal = SONY_SPRESENSE_FS_CMD_READ_ERROR;
    }

    return retVal;

#endif
}

/**
 * @brief Close file on SD card
 *
 */
void ei_sony_spresense_fs_close_sample_file(void)
{
#if (SAMPLE_MEMORY == MICRO_SD)
    spresense_closeFile((const char *)FILE_NAME_SAMPLE);
#endif
}

/**
 * @brief      Get block size (Smallest erasble block). 
 *
 * @return     Length of 1 block
 */
uint32_t ei_sony_spresense_fs_get_block_size(void)
{
#if (SAMPLE_MEMORY == RAM)
    return RAM_BLOCK_SIZE;
#elif (SAMPLE_MEMORY == SERIAL_FLASH)
    return MX25R_SECTOR_SIZE;
#elif (SAMPLE_MEMORY == MICRO_SD)
    if(sd_card_inserted == false) {
        return 0;
    }
    else {
        return FILE_BLOCK_SIZE;
    }
#endif
}

/**
 * @brief      Get available sample blocks
 *
 * @return     Sample memory size / block size
 */
uint32_t ei_sony_spresense_fs_get_n_available_sample_blocks(void)
{
#if (SAMPLE_MEMORY == RAM)
    return RAM_N_BLOCKS;
#elif (SAMPLE_MEMORY == SERIAL_FLASH)
    return (MX25R_CHIP_SIZE - MX25R_BLOCK64_SIZE) / MX25R_SECTOR_SIZE;
#elif (SAMPLE_MEMORY == MICRO_SD)
    return FILE_N_BLOCKS;
#endif
}

#if (SAMPLE_MEMORY == SERIAL_FLASH)
/**
 * @brief      Write a buffer to memory @ address
 *
 * @param[in]  address     The address
 * @param[in]  buffer      The buffer
 * @param[in]  bufferSize  The buffer size in bytes
 *
 * @return     ei_sony_spresense_ret_t
 */
static uint32_t flash_write(uint32_t address, const uint8_t *buffer, uint32_t bufferSize)
{
    int stat;
    int retry = MX25R_RETRY;
    int offset = 0;

    if (flash_wait_while_busy() == 0)
        return SONY_SPRESENSE_FS_CMD_WRITE_ERROR;

    do {
        uint32_t n_bytes;

        retry = MX25R_RETRY;
        do {
            flash_write_enable();
            stat = flash_status_register();
        } while (!(stat & MX25R_STAT_WEL) && --retry);

        if (!retry) {
            return SONY_SPRESENSE_FS_CMD_WRITE_ERROR;
        }

        if (bufferSize > MX25R_PAGE_SIZE) {
            n_bytes = MX25R_PAGE_SIZE;
            bufferSize -= MX25R_PAGE_SIZE;
        }
        else {
            n_bytes = bufferSize;
            bufferSize = 0;
        }

        /* If write overflows page, split up in 2 writes */
        if ((((address + offset) & 0xFF) + n_bytes) > MX25R_PAGE_SIZE) {

            int diff = MX25R_PAGE_SIZE - ((address + offset) & 0xFF);

            flash_program_page(address + offset, ((uint8_t *)buffer + offset), diff);

            if (flash_wait_while_busy() == 0)
                return SONY_SPRESENSE_FS_CMD_WRITE_ERROR;

            // retry = MX25R_RETRY;
            // do {
            // 	flash_write_enable();
            // 	stat = flash_status_register();
            // }while(!(stat & MX25R_STAT_WEL) && --retry);

            // if(!retry) {
            // 	return SONY_SPRESENSE_FS_CMD_WRITE_ERROR;
            // }

            /* Update index pointers */
            n_bytes -= diff;
            offset += diff;

            bufferSize += n_bytes;
        }

        else {
            flash_program_page(address + offset, ((uint8_t *)buffer + offset), n_bytes);

            offset += n_bytes;
        }

        // flash_program_page(address + offset, ((uint8_t *)buffer + offset), n_bytes);

        if (flash_wait_while_busy() == 0)
            return SONY_SPRESENSE_FS_CMD_WRITE_ERROR;

        // offset += MX25R_PAGE_SIZE;

    } while (bufferSize);

    return SONY_SPRESENSE_FS_CMD_OK;
}

/**
 * @brief      Erase mulitple flash sectors
 *
 * @param[in]  startAddress  The start address
 * @param[in]  nSectors      The sectors
 *
 * @return     ei_sony_spresense_ret_t
 */
static uint32_t flash_erase_sectors(uint32_t startAddress, uint32_t nSectors)
{
    int stat;
    int retry = MX25R_RETRY;
    uint32_t curSector = 0;

    do {
        if (flash_wait_while_busy() == 0) {
            return SONY_SPRESENSE_FS_CMD_ERASE_ERROR;
        }

        do {
            flash_write_enable();
            stat = flash_status_register();
        } while (!(stat & MX25R_STAT_WEL) && --retry);

        flash_erase_sector(startAddress + (MX25R_SECTOR_SIZE * curSector));

    } while (++curSector < nSectors);

    return SONY_SPRESENSE_FS_CMD_OK;
}

/**
 * @brief      Read status register and check WIP (write in progress)
 * @return     n retries, if 0 device is hanging
 */
static uint32_t flash_wait_while_busy(void)
{
    uint32_t stat;
    uint32_t retry = MX25R_RETRY;

    stat = flash_status_register();

    while ((stat & MX25R_STAT_WIP) && --retry) {

        stat = flash_status_register();
    }

    return retry;
}

/**
 * @brief      Send the write enable command over SPI
 */
static void flash_write_enable(void)
{
    //uint8_t spiTransfer[1];

    //spiTransfer[0] = MX25R_WREN;
    //Sony_SpresenseCspSpiTransferPoll(
    //    SONY_SPRESENSE_SPI_NUM,
    //    &spiTransfer[0],
    //    1,
    //    &spiTransfer[0],
    //    0,
    //    SONY_SPRESENSE_BSP_SPIFLASH_CS_NUM,
    //    eSpiSequenceFirstLast);
}

/**
 * @brief      Send a read status register frame over SPI
 *
 * @return     Chip status register
 */
static uint8_t flash_status_register(void)
{
    uint8_t spiTransfer[2];
    // TODO
    return spiTransfer[0];
}

/**
 * @brief      Send a sector erase frame with start address
 *
 * @param[in]  byteAddress  The byte address
 */
static void flash_erase_sector(uint32_t byteAddress)
{
    uint8_t spiTransfer[4];
    //TODO
}

/**
 * @brief      Send a block erase frame with start address
 *
 * @param[in]  byteAddress  The byte address
 */
static void flash_erase_block(uint32_t byteAddress)
{
    uint8_t spiTransfer[4];
    //TODO
}

/**
 * @brief      Send program page command and send data over SPI
 *
 * @param[in]  byteAddress  The byte address
 * @param      page         The page
 * @param[in]  pageBytes    The page bytes
 */
static void flash_program_page(uint32_t byteAddress, uint8_t *page, uint32_t pageBytes)
{
    uint8_t spiTransfer[4];
    //TODO
}

/**
 * @brief      Send read command and get data over SPI
 *
 * @param[in]  byteAddress  The byte address
 * @param      buffer       The buffer
 * @param[in]  readBytes    The read bytes
 *
 * @return     Sony_Spresense status
 */
static uint32_t flash_read_data(uint32_t byteAddress, uint8_t *buffer, uint32_t readBytes)
{
    uint8_t spiTransfer[4];
    //TODO
    return 0;
}
#endif
