#include "OSAL_Parser.h"

int OSAL_Script_FetchParser_Data(char *main_name, char *sub_name, int value[], int count)
{
	int ret;
	user_gpio_set_t  gpio_info;
	disp_gpio_set_t  *gpio_list;
	ret = script_parser_fetch(main_name, sub_name, value, count);
	if(ret < 0) {
	} else {
		if(count == 1) {
		} else if(count == sizeof(disp_gpio_set_t)) {
			memcpy(&gpio_info, value, sizeof(user_gpio_set_t));
			gpio_list = (disp_gpio_set_t  *)value;
			gpio_list->port = gpio_info.port;
			gpio_list->port_num = gpio_info.port_num;
			gpio_list->drv_level = gpio_info.drv_level;
			gpio_list->pull = gpio_info.pull;
			gpio_list->data = gpio_info.data;
		} else {
		}
	}
	
	return ret;
}

/* returns: 0:invalid, 1: int; 2:str, 3: gpio */
int OSAL_Script_FetchParser_Data_Ex(char *main_name, char *sub_name, int value[], script_parser_value_type_t *type, int count)
{
	int ret;
	ret = script_parser_fetch_ex(main_name, sub_name, value, type, count);
	if(ret < 0) {
		return 0;
	}

	if(*type == SCIRPT_PARSER_VALUE_TYPE_STRING)
		return 2;
	else if(*type == SCIRPT_PARSER_VALUE_TYPE_GPIO_WORD)
		return 3;

	return 0;
}

int OSAL_sw_get_ic_ver(void)
{
    /*
    enum sw_ic_ver ic_ver;
    int ret = 0xA;

    ic_ver = sw_get_ic_ver();
    if(ic_ver == MAGIC_VER_A)
    {
        ret = 0xA;
    }
    else if(ic_ver == MAGIC_VER_B)
    {
        ret = 0xB;
    }
    if(ic_ver == MAGIC_VER_C)
    {
        ret = 0xC;
    }
    
    return ret;
    */
    
    /* todo */
    return 0;
}
