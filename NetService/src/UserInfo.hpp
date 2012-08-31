#pragma once

struct userinfo
{
	//其中有很多用户信息
	char strUser[40];
	char passwordData[20];
	char strNickname[40];
	char strEmailAddr[40];
	int gender; //1 male 2female
	unsigned int uOld;
};//148byte



/*
____user1
admin
{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}
admin
123456789@gmail.com
1
18
____user2
test
{6,4,5,9,8,9,6,3,7,7,1,6,4,6,3,5,8,0,0,0}
test
test@test.com
0
20
____user3
able
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
iceable
645989637@qq.com
1
17
*/