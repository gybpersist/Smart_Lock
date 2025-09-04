#ifndef __COM_CONFIG_H
#define __COM_CONFIG_H

typedef enum
{
    Com_OK,        ///< 通信成功（操作正常完成，无错误）
    Com_ERROR,     ///< 通信错误（如协议错误、校验失败、硬件异常等）
    Com_TIMEOUT,   ///< 通信超时（在规定时间内未收到响应或完成操作）
    Com_OTHER      ///< 其他未明确分类的通信状态（预留扩展用）
} Com_Status;

#endif
