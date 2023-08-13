#include "display.h"
#include "lcd.h"
#include "touch.h"
/*********************************************************************************************************************/
/**
  * @brief  draw_grid() : ��������
  * @param  screen_width ��ȫ����     
  * @param  screen_height: ȫ����
  * @param  cell_width:  ÿ�������
  * @param  cell_height: ÿ�������
	* @param  grid_line_width: �����߿�
	* @param  COLOR: ��ɫ
  * @retval None
  */
	
//draw_grid(screen_width, screen_height, cell_width, cell_height, grid_line_width,COLOR);
/*********************************************************************************************************************/
void draw_grid(uint16_t screen_width, 
	             uint16_t screen_height, 
               uint16_t cell_width,    
               uint16_t cell_height,   
               uint8_t grid_line_width,
               uint32_t COLOR)
{
    
   // ����ֱ��
    for (uint16_t x = 0; x <= screen_width; x += cell_width) {
        for (uint16_t y = 0; y < screen_height; y++) {
            for (uint16_t offset = 0; offset < grid_line_width; offset++) {
                lcd_draw_point(x + offset, y, COLOR);
            }
        }
    }

    // ��ˮƽ��
    for (uint16_t y = 0; y <= screen_height; y += cell_height) {
        for (uint16_t x = 0; x < screen_width; x++) {
            for (uint16_t offset = 0; offset < grid_line_width; offset++) {
                lcd_draw_point(x, y + offset, COLOR);
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------*/

void touch_key(void)
{
  /*֧�ֵ�����˫��������*/

	
	

}
