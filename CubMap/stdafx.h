// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define REMINDER(str,info)  std::cout<< #str<<"   "<<info<<std::endl
#define WARNING_IF(status,errorStr,errorInfo,successStr,successInfo) \
 if(!status)\
{   \
    std::cout<< #errorStr<<"   "<<errorInfo<<std::endl \
 return ; }\
else {\
  std::cout<< #successStr<<"   "<<successInfo<<std::endl          \
}


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
