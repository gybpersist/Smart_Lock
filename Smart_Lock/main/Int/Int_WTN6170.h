#ifndef __INT_WTN6170_H__
#define __INT_WTN6170_H__

#include "driver/gpio.h"
#include "esp_task.h"
#include "sys/unistd.h"

#define GPIO_WTN6170_DATA GPIO_NUM_9
#define GPIO_WTN6170_HIGH gpio_set_level(GPIO_WTN6170_DATA, 1)  //拉高引脚
#define GPIO_WTN6170_LOW gpio_set_level(GPIO_WTN6170_DATA, 0)   //拉低引脚

#define sayWithoutInt()          Int_WTN6170_SendCmd(0xf3)   // 发送无中断语音命令（取消当前播放或重置语音状态）
#define sayCtrlVolume(x)         Int_WTN6170_SendCmd(x)      // 调节音量 (小 0xE0 ~ 0xEF 大)
#define sayNum(x)                Int_WTN6170_SendCmd(x + 1)  // 播报数字（x为0-9，对应芯片预设的0-9语音）
#define sayWaterDrop()           Int_WTN6170_SendCmd(11)     // 播报"水滴声"（预设水滴音效）
#define sayBuzzer()              Int_WTN6170_SendCmd(12)     // 播报"蜂鸣声"（预设蜂鸣提示音）
#define sayAlarm()               Int_WTN6170_SendCmd(13)     // 播报"警报声"（预设警报提示音）
#define sayDoorBell()            Int_WTN6170_SendCmd(14)     // 播报"门铃声"（预设门铃提示音）
#define sayFail()                Int_WTN6170_SendCmd(16)     // 播报"失败"（操作失败提示）
#define sayPassword()            Int_WTN6170_SendCmd(19)     // 播报"密码"（提示输入密码）
#define sayDoorOpen()            Int_WTN6170_SendCmd(25)     // 播报"门已打开"（门解锁成功提示）
#define sayDoorClose()           Int_WTN6170_SendCmd(26)     // 播报"门已关闭"（门锁定成功提示）
#define sayIllegalOperation()    Int_WTN6170_SendCmd(28)     // 播报"非法操作"（提示不允许的操作）
#define sayVerify()              Int_WTN6170_SendCmd(31)     // 播报"验证中"（提示正在进行身份验证）
#define sayFinish()              Int_WTN6170_SendCmd(33)     // 播报"完成"（操作完成提示）
#define sayPressSharp()          Int_WTN6170_SendCmd(34)     // 播报"请按#号键"（提示按#键确认）
#define sayDelSucc()             Int_WTN6170_SendCmd(36)     // 播报"删除成功"（删除操作成功提示）
#define sayDelFail()             Int_WTN6170_SendCmd(37)     // 播报"删除失败"（删除操作失败提示）
#define sayDelAll()              Int_WTN6170_SendCmd(39)     // 播报"全部删除"（提示执行全部删除）
#define sayDelInvalid()          Int_WTN6170_SendCmd(40)     // 播报"删除无效"（删除目标不存在）
#define sayVerifySucc()          Int_WTN6170_SendCmd(41)     // 播报"验证成功"（身份验证通过）
#define sayVerifyFail()          Int_WTN6170_SendCmd(42)     // 播报"验证失败"（身份验证未通过）
#define saySetSucc()             Int_WTN6170_SendCmd(43)     // 播报"设置成功"（参数设置成功提示）
#define saySetFail()             Int_WTN6170_SendCmd(44)     // 播报"设置失败"（参数设置失败提示）
#define sayOperateSucc()         Int_WTN6170_SendCmd(46)     // 播报"操作成功"（通用操作成功提示）
#define sayOperateFail()         Int_WTN6170_SendCmd(47)     // 播报"操作失败"（通用操作失败提示）
#define sayInvalid()             Int_WTN6170_SendCmd(48)     // 播报"无效"（输入或操作无效提示）
#define sayAddSucc()             Int_WTN6170_SendCmd(49)     // 播报"添加成功"（新增记录成功提示）
#define sayAddFail()             Int_WTN6170_SendCmd(50)     // 播报"添加失败"（新增记录失败提示）
#define sayAddUser()             Int_WTN6170_SendCmd(51)     // 播报"添加用户"（提示进入添加用户流程）
#define sayDelUser()             Int_WTN6170_SendCmd(52)     // 播报"删除用户"（提示进入删除用户流程）
#define sayUserFull()            Int_WTN6170_SendCmd(53)     // 播报"用户已满"（提示用户数量达上限）
#define sayAddAdmin()            Int_WTN6170_SendCmd(54)     // 播报"添加管理员"（提示进入添加管理员流程）
#define sayDelAdmin()            Int_WTN6170_SendCmd(55)     // 播报"删除管理员"（提示进入删除管理员流程）
#define sayAdmIntull()           Int_WTN6170_SendCmd(56)     // 播报"管理员不存在"（目标管理员不存在，宏名可能为sayAdmInvalid笔误）
#define sayCheckAdmin()          Int_WTN6170_SendCmd(57)     // 检查管理员
#define sayRetry()               Int_WTN6170_SendCmd(58)     // 播报"请重试"（提示用户重新操作）
#define sayInputUserPassword()   Int_WTN6170_SendCmd(64)     // 播报"请输入用户密码"（提示输入普通用户密码）
#define sayInputUserPasswordAgain() Int_WTN6170_SendCmd(65)  // 播报"请再次输入用户密码"（提示确认用户密码）
#define sayPasswordAddSucc()     Int_WTN6170_SendCmd(66)     // 播报"密码添加成功"（用户密码设置成功）
#define sayPasswordAddFail()     Int_WTN6170_SendCmd(67)     // 播报"密码添加失败"（用户密码设置失败）
#define sayPasswordVerifySucc()  Int_WTN6170_SendCmd(68)     // 播报"密码验证成功"（用户密码验证通过）
#define sayPasswordVerifyFail()  Int_WTN6170_SendCmd(69)     // 播报"密码验证失败"（用户密码验证未通过）
#define sayInputAdminPassword()  Int_WTN6170_SendCmd(70)     // 播报"请输入管理员密码"（提示输入管理员密码）
#define sayInputAdminPasswordAgain() Int_WTN6170_SendCmd(71) // 播报"请再次输入管理员密码"（提示确认管理员密码）
#define sayAddUserFingerprint()  Int_WTN6170_SendCmd(76)     // 播报"添加用户指纹"（提示进入添加指纹流程）
#define sayDelUserFingerprint()  Int_WTN6170_SendCmd(77)     // 播报"删除用户指纹"（提示进入删除指纹流程）
#define sayPlaceFinger()         Int_WTN6170_SendCmd(80)     // 播报"请放置手指"（提示用户放置手指录入指纹）
#define sayPlaceFingerAgain()    Int_WTN6170_SendCmd(81)     // 播报"请再次放置手指"（提示再次放置手指确认）
#define sayTakeAwayFinger()      Int_WTN6170_SendCmd(82)     // 播报"请移开手指"（提示结束指纹操作）
#define sayFingerprintAddSucc()  Int_WTN6170_SendCmd(83)     // 播报"指纹添加成功"（指纹录入成功提示）
#define sayFingerprintAddFail()  Int_WTN6170_SendCmd(84)     // 播报"指纹添加失败"（指纹录入失败提示）
#define sayFingerprintVerifySucc() Int_WTN6170_SendCmd(85)   // 播报"指纹验证成功"（指纹匹配通过）
#define sayFingerprintVerifyFail() Int_WTN6170_SendCmd(86)   // 播报"指纹验证失败"（指纹匹配未通过）

/**
 * @brief 初始化 语言模块WTN6170
 * 
 */
void Int_WTN6170_Init(void);

/**
 * @brief 发送命令
 * 
 * @param cmd 命令
 */
void Int_WTN6170_SendCmd(uint8_t cmd);

#endif /* __INT_WTN6170_H__ */
