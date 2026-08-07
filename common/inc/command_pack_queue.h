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
 * 协议包结构体（5 字节，已提供，无需修改）
 * ================================================================
 * 字节布局：
 *   [0] header[0] — 固定 0xA5
 *   [1] header[1] — 固定 0xA5
 *   [2] cmd[0]    — 命令高字节，如 0x10（闪烁次数）
 *   [3] cmd[1]    — 命令低字节，如 LED1_PIN（LED 掩码）
 *   [4] checksum   — 校验和 = (header[0]+header[1]+cmd[0]+cmd[1]) 的低 8 位
 *
 * 例如：{0xA5, 0xA5, 0x03, LED1_PIN, 0xXX} 表示"LED1 闪烁 3 次"
 */
#define HEADER_BYTE 0xA5U

typedef struct
{
    uint8_t header[2];   /* 帧头，固定 {0xA5, 0xA5} */
    uint8_t cmd[2];      /* 命令，{闪烁次数, LED 掩码} */
    uint8_t checksum;    /* 校验和，header[0]+header[1]+cmd[0]+cmd[1] 的低 8 位 */
} command_packet;

/* ================================================================
 * 校验和宏（已提供）
 * ================================================================
 * 用法：checksum = PACKET_CHECKSUM(pkt->header[0], pkt->header[1],
 *                                  pkt->cmd[0], pkt->cmd[1]);
 */
#define PACKET_CHECKSUM(h0, h1, c0, c1) ((uint8_t)((h0) + (h1) + (c0) + (c1)))

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
void command_pack_create(command_packet *pkt, uint8_t cmd_high, uint8_t cmd_low);

/* ---- 题目 3：位运算解包 ---- */
bool command_pack_unpack(const command_packet *pkt, uint8_t *cmd_high, uint8_t *cmd_low);

/* ---- 已封装的 LED 命令执行（学生直接调用即可） ---- */
void command_led_execute(uint8_t cmd_high, uint8_t cmd_low);

#ifdef __cplusplus
}
#endif

#endif /* COMMAND_PACK_QUEUE_H */
