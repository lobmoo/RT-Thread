#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"

/* 使用PWM设备 */
struct rt_device_pwm  *pwm_device = RT_NULL; //定义 pwm 设备指针
/***** 蜂鸣器 相关定义声明 ********/
//void MX_TIM3_Init(void);
#define BEEP_PWM_DEVICE  "pwm5"  //定时器5
#define BEEP_PWM_CH      1       //通道1
/***************************************************************/
/* defined the LED0 pin: PB1 */

#define PULSE_ENCODER_DEV_NAME    "pulse3"    /* 脉冲编码器名称 */

static int pulse_encoder_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    rt_device_t pulse_encoder_dev = RT_NULL;   /* 脉冲编码器设备句柄 */

    rt_int32_t count;

    /* 查找脉冲编码器设备 */
    pulse_encoder_dev = rt_device_find(PULSE_ENCODER_DEV_NAME);
    if (pulse_encoder_dev == RT_NULL)
    {
        rt_kprintf("pulse encoder sample run failed! can't find %s device!\n", PULSE_ENCODER_DEV_NAME);
        return RT_ERROR;
    }

    /* 以只读方式打开设备 */
    ret = rt_device_open(pulse_encoder_dev, RT_DEVICE_OFLAG_RDONLY);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", PULSE_ENCODER_DEV_NAME);
        return ret;
    }else
        {
                rt_kprintf("open %s device success!\n", PULSE_ENCODER_DEV_NAME);
        }

   for(rt_uint32_t i = 0; i <= 10; i++)
    {
        rt_thread_mdelay(500);
        /* 读取脉冲编码器计数值 */
        rt_device_read(pulse_encoder_dev, 0, &count, 1);
        /* 清空脉冲编码器计数值 */
        rt_device_control(pulse_encoder_dev, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);
        rt_kprintf("get count %d\n",count);
    }

    rt_device_close(pulse_encoder_dev);
    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(pulse_encoder_sample, pulse encoder sample);


#if 1
int main(void)
{


    int count = 1;

    /* 查找PWM设备 */
    pwm_device = (struct rt_device_pwm *)rt_device_find(BEEP_PWM_DEVICE);
    if (pwm_device == RT_NULL)
    {
        rt_kprintf("pwm device %s not found!\n", BEEP_PWM_DEVICE);
        return -RT_ERROR;
    }
    rt_pwm_set(pwm_device, BEEP_PWM_CH, 500000, 250000);   //channel,period,pulse
    rt_pwm_enable(pwm_device, BEEP_PWM_CH);                 //使能蜂鸣器对应的 PWM 通道

    while (count++)
    {
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

#endif

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);


