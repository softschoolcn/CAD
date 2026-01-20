#pragma once
//验证注册码，同时验证机器码
#define VALID_REG_KEY(strRegDate,lpKey)\
{\
	strRegDate=_T("21141127");\
}

//验证注册码，忽略机器码，提高校验速度
#define VALID_REG_KEY_INGOREID(strRegDate,lpKey)\
{\
	strRegDate=_T("21141127");\
}

#define CHECK_REG_BOOL(lpDevDate)\
{\
	return TRUE;\
}

#define CHECK_REG_VOID(lpDevDate)\
{\
}