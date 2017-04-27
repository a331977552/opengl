// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
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


// TODO:  在此处引用程序需要的其他头文件
