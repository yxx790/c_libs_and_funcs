/* USER CODE BEGIN PV */
uint8_t short_state = 0; //для кнопки
uint8_t long_state = 0; //для кнопки
uint32_t time_key1 = 0; //для кнопки

uint8_t page = 0; uint8_t selected = 0;
/* USER CODE END PV */

/* USER CODE BEGIN WHILE */
while (1){
	uint32_t ms = HAL_GetTick();
	uint8_t key1_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);//отслеживаемый пин
	if(key1_state == 0 && !short_state && (ms - time_key1) > 50) {short_state = 1; long_state = 0; time_key1 = ms;}
    else if(key1_state == 0 && !long_state && (ms - time_key1) > 500){//время длинного нажатия
        long_state = 1;
        //   действие на длинное нажатие
        if          (page == 0)						{page = 1; selected = 1; SSD1306_Clear();}//to menu
        else if 	(page == 1 && selected == 0)	{page = 0; selected = 0; SSD1306_Clear();}//exit
        else if 	(page == 1 && selected == 1)	{page = 2; selected = 2; SSD1306_Clear();}//to calibration
        else if 	(page == 1 && selected == 2)	{page = 3; selected = 1; SSD1306_Clear();}//settings
        else if 	(page == 1 && selected == 3)	{page = 4; selected = 0; SSD1306_Clear();}//params
        //.....  
	}
	else if(key1_state == 1 && short_state && (ms - time_key1) > 50){
	  short_state = 0;
	  time_key1 = ms;
	  if(!long_state){
        // действие на короткое нажатие
		if (page==0) {selected++; if (selected > 7) {selected=0;}} 					//0page
		if (page==1) {selected++; if (selected > 3) {selected=0;}} 					//menu
        //.....
	  }
	}

	if (page == 0) {page0draw();}
	if (page == 1) {pageMenuDraw();}
	if (page == 2) {pageCalibrationDraw();}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
}
  /* USER CODE END 3 */