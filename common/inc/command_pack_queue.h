/**
 * @file    command_pack_queue.h
 * @brief   命令协议包结构与包队列类型定义。
 *          本文件提供结构体类型，学生无需修改。
 *          队列实现与封包/解包函数在 command_pack_queue.c 中完成。
 */
#ifndef COMMAND_PACK_QUEUE_H
#define COMMAND_PACK_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================
 * 协议包结构体（4 字节，已提供，无需修改）
 * ================================================================
 * 字节布局：
 *   [0] header[0] — HEADER_HIGH_BYTE (0x0A)
 *   [1] header[1] — HEADER_LOW_BYTE  (0x5A)
 *   [2] cmd        — 命令字：高 4 位 = 闪烁次数，低 4 位 = LED 掩码
 *   [3] checksum   — 校验和 = (header[0]+header[1]+cmd) 的低 8 位
 *
 * 例如：{0x0A, 0x5A, 0x31, 0xXX} 表示"LED1 闪烁 3 次"（0x31 = (3<<4)|0x01）
 */
#define HEADER_HIGH_BYTE 0x0AU
#define HEADER_LOW_BYTE  0x5AU
#define HEADER_WORD      ((uint16_t)(((uint16_t)HEADER_HIGH_BYTE << 8) | HEADER_LOW_BYTE))

/* LED 掩码：bit0~bit3 对应 LED1~LED4 */
#define LED_MASK_LED1  0x01U
#define LED_MASK_LED2  0x02U
#define LED_MASK_LED3  0x04U
#define LED_MASK_LED4  0x08U

/* cmd 字节内的位段位置 */
#define CMD_BLINK_SHIFT 4U   /* 闪烁次数占高 4 位 */
#define CMD_LED_MASK    0x0FU /* LED 掩码占低 4 位 */

typedef struct
{
    uint8_t header[2];   /* 帧头 {0x0A, 0x5A} */
    uint8_t cmd;         /* 命令字：高4位次数 + 低4位掩码 */
    uint8_t checksum;    /* 校验和，header[0]+header[1]+cmd 的低 8 位 */
} command_packet;

/* ================================================================
 * 校验和宏（已提供）
 * ================================================================
 * 用法：checksum = PACKET_CHECKSUM(pkt->header[0], pkt->header[1], pkt->cmd);
 */
#define PACKET_CHECKSUM(h0, h1, c) ((uint8_t)((h0) + (h1) + (c)))

/* ================================================================
 * 包队列结构体（已提供，无需修改）
 * ================================================================
 * 用环形队列缓存多个 command_packet。
 * head/tail/count 的用法与课上讲的 ring_buffer 一致。
 */
#define PACKET_QUEUE_SIZE 16

typedef struct
{
    command_packet buf[PACKET_QUEUE_SIZE];
    uint8_t        head;
    uint8_t        tail;
    uint8_t        count;
} packet_queue;

/* ================================================================
 * 需要学生实现的函数声明
 * ================================================================ */

/* ---- 题目 1：队列操作 ---- */
void packet_queue_init(packet_queue *q);
bool packet_queue_push(packet_queue *q, const command_packet *pkt);
bool packet_queue_pop(packet_queue *q, command_packet *pkt);
bool packet_queue_is_empty(const packet_queue *q);
bool packet_queue_is_full(const packet_queue *q);

/* ---- 题目 2：位运算封包 ---- */
void command_pack_create(command_packet *pkt, uint8_t blink_count, uint8_t led_mask);

/* ---- 题目 3：位运算解包 ---- */
bool command_pack_unpack(const command_packet *pkt, uint8_t *blink_count, uint8_t *led_mask);

/* ---- 已封装的 LED 命令执行（学生直接调用即可） ---- */
void command_led_execute(uint8_t blink_count, uint8_t led_mask);

#ifdef __cplusplus
}
#endif

#endif /* COMMAND_PACK_QUEUE_H */
