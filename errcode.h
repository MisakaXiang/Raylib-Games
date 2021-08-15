// errcode.h 用于定义游戏错误码
#pragma once

const int SUCCESS = 0;                // 成功
const int NULL_POINTER_ERROR = 10001; // 空指针错误
const int SONG_ID_INVAILD = 10002;    // song_id不合法

char *ConvertErrCodeToString(int errCode)
{
    switch (errCode)
    {
    case 0:
        return "success";
    case 10001:
        return "null pointer error";
    case 10002:
        return "song_id invaild";
    default:
        return "unknown error";
    }
    return "";
}