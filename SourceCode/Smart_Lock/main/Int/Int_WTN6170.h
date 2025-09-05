#ifndef __INT_WTN6170_H__
#define __INT_WTN6170_H__

#include "driver/gpio.h"
#include "esp_task.h"
#include "sys/unistd.h"

#define GPIO_WTN6170_DATA GPIO_NUM_9
#define GPIO_WTN6170_HIGH gpio_set_level(GPIO_WTN6170_DATA, 1)  //��������
#define GPIO_WTN6170_LOW gpio_set_level(GPIO_WTN6170_DATA, 0)   //��������

#define sayWithoutInt()          Int_WTN6170_SendCmd(0xf3)   // �������ж��������ȡ����ǰ���Ż���������״̬��
#define sayCtrlVolume(x)         Int_WTN6170_SendCmd(x)      // �������� (С 0xE0 ~ 0xEF ��)
#define sayNum(x)                Int_WTN6170_SendCmd(x + 1)  // �������֣�xΪ0-9����ӦоƬԤ���0-9������
#define sayWaterDrop()           Int_WTN6170_SendCmd(11)     // ����"ˮ����"��Ԥ��ˮ����Ч��
#define sayBuzzer()              Int_WTN6170_SendCmd(12)     // ����"������"��Ԥ�������ʾ����
#define sayAlarm()               Int_WTN6170_SendCmd(13)     // ����"������"��Ԥ�辯����ʾ����
#define sayDoorBell()            Int_WTN6170_SendCmd(14)     // ����"������"��Ԥ��������ʾ����
#define sayFail()                Int_WTN6170_SendCmd(16)     // ����"ʧ��"������ʧ����ʾ��
#define sayPassword()            Int_WTN6170_SendCmd(19)     // ����"����"����ʾ�������룩
#define sayDoorOpen()            Int_WTN6170_SendCmd(25)     // ����"���Ѵ�"���Ž����ɹ���ʾ��
#define sayDoorClose()           Int_WTN6170_SendCmd(26)     // ����"���ѹر�"���������ɹ���ʾ��
#define sayIllegalOperation()    Int_WTN6170_SendCmd(28)     // ����"�Ƿ�����"����ʾ������Ĳ�����
#define sayVerify()              Int_WTN6170_SendCmd(31)     // ����"��֤��"����ʾ���ڽ��������֤��
#define sayFinish()              Int_WTN6170_SendCmd(33)     // ����"���"�����������ʾ��
#define sayPressSharp()          Int_WTN6170_SendCmd(34)     // ����"�밴#�ż�"����ʾ��#��ȷ�ϣ�
#define sayDelSucc()             Int_WTN6170_SendCmd(36)     // ����"ɾ���ɹ�"��ɾ�������ɹ���ʾ��
#define sayDelFail()             Int_WTN6170_SendCmd(37)     // ����"ɾ��ʧ��"��ɾ������ʧ����ʾ��
#define sayDelAll()              Int_WTN6170_SendCmd(39)     // ����"ȫ��ɾ��"����ʾִ��ȫ��ɾ����
#define sayDelInvalid()          Int_WTN6170_SendCmd(40)     // ����"ɾ����Ч"��ɾ��Ŀ�겻���ڣ�
#define sayVerifySucc()          Int_WTN6170_SendCmd(41)     // ����"��֤�ɹ�"�������֤ͨ����
#define sayVerifyFail()          Int_WTN6170_SendCmd(42)     // ����"��֤ʧ��"�������֤δͨ����
#define saySetSucc()             Int_WTN6170_SendCmd(43)     // ����"���óɹ�"���������óɹ���ʾ��
#define saySetFail()             Int_WTN6170_SendCmd(44)     // ����"����ʧ��"����������ʧ����ʾ��
#define sayOperateSucc()         Int_WTN6170_SendCmd(46)     // ����"�����ɹ�"��ͨ�ò����ɹ���ʾ��
#define sayOperateFail()         Int_WTN6170_SendCmd(47)     // ����"����ʧ��"��ͨ�ò���ʧ����ʾ��
#define sayInvalid()             Int_WTN6170_SendCmd(48)     // ����"��Ч"������������Ч��ʾ��
#define sayAddSucc()             Int_WTN6170_SendCmd(49)     // ����"��ӳɹ�"��������¼�ɹ���ʾ��
#define sayAddFail()             Int_WTN6170_SendCmd(50)     // ����"���ʧ��"��������¼ʧ����ʾ��
#define sayAddUser()             Int_WTN6170_SendCmd(51)     // ����"����û�"����ʾ��������û����̣�
#define sayDelUser()             Int_WTN6170_SendCmd(52)     // ����"ɾ���û�"����ʾ����ɾ���û����̣�
#define sayUserFull()            Int_WTN6170_SendCmd(53)     // ����"�û�����"����ʾ�û����������ޣ�
#define sayAddAdmin()            Int_WTN6170_SendCmd(54)     // ����"��ӹ���Ա"����ʾ������ӹ���Ա���̣�
#define sayDelAdmin()            Int_WTN6170_SendCmd(55)     // ����"ɾ������Ա"����ʾ����ɾ������Ա���̣�
#define sayAdmIntull()           Int_WTN6170_SendCmd(56)     // ����"����Ա������"��Ŀ�����Ա�����ڣ���������ΪsayAdmInvalid����
#define sayCheckAdmin()          Int_WTN6170_SendCmd(57)     // ������Ա
#define sayRetry()               Int_WTN6170_SendCmd(58)     // ����"������"����ʾ�û����²�����
#define sayInputUserPassword()   Int_WTN6170_SendCmd(64)     // ����"�������û�����"����ʾ������ͨ�û����룩
#define sayInputUserPasswordAgain() Int_WTN6170_SendCmd(65)  // ����"���ٴ������û�����"����ʾȷ���û����룩
#define sayPasswordAddSucc()     Int_WTN6170_SendCmd(66)     // ����"������ӳɹ�"���û��������óɹ���
#define sayPasswordAddFail()     Int_WTN6170_SendCmd(67)     // ����"�������ʧ��"���û���������ʧ�ܣ�
#define sayPasswordVerifySucc()  Int_WTN6170_SendCmd(68)     // ����"������֤�ɹ�"���û�������֤ͨ����
#define sayPasswordVerifyFail()  Int_WTN6170_SendCmd(69)     // ����"������֤ʧ��"���û�������֤δͨ����
#define sayInputAdminPassword()  Int_WTN6170_SendCmd(70)     // ����"���������Ա����"����ʾ�������Ա���룩
#define sayInputAdminPasswordAgain() Int_WTN6170_SendCmd(71) // ����"���ٴ��������Ա����"����ʾȷ�Ϲ���Ա���룩
#define sayAddUserFingerprint()  Int_WTN6170_SendCmd(76)     // ����"����û�ָ��"����ʾ�������ָ�����̣�
#define sayDelUserFingerprint()  Int_WTN6170_SendCmd(77)     // ����"ɾ���û�ָ��"����ʾ����ɾ��ָ�����̣�
#define sayPlaceFinger()         Int_WTN6170_SendCmd(80)     // ����"�������ָ"����ʾ�û�������ָ¼��ָ�ƣ�
#define sayPlaceFingerAgain()    Int_WTN6170_SendCmd(81)     // ����"���ٴη�����ָ"����ʾ�ٴη�����ָȷ�ϣ�
#define sayTakeAwayFinger()      Int_WTN6170_SendCmd(82)     // ����"���ƿ���ָ"����ʾ����ָ�Ʋ�����
#define sayFingerprintAddSucc()  Int_WTN6170_SendCmd(83)     // ����"ָ����ӳɹ�"��ָ��¼��ɹ���ʾ��
#define sayFingerprintAddFail()  Int_WTN6170_SendCmd(84)     // ����"ָ�����ʧ��"��ָ��¼��ʧ����ʾ��
#define sayFingerprintVerifySucc() Int_WTN6170_SendCmd(85)   // ����"ָ����֤�ɹ�"��ָ��ƥ��ͨ����
#define sayFingerprintVerifyFail() Int_WTN6170_SendCmd(86)   // ����"ָ����֤ʧ��"��ָ��ƥ��δͨ����

/**
 * @brief ��ʼ�� ����ģ��WTN6170
 * 
 */
void Int_WTN6170_Init(void);

/**
 * @brief ��������
 * 
 * @param cmd ����
 */
void Int_WTN6170_SendCmd(uint8_t cmd);

#endif /* __INT_WTN6170_H__ */
