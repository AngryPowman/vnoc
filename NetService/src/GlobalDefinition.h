#ifndef VNOC_GLOBAL_DEFINITION_H_
#define VNOC_GLOBAL_DEFINITION_H_

//������ض���
#define MAX_ROOM_USER_LIMIT                 1024    //���������������ѧ����
#define MAX_ROOM_NAME_LEN                   12      //����������󳤶�
#define MAX_NICKNAME_LEN                    16      //�ǳ���󳤶�

//�����
#define DEFAULT_ROOM_POOL_SIZE              256     //����س�ʼ��С
#define DEFAULT_ROOM_POOL_INCREASE_SIZE     100     //�����ÿ������ߴ����

typedef uint32 RoomID_t;

//�������
enum VNOCUserType
{
    UT_UNCERTAIN   = 0,  //δ��Ч��δ֪���
    UT_TEACHER     = 1,  //ѧ��
    UT_STUDENT     = 2   //��ʦ
};

//����Ȩ��
enum VNOCUserPermission
{
    PER_CHAT = 0,

};

#endif