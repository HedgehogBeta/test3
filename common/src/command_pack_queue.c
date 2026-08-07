/**
 * @file    command_pack_queue.c
 * @brief   命令协议包队列实现与封包/解包函数。
 *          学生在此文件中完成 TODO 标注的函数。
 */
#include "command_pack_queue.h"
#include "led.h"

/* ================================================================
 * 题目 1：队列实现
 * ================================================================
 * 参考课上环形队列的 push/pop 写法，补全下面 5 个函数。
 * 提示：
 *   - head 指向下一个要读的位置，tail 指向下一个要写的位置
 *   - 用 count 区分"空"和"满"
 *   - push 时队列满则返回 false；pop 时队列空则返回 false
 */

/* TODO: 初始化队列，将 head/tail/count 归零 */
void packet_queue_init(packet_queue *q)
{
    /* 在此实现 */
}

/* TODO: 入队，成功返回 true，队列满返回 false */
bool packet_queue_push(packet_queue *q, const command_packet *pkt)
{
    /* 在此实现 */
}

/* TODO: 出队，将数据写入 *pkt，成功返回 true，队列空返回 false */
bool packet_queue_pop(packet_queue *q, command_packet *pkt)
{
    /* 在此实现 */
}

/* TODO: 判空 */
bool packet_queue_is_empty(const packet_queue *q)
{
    /* 在此实现 */
}

/* TODO: 判满 */
bool packet_queue_is_full(const packet_queue *q)
{
    /* 在此实现 */
}

/* ================================================================
 * 题目 2：位运算封包（含校验和）
 * ================================================================
 * 用移位和按位或将参数填入 command_packet，并计算校验和。
 * 要求：使用位运算完成，不得逐字节赋值成员。
 *
 * 例如 command_pack_create(&pkt, 0x03, LED1_PIN)：
 *   pkt.header   = {0xA5, 0xA5}
 *   pkt.cmd      = {0x03, LED1_PIN}
 *   pkt.checksum = (0xA5 + 0xA5 + 0x03 + LED1_PIN) 的低 8 位
 *
 * 提示：HEADER_BYTE 和 PACKET_CHECKSUM 宏已定义在头文件中。
 */

/* TODO: 用位运算完成封包，并填入校验和 */
void command_pack_create(command_packet *pkt, uint8_t cmd_high, uint8_t cmd_low)
{
    /* 在此实现 */
}

/* ================================================================
 * 题目 3：位运算解包（含校验）
 * ================================================================
 * 从 command_packet 中解出命令字节，校验包头和校验和。
 *
 * 要求：
 *   1. 用位运算将两个 header 字节拼成 uint16_t 与 0xA5A5 比较；
 *   2. 用 PACKET_CHECKSUM 宏重新计算校验和并与 pkt->checksum 比较；
 *   3. 校验通过则将 cmd_high / cmd_low 写入输出参数，返回 true；
 *      任意一项校验失败返回 false。
 *
 * 提示：uint16_t header_word = ((uint16_t)header[0] << 8) | header[1];
 */

/* TODO: 用位运算校验包头和校验和，解出 cmd_high / cmd_low */
bool command_pack_unpack(const command_packet *pkt, uint8_t *cmd_high, uint8_t *cmd_low)
{
    /* 在此实现 */
}

/* ================================================================
 * LED 命令执行（已封装，直接调用即可）
 * ================================================================
 * 根据解包出的命令控制 LED 闪烁。
 *   cmd_high：闪烁次数
 *   cmd_low：LED 引脚掩码
 */
void command_led_execute(uint8_t cmd_high, uint8_t cmd_low)
{
    uint8_t count = cmd_high;
    uint8_t mask  = cmd_low;

    for (uint8_t i = 0U; i < count; i++)
    {
        led_on(mask);
        HAL_Delay(200U);
        led_off(mask);
        HAL_Delay(200U);
    }
}
