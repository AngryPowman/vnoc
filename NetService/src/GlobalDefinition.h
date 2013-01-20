#ifndef VNOC_GLOBAL_DEFINITION_H_
#define VNOC_GLOBAL_DEFINITION_H_

//房间相关定义
#define MAX_ROOM_USER_LIMIT                 1024    //单个房间最大容纳学生数
#define MAX_ROOM_NAME_LEN                   12      //房间名称最大长度
#define MAX_NICKNAME_LEN                    16      //昵称最大长度

//房间池
#define DEFAULT_ROOM_POOL_SIZE              256     //房间池初始大小
#define DEFAULT_ROOM_POOL_INCREASE_SIZE     100     //房间池每次扩充尺寸基数

typedef uint32 RoomID_t;

//身份类型
enum VNOCUserType
{
    UT_UNCERTAIN   = 0,  //未生效，未知身份
    UT_TEACHER     = 1,  //学生
    UT_STUDENT     = 2   //教师
};

//在线权限
enum VNOCUserPermission
{
    PER_CHAT = 0,

};

#endif