//https://github.com/viktorvano/STM32F3Discovery_internal_FLASH/blob/master/Src/main.c
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* USER CODE BEGIN PD */
#define FLASH_STORAGE 0x0800FC00            //64 page for stm32f030c8t6
#define page_size 0x400                     //1 kb
/* USER CODE END PD */

/* USER CODE BEGIN 0 */
void erase_flash(void)
{
	HAL_FLASH_Unlock();
//	HAL_FLASH_OB_Unlock();
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = FLASH_STORAGE;
	EraseInitStruct.NbPages = 1;
	uint32_t PageError;
	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	HAL_FLASH_Lock();
//	HAL_FLASH_OB_Lock();
}
void save_to_flash(uint8_t *data)
{
	volatile uint32_t data_to_FLASH[(strlen((char*)data)/4)	+ (int)((strlen((char*)data) % 4) != 0)];
	memset((uint8_t*)data_to_FLASH, 0, strlen((char*)data_to_FLASH));
	strcpy((char*)data_to_FLASH, (char*)data);
	volatile uint32_t data_length = (strlen((char*)data_to_FLASH) / 4)
									+ (int)((strlen((char*)data_to_FLASH) % 4) != 0);
	volatile uint16_t pages = (strlen((char*)data)/page_size)
									+ (int)((strlen((char*)data)%page_size) != 0);
	  HAL_FLASH_Unlock();
	  HAL_FLASH_OB_Unlock();
	  FLASH_EraseInitTypeDef EraseInitStruct;
	  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	  EraseInitStruct.PageAddress = FLASH_STORAGE;
	  EraseInitStruct.NbPages = pages;
	  uint32_t PageError;
	  volatile uint32_t write_cnt=0, index=0;
	  volatile HAL_StatusTypeDef status;
	  status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	  while(index < data_length)
	  {
		  if (status == HAL_OK)
		  {
			  status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_STORAGE+write_cnt, data_to_FLASH[index]);
			  if(status == HAL_OK)
			  {
				  write_cnt += 4;
				  index++;
			  }
		  }
	  }
	  HAL_FLASH_OB_Lock();
	  HAL_FLASH_Lock();
}

void read_flash(uint8_t* data)
{
	volatile uint32_t read_data;
	volatile uint32_t read_cnt=0;
	do
	{
		read_data = *(uint32_t*)(FLASH_STORAGE + read_cnt);
		if(read_data != 0xFFFFFFFF)
		{
			data[read_cnt] = (uint8_t)read_data;
			data[read_cnt + 1] = (uint8_t)(read_data >> 8);
			data[read_cnt + 2] = (uint8_t)(read_data >> 16);
			data[read_cnt + 3] = (uint8_t)(read_data >> 24);
			read_cnt += 4;
		}
	}while(read_data != 0xFFFFFFFF);
}
/* USER CODE END 0 */

  char flash_data[50];
  memset(flash_data, 0, sizeof(flash_data));
  read_flash((uint8_t*)flash_data);
  if (flash_data[0] == 0x00) {strcpy(flash_data, "0244");}
  else {flash_data[3]++;}
  save_to_flash((uint8_t*)flash_data);
//  erase_flash();


  char flash_data[50];
  memset(flash_data, 0, sizeof(flash_data));
  strcpy(flash_data, "Hello World!!!");

  save_to_flash((uint8_t*)flash_data);

  char read_data[50];
  memset(read_data, 0, sizeof(read_data));

  read_flash((uint8_t*)read_data);

  float write_number = 235.756f;
  float *pointer_write = &write_number;
  save_to_flash((uint8_t*)pointer_write);

  float read_number = 0.0f;
  float *pointer_read = &read_number;
  read_flash((uint8_t*)pointer_read);
  